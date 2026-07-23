#include "stdafx.h"


#include <mxodbc\mxodbc.h>
#include "DbFreigabe.h"
#include "..\waxprj\BefehlUpdateStatistik.h"
#include "WpPreisanfrage.h"
#include ".\Fortschrittanzeige.h"


//Konstruktor
DbFreigabe::DbFreigabe(WaxPart* wa, DWORD dbhandle): DbBase( dbhandle), werkauftrag(*(wa->get_werkauftrag())), leitkarte(wa->get_leitkarte())
{
	/*
	Konstruktor
	offset ist der externe Zähler für Aufträge innerhalb eines Werkauftrages.
	offset -1 schaltet die Prüfung
	*/
	pt = wa;
	// internes Flag setzen, ob aus der AV freigegeben wird.
	av = WaxSys::isAV();

	tabPreisanfrage = "preisanfrage";

}

//Freigabe des Werkauftrages durchführen, bzw. Freigabedaten aktualisieren
bool DbFreigabe::freigeben()
{
	/*
	Freigabe
	Erstellt einen neuen Eintrag in der Tabelle Werkauftrag auf dem Waxserver. 
	Ist bereits ein Auftrag freigeben worden, werden die Daten geändert. Wenn bereits
	eine WA Nummer vergeben wurde, wird die Freigabe verweigert.
	*/	

	CString str;
	
	Server serverdb(Server::save, &werkauftrag, this->dbhandle);	
	CString server = serverdb.checkServerDB();
	if (!server.GetLength())
		return false;

	// Freigabeid setzen
	unsigned long waid = getfreigabeid(this->dbhandle);
	if (!waid)
	{
		str = getStammdaten().getText("IDS_KEIN_NEUER_WA").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONERROR);
		return false;
	}

	bool ret = true;
	werkauftrag.set_waid(waid);
	werkauftrag.get_status().set_freigabecounter(werkauftrag.get_status().get_freigabecounter()+1);

	if (werkauftrag.get_status().get_status() == Status::NichtGesetzt ||
		werkauftrag.get_status().get_status() == Status::Positioniert)
		werkauftrag.get_status().set_status(Status::Freigegeben);

	TRY
	{
		// Prüfung, Tabelle Status gelesen werden kann
		serverdb.set_modus(Server::load);
		if (!serverdb.testStatusTab())
		{
			AfxMessageBox(getStammdaten().getText("IDS_STATUS_NICHT").c_str(), MB_OK | MB_ICONERROR);
			return false;
		}
		serverdb.set_modus(Server::save);

		// Wenn die Werkauftragsnummer bereits vergeben wurde, darf auch keine Änderung
		// mehr möglich sein, es sei denn von seiten der AV
		if (werkauftrag.get_status().get_wanr() && !av) 
		{
			str = getStammdaten().getText("IDS_WERKAUFTRAG_NICHT").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!updateStatus(serverdb))
		{
			str = getStammdaten().getText("IDS_STATUS_SETZEN").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			return false;
		}
		
		if (ret && !serverdb.updateLeitkarte(pt->GetRuntimeClass()->m_lpszClassName))
			ret = false;
		
		if (ret && !serverdb.updateMaterial())
			ret = false;
		
		if (ret && !serverdb.updateAnhang())
			ret = false;

		if (ret && !serverdb.updateRohrleitung())
			ret = false;

		if (ret && !serverdb.updateElemente())
			ret = false;
		
		if (ret && !serverdb.updateObject())
			ret = false;
		
		if (ret && !serverdb.updateSoko())
			ret = false;
		
		//Wenn Freigabe in der AV erfolgt, Statistik in DB schreiben		
		if (WaxSys::isAV())
		{
			WerkauftragListe listeWa;
			listeWa.append(&werkauftrag);

			BefehlUpdateStatistik befehl;
			befehl.set_werkauftraege(&listeWa);
			befehl.set_server(&serverdb);

			if (!befehl.start())
				ret = false;
		}	
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		ret = false;
	}
	END_CATCH_ALL

	if (ret)
	{
		// Wenn der Werkauftrag ausgelöst wurde, muss die waid gesetzt werden. 
		if (!werkauftrag.get_waid())
			werkauftrag.set_waid(waid);
	}
	return ret;
}




//Öffnet oder erstellt einen neuen Freigabedatensatz
unsigned long DbFreigabe::getfreigabeid(DWORD dbhandle)
{
	/*
	Freigabe id erzeugen 
	Die Freigabeid (waid) ist eine interne Referenz auf einen Datensatz in der Tabelle Werkauftrag.
	Hat der WA noch keine waid bekommen (wird mit dem Projekt abgespeichert), so wird ein neuer
	Datensatz erzeugt und die neue waid zurückgeliefert.
	@rdesc waid als Zugriffsschlüssel zur Werkauftrag Tabelle
	*/
	if (werkauftrag.get_waid())
			return werkauftrag.get_waid(); // Werkauftrag besteht schon

	MXTable wa(dbhandle, "werkauftrag");
	LongCol waid("waid");
	DECLARE_COL(BigInt, sdauftragsnr, wa);
	DECLARE_COL(Int, sdpositionsnr, wa);
	DECLARE_COL(Int, nl, wa);
	DECLARE_COL(String, sachbearbeiternl, wa);
	DECLARE_COL(String, auftragsbez, wa);
	DECLARE_COL(String, classid, wa);
	DECLARE_COL(String, ippeMatNr, wa)
	DECLARE_COL(String, ippeKnotenNr, wa)
	DECLARE_COL(Bool,   isNextGeneration, wa)

	// Neuen Werkauftrag anlegen
	TRY
	{
		
		wa.load("waid=-1", "", false);
		wa.addNew();
		*auftragsbez.data = leitkarte.get_prj_auftragsbez();
		nl.data  = leitkarte.get_prj_nl();
		*sachbearbeiternl.data = leitkarte.get_sachbearbeiterNL();
		*classid.data = pt->GetRuntimeClass()->m_lpszClassName;
		sdauftragsnr.data = leitkarte.get_sdAuftragsnr();
		sdpositionsnr.data = leitkarte.get_sdPositionsnr();
		*ippeMatNr.data  = leitkarte.get_ippeMatNr();
		*ippeKnotenNr.data = leitkarte.get_ippeKnotenNr();
		isNextGeneration.data = leitkarte.get_IsChoosenIppeNumbers();
		
		wa.update();
		wa.close();

		// Jetzt wird der Werkauftrag nochmals geladen. Die waid muss gelesen werden.
		CString select, sort;
		if(leitkarte.get_IsChoosenIppeNumbers())
		{
			select.Format("ippeMatNr = \"%s\"", leitkarte.get_ippeMatNr());
		}
		else
		{
			select.Format("sdauftragsnr = %I64d AND sdpositionsnr = %i",leitkarte.get_sdAuftragsnr(), leitkarte.get_sdPositionsnr());
		}
		sort = "waid DESC";
		
		wa.attach(&waid);
		wa.load(select, sort, true);
		XASSERT(waid.data);

		werkauftrag.set_waid(waid.data);
		
		return waid.data;
	}
	CATCH(CDBException, e)
	{
		e->ReportError();
		return 0;
	}
	END_CATCH
}



//Öffnet oder erstellt einen neuen Freigabedatensatz
unsigned long DbFreigabe::getfreigabeid(WaxPart *pt)
{
	if (pt->get_werkauftrag()->get_waid())
		return pt->get_werkauftrag()->get_waid(); // Werkauftrag besteht schon


	MXTable tabwa(dbhandle, "werkauftrag");
	DECLARE_COL(BigInt, sdauftragsnr, tabwa);
	DECLARE_COL(Int, sdpositionsnr, tabwa);
	DECLARE_COL(Int, nl, tabwa);
	DECLARE_COL(String, sachbearbeiternl, tabwa);
	DECLARE_COL(String, auftragsbez, tabwa);
	DECLARE_COL(String, classid, tabwa);

	DECLARE_COL(String, ippeMatNr, tabwa)
	DECLARE_COL(String, ippeKnotenNr, tabwa)
	DECLARE_COL(Bool, isNextGeneration, tabwa)

	// Neuen Werkauftrag anlegen
	TRY
	{
		tabwa.load("waid=-1", "", false);
		tabwa.addNew();

		*auftragsbez.data = pt->get_werkauftrag()->get_leitkarte().get_prj_auftragsbez();
		nl.data  = pt->get_werkauftrag()->get_leitkarte().get_prj_nl();
		*sachbearbeiternl.data = pt->get_werkauftrag()->get_leitkarte().get_sachbearbeiterNL();
		*classid.data = pt->GetRuntimeClass()->m_lpszClassName;
		sdauftragsnr.data = pt->get_werkauftrag()->get_leitkarte().get_sdAuftragsnr();
		sdpositionsnr.data = pt->get_werkauftrag()->get_leitkarte().get_sdPositionsnr();
		*ippeMatNr.data  = leitkarte.get_ippeMatNr();
		*ippeKnotenNr.data = leitkarte.get_ippeKnotenNr();
		isNextGeneration.data = leitkarte.get_IsChoosenIppeNumbers();

		tabwa.update();
		tabwa.close();

		
		/**
		Hinweis: wenn es keine MySql DB ist, dann muß hier eine andere Lösung gefunden werden!
		*/
		
		string sql = "SELECT LAST_INSERT_ID()";
		CString res = tabwa.executeSQL(sql.c_str());
		
		int waid = atoi(res);
		XASSERT(waid);

		return waid;
	}
	CATCH(CDBException, e)
	{
		e->ReportError();
		return 0;
	}
	END_CATCH
}




//Liest aus der Serverdatenbank den Status aus und setzt die Werte in der 
//Leitkarte
bool DbFreigabe::updateStatus(Server &serverdb)
{
	/*
	Status updaten
	Für jeden Werkauftrag gibt es einen Eintrag in der Tabelle Status. Der Eintrag wird
	bei der ersten Freigabe automatisch erstellt. Nach der ersten Freigabe besteht nur noch 
	ein Lesezugriff für die NL.
	*/
	if (!werkauftrag.get_waid())
	{
		if (!serverdb.updateStatus(false))
		{
			CString str; str = getStammdaten().getText("IDS_FREIGABE_NICHT").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			AfxThrowNotSupportedException();
		}
		return false;

	}
	return serverdb.updateStatus(false);
}


//Test auf vollständige Eingabe der Leitkarte
bool DbFreigabe::check_freigabe_plausis()
{
	/*
	Prüfung ob Werkauftrag schon in Bearbeitung
	Prüfung der Daten in der Leitkarte auf Vollständigkeit
	Es wird geprüft, ob 

	  - ein Sachbearbeiter eingegeben wurde
	  - die Projektdaten eingegeben wurden
	  - SD Auftragsnummer und SD Positionsnummer bereits vergeben sind (Tippfehler)
	 */
	Server serverdb(Server::load, &werkauftrag, dbhandle);
	CString server = serverdb.checkServerDB();
	if (!server.GetLength())
		return false;

	// Beschichtung zu SDMatnr checken 
	bool checksap = WaxSys::para_int(CHECKSAP) ? true : false;
	if (checksap)
		if (!checkBeschichtung())
			return false;

	if (!av)
	{ // In der AV kann alles mögliche eingegeben werden
		CString err(leitkarte.check());
		if (err.GetLength())
		{
			AfxMessageBox(err, MB_ICONERROR | MB_OK);
			return false;
		}
	}
		
	// Prüfung, ob die SD Nummern bereits vergeben wurden bzw. ob die waid übereinstimmt
	MXTable wa(dbhandle, "werkauftrag");
	BigIntCol sdnummer("sdauftragsnr");
	IntCol sdpos("sdpositionsnr");
	LongCol waid("waid");
	StringCol strIppeMat("ippeMatNr");
	StringCol strIppeKnotenNr("ippeKnotenNr");

	MXTable status(dbhandle, "status");
	IntCol wanr("wanr");
 	BoolCol storniert("storniert");
	LongCol waids("waid");

	TRY
	{
		wa.attach(&sdnummer);
		wa.attach(&sdpos);
		wa.attach(&waid);
		wa.attach(&strIppeMat);
		wa.attach(&strIppeKnotenNr);

		status.attach(&wanr);
		status.attach(&storniert);
		status.attach(&waids);
		
		CString select;
		if(leitkarte.get_IsChoosenIppeNumbers())
		{
			select.Format("ippeMatNr = \"%s\"",leitkarte.get_ippeMatNr());
		}
		else
		{
			select.Format("sdauftragsnr = %I64d AND sdpositionsnr = %i",leitkarte.get_sdAuftragsnr(), leitkarte.get_sdPositionsnr());
		}
		wa.load(select, (CString)"waid", true);
		
		select.Format("waid = %i", (int)waid.data);
		status.load(select, (CString)"waid", true);

		if (wa.get_count() && werkauftrag.get_waid() != (int)waid.data && !storniert.data)
		{
			CString str;
			if(leitkarte.get_IsChoosenIppeNumbers())
			{
				str.Format(getStammdaten().getText("IDS_IPPENUMMERN_VERGEBEN").c_str(), leitkarte.get_ippeMatNr());
			}
			else
			{
				str.Format(getStammdaten().getText("IDS_SDNUMMERN_VERGEBEN").c_str(), leitkarte.get_sdAuftragsnr(), leitkarte.get_sdPositionsnr());
			}

			CWnd::GetActiveWindow()->MessageBox(str, leitkarte.get_prj_auftragsbez(),MB_OK | MB_ICONINFORMATION);
			return false;
		}
		wa.close();

		// Prüfung, ob Werkauftrag schon in Bearbeitung
		if (wa.get_count() && wanr.data != 0 && !storniert.data && !WaxSys::isAV())
		{
			CString str; str = getStammdaten().getText("IDS_WERKAUFTRAG_NICHT").c_str();
			CWnd::GetActiveWindow()->MessageBox(str, leitkarte.get_prj_auftragsbez(), MB_OK | MB_ICONINFORMATION);
			return false;
		}
	}
	CATCH(CDBException, e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH
	return true;
}

bool DbFreigabe::checkBeschichtung()
{
	bool verzinkt = false;
	if (werkauftrag.get_beschichtung().get_typ() == Beschichtung::VERZINKT)
		verzinkt = true;	

	for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos;)
	{
		DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
		
		if ((int)werkauftrag.get_leitkarte().get_sdMaterialnummer() == dat->get_nummer())
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





/**
Preisanfrage freigeben. Wenn noch keine PA-Nr zugeordent ist wird automatisch eine neue angelegt
*/
bool DbFreigabe::preisanfrageFreigeben(Fortschrittanzeige *fortschritt)
{
	WpPreisanfrage* pa = dynamic_cast<WpPreisanfrage*>(pt);
	if (!pa || pa->get_deleted())
		return false;

	//pürfen ob Verbindung zum Server besteht
	Server serverdb(Server::save, pa->get_werkauftrag(), dbhandle);
    CString server = serverdb.checkServerDB();
	if (!server.GetLength())
		return false;


	CString str;
	if (fortschritt)
	{
		fortschritt->start();

		//Auftragsbez
		str.Format("%s: %s", getStammdaten().getText("msg_auftrag").c_str(), pa->get_werkauftrag()->get_leitkarte().get_prj_auftragsbez());
		fortschritt->setAnzeige1(str);
	}

	this->beginTransaction();

	//Evt. eine neue Nummer anlegen
	int panr = pa->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer();
	int sammelCounter = 1;
	if (panr <= 0)
	{
		//Neue Nummer anlegen	
		if (!insertNewPreisanfrage(panr, sammelCounter))
		{
			this->rollbackTransaction();
			return false;
		}
		pa->get_werkauftrag()->get_leitkarte().get_preisanfrage().set_nummer(panr);
	}

	
	for (POSITION pos = pa->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		//prüfen ob Sammelauftrag
		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(pa->get_subobjects().GetNext(pos));
		if (!sammel || !sammel->get_werkauftrag() || sammel->get_deleted())
			continue;

		if (!insertNewPreisanfrage(panr, sammelCounter))
		{
			this->rollbackTransaction();
			return false;
		}
		//Werte dem Sammelauftrag zuordnen
		sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().set_nummer(panr);
		sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().set_position(sammelCounter);
		sammel->get_werkauftrag()->get_status().set_status(Status::Preisanfrage);
		

		if (fortschritt)
		{
			//Bauabschnitt
			str.Format("%s: %s", getStammdaten().getText("IDS_BAUABSCHNITT").c_str(), sammel->get_werkauftrag()->get_leitkarte().get_prj_bauAbschnitt());
			fortschritt->setAnzeige2(str);
		}

		//Positionen des Sammelauftrages freigeben
		for (POSITION poss = sammel->get_subobjects().GetHeadPosition(); poss != NULL; )
		{
			WaxPart* wp = dynamic_cast<WaxPart*>(sammel->get_subobjects().GetNext(poss));
			if (!wp || !wp->get_werkauftrag() || wp->get_deleted())
				continue;

			if (panr > 0)
			{
				wp->get_leitkarte().get_preisanfrage().set_nummer(panr);
				wp->get_leitkarte().get_preisanfrage().set_position(sammelCounter);
			}

			//Status setzen
			wp->get_werkauftrag()->get_status().set_status(Status::Preisanfrage);
			
			if (fortschritt)
			{
				//Bezeichnung setzen
				str.Format("%s: %s", getStammdaten().getText("msg_position").c_str(), wp->get_werkauftrag()->get_posNr());
				fortschritt->setAnzeige3(str);

				fortschritt->nextStep();
				fortschritt->updateCounterAnzeige();
			}

			//Werkauftrag in Db schreiben (Ohne Stückliste, da diese beim Einlesen neuer Aufträge erzeugt wird.)
			if (!freigabeWerkauftrag(wp, false))
			{
				this->rollbackTransaction();
				return false;
			}			
		}

		sammelCounter++;
	}
	
	return this->commitTransaction();
}

/**
Neue Preisanfrage in die DB schreiben
*/
bool DbFreigabe::insertNewPreisanfrage(int &panr, int papos)
{
	MXTable preisanfrage(dbhandle, tabPreisanfrage, false);
	DECLARE_COL(Int, nr, preisanfrage);
	DECLARE_COL(Int, pos, preisanfrage);

	CString select;
	select.Format("nr = %i and pos=%i", panr, papos);
	
	TRY
	{
		preisanfrage.load((LPCSTR)select, (LPCSTR)"", false);

		//Wenn Datensatz schon existiert, Methode wieder verlassen
		if (!preisanfrage.isEof())
			return true;

		preisanfrage.addNew();

		//Wenn panr == 0, wird über autoincrement eine neue Nummer erzeugt
		if (panr > 0)
			nr.data = panr;

		pos.data = papos;
		preisanfrage.update();
		preisanfrage.close();

		/**
		Hinweis: wenn es keine MySql DB ist, dann muß hier eine andere Lösung gefunden werden!
		*/
		if (panr <= 0)
		{
			string sql = "SELECT LAST_INSERT_ID()";
			CString res = preisanfrage.executeSQL(sql.c_str());
			
			panr = atoi(res);
		}
	}
	CATCH(CDBException, e)
	{
		e->ReportError();
		return 0;
	}
	END_CATCH

	return true;
}



bool DbFreigabe::freigabeWerkauftrag(WaxPart *pt, bool writeStl)
{
	Server serverdb(Server::save, pt->get_werkauftrag(), dbhandle);

	// Freigabeid setzen
	unsigned long waid =  getfreigabeid(pt);
	if (!waid)
	{
		CString str; str = getStammdaten().getText("IDS_KEIN_NEUER_WA").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONERROR);
		return false;
	}

	pt->get_werkauftrag()->set_waid(waid);
	pt->get_werkauftrag()->get_status().set_freigabecounter(werkauftrag.get_status().get_freigabecounter()+1);

	TRY
	{
		// Prüfung, Tabelle Status gelesen werden kann
		serverdb.set_modus(Server::load);
		if (!serverdb.testStatusTab())
		{
			AfxMessageBox(getStammdaten().getText("IDS_STATUS_NICHT").c_str(), MB_OK | MB_ICONERROR);
			return false;
		}
		serverdb.set_modus(Server::save);

		// Wenn die Werkauftragsnummer bereits vergeben wurde, darf auch keine Änderung
		// mehr möglich sein, es sei denn von seiten der AV
		if (werkauftrag.get_status().get_wanr() && !av) 
		{
			CString str; str = getStammdaten().getText("IDS_WERKAUFTRAG_NICHT").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			return false;
		}

		if (!serverdb.updateStatus(false))
			return false;

		if (!serverdb.updateLeitkarte(pt->GetRuntimeClass()->m_lpszClassName))
			return false;
	
		if (!serverdb.updateMaterial())
			return false;
		
		if (!serverdb.updateAnhang())
			return false;

		if (!serverdb.updateRohrleitung())
			return false;

		if (!serverdb.updateElemente())
			return false;

		if (!serverdb.updateObject())
			return false;
		
		if (!serverdb.updateSoko())
			return false;

		//Statistik in DB schreiben		
		if (writeStl)
		{
			WerkauftragListe listeWa;
			listeWa.append(pt->get_werkauftrag());

			BefehlUpdateStatistik befehl;
			befehl.set_werkauftraege(&listeWa);
			befehl.set_server(&serverdb);

			if (!befehl.start())
				return false;
		}
		
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH_ALL


	return true;
}

