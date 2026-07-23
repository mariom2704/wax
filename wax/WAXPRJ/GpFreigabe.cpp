#include "stdafx.h"


#include "GpFreigabe.h"
#include "DbFreigabe.h"
#include "WpPreisanfrage.h"
#include <afxadv.h> // fuer CSharedFile
#include <mxodbc\mxodbc.h>


GpFreigabe::GpFreigabe(void)
{
}

GpFreigabe::~GpFreigabe(void)
{
}

/**
Freigabe der Preisanfrage in der AV, nachdem diese einen Preis zugeordet hat. Der Status wechselt bei der Freiagbe 
von "Preisanfrage" nach "Angebot"
*/
bool GpFreigabe::freigabeAngebot(WaxPart *wp)
{
	//Prüfungen starten
	if (!checkWerte(wp, false, true, true, Status::Preisanfrage))
		return false;


	//Prüfen ob Gliederung
	WpPreisanfrage* gliederung = dynamic_cast<WpPreisanfrage*>(wp);
	if (!gliederung)
	{
		AfxMessageBox(getStammdaten().getText("KEINE_PA").c_str(), MB_ICONERROR | MB_OK);
		return false;
	}

	//Sicherheitsabfrage
	if (AfxMessageBox(getStammdaten().getText("IDS_PA_FREIGABE").c_str(), MB_ICONQUESTION | MB_YESNO) == IDNO)
		return false;


	//Daten sichern
	CMemFile f;
	if (!saveWaxPart(wp, &f))
		return false;


	//Daten in DB schreiben
	//Transaction vorbereiten
	MXTable activeDB(WaxSys::getServerdb(), "Status");
	DbBase transation(activeDB.get_Db());
	transation.beginTransaction();
	

	bool result = true;

	TRY
	{
		for (POSITION posG = gliederung->get_subobjects().GetHeadPosition(); posG != NULL; )
		{
			WaxPart* waxpt = dynamic_cast<WaxPart*>(gliederung->get_subobjects().GetNext(posG));
			if (!waxpt || waxpt->get_deleted())
				continue;

			SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(waxpt);
			
			//Preis in DB schreiben
			Server serv(Server::save, sammel->get_werkauftrag(), activeDB.get_Db());	
			if (!serv.updatePreisanfrage())
			{
				result = false;
				break;
			}

			//Status aller Positionen im Sammelauftrag setzen			
			for (POSITION posS = sammel->get_subobjects().GetHeadPosition(); posS; )
			{
				WaxPart* next = dynamic_cast<WaxPart*>(sammel->get_subobjects().GetNext(posS));
				XASSERT(next);
				if (!next || next->get_deleted())
					continue;

				//Status auf Angebot setzen
				next->get_werkauftrag()->get_status().set_status(Status::Angebot);

				//Status in Db schreiben
				Server serv(Server::save, next->get_werkauftrag(), activeDB.get_Db());	
				if (!serv.updateStatus())
				{
					result = false;
					break;
				}

				//Lieferterin auch updaten, da dieser von dem Angebot abhängig ist
				if (!serv.updateLiefertermin())
				{
					result = false;
					break;
				}
			}

			if (result == false)
				break;

			//Status an Sammelauftrag übergeben
			WaxPart* sub = sammel->getLastSubobject();
			sammel->get_werkauftrag()->get_status().copy(sub->get_werkauftrag()->get_status());
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		
		MxString st = "Freigabefehler: " + e->ReportError();
		LOG(st.getString());
		result = false;
	}
	END_CATCH_ALL


	//Transaktion abschließen
	if (result == true)
		transation.commitTransaction();
	else
	{
		transation.rollbackTransaction();
		restoreWaxPart(&f, wp);
	}
	

	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);
	
	return result;
}

/**
Status von WaxPart und aller Unterordner aktualisieren
*/
bool GpFreigabe::updateStatus(WaxPart *wp)
{
	if (!wp)
		return false;

	if (wp->get_subobject_count() > 0)
	{
		//Unterpositionen freigeben
		for (POSITION posS = wp->get_subobjects().GetHeadPosition(); posS; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(wp->get_subobjects().GetNext(posS));
			if (!updateStatus(next))
				return false;
		}

		//Wenn Sammelauftrag, dann Status auch im Sammelauftrag setzen
		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(wp);
		if (sammel)
		{
			WaxPart* sub = sammel->getLastSubobject();
			sammel->get_werkauftrag()->get_status().copy(sub->get_werkauftrag()->get_status());

			//Preis updaten
			Server serv(Server::load, sammel->get_werkauftrag(), NULL);	
			if (!serv.updatePreisanfrage())
				return false;
		}
	}
	else
	{
		//Werkauftrag freigeben
		Werkauftrag *wa = wp->get_werkauftrag();
		if (!wa)
			return false;

		if (wa->get_waid() > 0)
		{
			//Status einlesen
			Server serv(Server::load, wa, NULL);	
			if (!serv.updateStatus())
				return false;
		}
		else
			wa->get_status().set_status(Status::Positioniert); //in Bearbeitung, noch nicht freigegeben
	}

	return true;
}




/**
Freigabe der Preisanfrage in der Niederlassung. Es wird eine neue PA Nummer erstellt und alle Daten der Positionen in die
Tabellen geschrieben. (Wie bei der normalen Freigabe, nur mit dem Status Preisanfrage)
*/
bool GpFreigabe::freigabePreisanfrage(WaxPart *wp)
{
	//Prüfen ob Preisanfrage
	WpPreisanfrage* pa = dynamic_cast<WpPreisanfrage*>(wp);
	if (!pa)
	{
		AfxMessageBox(getStammdaten().getText("KEINE_PA").c_str(), MB_ICONERROR | MB_OK);
		return false;
	}

	if (pa->get_subobject_count() == 0)
	{
		AfxMessageBox(getStammdaten().getText("KEINE_PA_UNTERAUFTRAEGE").c_str(), MB_ICONERROR | MB_OK);
		return false;
	}

	bool showWarnung = false;
	if (pa->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer() > 0)
	{
		//Prüfung ob ein Preis gesetzt wurde
		for (POSITION pos = pa->get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(pa->get_subobjects().GetNext(pos));
			if (!sammel || sammel->get_deleted())
				continue;
			if (sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_preis() > 0.001 || sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_info().length() > 0)
				showWarnung = true;
		}	

		//Wenn Nummer schon gesetzt ist, Status prüfen, ob evt. schon ein Angebot gemacht wurde
		if (!updateStatus(pa))
			return false;
	}

	//Sicherheitsabfrage
	if (AfxMessageBox(getStammdaten().getText("IDS_PA_FREIGABE").c_str(), MB_ICONQUESTION | MB_YESNO) == IDNO)
		return false;


	//Prüfen ob Unterorder Sammelaufträge und Plausis ok sind
	for (POSITION pos = pa->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart* waxpt = dynamic_cast<WaxPart*>(pa->get_subobjects().GetNext(pos));
		if (!waxpt || waxpt->get_deleted())
			continue;

		//prüfen ob Sammelauftrag
		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(waxpt);
		if (!sammel || !sammel->get_werkauftrag())
		{
			AfxMessageBox(getStammdaten().getText("FALSCHER_AUFTRAG").c_str(), MB_ICONERROR | MB_OK);
			return false;
		}

		if (sammel->get_subobject_count() == 0)
		{
			AfxMessageBox(getStammdaten().getText("SAMMEL_OHNE_POS").c_str(), MB_ICONERROR | MB_OK);
			return false;
		}

	
		if (sammel->get_werkauftrag()->get_status().get_status() != Status::NichtGesetzt &&
			sammel->get_werkauftrag()->get_status().get_status() != Status::Positioniert &&
			sammel->get_werkauftrag()->get_status().get_status() != Status::Preisanfrage)
		{
			AfxMessageBox(getStammdaten().getText("STATUS_NICHT_PA").c_str(), MB_ICONERROR | MB_OK);
			return false;
		}

		//Pürfen ob Werte ok sind
		if (!checkPlausis(waxpt))
			return false;

		
	}

	if (showWarnung)
		AfxMessageBox(getStammdaten().getText("PREIS_NICHT_FREI").c_str(), MB_ICONERROR | MB_OK);

	CMemFile f;
	if (!saveWaxPart(wp, &f))
		return false;

	//Fortschrittsanzeige initialisieren
	int max = countWerkauftraege(wp);
	Fortschrittanzeige fortschritt(0, max, getStammdaten().getText("ID_21132").c_str());

	//Freigabe starten
	bool result = true;
	DbFreigabe freigabe(wp, NULL);
	if (!freigabe.preisanfrageFreigeben(&fortschritt))
	{
		//Alte Werte von Waxpart wieder herstellen
		restoreWaxPart(&f, wp);
		result = false;
	}

	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);

	return result;
}


bool GpFreigabe::saveWaxPart(WaxPart *wp, CMemFile *f)
{
	TRY
	{
		BinaryArchive ar(f, BinaryArchive::store);
		ar.set_version(1000);
		wp->serialize(ar);
		ar.close();
	}
	CATCH(CException, e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH

	return true;
}

bool GpFreigabe::restoreWaxPart(CMemFile *f, WaxPart *wp)
{
	TRY
	{
		//Alle Unterordner löschen
		wp->get_subobjects().deleteAllElemente();
		
		f->SeekToBegin();
		BinaryArchive ar(f, BinaryArchive::load);
		ar.set_version(1000); // letzte Version
		wp->serialize(ar);
		ar.close();
	}
	CATCH(CException, e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH

	return true;
}


int GpFreigabe::countWerkauftraege(WaxPart *pt)
{
	int counter = 0;

	if (!pt || pt->get_deleted())
		return 0;

	if (!dynamic_cast<PrjGliederung*>(pt) &&
		!dynamic_cast<PrjLeitkarteData*>(pt) &&
		!dynamic_cast<SammelAuftrag*>(pt) &&
		!dynamic_cast<WpPreisanfrage*>(pt))
		counter++;


	for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart* waxpt = dynamic_cast<WaxPart*>(pt->get_subobjects().GetNext(pos));
		if (!waxpt || waxpt->get_deleted())
			continue;

		counter += countWerkauftraege(waxpt);
	}

	return counter;
}


bool GpFreigabe::checkBeschichtung(Werkauftrag *werkauftrag)
{
	bool verzinkt = false;
	if (werkauftrag->get_beschichtung().get_typ() == Beschichtung::VERZINKT)
		verzinkt = true;	

	for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos;)
	{
		DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
		
		if ((int)werkauftrag->get_leitkarte().get_sdMaterialnummer() == dat->get_nummer())
		{
			// bei Strang gibs nur 1 Nr 
			if (dat->get_klasse() == "Strangrohr ApaxWerkauftrag SammelWerkauftrag")
				break;
			if (dat->get_verzinkt() != verzinkt)
			{
				CString str; str = getStammdaten().getText("IDS_SDMATNR_BESCH").c_str();
				AfxMessageBox(str, MB_OK | MB_ICONERROR);
				if (!WaxSys::isAV()) return false;
			}
			break;
		}
	}
	return true;
}


bool GpFreigabe::checkPlausis(WaxPart *wp)
{
	CString tmp, err;

	if (!wp)
		return false;

	//Falls Modell noch nicht ertstellt wurde, dies nachholen
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(wp->get_werkauftrag());
	if (rohr)
	{
		if (rohr->get_elementList().IsEmpty())
			rohr->createModell();

		err = rohr->check_rohrstutzen();			
		if (err.GetLength() > 0)
		{
			AfxMessageBox(err, MB_ICONERROR | MB_OK);
			return false;
		}
	}
	
	//Prüfen ob  Werte der Leitkarte richtig sind
	if (!wp->testLeitkarte(true))
		return false;

	if (!checkBeschichtung(wp->get_werkauftrag()))
		return false;

	err = wp->get_werkauftrag()->checkPlausis();
	if (err.GetLength() > 0)
	{
		AfxMessageBox(err, MB_ICONERROR | MB_OK);
		return false;
	}


	SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(wp);
	if (sammel)
	{
		if (!sammel->checkAVNachbearbeitung())
			return false;

		//Plausis der einzelnen Positionen prüfen
		POSITION pos;
		for (pos = sammel->get_subobjects().GetHeadPosition(); pos; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(sammel->get_subobjects().GetNext(pos));
			XASSERT(next);
			if (!next || next->get_deleted())
				continue;
			
			Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(next->get_werkauftrag());
			if (rohr)
			{
				if (rohr->get_elementList().IsEmpty())
					rohr->createModell();
			}

			err = next->get_werkauftrag()->checkPlausis();
			if (err.GetLength() > 0)
			{
				tmp.Format(getStammdaten().getText("err_falsche_daten").c_str(), next->get_werkauftrag()->get_posNr());
				err = tmp + "\n" + err;
				AfxMessageBox(err, MB_ICONERROR | MB_OK);
				return false;
			}

			// Pruefung Rohrstutzen auf Abschlusselement
			if (rohr)
			{
				err = rohr->check_rohrstutzen();			
				if (err.GetLength() > 0)
				{
					AfxMessageBox(err, MB_ICONERROR | MB_OK);
					return false;
				}
			}
		}

	}

	return true;
}


/**
Bestätigung des Angebotspreises in der Niederlassung. Der Status wechselt von Angebot auf Freigegegen. Es werden KEINE Positionsdaten mehr 
in die Serverddatenbank gesschieben. Es wird nur der Status geändert.
*/
bool GpFreigabe::freigabeAngebotspreis(WaxPart *wp)
{
	//Prüfen ob Angebot und richtige Werte gesetzt sind
	if (!checkWerte(wp, true, true, true, Status::Angebot))
		return false;

	WpPreisanfrage* gliederung = dynamic_cast<WpPreisanfrage*>(wp);
	if (!gliederung)
	{
		AfxMessageBox(getStammdaten().getText("KEINE_PA").c_str(), MB_ICONERROR | MB_OK);
		return false;
	}

	//Sicherheitsabfrage
	if (AfxMessageBox(getStammdaten().getText("IDS_PA_PREIS_FREIGABE").c_str(), MB_ICONQUESTION | MB_YESNO) == IDNO)
		return false;


	//Daten sichern
	CMemFile f;
	if (!saveWaxPart(wp, &f))
		return false;


	//Daten in DB schreiben
	//Transaction vorbereiten
	MXTable activeDB(WaxSys::getServerdb(), "Status");
	DbBase transation(activeDB.get_Db());
	transation.beginTransaction();

	bool result = true;

	//In der Db den Status auf Freigegeben setzen
	TRY
	{
		for (POSITION posG = gliederung->get_subobjects().GetHeadPosition(); posG != NULL; )
		{
			SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(gliederung->get_subobjects().GetNext(posG));
			if (!sammel || sammel->get_deleted())
				continue;		

			//Status aller Positionen im Sammelauftrag setzen			
			for (POSITION posS = sammel->get_subobjects().GetHeadPosition(); posS; )
			{
				WaxPart* next = dynamic_cast<WaxPart*>(sammel->get_subobjects().GetNext(posS));
				XASSERT(next);
				if (!next || next->get_deleted())
					continue;

				//Status auf Angebot setzen
				next->get_werkauftrag()->get_status().set_status(Status::Freigegeben);

				Server serv(Server::save, next->get_werkauftrag(), activeDB.get_Db());	
				if (!serv.updateSdNummer())
				{
					result = false;
					break;
				}
				if (!serv.updateStatus())
				{
					result = false;
					break;
				}
			}

			if (result == false)
				break;

			//Status an Sammelauftrag übergeben
			WaxPart* sub = sammel->getLastSubobject();
			sammel->get_werkauftrag()->get_status().copy(sub->get_werkauftrag()->get_status());
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		result = false;
		MxString st = "Freigabefehler: " + e->ReportError();
		LOG(st.getString());
	}
	END_CATCH_ALL


	//Transaktion abschließen
	if (result == true)
		transation.commitTransaction();
	else
	{
		transation.rollbackTransaction();
		restoreWaxPart(&f, wp);
	}

	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);

	return result;
}


bool GpFreigabe::checkWerte(WaxPart *wp, bool checkSDNr, bool checkPaNrIsSet, bool checkPreisIsSet, Status::STATUS stat)
{
	if (!wp)
		return false;

	//Prüfungen starten

	//Prüfen ob Gliederung
	WpPreisanfrage* gliederung = dynamic_cast<WpPreisanfrage*>(wp);
	if (!gliederung)
	{
		AfxMessageBox(getStammdaten().getText("KEINE_PA").c_str(), MB_ICONERROR | MB_OK);
		return false;
	}

	//Prüfen ob Unterorder Sammelaufträge sind und alle die gleiche Preisanfage haben
	int paNr = 0;
	for (POSITION pos = gliederung->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart* waxpt = dynamic_cast<WaxPart*>(gliederung->get_subobjects().GetNext(pos));
		if (!waxpt || waxpt->get_deleted())
			continue;

		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(waxpt);

		//prüfen ob Sammelauftrag
		if (!sammel || !sammel->get_werkauftrag())
		{
			AfxMessageBox(getStammdaten().getText("FALSCHER_AUFTRAG").c_str(), MB_ICONERROR | MB_OK);
			return false;
		}

		//Prüfen ob richtiger Status
		if (sammel->get_werkauftrag()->get_status().get_status() != stat)
		{
			if (stat == Status::Preisanfrage)
				AfxMessageBox(getStammdaten().getText("STATUS_NICHT_PA").c_str(), MB_ICONERROR | MB_OK);
			else if (stat == Status::Angebot)
				AfxMessageBox(getStammdaten().getText("STATUS_NICHT_ANGEBOT").c_str(), MB_ICONERROR | MB_OK);

			return false;
		}

		if (checkPaNrIsSet)
		{
			//prüfen ob PA Nummer gesetzt ist
			if (paNr == 0 && sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer() == 0)
			{
				AfxMessageBox(getStammdaten().getText("KEINE_UNTER_PA").c_str(), MB_ICONERROR | MB_OK);
				return false;
			}

			//Prüfen ob PA Nummer bei allen Sammelaufträgen gleich ist
			if (paNr == 0)
				paNr = 	sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer();
			else if (paNr != sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer())
			{
				AfxMessageBox(getStammdaten().getText("DIFF_PA").c_str(), MB_ICONERROR | MB_OK);
				return false;
			}
		}

		if (checkPreisIsSet && sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_preis() <= 0)
		{
			AfxMessageBox(getStammdaten().getText("KEIN_PA_PREIS").c_str(), MB_ICONERROR | MB_OK);
			return false;
		}

		//Leitkarte prüfen
		if (checkSDNr)
		{
			CString err = sammel->get_leitkarte().checkVars(true, true);
			if (err.GetLength() > 0)
			{
				err = getStammdaten().getText("IDS_FEHLENDE_DATEN1b").c_str() + err;
				AfxMessageBox(err, MB_ICONERROR | MB_OK);
				return false;
			}

			DbFreigabe frei(sammel, NULL);
			if (!frei.check_freigabe_plausis())
				return false;

		}

		//Plausis des Sammelauftrages 
		if (!checkPlausis(sammel))
			return false;
	}

	return true;
}
