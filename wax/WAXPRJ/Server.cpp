#include "stdafx.h"
#include "resource.h"


#include <mxodbc\mxodbc.h>
#include "Server.h"
#include "..\rohrkonst\EnStueckliste.h"
#include "..\rohrkonst\GpConvertOldVersion.h"
#include "..\rohrkonst\cadwerkauftrag.h"
#include "convcad2xml.h"

using namespace MxErrorHandling;

#define ASSERT_WAID \
XASSERT(werkauftrag->get_waid());\
if (!werkauftrag->get_waid())\
	return false;



//Konstruktor
Server::Server(enum MODE  modus, Werkauftrag * wa, DWORD  dbhandle  ) : DbBase( dbhandle)
{
	werkauftrag = wa;
	this->modus = modus; // Speichern oder Laden
	m_pFortschritt = NULL;
	sapWittenbergeAuftragsdaten = NULL;
}

//Konstruktor
Server::Server(enum MODE modus, Werkauftrag* wa, CString dbname, CString tabname) : DbBase(NULL)
{
	werkauftrag = wa;
	this->modus = modus; // Speichern oder Laden
	opentab = new MXTable(dbname, tabname);
	this->dbhandle = opentab->get_Db();
	m_pFortschritt = NULL;
}


//Defaultkonstruktor
Server::~Server()
{
	
}


void Server::set_modus(enum DbBase::MODE value)
{
	modus = value;
}

///Pruefung, ob Tabelle Status gelesen werden kann
bool Server::testStatusTab()
{
	MXTable wa(dbhandle, "status");
	
	DECLARE_COL(Long, waid, wa);
	DECLARE_COL(Int, wanr, wa);
	DECLARE_COL(Long, crc, wa);

	CString select;
	TRY
	{
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);
		XASSERT( wa.get_count() == 0 || wa.get_count() == 1); 
		if (wa.get_count() > 1 || wa.get_count() < 0) 
			AfxThrowNotSupportedException();

		int nr = 0;
		nr = wanr.data;
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	wa.close();
	return true;
}

//Zugriff auf die Status Tabelle der Serverdatenbank
bool Server::updateStatus(bool updateWaNr)
{
	/**
	Status schreiben oder lesen
	Statusdaten sind
		- waid: Zähler der Werkaufträge
		- wanr: Werkauftragsnummer
		- mbtermin: Materialbereitstellungstermin
		- sachbearbeiterSegment: Sachbearbeiter in der AV
		- storniert: wurde der Auftrag geloescht
		- freigabecounter: Anzahl der vorgenommenen Freigaben

	Fehler sind: waid nicht gesetzt, keiner oder mehr als ein Datensatz in der Tabelle
	enthalten. Letzteres ist ein schwerwiegender Fehler, der besonderer Behandlung bedarf.

	Wenn kein Eintrag in der Statustabelle vorliegt, wird ein neuer erzeugt

	Zur Initialisierung des MB_TERMINS: Wenn keine WA Nummer vergeben ist, wird der MBTermin auf 
	den Liefertermin - Anzahl Tage aus dem Parameter MBTERMIN gesetzt. 
	*/
	bool av = WaxSys::isAV();
	if (!av)
		updateWaNr = false; // Immer false für NL, bei AV bedingt true 
	MXTable wa(dbhandle, "status", true);
	
	DECLARE_COL(Long, waid, wa);
	DECLARE_COL(Int, wanr, wa);
	DECLARE_COL(String, sachbesegment, wa);
	DECLARE_COL(Date, mbtermin, wa);
	DECLARE_COL(Int, freigabecounter, wa);
	DECLARE_COL(Bool, storniert, wa);
	DECLARE_COL(String, storniert_von, wa);
	DECLARE_COL(Int, status, wa);
	
	CString select;

	ASSERT_WAID;
	
	Status &stat = werkauftrag->get_status();
	Leitkarte &leitkarte = werkauftrag->get_leitkarte();

	TRY
	{
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);
		XASSERT( wa.get_count() == 0 || wa.get_count() == 1); 
		if (wa.get_count() > 1 || wa.get_count() < 0) 
			AfxThrowNotSupportedException();

		// Wenn Status aktualisiert werden soll und der Eintrag existiert noch nicht, sollen keine Daten
		// im Werkauftrag geändert werden
		if (modus == load  && !wa.get_count())
		{
			return true;
		}
		if (modus == save)
		{
			if (wa.get_count() == 1)
			{
				wa.edit();
				status.data = (int)stat.get_status();
			}
			else
			{
				wa.addNew();
				*sachbesegment.data = "";
				wanr.data = 0;
				freigabecounter.data = 0;
				status.data = stat.get_status();
			}
			waid.data = werkauftrag->get_waid();

			if (!storniert.data && stat.get_storniert())
			{
				// Bei der ersten Stornierung den Usernamen setzen
				*storniert_von.data = WaxSys::get_username().c_str();
			}
			storniert.data = stat.get_storniert()? 1 : 0;

			if (stat.get_storniert())
				status.data = (int)Status::Storniert;
			
			
			if (av)
			{
				*sachbesegment.data = stat.get_sachbearbeiterSegment();
				mbtermin.set_datum(stat.get_mbtermin());
				wanr.data = stat.get_wanr();
			}
			freigabecounter.data = freigabecounter.data++;
			wa.update();
		}
		else
		{
			if (!wa.checkCrc())
				stat.setCRCError("Status", werkauftrag->get_bezeichnung());
			// In der AV wird jedem Werkauftrag erstmal eine WA Nummer verpasst
			// Der Sachbearbeiter wird auch gleich mit abgespeichert, damit (möglichst) immer einer
			// zuständig ist.
			if (updateWaNr && !wanr.data)
			{
				wa.edit();
				if(leitkarte.get_IsChoosenIppeNumbers())
				{
					wanr.data = getNewWaNr(true, leitkarte.get_ippeMatNr(), leitkarte.get_sdMaterialnummer(), leitkarte.get_sdAuftragsnr(), leitkarte.get_sdPositionsnr());
				}
				else
				{
					wanr.data = getNewWaNr(false, "", leitkarte.get_sdMaterialnummer(), leitkarte.get_sdAuftragsnr(), leitkarte.get_sdPositionsnr());
				}
				*sachbesegment.data = stat.get_sachbearbeiterSegment();
	
				status.data = (int)Status::InBearbeitungAv;


				// Initialisierung des MB_TERMINS
				int days = WaxSys::para_int(MBTERMIN);
				COleDateTimeSpan matbereit(days, 0, 0, 0);
				//mbtermin.set_datum(leitkarte.get_wunschliefertermin() - matbereit);
				mbtermin.set_datum(MXDate::GetCurrentTime() - matbereit);
				wa.update();
			}
			
			stat.set_sachbearbeiterSegment(*sachbesegment.data);
			stat.set_mbtermin(mbtermin.get_datum());
			stat.set_wanr(wanr.data);
			stat.set_storniert((storniert.data==0)?false:true);

			stat.set_status((Status::STATUS)status.data);
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	wa.close();
	return updateStatusDatum(updateWaNr);
}


//Zugriff auf die Status Tabelle der Serverdatenbank. Die Datumsfelder werden 
//hier explizit gelesen/gesetzt.
bool Server::updateStatusDatum(bool updateWaNr)
{
	/*
	Das Freigabedatum wird von der NL gesetzt und von der AV, wenn der WA in der AV erstellt wurde.
	Das Bearbeitungsdatum wird nur von der AV gesetzt
	*/
	bool av = WaxSys::isAV();
	
	MXTable wa(dbhandle, "status");
	
	DECLARE_COL(Long, waid, wa);
	DateCol bearbeitungsdatum("bearbeitungsdatum");
	DateCol freigabedatum("freigabedatum");
	
	bearbeitungsdatum.set_dateOnly(false);
	freigabedatum.set_dateOnly(false);

	wa.attach(&freigabedatum);
	wa.attach(&bearbeitungsdatum);

	
	CString select;
	
	ASSERT_WAID;

	Status &status = werkauftrag->get_status();

	TRY
	{
		MXDate now = MXDate::GetCurrentTime();
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);
		XASSERT(wa.get_count() == 1 || wa.get_count() == 0); 
		if (wa.get_count() > 1 || wa.get_count() < 0) 
			AfxThrowNotSupportedException();
		
		// Falls der Eintrag noch nicht existiert bei der Statusabfrage (beim ersten Freigeben passiert dies genau einmal)
		// Dieses überflüssige Abfragen sollte verhindert werden!
		if (!wa.get_count())
			return true;
		
		if (modus == save)
		{
			// Damit auch bei Freigabe ueber Bauabschnitt das Aenderungsdatum aktualisiert wird.
			werkauftrag->set_aenderungsdatum(now);
			wa.edit();
			if (av)
			{
				status.set_bearbeitungsdatum(now);
				bearbeitungsdatum.set_datum(now);
				
				// WA aus AV heraus muss auch ein Freigabedatum bekommen
				if (status.get_freigabedatum().m_dt <= 0.0)
				{
					status.set_freigabedatum(now);
				}
				freigabedatum.set_datum(status.get_freigabedatum());
			}
			else
			{
				status.set_freigabedatum(now);
				freigabedatum.set_datum(now);
			}
			wa.update();
		}
		else
		{
			if (updateWaNr)
			{
				wa.edit();
				status.set_bearbeitungsdatum(now);
				bearbeitungsdatum.set_datum(now);
				wa.update();
			}
			status.set_freigabedatum(freigabedatum.get_datum());
			status.set_bearbeitungsdatum(bearbeitungsdatum.get_datum());
			werkauftrag->set_aenderungsdatum(status.get_bearbeitungsdatum());
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	return true;
}


//Zugriff auf die AnhangTabelle der Serverdatenbank
bool Server::updateAnhang( )
{
	/*
	Anhang laden oder speichern
	Ist ein Anhang zum Werkauftrag vorhanden, werden die Daten in ein Blob Feld in der Tabelle Anhang gestellt. Die Grösse des Anhangs ist künstlich begrenzt. 
	Ausserdem kann bis jetzt nur ein Anhang definiert werden.
	Beim Laden wird die Tabelle 'Anhang' über die waid geöffnet und, wenn vorhanden, der binäre Datensatz
	geladen. 
	Fix: Wenn kein Datensatz vorhanden ist, gibt es eine Exception, weil CByteArray von dat auf
	Länge Null gesetzt wird. Dies passiert nur, wenn bei Mysql nicht das Flag 'Dont optimize column width'
	gesetzt ist (Systemsteuerung-ODBC ). Passiert bei Access nicht, da Access als Grösse immer 2GB zurückliefert,
	und dann die Länge auf maxlen_anhang gesetzt wird. Das Mysql Feature ist aber eigentlich ganz ok...			
	*/
	ASSERT_WAID;
	MXTable wa(dbhandle, "anhang");
	
	DECLARE_COL(Long, waid, wa);
	BlobCol dat("data", werkauftrag->get_anhang()->MAXLEN_ANHANG);
	DECLARE_COL(String, bezeichnung, wa);
	DECLARE_COL(String, filename, wa);
	wa.attach(&dat);

	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());

	TRY
	{
		if (modus == save)
		{
			dat.setData(werkauftrag->get_anhang()->get_data(), werkauftrag->get_anhang()->get_len());
			CString delanhang;
				
			deleteWerkauftragInTable(wa);
			openTable(wa);

			if (werkauftrag->get_anhang()->get_len() == 0)
				return true; // Kein Anhang

			wa.addNew();
			waid.data = werkauftrag->get_waid();
			dat.setData(werkauftrag->get_anhang()->get_data(), werkauftrag->get_anhang()->get_len());
			*bezeichnung.data = werkauftrag->get_anhang()->get_bezeichnung();
			*filename.data = werkauftrag->get_anhang()->get_filename();
			unsigned long t1 = ::GetTickCount();
			wa.update();
			unsigned long t2 = ::GetTickCount();
			TRACE("Anhang übertragen Grösse=%i, Zeit=%i msec\n", werkauftrag->get_anhang()->get_len(), t2-t1); 
		}
		else
		{

			// Fix: siehe oben
			if (wa.getCount(select) <= 0)
				return true;

			wa.load(select, (CString)"", true);
			
			if (!wa.get_count())
				return true; // Kein Anhang ist ok
			void *d;
			unsigned int len;
			dat.copyData(len, &d);
			werkauftrag->get_anhang()->set_data(d, len);
			werkauftrag->get_anhang()->set_bezeichnung(*bezeichnung.data);
			werkauftrag->get_anhang()->set_filename(*filename.data);
			wa.close();
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

//Zugriff auf die Material Tabelle der Serverdatenbank
bool Server::updateMaterial( )
{
	MXTable ma(dbhandle, "material", true);
	
	DECLARE_COL(Long, waid, ma);
	DECLARE_COL(String, posnr, ma);
	DECLARE_COL(Int, anzahl, ma);
	DECLARE_COL(Int, artikelnr, ma);
	DECLARE_COL(Double, preis, ma);
	DECLARE_COL(Double, pulverpreis, ma);
	DECLARE_COL(Double, gewicht, ma);
	DECLARE_COL(String, bezeichnung, ma);
	DECLARE_COL(Int, mass1, ma);
	DECLARE_COL(Int, mass2, ma);
	
	CString select;
	TRY
	{
		if (modus == save)
		{
			deleteWerkauftragInTable(ma);
			openTable(ma);

			for (POSITION pos = werkauftrag->get_material().get_matlist().GetHeadPosition(); pos;)
			{
				Material *mat;
				mat = (Material *)werkauftrag->get_material().get_matlist().GetNext(pos);
				XASSERT(mat);

				if (mat)
				{ 
					ma.addNew();
					waid.data = werkauftrag->get_waid();
					*posnr.data = mat->get_posnr();
					anzahl.data = mat->get_anzahl();
					artikelnr.data = mat->get_artikelnr_ext(); // In den oberen Bits von artikelnr ist eine Kennung für Profile enthalten
					preis.data = mat->get_sapPreis();
					pulverpreis.data = mat->get_pulverpreis();
					gewicht.data = mat->get_gewicht();
					*bezeichnung.data = mat->get_bezeichnung();
					mass1.data = mat->get_mass1();
					mass2.data = mat->get_mass2();
					ma.update();
				}
			}
		}
		else
		{
			select.Format("waid = %i", werkauftrag->get_waid());
			ma.load((LPCSTR)select);
			while (!ma.isEof())
			{
				Material *mat = new Material;
				werkauftrag->get_material().get_matlist().AddTail(mat);
				mat->set_id(waid.data);
				mat->set_posnr(*posnr.data);
				mat->set_anzahl( anzahl.data);
				mat->set_artikelnr(artikelnr.data);
				// Wegen Halbzeug, Einkauf u. Gemeinkostenindex
				mat->initStammdaten();
				mat->set_sapPreis(preis.data);
				mat->set_pulverpreis(pulverpreis.data);
				mat->set_gewicht(gewicht.data);
				mat->set_bezeichnung(*bezeichnung.data);
				mat->set_mass1(mass1.data);
				mat->set_mass2(mass2.data);
				if (!ma.checkCrc())
					werkauftrag->get_status().setCRCError("Material", *bezeichnung.data);
				ma.moveNext();
			}
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

//Testet auf Connect zur Serverdatenbank
CString Server::checkServerDB()
{
	//Testet auf Connect zur Serverdatenbank
	MXTable testserver(WaxSys::getServerdb(), "werkauftrag");
	CString sql = "select max(waid) from " + testserver.get_tableName();

	CString res = testserver.executeSQL(sql);
	if (atoi(res) <= 0)
	{
		AfxMessageBox(getStammdaten().getText("IDS_NOWAXSERVER").c_str(), MB_OK);
		return "";
	}
	return WaxSys::getServerdb();
}



//Zugriff auf die Arbeit Tabelle der Serverdatenbank
bool Server::updateArbeit(const CString& tabname, CObList* liste)
{
	MXTable arbeit(dbhandle, (LPCSTR)tabname, true);

	DECLARE_COL(Long,	waid,	arbeit);
	DECLARE_COL(Long,	posnr,	arbeit);
	DECLARE_COL(Double, anzahl, arbeit);
	DECLARE_COL(String, txt,	arbeit);
	DECLARE_COL(Int,	kostst, arbeit);
	DECLARE_COL(Int,	arbpl,	arbeit);		//aus Kompatibilitätsgründen wird dieses Feld erstmal weiter gefüllt
	DECLARE_COL(String,	arbeitsplatz, arbeit);	
	DECLARE_COL(Double, preis,	arbeit);
	DECLARE_COL(Double, festpreis,	arbeit);
	DECLARE_COL(Double, zeit,	arbeit);
	DECLARE_COL(Double, ruestzeit, arbeit);
	DECLARE_COL(Int,	id,		arbeit);
	DECLARE_COL(Bool,	akkord, arbeit);
	DECLARE_COL(Bool,	sap,	arbeit);
	DECLARE_COL(Bool,   isTextGeaendert,arbeit);


	CString select;
	TRY
	{
		if (modus == save)
		{
			deleteWerkauftragInTable(arbeit);
			openTable(arbeit);

			for (POSITION pos = liste->GetHeadPosition(); pos;)
			{
				Arbeitsfolge *arb = dynamic_cast<Arbeitsfolge *>(liste->GetNext(pos));
				XASSERT(arb);

				if (arb)
				{ 
					arbeit.addNew();
					waid.data = werkauftrag->get_waid();
					id.data = arb->get_id();
					posnr.data = arb->get_nr();
					anzahl.data = arb->get_anz();
					*txt.data = arb->get_txt().GetLength() ? arb->get_txt() : "-";
					kostst.data = arb->get_kostst();
					arbpl.data = 0; //ab 15.12.2007 auf String Feld geändert, wegen SAP
					*arbeitsplatz.data = arb->get_arbpl();
					preis.data = arb->get_ppstd();
					festpreis.data = arb->get_preis();
					zeit.data = arb->get_zeit();
					ruestzeit.data = arb->get_ruestzeit();
					akkord.data = 0;
					sap.data = arb->get_sap();
					isTextGeaendert.data = arb->get_isTextGeandert();
					arbeit.update();
				}
			}
		}
		else
		{
			select.Format("waid = %i", werkauftrag->get_waid());
			arbeit.load((LPCSTR)select, (LPCSTR)"posnr");
			while (!arbeit.isEof())
			{
				Arbeitsfolge *arb = new Arbeitsfolge;
				liste->AddTail(arb);
				arb->set_id(id.data);
				arb->set_nr(posnr.data);
				arb->set_anz(anzahl.data);
				arb->set_txt(*txt.data);
				arb->set_kostst(kostst.data);

				//bei alten Projekten wird der Arbeitsplatz aus dem Int-Wert gelesen
				if (arbeitsplatz.data->IsEmpty())
				{
					CString txt;
					txt.Format("%i",arbpl.data);
					arb->set_arbpl(txt);
				}
				else
					arb->set_arbpl(*arbeitsplatz.data);
							
				arb->set_ppstd(preis.data);
				arb->set_preis(festpreis.data);
				arb->set_zeit(zeit.data);
				arb->set_ruestzeit(ruestzeit.data);
				arb->set_sap(sap.data ? true : false);
				arb->set_isTextGeandert(isTextGeaendert.data ? true : false);
				if (!arbeit.checkCrc())
					werkauftrag->get_status().setCRCError("Arbeitsfolgen", *txt.data);
				arbeit.moveNext();
			}
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

void Server::set_werkauftrag(Werkauftrag* value)
{
	werkauftrag = value;
}

//Zugriff auf die Werkauftrag Tabelle der Serverdatenbank mit den 
//Informationen der Leitkarte, den Objektinformationen,  dem Bemerkungsfeld 
//und der Beschichtung.
bool Server::updateLeitkarte(CString classID)
{
	/*
	Achtung: Bei Änderungen/Hinzufügen/Löschen von Attributen aus der Leitkarte muss nicht nur hier sondern
	auch ServerDBOpen::createSammelauftrag aktualisiert werden.
	to do: Nur eine Funktion für diese Aktion
	*/
	Leitkarte& leitkarte = werkauftrag->get_leitkarte();

	TRY
	{
		MXTable wa(dbhandle, "werkauftrag", true);

		DECLARE_COL(Long, waid, wa);
		DECLARE_COL(String, classid, wa);
		DECLARE_COL(String, bezeichnung, wa);
		DECLARE_COL(String, auftragsbez, wa);
		DECLARE_COL(String, bauAbschnitt, wa);
		DECLARE_COL(String, bauort, wa);
		DECLARE_COL(String, bauplz, wa);
		DECLARE_COL(Int, nl, wa);
		DECLARE_COL(String, sachbearbeiternl, wa);
		DECLARE_COL(String, emailnl, wa);
		DECLARE_COL(String, pspelement, wa);
		DECLARE_COL(String, coauftrag, wa);
		DECLARE_COL(BigInt, sdauftragsnr, wa);
		DECLARE_COL(Long, sdmaterialnr, wa);
		DECLARE_COL(Long, sdpositionsnr, wa);

		DateCol erstellungsdatum("erstellungsdatum");
		erstellungsdatum.set_dateOnly(false);
		wa.attach(&erstellungsdatum);
		
		DECLARE_COL(Date, wunschliefertermin, wa);
		DECLARE_COL(Int, beschtyp, wa);
		DECLARE_COL(Int, beschral, wa);
		DECLARE_COL(String, beschbez, wa);
		DECLARE_COL(Bool, imSichtbereich, wa);
		DECLARE_COL(Int, spezialloesch, wa);
		DECLARE_COL(Int, vk3, wa);
		DECLARE_COL(String, posnr, wa);
		DECLARE_COL(Int, anzahl, wa);

		BlobCol bemerkung("bemerkung", 4000);
		wa.attach(&bemerkung);
		DECLARE_COL(Bool, zeugnis, wa);
		DECLARE_COL(Int, kolliliste, wa);
		DECLARE_COL(Int, fertigungsstaette, wa);
		DECLARE_COL(Int, mindermenge, wa);
		DECLARE_COL(Bool, vk3abrechnung, wa);
		DECLARE_COL(BigInt, kcauftragsnr, wa);
		DECLARE_COL(Long, kcposnr, wa);
		DECLARE_COL(Bool, cadpos, wa);

		DECLARE_COL(Int, preisanfrageNr, wa);
		DECLARE_COL(Int, preisanfragePos, wa);
		DECLARE_COL(Bool, fertigungMd, wa);

		DECLARE_COL(String, ippeMatNr, wa)
		DECLARE_COL(String, ippeKnotenNr, wa)
		DECLARE_COL(Bool, isNextGeneration, wa)
		DECLARE_COL(Int, zulassungsstelle, wa)


		CString select;
			
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);
		if (wa.get_count() != 1)
		{
			CString str; str = getStammdaten().getText("IDS_INTERNER_FEHLER").c_str();
			AfxMessageBox(str);
			return false;
		}

		if (modus == save)
		{
			wa.edit();
			
			// Projektleitkartendaten werden für jeden Werkauftrag einzeln übertragen, damit in 
			// der AV die einzelnen Werkaufträge auch einzeln ihre Projektdaten anzeigen können.
			*classid.data = classID;
			
			*bauAbschnitt.data  = leitkarte.get_prj_bauAbschnitt();
			*auftragsbez.data = leitkarte.get_prj_auftragsbez();
			*bauort.data  = leitkarte.get_prj_bauOrt();
			*bauplz.data  = leitkarte.get_prj_bauPlz();
			nl.data  = leitkarte.get_prj_nl();

			*bezeichnung.data = werkauftrag->get_bezeichnung();
			*sachbearbeiternl.data = leitkarte.get_sachbearbeiterNL();
			zulassungsstelle.data = leitkarte.get_zulassungsnummer();
			*emailnl.data = leitkarte.get_emailNl();
			erstellungsdatum.set_datum(werkauftrag->get_erstellungsdatum());
			if(WaxSys::isAV())
			{
				wunschliefertermin.set_datum(leitkarte.get_wunschliefertermin());
			}
			else
			{
				wunschliefertermin.data = 0;
			}
			
			if(leitkarte.get_IsChoosenIppeNumbers())
			{
				spezialloesch.data = leitkarte.get_Brandschutztechnik();
			}
			else
			{
				spezialloesch.data = leitkarte.get_prj_produktlinie();
			}


			vk3.data = leitkarte.get_vk3();

			preisanfrageNr.data = leitkarte.get_preisanfrage().get_nummer();
			preisanfragePos.data = leitkarte.get_preisanfrage().get_position();
				

			beschtyp.data = werkauftrag->get_beschichtung().get_typ() | (werkauftrag->get_beschichtung().get_verzinkerei() << 8);

			beschral.data = werkauftrag->get_beschichtung().get_ral();
			*beschbez.data = werkauftrag->get_beschichtung().get_bezeichnung();
			
			*pspelement.data  = leitkarte.get_prj_pspElement();
			*coauftrag.data = leitkarte.get_prj_COauftrag();
			sdauftragsnr.data  = leitkarte.get_sdAuftragsnr();
			sdmaterialnr.data  = leitkarte.get_sdMaterialnummer();
			sdpositionsnr.data  = leitkarte.get_sdPositionsnr();

			kcauftragsnr.data = _atoi64(leitkarte.get_kcAuftragsnr());
			kcposnr.data = leitkarte.get_kcPositionsnr();
						
			imSichtbereich.data = werkauftrag->get_sichtbereich();
			zeugnis.data = werkauftrag->get_zeugnis();
			if (WaxSys::isAV())
			{
				kolliliste.data = werkauftrag->get_KollilisteExport();
			}
			else
			{
				kolliliste.data = 0;
			}
			fertigungsstaette.data = werkauftrag->Get_Fertigungsstaette();
			mindermenge.data = werkauftrag->get_mindermenge();
			*posnr.data = werkauftrag->get_posNr();
			anzahl.data = werkauftrag->get_anzahl();
			
			if (!werkauftrag->get_bemerkung().GetLength())// sonst Schutzverletzung bei 0 Byte !!!
			{
				werkauftrag->set_bemerkung("-");
			}
			bemerkung.setData(werkauftrag->get_bemerkung(), werkauftrag->get_bemerkung().GetLength());
			
			vk3abrechnung.data = werkauftrag->get_preis().get_vk3flag();
			cadpos.data = werkauftrag->get_cadpos();
			fertigungMd.data = true;

			*ippeMatNr.data  = leitkarte.get_ippeMatNr();
			*ippeKnotenNr.data = leitkarte.get_ippeKnotenNr();
			isNextGeneration.data = leitkarte.get_IsChoosenIppeNumbers();
	
			wa.update();
		}
		else
		{
			// Ein Update der Projektleitkartendaten betrifft das ganze Projekt, wenn eine Projektleitkarte
			// definiert ist. Dies ist bei NL Projekten immer der Fall. Es muss deshalb nach einem Update
			// in der NL Version eine Änderung dieser Daten abgefragt werden.
			leitkarte.set_prj_bauAbschnitt(*bauAbschnitt.data);
			leitkarte.set_prj_auftragsbez(*auftragsbez.data);
			leitkarte.set_prj_bauOrt(*bauort.data);
			leitkarte.set_prj_bauPlz(*bauplz.data);
			leitkarte.set_prj_nl(nl.data);
			/*
			if(isNextGeneration.data == 1)
			{
				leitkarte.set_Brandschutztechnik(spezialloesch.data);
			}
			else
			{
				leitkarte.set_prj_produktlinie(spezialloesch.data);
			}
*/
			if (*bezeichnung.data != werkauftrag->get_stdbezeichnung())
			{
				werkauftrag->set_bezeichnung(*bezeichnung.data);
			}
			leitkarte.set_sachbearbeiterNL(*sachbearbeiternl.data);
			leitkarte.set_zulassungsnummer(zulassungsstelle.data);
			leitkarte.set_emailNl(*emailnl.data);
			werkauftrag->set_erstellungsdatum(erstellungsdatum.get_datum());
			leitkarte.set_wunschliefertermin(wunschliefertermin.get_datum());
			leitkarte.set_vk3(vk3.data ? true : false);

			werkauftrag->get_beschichtung().set_typ(beschtyp.data & 0x0f);
			werkauftrag->get_beschichtung().set_verzinkerei(beschtyp.data >> 8);
			werkauftrag->get_beschichtung().set_ral(beschral.data);
			werkauftrag->get_beschichtung().set_bezeichnung(*beschbez.data);
			werkauftrag->set_posNr(*posnr.data);
			werkauftrag->set_anzahl(anzahl.data);
			
			leitkarte.set_prj_pspElement(*pspelement.data);
			leitkarte.set_prj_COauftrag(*coauftrag.data);
			leitkarte.set_sdAuftragsnr(sdauftragsnr.data);
			leitkarte.set_sdMaterialnummer(sdmaterialnr.data);

			
			
			leitkarte.get_preisanfrage().set_nummer(preisanfrageNr.data);
			leitkarte.get_preisanfrage().set_position(preisanfragePos.data);
			
			werkauftrag->set_sichtbereich(imSichtbereich.data);
			werkauftrag->set_zeugnis(zeugnis.data);
			werkauftrag->set_KollilisteExport(kolliliste.data);
			werkauftrag->Set_Fertigungsstaette(fertigungsstaette.data);
			werkauftrag->set_mindermenge(mindermenge.data ? true : false);
			werkauftrag->set_bemerkung("-");
			werkauftrag->set_bemerkung(bemerkung.getString());
			werkauftrag->get_preis().set_vk3flag(vk3abrechnung.data ? true : false); 
			werkauftrag->set_cadpos(cadpos.data ? true : false);
			
			
			leitkarte.set_ippeMatNr(*ippeMatNr.data);
			leitkarte.set_ippeKnotenNr(*ippeKnotenNr.data);
			leitkarte.set_IsChoosenIppeNumbers(isNextGeneration.data ? true : false);
			
			if(werkauftrag->get_GUID().IsEmpty())
			{
				leitkarte.set_sdPositionsnr(sdpositionsnr.data);
				CString tmp; 
				tmp.Format("%I64d",kcauftragsnr.data);
				leitkarte.set_kcAuftragsnr(tmp);
				leitkarte.set_kcPositionsnr(kcposnr.data);
				leitkarte.set_prj_produktlinie(spezialloesch.data);
				leitkarte.set_prj_pspElement(*pspelement.data);
			}
			else
			{
				CString kcAuftrag;
				int kcPos = 0;
				CString psp;
				int nllocal = 0, pl = 0;
				CString strIppeMatNr; 
				COleDateTime dtWunschtermin;
				BOOL bFound = FALSE;
				int sdPos = 0;
						
				if(sapWittenbergeAuftragsdaten != NULL)
				{
					bFound = sapWittenbergeAuftragsdaten->getKcNummer( werkauftrag->get_GUID(), sdPos, kcAuftrag, kcPos, psp, nllocal, pl, dtWunschtermin);
					if( bFound == TRUE)
					{
						leitkarte.set_sdPositionsnr(sdPos);
						leitkarte.set_kcAuftragsnr(kcAuftrag);
						leitkarte.set_kcPositionsnr(kcPos);	
						leitkarte.set_prj_produktlinie(pl);
						leitkarte.set_prj_pspElement(psp);
						leitkarte.set_wunschliefertermin(dtWunschtermin);
					}
					else
					{
						leitkarte.set_sdPositionsnr(sdpositionsnr.data);
					}
				}
			}

			if (!wa.checkCrc())
				werkauftrag->get_status().setCRCError("Werkauftrag");
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

//Zugriff auf die rohrleitung Tabelle der Serverdatenbank
bool Server::updateRohrleitung( )
{
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	if (!rohr)
		return true;
	MXTable rohrtab(dbhandle, "rohrleitung", true);

	DECLARE_COL(Int, waid, rohrtab);
	DECLARE_COL(Int, anzahl, rohrtab);
	DECLARE_COL(Int, dnvl, rohrtab);
	DECLARE_COL(Int, laenge, rohrtab);
	DECLARE_COL(Int, nrohre, rohrtab);
	DECLARE_COL(Int, rohrquali, rohrtab);
	DECLARE_COL(Int, rohrverb, rohrtab);
	DECLARE_COL(Int, rohranfang, rohrtab);
	DECLARE_COL(Int, rohrende, rohrtab);
	DECLARE_COL(Int, rohrkfarbe, rohrtab);
	DECLARE_COL(Int, rohrdruckstufe, rohrtab);
	DECLARE_COL(Int, avnachbearbeitung, rohrtab);
	DECLARE_COL(Int, schnittlaenge, rohrtab);
	DECLARE_COL(Int, ueberlaenge, rohrtab);
	DECLARE_COL(Bool, IsApaxBauteil, rohrtab);


	BlobCol fertigungshinweis("fertigungshinweis", 4000);
	rohrtab.attach(&fertigungshinweis);
	

	TRY
	{
		if (modus == save)
		{
			
			deleteWerkauftragInTable(rohrtab);
			openTable(rohrtab);

			rohrtab.addNew();
			waid.data = werkauftrag->get_waid();
			dnvl.data = rohr->get_dnvl();
			laenge.data = rohr->get_laenge();
			anzahl.data = rohr->get_anzahl();
			nrohre.data = rohr->get_nRohre();
			rohrquali.data = rohr->get_rohrQuali();
			rohrverb.data = rohr->get_rohrVerb();
			rohranfang.data = rohr->get_rohrAnfang();
			rohrende.data = rohr->get_rohrEnde();
			rohrkfarbe.data = rohr->get_rohrKFarbe();
			rohrdruckstufe.data = rohr->get_rohrDruckstufe();
			avnachbearbeitung.data = rohr->get_avnachbearbeitung_per_zeichnung();
			
			schnittlaenge.data  = rohr->get_schnittlaenge();
			ueberlaenge.data	= rohr->get_Ueberlaenge();
			IsApaxBauteil.data  = rohr->get_IsApaxBauteil(); 
			// sonst Schutzverletzung bei 0 Byte !!!
			CString fh = rohr->get_fertigungshinweis().GetLength() ? rohr->get_fertigungshinweis() : "-";
			fertigungshinweis.setData(fh, fh.GetLength());
			
			rohrtab.update();
		}
		else
		{
			CString select;
			select.Format("waid = %i", werkauftrag->get_waid());

			rohrtab.load((LPCSTR)select, (LPCSTR)"", true);
			if (rohrtab.get_count())
			{
				XASSERT(rohrtab.get_count() == 1); // Es muss genau ein Eintrag vorhanden sein
				
				if (!rohrtab.get_count())
					return false;
				rohr->set_dnvl(dnvl.data);
				rohr->set_laenge(laenge.data);
				rohr->set_anzahl(anzahl.data);
				rohr->set_nRohre(nrohre.data);
				rohr->set_rohrQuali(rohrquali.data);
				
				//Automatisch alte FlanschId konvertieren
				rohr->set_rohrVerb(GpConvertOldVersion::getNewFlanschId(rohrverb.data,dnvl.data));
				rohr->set_rohrAnfang(GpConvertOldVersion::getNewFlanschId(rohranfang.data,dnvl.data));
				rohr->set_rohrEnde(GpConvertOldVersion::getNewFlanschId(rohrende.data,dnvl.data));

				rohr->set_rohrKFarbe(rohrkfarbe.data);
				rohr->set_rohrDruckstufe(rohrdruckstufe.data);
				rohr->set_avnachbearbeitung_per_zeichnung(avnachbearbeitung.data ? true : false);
				rohr->set_fertigungshinweis("-");
				rohr->set_fertigungshinweis(fertigungshinweis.getString());
				rohr->set_schnittlaenge((schnittlaenge.data > 0) ? schnittlaenge.data : 6000);
				rohr->set_Ueberlaenge(ueberlaenge.data ? true:false);
				rohr->set_IsApaxBauteil( IsApaxBauteil.data ? true:false); 
			

				if (!rohrtab.checkCrc())
					werkauftrag->get_status().setCRCError("Rohrleitung");
			}
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


bool Server::updatePreisanfrage()
{

	MXTable patab(dbhandle, "preisanfrage", false);

	DECLARE_COL(Int, nr, patab);
	DECLARE_COL(Int, pos, patab);
	DECLARE_COL(Double, preis, patab);
	DECLARE_COL(Date, gueltigBis, patab);
	DECLARE_COL(String, info, patab);
	

	TRY
	{
		if (modus == save)
		{
			//Datensatz laden
			CString sql;
			sql.Format("nr=%i and pos=%i", werkauftrag->get_leitkarte().get_preisanfrage().get_nummer(), werkauftrag->get_leitkarte().get_preisanfrage().get_position());			
			patab.load(sql, 0, false);

			//int test = patab.get_count();

			if (patab.isEof()) 
				patab.addNew();
			else
				patab.edit();

			nr.data = werkauftrag->get_leitkarte().get_preisanfrage().get_nummer();
			pos.data = werkauftrag->get_leitkarte().get_preisanfrage().get_position();
			preis.data = werkauftrag->get_leitkarte().get_preisanfrage().get_preis();
			gueltigBis.set_datum(werkauftrag->get_leitkarte().get_preisanfrage().get_gueltigBis());
			*info.data = werkauftrag->get_leitkarte().get_preisanfrage().get_info().c_str();
			
			patab.update();
		}
		else
		{
			CString select;
			select.Format("nr=%i and pos=%i", werkauftrag->get_leitkarte().get_preisanfrage().get_nummer(), werkauftrag->get_leitkarte().get_preisanfrage().get_position());

			patab.load((LPCSTR)select, (LPCSTR)"", true);
			if (patab.get_count())
			{
				XASSERT(patab.get_count() == 1); // Es muss genau ein Eintrag vorhanden sein
				if (!patab.get_count())
					return false;
				
				werkauftrag->get_leitkarte().get_preisanfrage().set_preis(preis.data);	
				werkauftrag->get_leitkarte().get_preisanfrage().set_gueltigBis(gueltigBis.get_datum());
				std::string tmp = *info.data;
				werkauftrag->get_leitkarte().get_preisanfrage().set_info(tmp);
			}
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



///Zugriff auf die Tabelle Stückliste für die Statistik
bool Server::updateStuecklisteWerkauftraege(WerkauftragListe *liste)
{
	if (!liste)
		return false;

	CString			stMeldung;

	EnStueckliste *pListeStl;

	int				waidFromEl;


	//Tabelle definieren
	MXTable stueckliste(dbhandle, "stuecklistewa", true);
	
	DECLARE_COL(Int,	waid,			stueckliste);
	DECLARE_COL(Int,	elemid,			stueckliste);
	DECLARE_COL(Double,	anzahllaenge,	stueckliste);
	DECLARE_COL(Int,	artikelnr,		stueckliste);
	DECLARE_COL(Int,	dn,				stueckliste);
	DECLARE_COL(Int,	anschlussdn,	stueckliste);
	DECLARE_COL(Double, stueckpreis,	stueckliste);
	DECLARE_COL(String,	bez,			stueckliste);


	long lAnzahl = liste->getSize();

	//Fortschrittsanzeige ínitialisieren
	if (m_pFortschritt)
	{
		CString st; st = getStammdaten().getText("IDS_WRITE_STL").c_str();
		m_pFortschritt->set_maxWert(lAnzahl);
		m_pFortschritt->set_minWert(0);
		m_pFortschritt->set_ueberschrift(st);
		m_pFortschritt->start();
	}


	if (modus == save)
	{

		//Tabelle öffnen
		openTable(stueckliste);


		MxIterator iter(*liste);
		while (iter.next())
		{			
			Werkauftrag* pWerkauftrag = dynamic_cast<Werkauftrag*>(liste->getAt(iter));
			if (!pWerkauftrag)
				continue;

			waidFromEl = pWerkauftrag->get_waid();

			//Fortschritt
			if (m_pFortschritt)
			{
				int iProzent = m_pFortschritt->nextStep(iter.getIndex());

				stMeldung.Format("%d%s, Datensatz %d von %d, (WaID = %d)",iProzent,"%",iter.getIndex(),lAnzahl,waidFromEl);
				m_pFortschritt->setAnzeige1(stMeldung);
			}
			
			if (waidFromEl <= 0)
				continue;


			TRY
			{
				//Werkauftrag setzen, damit mit deleteWerkauftragInTable die Datensätze zur wanr gelöscht werden können
				set_werkauftrag(pWerkauftrag);
				
				deleteWerkauftragInTable(stueckliste);

				//Stueckliste von Werkauftrag. Es muß eine Stueckliste vorhanden sein
				pListeStl = liste->getStuecklisteFromElement(iter);
				if (!pListeStl)
					continue;
				
				if (pListeStl->isEmpty())
					continue;
				
				//Stückliste des Werkauftrages abarbeiten
				MxCObListIterator iterStl(*pListeStl);
				while (iterStl.next())
				{
					stueckliste.addNew();
					waid.data				= waidFromEl;
					elemid.data				= pListeStl->getIdFromElement(iterStl);
					anzahllaenge.data		= pListeStl->getMengeLaengeFromElement(iterStl);
					artikelnr.data			= pListeStl->getArtikelNrFromElement(iterStl);
					stueckpreis.data		= pListeStl->getPreisFromElement(iterStl);
					dn.data					= pListeStl->getDnFromElement(iterStl);
					anschlussdn.data        = pListeStl->getDnRohrFromElement(iterStl);
					*bez.data				= pListeStl->getBezeichnungFromElement(iterStl).Left(100);
					stueckliste.update();
				}
			}
			CATCH_ALL(e)
			{
				//e->ReportError();
				TCHAR   szCause[255];
				e->GetErrorMessage(szCause,255);
				MxError error;
				error.setErrorDescription(szCause, MxError::Critical);

				appendError(error);
			}
			END_CATCH_ALL

		}	
	}

	if (m_pFortschritt)
		m_pFortschritt->ende();

	return true;
}





//Zugriff auf die Elemente Tabelle der Serverdatenbank
bool Server::updateElemente( )
{
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	if (!rohr)
		return true;

	return updateElementTab(rohr->getTabname(), rohr->getElementArray());
}

//Speichert eine Elementliste entweder in der avelement (Strangrohr, 
//Stationsverteiler)Tabelle oder der element Tabelle (Verteiler). Es sind 
//zwei Tabellen gleicher Struktur vorhanden, damit sich die Daten besser 
//verteilen.
bool Server::updateElementTab(const CString& tabname, EnContainerRohrElemente& elemlist)
{
	MXTable elemente(dbhandle, (LPCSTR)tabname, true);
	
	DECLARE_COL(Int, waid, elemente);
	DECLARE_COL(Int, rohrid, elemente);
	DECLARE_COL(Int, anzahl, elemente);
	DECLARE_COL(Int, artikelnr, elemente);
	DECLARE_COL(Double, preis, elemente);
	DECLARE_COL(Double, gewicht, elemente);
	DECLARE_COL(Double, durchmesser, elemente);
	DECLARE_COL(String, bezeichnung, elemente);
	DECLARE_COL(Int, id, elemente);
	DECLARE_COL(Int, mass1, elemente);
	DECLARE_COL(Int, mass2, elemente);
	DECLARE_COL(Int, dn, elemente);
	DECLARE_COL(Int, anschlussdn, elemente);
	DECLARE_COL(Int, laenge, elemente);
	DECLARE_COL(Int, abstabs, elemente);
	DECLARE_COL(Int, abstrel, elemente);
	DECLARE_COL(Int, winkel, elemente);
	DECLARE_COL(Int, pos, elemente);
	DECLARE_COL(Int, verkettung, elemente);
	DECLARE_COL(Int, verkettungsabstand, elemente);
	DECLARE_COL(Bool, makeanlage, elemente);
	//Fitting
	DECLARE_COL(Int, nurFitting, elemente);
	DECLARE_COL(Int, zmass, elemente);
	DECLARE_COL(Int, zmass2, elemente);
	DECLARE_COL(Int, zmass3, elemente);
	DECLARE_COL(Int, kopfstueck, elemente);
	DECLARE_COL(String, kurztext, elemente);
	DECLARE_COL(Bool, changeDN, elemente);
	
	TRY
	{
		if (modus == save)
		{
			deleteWerkauftragInTable(elemente);
			openTable(elemente);
			for (int i = 0; i < elemlist.GetSize(); i++)
			{
				Element *ele = elemlist.get_element_at(i);
				if (ele)
				{
					elemente.addNew();
					waid.data = werkauftrag->get_waid();
					rohrid.data = ele->get_id();
					anzahl.data = ele->get_anzahl();
					artikelnr.data = ele->get_artikelnr();
					preis.data = ele->get_festPreis();
					gewicht.data = ele->get_gewicht();
					durchmesser.data = ele->get_durchmesser();
					*bezeichnung.data = ele->get_bezeichnung();
					id.data = ele->get_id();
					mass1.data = ele->get_mass1();
					mass2.data = ele->get_mass2();
					dn.data = ele->get_dn();							
					anschlussdn.data = ele->get_anschlussDN();
					laenge.data = ele->get_laenge();
					abstabs.data = ele->get_abstAbs();
					abstrel.data = ele->get_abstRel();
					winkel.data = ele->get_winkel();
					pos.data = ele->get_pos();
					verkettung.data = ele->get_verkettung();
					verkettungsabstand.data = ele->get_verkettungAbst();
					makeanlage.data = ele->get_makeAnlage();
					//Fitting
					nurFitting.data = ele->get_nurFitting() ? 1 : 0;
					zmass.data = ele->get_zmass();
					zmass2.data = ele->get_zmass2();
					zmass3.data = ele->get_zmass3();
					kopfstueck.data = ele->get_kopfstueck();
					*kurztext.data = ele->get_kurztext();
					changeDN.data = ele->get_changeDN();

					elemente.update();
				}
			}	
		}
		else
		{
			CString select;
			select.Format("waid = %i", werkauftrag->get_waid());
			elemente.load((LPCSTR)select, (LPCSTR)"counter", true);
			//elemente.load((LPCSTR)select);
			if (!elemente.get_count())
				return false;
			
			elemlist.deleteAll();
			
			for(;!elemente.isEof(); elemente.moveNext())
			{
				Element *ele = new Element;
				ele->set_id(rohrid.data);
				ele->set_anzahl(anzahl.data);
				ele->set_artikelnr(artikelnr.data);
				ele->set_festPreis(preis.data);
				ele->set_gewicht(gewicht.data);
				ele->set_durchmesser(durchmesser.data);
				ele->set_bezeichnung(*bezeichnung.data);

				//Automatisch alte FlanschId konvertieren
				ele->set_id(GpConvertOldVersion::getNewFlanschId(id.data,dn.data));

				ele->set_mass1(mass1.data);
				ele->set_mass2(mass2.data);
				ele->set_dn(dn.data);
				ele->set_anschlussDN(anschlussdn.data);
				ele->set_laenge(laenge.data);
				ele->set_abstAbs(abstabs.data);
				ele->set_abstRel(abstrel.data);
				ele->set_winkel(winkel.data);
				ele->set_pos(pos.data);
				ele->set_verkettung(verkettung.data);
				ele->set_verkettungAbst(verkettungsabstand.data);
				ele->set_makeAnlage((makeanlage.data==1)?true:false);
				//Fitting
				ele->set_nurFitting(nurFitting.data ? true : false);
				ele->set_zmass(zmass.data);
				ele->set_zmass2(zmass2.data);
				ele->set_zmass3(zmass3.data);
				ele->set_kopfstueck(kopfstueck.data);
				ele->set_kurztext(*kurztext.data);
				ele->set_changeDN((changeDN.data==1)?true:false);

				elemlist.Add(ele);
				if (!elemente.checkCrc())
					werkauftrag->get_status().setCRCError(tabname, *bezeichnung.data);
			}	
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

/**
Zugriff auf die objektspezifischen Tabellen der Serverdatenbank, z.B. für 
Verteiler die Verteilertabelle, für Strangrohr die Strangrohtabell usw.
*//*
Änderung: 19.05.2006: Tabelle stueckliste und stuecklistesap werden nicht mehr gefüllt
*/
bool Server::updateObject()
{
	if (WaxSys::isAV())
	{
		/**
		Nur die AV benötigt die Daten aus der Tabelle arbeit, in der NL Version werden 
		die Afos automatisch generiert.
		*/
		if (!updateArbeit("arbeit", &werkauftrag->get_arbeit().get_arbeitsfolgen()))
			return false;
	}

	if (dynamic_cast<Gewinderohr*>(werkauftrag))
		return updateGewinderohr();
	else if (dynamic_cast<Strangrohr*>(werkauftrag))
		return updateStrangrohr();
	else if (dynamic_cast<Stationsverteiler*>(werkauftrag))
		return updateStationsverteiler();
	else if (dynamic_cast<CMx2001230Verteiler*>(werkauftrag))
		return updateCOIIMX1230200Verteiler();
	else if (dynamic_cast<COIIVerteiler*>(werkauftrag))
		return updateCOIIMX1230200Verteiler();
	else if (dynamic_cast<EntnahmeleitungDWB*>(werkauftrag))
		return updateEntnahmeleitung();
	else if (dynamic_cast<Pumpenprobierleitung*>(werkauftrag))
		return updatePumpenprobierleitung();
	else if (dynamic_cast<Pumpenverteiler*>(werkauftrag))
		return updatePumpenverteiler();
	else if (dynamic_cast<Verteiler*>(werkauftrag))
		return updateVerteiler();
	else if (dynamic_cast<CadWerkauftrag*>(werkauftrag))
		return updateCadWerkauftrag();
	else if (dynamic_cast<ApaxWerkauftrag*>(werkauftrag))
		return updateApax();
	return true; // i.a. Formstücke und Fittinge
}


//Holt aus der Statustabelle eine neue WA Nummer. Der Nummernkreis wird durch 
//die Materialnummer festgelegt. Wenn eine Werkauftrag vorliegt, 
//der das selbe PSP Element, SD-Aufnr, SD-Matnr u. SD-Posnr hat, wird 
//dessen WA Nummer genommen, wenn sie nicht 0 ist.
UINT Server::getNewWaNr(bool isNextGeneration, CString strIppeMatNr,  int materialnummer, __int64 auftragsnummer, int positionsnummer) const
{
	const int MAXNUMMERNKREIS = 5;
	/*
	Automatische WA Nummern Vergabe
	Erstmal wird geprüft, ob in der Statustabelle bereits ein Datensatz mit WA Nummer vorhanden ist.
	Ist dies der Fall, handelt es sich um den Teil eines Sammelauftrages. Es wird die WA Nummer zurückgeliefert.
	Falls nicht, wird auf die höchste vorhandene Nummer (Nummerkreis ist abhängig von der materialnummer) die
	nächste Zahl zurückgeliefert.
	*/
	// Dummy-Tabellen Objekt zur Ausführung von SQL Abfragen
	MXTable abfrage(dbhandle, "status");

	CString sql, ret;
	if(isNextGeneration)
	{
		sql.Format("SELECT MAX(status.wanr) FROM werkauftrag INNER JOIN status ON werkauftrag.waid = status.waid  WHERE ippeMatNr = \"%s\"", strIppeMatNr);
		ret = abfrage.executeSQL(sql);
		if (atoi(ret) > 0) // Leer oder 0 (bei CAD Werkaufträgen) heisst uninitialisiert
			return atoi(ret);// gefundene Nummer zurückliefern
	}
	else
	{
		if(auftragsnummer)
		{
			// Grösste Nummer muss wanr sein
			sql.Format("SELECT MAX(status.wanr) FROM werkauftrag INNER JOIN status ON werkauftrag.waid = status.waid  WHERE sdauftragsnr = %I64d AND sdpositionsnr = %i AND NOT status.storniert", auftragsnummer, positionsnummer);
			ret = abfrage.executeSQL(sql);
			if (atoi(ret) > 0) // Leer oder 0 (bei CAD Werkaufträgen) heisst uninitialisiert
				return atoi(ret);// gefundene Nummer zurückliefern
		}
	}
	// Keine WA Nummer, neue erzeugen
	const int nummernkreise[MAXNUMMERNKREIS][7] = { 
		/* Verteiler	*/ 400010, 400011, 400122, 0, 0, 0, 0,
		/* Strangrohre	*/ 400020, 0,      0, 0, 0, 0, 0,
		/* Gewax		*/ 400030, 400031, 0, 0, 0, 0, 0,
		/* Fo/Fi&Entn	*/ 400035, 400036, 0, 0, 0, 0, 0,
		/* APAX			*/ 400040, 400041, 400015, 400016, 400032, 400033, 0,
	};

	// Die Startwerte beruhen auf der Annahme, dass Ende 2003 das alte Wax abgelöst wurde
	// und die Anzahl der Aufträge nicht extrem grösser ausfällt als 2002.
	const int startnummer[MAXNUMMERNKREIS+1] = {
		30001, 700000, 220000, 304001, 408001, 500000
			 //115001
			 //700000
	};

	int nrkreis;
	// Nummernkreis suchen
	for (nrkreis = 0; nrkreis < MAXNUMMERNKREIS; nrkreis++)
	{
		int i;
		for (i = 0; nummernkreise[nrkreis][i]; i++)
			if (nummernkreise[nrkreis][i] == materialnummer)
				break;
		if (nummernkreise[nrkreis][i])
			break; // gefunden
	}
	
	// Falls keine reguläre SD Materialnummer vergeben wurde, kann auch keine automatisch Wanr vergeben werden
	if (nrkreis >= MAXNUMMERNKREIS)
	{
		CString msg, str;
		str = getStammdaten().getText("IDS_ERROR_SD_MATNR").c_str();
		msg.Format(str, auftragsnummer);
		AfxMessageBox(msg, MB_ICONERROR);
		return 0;
	}


	// Höchste wanummer suchen
	sql = "SELECT MAX(wanr) FROM werkauftrag INNER JOIN status ON werkauftrag.waid = status.waid WHERE ( ";
	for (int i = 0; nummernkreise[nrkreis][i]; i++)
	{
		CString matnr;
		matnr.Format(" sdmaterialnr = %i OR ", nummernkreise[nrkreis][i]);
		sql += matnr;
	}
	sql = sql.Left(sql.GetLength() - 3); // letztes OR entfernen
	CString maxnr;
	maxnr.Format("%i", startnummer[nrkreis+1]);
	sql += ") AND wanr < " + maxnr;
	
	ret = abfrage.executeSQL(sql);

	int nr = atoi(ret);

	if (!nr) // Erster Auftrag in diesem Nummernkreis, Standardwert zurückliefern
		return startnummer[nrkreis];
	
	// Prüfen, ob irrtümlich eine falsche Wa-Nummer herausgesucht wurde (Nummernkreise überprüfen, bzw. ein Bereich
	// könnte übergelaufen sein
	int maxtest = 10000;
	while (!checkUniqueWanr(++nr) && maxtest--);
	XASSERT(maxtest);
	if (!maxtest)
		return 0; //  Sollte niemals passieren...
	return nr;
}



//Zugriff auf die Soko Tabelle der Serverdatenbank
bool Server::updateSoko()
{
	TRY
	{
		MXTable sokotab(dbhandle, "soko", true);

		DECLARE_COL(Long,	waid,	sokotab);                        
		DECLARE_COL(Double, preis,	sokotab);
		DECLARE_COL(String, text,	sokotab);
		DECLARE_COL(Int,	nr,		sokotab);
		DECLARE_COL(Int,	isfix,		sokotab);
		
		if (modus == save)
		{

			deleteWerkauftragInTable(sokotab);
			openTable(sokotab);
		
			CObList &soko = werkauftrag->get_preis().get_sokolist();
			for (POSITION pos = soko.GetHeadPosition(); pos; )
			{
				Preis::Sonderkosten *sk = dynamic_cast<Preis::Sonderkosten*>(soko.GetNext(pos));
				sokotab.addNew();
				waid.data = werkauftrag->get_waid();
				preis.data = sk->get_preis();
				*text.data = sk->get_text();
				nr.data = sk->get_nr();
				isfix.data = sk->get_isfix();
		
				sokotab.update();
			}
		}
		else
		{
			CString select;
			select.Format("waid = %i", werkauftrag->get_waid());
			sokotab.load((LPCSTR)select, (LPCSTR)"counter", true);

			if (!sokotab.get_count())
				return false;
			
			// Sokoliste loeschen
			CObList &soko = werkauftrag->get_preis().get_sokolist();
			for (POSITION pos = soko.GetHeadPosition(); pos; )
			{
				Preis::Sonderkosten *sk = dynamic_cast<Preis::Sonderkosten*>(soko.GetNext(pos));
				delete sk;
			}
			soko.RemoveAll();
			
			while (!sokotab.isEof())
			{
				Preis::Sonderkosten *newsk = new Preis::Sonderkosten;
				newsk->set_preis(preis.data);
				newsk->set_text(*text.data);
				newsk->set_nr(nr.data);
				newsk->set_isfix(isfix.data ? true : false);
				soko.AddTail(newsk);
				
				if (!sokotab.checkCrc())
				{
					CString meld;
					meld.Format("Sonderkosten: %i", nr.data);
					werkauftrag->get_status().setCRCError(meld);
				}
				sokotab.moveNext();
			}
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

///Zugriff auf die Sap Tabelle der Serverdatenbank/Logdatenbank
bool Server::updateSap(int nr, double price)
{
	TRY
	{
		MXTable saptab(dbhandle, "sap");

		DECLARE_COL(Long,	wanr,  saptab);                        
		DECLARE_COL(Date,   datum, saptab); 
		DECLARE_COL(Double, preis, saptab);

		datum.set_dateOnly(false);

		if (modus == save)
		{
			saptab.load();

			saptab.addNew();

			wanr.data = nr;
			preis.data = price;
			MXDate now = MXDate::GetCurrentTime();
			datum.set_datum(now);

			saptab.update();
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

const DWORD& Server::get_dbhandle() const
{
	return dbhandle;
}


//Zugriff auf die objektspezifischen Tabelle für Verteilerfreigaben
bool Server::updateVerteiler( )
{
	/*
	Freigabe der Verteilerdaten
	Achtung: Rohrlängenfeld bis jetzt auf 512 Zeichen begrenzt
	*/
	Verteiler *rohr = dynamic_cast<Verteiler*>(werkauftrag);
	XASSERT(rohr);
	if (!rohr)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());

	TRY
	{
		MXTable verteiler(dbhandle, "verteiler", true);

		DECLARE_COL(Int, waid, verteiler);
		DECLARE_COL(String, rohrdeflaengen, verteiler);
		DECLARE_COL(String, rohrdeflaengen2, verteiler);
		
		if (modus == save)
		{
			deleteWerkauftragInTable(verteiler);
			openTable(verteiler);

			verteiler.addNew();
			waid.data = werkauftrag->get_waid();
			*rohrdeflaengen.data = rohr->get_rohrDefLaengen().Left(255);

			if (rohr->get_rohrDefLaengen().GetLength() > 255)
				*rohrdeflaengen2.data = rohr->get_rohrDefLaengen().Right(rohr->get_rohrDefLaengen().GetLength()-255);
			else
				*rohrdeflaengen2.data = "-";
			verteiler.update();
		}
		else
		{
			verteiler.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(verteiler.get_count() == 1); // Es kann nur einen Eintrag geben
			if (!verteiler.get_count())
				return false;

			if (*rohrdeflaengen2.data != "-")
				rohr->set_rohrDefLaengen(*rohrdeflaengen.data+*rohrdeflaengen2.data);
			else
				rohr->set_rohrDefLaengen(*rohrdeflaengen.data);
			if (!verteiler.checkCrc())
				werkauftrag->get_status().setCRCError("Verteiler");
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


//Zugriff auf die objektspezifischen Tabelle für Strangrohrfreigaben
bool Server::updateStrangrohr( )
{
	/*
	Freigabe der Verteilerdaten
	Achtung: Rohrlängenfeld bis jetzt auf 512 Zeichen begrenzt
	*/
	Strangrohr *rohr = dynamic_cast<Strangrohr*>(werkauftrag);
	XASSERT(rohr);
	if (!rohr)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());

	TRY
	{
		MXTable strang(dbhandle, "strangrohr", true);

		DECLARE_COL(Int, waid, strang);
		DECLARE_COL(Int, rohrTAnlage, strang);
		DECLARE_COL(Int, muffenGekehlt, strang);
		DECLARE_COL(Int, muffenDN, strang);
		DECLARE_COL(Int, muffenID, strang);
		DECLARE_COL(Int, muffenWinkel, strang);
		DECLARE_COL(Int, muffenRestabstand, strang);
		DECLARE_COL(Int, muffenRelAbst1, strang);
		DECLARE_COL(Int, muffenRelAbst2, strang);
		DECLARE_COL(Int, muffenRelAbst3, strang);
		DECLARE_COL(Int, muffenRelAbst4, strang);
		DECLARE_COL(Int, muffenRelAbst5, strang);
		DECLARE_COL(Int, muffenRelAbst6, strang);
		DECLARE_COL(Int, muffenRelAbst7, strang);
		DECLARE_COL(Int, muffenRelAbst8, strang);
		DECLARE_COL(Int, muffenRelAbst9, strang);
		DECLARE_COL(Int, muffenRelAbst10, strang);
		DECLARE_COL(Double, bohrdurchmesser, strang);
		DECLARE_COL(Int, muffenlaenge, strang);

		muffenlaenge.set_crcfield(false);

		if (modus == save)
		{
			deleteWerkauftragInTable(strang);
			openTable(strang);

			strang.addNew();
			waid.data = werkauftrag->get_waid();
			
			rohrTAnlage.data = rohr->get_trockenanlage();
			muffenGekehlt.data = rohr->get_muffenGekehlt();
			muffenDN.data = rohr->get_muffenDN();
			muffenID.data = rohr->get_muffenID();
			muffenWinkel.data = rohr->get_muffenWinkel();
			muffenRestabstand.data = rohr->get_muffenRestabstand();
			int i = 0;
			muffenRelAbst1.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst2.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst3.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst4.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst5.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst6.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst7.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst8.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst9.data = rohr->get_muffenRelAbst(i++);
			muffenRelAbst10.data = rohr->get_muffenRelAbst(i++);

			bohrdurchmesser.data = rohr->get_durchmesser();

			muffenlaenge.data = rohr->get_sonderMuffenLaenge();

			strang.update();
		}
		else
		{
			strang.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(strang.get_count() == 1); // Genau ein Eintrag wird erwartet

			rohr->set_trockenanlage(rohrTAnlage.data ? true:false);
			rohr->set_muffenGekehlt(muffenGekehlt.data ? true:false);
			rohr->set_muffenDN(muffenDN.data);
			rohr->set_muffenID(muffenID.data);
			rohr->set_muffenWinkel(muffenWinkel.data);
			rohr->set_muffenRestabstand(muffenRestabstand.data);
			int i = 0;
			rohr->set_muffenRelAbst(i++, muffenRelAbst1.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst2.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst3.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst4.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst5.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst6.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst7.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst8.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst9.data);
			rohr->set_muffenRelAbst(i++, muffenRelAbst10.data);

			rohr->set_durchmesser(bohrdurchmesser.data);

			rohr->set_sonderMuffenLaenge(muffenlaenge.data);

			if (!strang.checkCrc())
				werkauftrag->get_status().setCRCError("Strangrohr");
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



bool Server::updateCOIIMX1230200Verteiler()
{
	
	COIIVerteiler *pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(werkauftrag);
	XASSERT(pCOIIVerteiler);
	if (!pCOIIVerteiler)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());

	TRY
	{
		MXTable COIIVerteilerTable(dbhandle, "COIIMXVerteiler", true);

		DECLARE_COL(Int, waid, COIIVerteilerTable);
		DECLARE_COL(Int, AbgangEntleerungVorhanden, COIIVerteilerTable);
		DECLARE_COL(Int, AbgangEntleerungRechts, COIIVerteilerTable);
		DECLARE_COL(Int, AbgangSicherheitsventilVorhanden, COIIVerteilerTable);
		DECLARE_COL(Int, AbgangSicherheitsventilRechts, COIIVerteilerTable);

		DECLARE_COL(Int, AbstandBockAbsolut0, COIIVerteilerTable);
		DECLARE_COL(Int, AbstandBockAbsolut1, COIIVerteilerTable);
		DECLARE_COL(Int, AbstandBockAbsolut2, COIIVerteilerTable);
		DECLARE_COL(Int, TrestdleToRightTubeEnd, COIIVerteilerTable);

		DECLARE_COL(Int, AnzahlAnschluesse, COIIVerteilerTable);

		DECLARE_COL(Int, elementRelAbst0, COIIVerteilerTable);
		DECLARE_COL(Int, elementRelAbst1, COIIVerteilerTable);
		DECLARE_COL(Int, elementRelAbst2, COIIVerteilerTable);
		DECLARE_COL(Int, elementRelAbst3, COIIVerteilerTable);
		DECLARE_COL(Int, elementRelAbst4, COIIVerteilerTable);
		DECLARE_COL(Int, elementRelAbst5, COIIVerteilerTable);
		DECLARE_COL(Int, elementRelAbst6, COIIVerteilerTable);

		DECLARE_COL(Int, elementDN0, COIIVerteilerTable);
		DECLARE_COL(Int, elementDN1, COIIVerteilerTable);
		DECLARE_COL(Int, elementDN2, COIIVerteilerTable);
		DECLARE_COL(Int, elementDN3, COIIVerteilerTable);
		DECLARE_COL(Int, elementDN4, COIIVerteilerTable);
		DECLARE_COL(Int, elementDN5, COIIVerteilerTable);
		DECLARE_COL(Int, elementDN6, COIIVerteilerTable);

		DECLARE_COL(Int, elementTypAbgangEnde0, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypAbgangEnde1, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypAbgangEnde2, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypAbgangEnde3, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypAbgangEnde4, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypAbgangEnde5, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypAbgangEnde6, COIIVerteilerTable);

		DECLARE_COL(Int, elementTypGegenFLansch0, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypGegenFLansch1, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypGegenFLansch2, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypGegenFLansch3, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypGegenFLansch4, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypGegenFLansch5, COIIVerteilerTable);
		DECLARE_COL(Int, elementTypGegenFLansch6, COIIVerteilerTable);

		DECLARE_COL(Int, elementLaengeGegenFLansche0, COIIVerteilerTable);
		DECLARE_COL(Int, elementLaengeGegenFLansche1, COIIVerteilerTable);
		DECLARE_COL(Int, elementLaengeGegenFLansche2, COIIVerteilerTable);
		DECLARE_COL(Int, elementLaengeGegenFLansche3, COIIVerteilerTable);
		DECLARE_COL(Int, elementLaengeGegenFLansche4, COIIVerteilerTable);
		DECLARE_COL(Int, elementLaengeGegenFLansche5, COIIVerteilerTable);
		DECLARE_COL(Int, elementLaengeGegenFLansche6, COIIVerteilerTable);

		DECLARE_COL(Int, elementEndschalterTyp, COIIVerteilerTable);
				
		
		if (modus == save)
		{
			deleteWerkauftragInTable(COIIVerteilerTable);
			openTable(COIIVerteilerTable);

			COIIVerteilerTable.addNew();
			waid.data = werkauftrag->get_waid();
			AbgangEntleerungVorhanden.data			= pCOIIVerteiler->GetAbgangEntleerungVorhanden();
			AbgangEntleerungRechts.data				= pCOIIVerteiler->GetAbgangEntleerungRechts(); 
			AbgangSicherheitsventilVorhanden.data	= pCOIIVerteiler->GetAbgangSicherheitsventilVorhanden();
			AbgangSicherheitsventilRechts.data		= pCOIIVerteiler->GetAbgangSicherheitsventilRechts();
			
			

			AbstandBockAbsolut0.data	= pCOIIVerteiler->GetAbstandBockZumLinkenRohrEnde();
			AbstandBockAbsolut1.data	= pCOIIVerteiler->GetAbstandErsterMittlererBockZumLinkenRohrEnde();
			AbstandBockAbsolut2.data	= pCOIIVerteiler->GetAbstandZweiterMittlererBockZumErstenMittlerenBock();
			TrestdleToRightTubeEnd.data = pCOIIVerteiler->GetAbstandBockZumRechtenRohrEnde();

			AnzahlAnschluesse.data		= pCOIIVerteiler->get_AnzahlAnschluesse();
			
			elementRelAbst0.data = pCOIIVerteiler->get_elementRelAbst(0);
			elementRelAbst1.data = pCOIIVerteiler->get_elementRelAbst(1);
			elementRelAbst2.data = pCOIIVerteiler->get_elementRelAbst(2);
			elementRelAbst3.data = pCOIIVerteiler->get_elementRelAbst(3);
			elementRelAbst4.data = pCOIIVerteiler->get_elementRelAbst(4);
			elementRelAbst5.data = pCOIIVerteiler->get_elementRelAbst(5);
			elementRelAbst6.data = pCOIIVerteiler->get_elementRelAbst(6);

			elementDN0.data = pCOIIVerteiler->get_elementDN(0);
			elementDN1.data = pCOIIVerteiler->get_elementDN(1);
			elementDN2.data = pCOIIVerteiler->get_elementDN(2);
			elementDN3.data = pCOIIVerteiler->get_elementDN(3);
			elementDN4.data = pCOIIVerteiler->get_elementDN(4);
			elementDN5.data = pCOIIVerteiler->get_elementDN(5);
			elementDN6.data = pCOIIVerteiler->get_elementDN(6);
		
			elementTypAbgangEnde0.data = pCOIIVerteiler->GetElementTypAbgangEnde(0);
			elementTypAbgangEnde1.data = pCOIIVerteiler->GetElementTypAbgangEnde(1);
			elementTypAbgangEnde2.data = pCOIIVerteiler->GetElementTypAbgangEnde(2);
			elementTypAbgangEnde3.data = pCOIIVerteiler->GetElementTypAbgangEnde(3);
			elementTypAbgangEnde4.data = pCOIIVerteiler->GetElementTypAbgangEnde(4);
			elementTypAbgangEnde5.data = pCOIIVerteiler->GetElementTypAbgangEnde(5);
			elementTypAbgangEnde6.data = pCOIIVerteiler->GetElementTypAbgangEnde(6);

			elementTypGegenFLansch0.data = pCOIIVerteiler->GetElementTypGegenFLansch(0);
			elementTypGegenFLansch1.data = pCOIIVerteiler->GetElementTypGegenFLansch(1);
			elementTypGegenFLansch2.data = pCOIIVerteiler->GetElementTypGegenFLansch(2);
			elementTypGegenFLansch3.data = pCOIIVerteiler->GetElementTypGegenFLansch(3);
			elementTypGegenFLansch4.data = pCOIIVerteiler->GetElementTypGegenFLansch(4);
			elementTypGegenFLansch5.data = pCOIIVerteiler->GetElementTypGegenFLansch(5);
			elementTypGegenFLansch6.data = pCOIIVerteiler->GetElementTypGegenFLansch(6);

			elementLaengeGegenFLansche0.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(0);
			elementLaengeGegenFLansche1.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(1);
			elementLaengeGegenFLansche2.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(2);
			elementLaengeGegenFLansche3.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(3);
			elementLaengeGegenFLansche4.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(4);
			elementLaengeGegenFLansche5.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(5);
			elementLaengeGegenFLansche6.data = pCOIIVerteiler->GetElementLaengeGegenFLansche(6);

			elementEndschalterTyp.data = pCOIIVerteiler->GetElementEndschalterTyp();
			
			
			COIIVerteilerTable.update();			
		}
		else
		{
			COIIVerteilerTable.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(COIIVerteilerTable.get_count() == 1); // Genau ein Eintrag wird erwartet
						
			pCOIIVerteiler->SetAbgangEntleerungVorhanden(AbgangEntleerungVorhanden.data);
			pCOIIVerteiler->SetAbgangEntleerungRechts(AbgangEntleerungRechts.data);

			pCOIIVerteiler->SetAbgangSicherheitsventilVorhanden(AbgangSicherheitsventilVorhanden.data);
			pCOIIVerteiler->SetAbgangSicherheitsventilRechts(AbgangSicherheitsventilRechts.data);
			
			
			pCOIIVerteiler->SetAbstandBockZumLinkenRohrEnde(AbstandBockAbsolut0.data);
			pCOIIVerteiler->SetAbstandErsterMittlererBockZumLinkenRohrEnde(AbstandBockAbsolut1.data);
			pCOIIVerteiler->SetAbstandZweiterMittlererBockZumErstenMittlerenBock(AbstandBockAbsolut2.data > 32000 ? 0 : AbstandBockAbsolut2.data);
			pCOIIVerteiler->SetAbstandBockZumRechtenRohrEnde(TrestdleToRightTubeEnd.data);

			pCOIIVerteiler->set_AnzahlAnschluesse(AnzahlAnschluesse.data);
			
			pCOIIVerteiler->set_elementRelAbst(0, elementRelAbst0.data);
			pCOIIVerteiler->set_elementRelAbst(1, elementRelAbst1.data);
			pCOIIVerteiler->set_elementRelAbst(2, elementRelAbst2.data);
			pCOIIVerteiler->set_elementRelAbst(3, elementRelAbst3.data);
			pCOIIVerteiler->set_elementRelAbst(4, elementRelAbst4.data);
			pCOIIVerteiler->set_elementRelAbst(5, elementRelAbst5.data);
			pCOIIVerteiler->set_elementRelAbst(6, elementRelAbst6.data);

			pCOIIVerteiler->set_elementDN(0, elementDN0.data);
			pCOIIVerteiler->set_elementDN(1, elementDN1.data);
			pCOIIVerteiler->set_elementDN(2, elementDN2.data);
			pCOIIVerteiler->set_elementDN(3, elementDN3.data);
			pCOIIVerteiler->set_elementDN(4, elementDN4.data);
			pCOIIVerteiler->set_elementDN(5, elementDN5.data);
			pCOIIVerteiler->set_elementDN(6, elementDN6.data);

			pCOIIVerteiler->SetElementTypAbgangEnde(0, elementTypAbgangEnde0.data);
			pCOIIVerteiler->SetElementTypAbgangEnde(1, elementTypAbgangEnde1.data);
			pCOIIVerteiler->SetElementTypAbgangEnde(2, elementTypAbgangEnde2.data);
			pCOIIVerteiler->SetElementTypAbgangEnde(3, elementTypAbgangEnde3.data);
			pCOIIVerteiler->SetElementTypAbgangEnde(4, elementTypAbgangEnde4.data);
			pCOIIVerteiler->SetElementTypAbgangEnde(5, elementTypAbgangEnde5.data);
			pCOIIVerteiler->SetElementTypAbgangEnde(6, elementTypAbgangEnde6.data);

			pCOIIVerteiler->SetElementTypGegenFLansch(0, elementTypGegenFLansch0.data);
			pCOIIVerteiler->SetElementTypGegenFLansch(1, elementTypGegenFLansch1.data);
			pCOIIVerteiler->SetElementTypGegenFLansch(2, elementTypGegenFLansch2.data);
			pCOIIVerteiler->SetElementTypGegenFLansch(3, elementTypGegenFLansch3.data);
			pCOIIVerteiler->SetElementTypGegenFLansch(4, elementTypGegenFLansch4.data);
			pCOIIVerteiler->SetElementTypGegenFLansch(5, elementTypGegenFLansch5.data);
			pCOIIVerteiler->SetElementTypGegenFLansch(6, elementTypGegenFLansch6.data);

			pCOIIVerteiler->SetElementLaengeGegenFLansche(0, elementLaengeGegenFLansche0.data);
			pCOIIVerteiler->SetElementLaengeGegenFLansche(1, elementLaengeGegenFLansche1.data);
			pCOIIVerteiler->SetElementLaengeGegenFLansche(2, elementLaengeGegenFLansche2.data);
			pCOIIVerteiler->SetElementLaengeGegenFLansche(3, elementLaengeGegenFLansche3.data);
			pCOIIVerteiler->SetElementLaengeGegenFLansche(4, elementLaengeGegenFLansche4.data);
			pCOIIVerteiler->SetElementLaengeGegenFLansche(5, elementLaengeGegenFLansche5.data);
			pCOIIVerteiler->SetElementLaengeGegenFLansche(6, elementLaengeGegenFLansche6.data);

			pCOIIVerteiler->SetElementEndschalterTyp(elementEndschalterTyp.data);
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


//Zugriff auf die objektspezifischen Tabelle für Stationsverteilerfreigaben
bool Server::updateStationsverteiler( )
{
	Stationsverteiler *stat = dynamic_cast<Stationsverteiler*>(werkauftrag);
	XASSERT(stat);
	if (!stat)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());

	TRY
	{
		MXTable station(dbhandle, "stationsverteiler", true);

		DECLARE_COL(Int, waid, station);
		DECLARE_COL(Int, extraAbgAbsAbst, station);
		DECLARE_COL(Int, extraAbgDN, station);
		DECLARE_COL(Int, extraAbgID, station);
		DECLARE_COL(Int, extraAbgLaenge, station);
		DECLARE_COL(Int, elementRelAbst1, station);
		DECLARE_COL(Int, elementRelAbst2, station);
		DECLARE_COL(Int, elementRelAbst3, station);
		DECLARE_COL(Int, elementRelAbst4, station);
		DECLARE_COL(Int, elementRelAbst5, station);
		DECLARE_COL(Int, elementRelAbst6, station);
		DECLARE_COL(Int, elementDN1, station);
		DECLARE_COL(Int, elementDN2, station);
		DECLARE_COL(Int, elementDN3, station);
		DECLARE_COL(Int, elementDN4, station);
		DECLARE_COL(Int, elementDN5, station);
		DECLARE_COL(Int, elementDN6, station);
		DECLARE_COL(Int, elementLaenge, station);
		DECLARE_COL(Int, elementlEnde, station);
		DECLARE_COL(Int, elementrEnde, station);
		DECLARE_COL(Int, extraAbgObenAbsAbst, station);
		DECLARE_COL(Int, extraAbgObenDN, station);
		DECLARE_COL(Int, extraAbgObenID, station);
		DECLARE_COL(Int, extraAbgObenLaenge, station);

		if (modus == save)
		{
			deleteWerkauftragInTable(station);
			openTable(station);

			station.addNew();
			waid.data = werkauftrag->get_waid();
			extraAbgAbsAbst.data = stat->get_extraAbgAbsAbst();
			extraAbgDN.data = stat->get_extraAbgDN();
			extraAbgID.data = stat->get_extraAbgID();
			extraAbgLaenge.data = stat->get_extraAbgLaenge();
			int i = 0;
			elementRelAbst1.data = stat->get_elementRelAbst(i++);
			elementRelAbst2.data = stat->get_elementRelAbst(i++);
			elementRelAbst3.data = stat->get_elementRelAbst(i++);
			elementRelAbst4.data = stat->get_elementRelAbst(i++);
			elementRelAbst5.data = stat->get_elementRelAbst(i++);
			elementRelAbst6.data = stat->get_elementRelAbst(i++);
			XASSERT(i == Stationsverteiler::MAXCOUNT);
			i = 0;
			elementDN1.data = stat->get_elementDN(i++);
			elementDN2.data = stat->get_elementDN(i++);
			elementDN3.data = stat->get_elementDN(i++);
			elementDN4.data = stat->get_elementDN(i++);
			elementDN5.data = stat->get_elementDN(i++);
			elementDN6.data = stat->get_elementDN(i++);
			XASSERT(i == Stationsverteiler::MAXCOUNT);
			elementLaenge.data = stat->get_elementLaenge();
			elementlEnde.data = stat->get_elementlEnde();
			elementrEnde.data = stat->get_elementrEnde();
	
			extraAbgObenAbsAbst.data = stat->get_extraAbgObenAbsAbst();
			extraAbgObenDN.data = stat->get_extraAbgObenDN();
			extraAbgObenID.data = stat->get_extraAbgObenID();
			extraAbgObenLaenge.data = stat->get_extraAbgObenLaenge();
			
			station.update();
		}
		else
		{
			station.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(station.get_count() == 1); // Genau ein Eintrag wird erwartet
			stat->set_extraAbgAbsAbst(extraAbgAbsAbst.data);
			stat->set_extraAbgDN(extraAbgDN.data);
			stat->set_extraAbgID(extraAbgID.data);
			stat->set_extraAbgLaenge(extraAbgLaenge.data);
			int i = 0;
			stat->set_elementRelAbst(i++, elementRelAbst1.data);
			stat->set_elementRelAbst(i++, elementRelAbst2.data);
			stat->set_elementRelAbst(i++, elementRelAbst3.data);
			stat->set_elementRelAbst(i++, elementRelAbst4.data);
			stat->set_elementRelAbst(i++, elementRelAbst5.data);
			stat->set_elementRelAbst(i++, elementRelAbst6.data);
			XASSERT(i == Stationsverteiler::MAXCOUNT);
			i = 0;
			stat->set_elementDN(i++, elementDN1.data);
			stat->set_elementDN(i++, elementDN2.data);
			stat->set_elementDN(i++, elementDN3.data);
			stat->set_elementDN(i++, elementDN4.data);
			stat->set_elementDN(i++, elementDN5.data);
			stat->set_elementDN(i++, elementDN6.data);
			XASSERT(i == Stationsverteiler::MAXCOUNT);

			stat->set_elementLaenge(elementLaenge.data);

			//alte FlanschID auto. konvertieren
			stat->set_elementlEnde(GpConvertOldVersion::getNewFlanschId(elementlEnde.data,stat->get_dnvl()));
			stat->set_elementrEnde(GpConvertOldVersion::getNewFlanschId(elementrEnde.data,stat->get_dnvl()));

			stat->set_extraAbgObenAbsAbst(extraAbgObenAbsAbst.data);
			stat->set_extraAbgObenDN(extraAbgObenDN.data);
			stat->set_extraAbgObenID(extraAbgObenID.data);
			stat->set_extraAbgObenLaenge(extraAbgObenLaenge.data);

			if (!station.checkCrc())
				werkauftrag->get_status().setCRCError("Stationsverteiler");
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

//Zugriff auf die objektspezifischen Tabelle für Gewinderohrfreigaben
bool Server::updateGewinderohr()
{
	Gewinderohr* grohr = dynamic_cast<Gewinderohr *>(werkauftrag);
	XASSERT(grohr);
	if (!grohr)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());
	TRY
	{
		MXTable grohrtab(dbhandle, "gewinderohr", true);

		DECLARE_COL(Int, waid,			grohrtab);
		DECLARE_COL(Int, typ,			grohrtab);
		DECLARE_COL(Int, dichtmittel,	grohrtab);
		DECLARE_COL(Int, zmassLinks,	grohrtab);
		DECLARE_COL(Int, zmassTyp,		grohrtab);
		DECLARE_COL(Int, zmassNennweite,grohrtab);
		DECLARE_COL(Int, kaltband,		grohrtab);

		if (modus == save)
		{
			deleteWerkauftragInTable(grohrtab);
			openTable(grohrtab);

			grohrtab.addNew();
			waid.data = werkauftrag->get_waid();
			typ.data = grohr->get_typ();
			dichtmittel.data = grohr->get_dichtmittel();
			zmassLinks.data = grohr->get_zmassLinks();
			zmassTyp.data = grohr->get_zmassTyp();
			zmassNennweite.data = grohr->get_zmassRefDN();
			kaltband.data = 0;
			grohrtab.update();
		}
		else
		{
			grohrtab.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(grohrtab.get_count() == 1); // Genau ein Eintrag wird erwartet
			grohr->set_typ(typ.data);
			grohr->set_dichtmittel(dichtmittel.data);
			grohr->set_zmassLinks(zmassLinks.data);
			grohr->set_zmassTyp(zmassTyp.data);
			grohr->set_zmassRefDN(zmassNennweite.data);
			grohr->setLaengeDnvl();
			grohr->set_kaltbandgeschweisst(kaltband.data);
			if (!grohrtab.checkCrc())
				werkauftrag->get_status().setCRCError("Gewinderohr");
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

//Zugriff auf die Tabelle mit Entnahmeleitungen der Serverdatenbank
bool Server::updateEntnahmeleitung()
{
	EntnahmeleitungDWB *entnahme = dynamic_cast<EntnahmeleitungDWB*>(werkauftrag);
	XASSERT(entnahme);
	if (!entnahme)
		return false;
	CString select;
	TRY
	{
		MXTable entnahmetab(dbhandle, "entnahmeleitung", true);

		DECLARE_COL(Int, waid, entnahmetab);
		DECLARE_COL(Int, typ, entnahmetab);
		DECLARE_COL(Int, laenge1, entnahmetab);
		DECLARE_COL(Int, laenge2, entnahmetab);
		DECLARE_COL(Bool, entleerung, entnahmetab);
		DECLARE_COL(Int, dnvl, entnahmetab);
		DECLARE_COL(Bool, entleerungrechts, entnahmetab);

		select.Format("waid = %i", werkauftrag->get_waid());

		if (modus == save)
		{

			deleteWerkauftragInTable(entnahmetab);
			openTable(entnahmetab);

			entnahmetab.addNew();
			waid.data = werkauftrag->get_waid();
			dnvl.data = entnahme->get_dnvl();
			typ.data = entnahme->get_entnahmeleitung_typ();
			laenge1.data = entnahme->get_laenge1();
			laenge2.data = entnahme->get_laenge2();
			entleerung.data = entnahme->get_entleerung();
			entleerungrechts.data = entnahme->get_entleerungRechts();
			entnahmetab.update();
		}
		else
		{
			entnahmetab.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(entnahmetab.get_count() == 1); // Genau ein Eintrag wird erwartet
		
			entnahme->set_entnahmeleitung_typ(typ.data);
			entnahme->set_dnvl(dnvl.data);
			entnahme->set_laenge1(laenge1.data);
			entnahme->set_laenge2(laenge2.data);
			entnahme->set_entleerung(entleerung.data);
			entnahme->set_entleerungRechts(entleerungrechts.data ? true : false);
			if (!entnahmetab.checkCrc())
				werkauftrag->get_status().setCRCError("Entnahmeleitung");
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

//Lädt die WA Nummer aus der Statustabelle
bool Server::get_wanr()
{
	MXTable wa(dbhandle, "status");
	
	DECLARE_COL(Long, waid, wa);
	DECLARE_COL(Int, wanr, wa);

	CString select;

	ASSERT_WAID;
	XASSERT(modus == load);
	if (modus == save)
		return false;
	
	TRY
	{
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);
		XASSERT( wa.get_count() == 0 || wa.get_count() == 1); 
		if (wa.get_count() > 1 || wa.get_count() < 0) 
			AfxThrowNotSupportedException();
		werkauftrag->get_status().set_wanr(wanr.data);
	}
	CATCH(CDBException, e)
	{ 
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	return true;
}

//Zugriff auf die Tabelle mit Pumpenverteiler der Serverdatenbank
bool Server::updatePumpenverteiler()
{
	Pumpenverteiler *pumpvert = dynamic_cast<Pumpenverteiler *>(werkauftrag);
	XASSERT(pumpvert);
	if (!pumpvert)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());
	TRY
	{
		MXTable pumpenverteilertab(dbhandle, "pumpenverteiler", true);

		DECLARE_COL(Int, waid, pumpenverteilertab);
		DECLARE_COL(Int, reduziert, pumpenverteilertab);
		DECLARE_COL(Int, gesamthoehe, pumpenverteilertab);
		DECLARE_COL(Int, dnPumpDruckSt, pumpenverteilertab);
		DECLARE_COL(Int, winkelRohrnippel, pumpenverteilertab);
		DECLARE_COL(Int, dnProbAnschl, pumpenverteilertab);
		DECLARE_COL(Int, probAnschl, pumpenverteilertab);
		DECLARE_COL(Int, laengeProbAnschl, pumpenverteilertab);
		DECLARE_COL(Int, abstandProbAnschl, pumpenverteilertab);
		DECLARE_COL(Int, dnSprAnlAnschl, pumpenverteilertab);
		DECLARE_COL(Int, sprAnlAnschl, pumpenverteilertab);
		DECLARE_COL(Int, schwMuffe, pumpenverteilertab);
		DECLARE_COL(Int, winkelSchwMuffe, pumpenverteilertab);
		DECLARE_COL(Int, abstandSchwMuffe, pumpenverteilertab);
	
		if (modus == save)
		{

			deleteWerkauftragInTable(pumpenverteilertab);
			openTable(pumpenverteilertab);

			pumpenverteilertab.addNew();
			waid.data = werkauftrag->get_waid();
			reduziert.data = pumpvert->get_reduziert();
			gesamthoehe.data = pumpvert->get_gesamthoehe();
			dnPumpDruckSt.data = pumpvert->get_dnPumpDruckSt();
			winkelRohrnippel.data = pumpvert->get_winkelRohnippel();
			dnProbAnschl.data = pumpvert->get_dnProbAnschl();
			probAnschl.data = pumpvert->get_probAnschl();
			laengeProbAnschl.data = pumpvert->get_laengeProbAnschl();
			abstandProbAnschl.data = pumpvert->get_abstandProbAnschl();
			dnSprAnlAnschl.data = pumpvert->get_dnSprAnlAnschl();
			sprAnlAnschl.data = pumpvert->get_sprAnlAnschl();
			schwMuffe.data = pumpvert->get_schwMuffe();
			winkelSchwMuffe.data = pumpvert->get_winkelSchwMuffe();
			abstandSchwMuffe.data = pumpvert->get_abstandSchwMuffe();
			pumpenverteilertab.update();
		}
		else
		{
			pumpenverteilertab.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(pumpenverteilertab.get_count() == 1); // Genau ein Eintrag wird erwartet
			pumpvert->set_reduziert(reduziert.data ? true : false);
			pumpvert->set_gesamthoehe(gesamthoehe.data);
			pumpvert->set_dnPumpDruckSt(dnPumpDruckSt.data);
			pumpvert->set_winkelRohnippel(winkelRohrnippel.data);
			pumpvert->set_dnProbAnschl(dnProbAnschl.data);
			pumpvert->set_probAnschl(probAnschl.data);
			pumpvert->set_laengeProbAnschl(laengeProbAnschl.data);
			pumpvert->set_abstandProbAnschl(abstandProbAnschl.data);
			pumpvert->set_dnSprAnlAnschl(dnSprAnlAnschl.data);
			pumpvert->set_sprAnlAnschl(sprAnlAnschl.data);
			pumpvert->set_schwMuffe(schwMuffe.data ? true : false);
			pumpvert->set_winkelSchwMuffe(winkelSchwMuffe.data);
			pumpvert->set_abstandSchwMuffe(abstandSchwMuffe.data);
			if (!pumpenverteilertab.checkCrc())
				werkauftrag->get_status().setCRCError("Pumpenverteiler");
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



//Zugriff auf die Tabelle mit Pumpenverteiler der Serverdatenbank
bool Server::updateApax()
{
	ApaxWerkauftrag *apax = dynamic_cast<ApaxWerkauftrag *>(werkauftrag);
	XASSERT(apax);
	if (!apax)
		return false;
	CString select;
	select.Format("waid = %i", werkauftrag->get_waid());
	TRY
	{
		MXTable apaxTab(dbhandle, "apax", true);

		DECLARE_COL(Int, waid, apaxTab);
		DECLARE_COL(Double, preis, apaxTab);
		
	
		if (modus == save)
		{

			deleteWerkauftragInTable(apaxTab);
			openTable(apaxTab);

			apaxTab.addNew();
			waid.data = werkauftrag->get_waid();
			preis.data = apax->get_vk3Preis();
			
			apaxTab.update();
		}
		else
		{
			apaxTab.load((LPCSTR)select, (LPCSTR)"", true);

			//nur Einlesen wenn gesetzt
			if (apaxTab.get_count() == 1)
			{			
				apax->set_vk3Preis(preis.data);
			}
			
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



//Zugriff auf die Tabelle mit Pumpenprobierleitung der Serverdatenbank
bool Server::updatePumpenprobierleitung()
{
	Pumpenprobierleitung *pumpenprobier = dynamic_cast<Pumpenprobierleitung *>(werkauftrag);
	XASSERT(pumpenprobier);
	if (!pumpenprobier)
		return false;
	CString select;
	TRY
	{
		MXTable pumpenprobiertab(dbhandle, "pumpenprobierleitung", true);

		DECLARE_COL(Int, waid, pumpenprobiertab);
		DECLARE_COL(Int, dn1, pumpenprobiertab);
		DECLARE_COL(Int, dn2, pumpenprobiertab);
		DECLARE_COL(Int, laenge1, pumpenprobiertab);
		DECLARE_COL(Int, laenge2, pumpenprobiertab);
		DECLARE_COL(Int, laenge3, pumpenprobiertab);
		DECLARE_COL(Int, anschlussA, pumpenprobiertab);
		DECLARE_COL(Int, anschlussB, pumpenprobiertab);

		select.Format("waid = %i", werkauftrag->get_waid());
		if (modus == save)
		{

			deleteWerkauftragInTable(pumpenprobiertab);
			openTable(pumpenprobiertab);

			pumpenprobiertab.addNew();
			waid.data = werkauftrag->get_waid();
			dn1.data = pumpenprobier->get_dn1();
			dn2.data = pumpenprobier->get_dn2();
			laenge1.data = pumpenprobier->get_laenge1();
			laenge2.data = pumpenprobier->get_laenge2();
			laenge3.data = pumpenprobier->get_laenge3();
			anschlussA.data = pumpenprobier->get_anschlussA();
			anschlussB.data = pumpenprobier->get_anschlussB();
			pumpenprobiertab.update();
		}
		else
		{
			pumpenprobiertab.load((LPCSTR)select, (LPCSTR)"", true);
			XASSERT(pumpenprobiertab.get_count() == 1); // Genau ein Eintrag wird erwartet
			pumpenprobier->set_dn1(dn1.data);
			pumpenprobier->set_dn2(dn2.data);
			pumpenprobier->set_laenge1(laenge1.data);
			pumpenprobier->set_laenge2(laenge2.data);
			pumpenprobier->set_laenge3(laenge3.data);
			pumpenprobier->set_anschlussA(anschlussA.data);
			pumpenprobier->set_anschlussB(anschlussB.data);
			if (!pumpenprobiertab.checkCrc())
				werkauftrag->get_status().setCRCError("Pumpenprobierleitung");
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


bool Server::deleteWerkauftragInTable(MXTable& tab)
{
	CString select;
	if (werkauftrag->get_waid()) // Werkauftrag wurde schon freigegeben, Rohrleitung muss gelöscht werden
	{
		CString sql;
		sql.Format("DELETE FROM %s WHERE waid = %i",tab.get_tableName(), werkauftrag->get_waid());
		CString ret = tab.executeSQL(sql);
	}
	return true;
}


///öffnet die Tabelle
bool Server::openTable(MXTable& tab)
{
	//als waid wird ein Datensatz genommen, der NICHT in der Db vorkommt, damit beim Öffnen
	//keine Datensätze eingelesen werden.
/*	unsigned long t1 = ::GetTickCount();
	CString select;
	if (werkauftrag)
		select.Format("waid=%i", werkauftrag->get_waid());
*/
	bool ret = tab.load((LPCSTR)"waid=-1", (LPCSTR)"", false);
//	unsigned long t2 = ::GetTickCount();
//	TRACE("Öffnen der Tabelle (sql=%s) %s = %i msec\n", select, tab.get_tableName(), t2-t1); 
	return ret;
}


//Setzt, bzw. versucht eine bestimmte Wanr in der Tabelle Status zu setzen.
bool Server::ResetWanr()
{
	// Kann auftreten, wenn ein Sammelauftrag geändert wird
	if (!werkauftrag || !werkauftrag->get_waid())
		return false;
	
	MXTable wa(dbhandle, "status", true);
	int  newwanr = werkauftrag->get_status().get_wanr();
	if (newwanr)
	{
		CString sqlex;
		sqlex.Format("UPDATE status SET wanr = 0, sachbesegment = '', status = 2 WHERE wanr=%i", newwanr);
		CString count = wa.executeSQL(sqlex);
		return true;
	}
	else
	{
		return false;
	}
}




//Setzt, bzw. versucht eine bestimmte Wanr in der Tabelle Status zu setzen.
bool Server::setwanr(int newwanr, bool testUnique)
{
	// Kann auftreten, wenn ein Sammelauftrag geändert wird
	if (!werkauftrag || !werkauftrag->get_waid())
		return false;
	
	MXTable wa(dbhandle, "status", true);
	
	// Erstmal testen, ob die WA Nr schon vergeben ist, wenn nicht 0 angegeben wurde
	if (newwanr && testUnique)
	{
		CString sqlex;
		sqlex.Format("SELECT COUNT(*) FROM status WHERE wanr = %i", newwanr);
		CString count = wa.executeSQL(sqlex);
		if (atoi(count))
		{
			CString str; str = getStammdaten().getText("IDS_WANR_VERGEBEN").c_str();
			AfxMessageBox(str, MB_OK, MB_ICONINFORMATION);
			return false;
		}
	}
	DECLARE_COL(Long, waid, wa);
	DECLARE_COL(Int, wanr, wa);
	DECLARE_COL(String, sachbesegment, wa);
	DECLARE_COL(Date, mbtermin, wa);
	DECLARE_COL(Int, freigabecounter, wa);
	DECLARE_COL(Int, status, wa);
	
	CString sel;
	sel.Format("waid = %i", werkauftrag->get_waid());
	wa.load(sel);

	wa.edit();
	wanr.data = newwanr;

	if (status.data != (int)Status::Storniert)
		status.data = (int)Status::InBearbeitungAv;
	wa.update();
	
	return true;
}

bool Server::checkUniqueWanr(int testwanr) const
{
	MXTable wa(dbhandle, "status");
	
	CString sql;
	// Prüfung der Tabelle status
	sql.Format("SELECT COUNT(*) FROM status WHERE wanr = %i", testwanr);
	CString count_status = wa.executeSQL(sql);
	// Prüfung der Tabelle gesperrte_wanr
	sql.Format("SELECT COUNT(*) FROM gesperrte_wanr WHERE wanr = %i", testwanr);
	CString count_locked = wa.executeSQL(sql);

	// Liefert true, wenn in beiden Tabellen keine Nummer gefunden wurde
	return count_status == "0" && count_locked == "0";
}
/**
Liste mit VK3 Elementen erstellen.
nur für Statistik , deshalb nur in der AV Version speichern
*/
bool Server::updateVK3liste(WerkauftragListe *liste)
{
    if (!WaxSys::isAV())
		return true;

	CString stMeldung;

	int waidFromEl;


	MXTable vk3liste(dbhandle, "vk3listewa", true);
	
	DECLARE_COL(Int,	waid,				vk3liste);
	DECLARE_COL(Int,	eleid,				vk3liste);
	DECLARE_COL(Int,	artikelnr,			vk3liste);
	DECLARE_COL(Double,	anzahllaenge,		vk3liste);
	DECLARE_COL(Int,	dn,					vk3liste);
	DECLARE_COL(Int,	anschlussdn,		vk3liste);
	DECLARE_COL(Double, preis,				vk3liste);
	DECLARE_COL(Bool,	isvk3abr,			vk3liste);
	
	long lAnzahl = liste->getSize();

	//Fortschrittsanzeige ínitialisieren
	if (m_pFortschritt)
	{
		CString st; st = getStammdaten().getText("IDS_WRITE_VK3_STL").c_str();
		m_pFortschritt->set_maxWert(lAnzahl);
		m_pFortschritt->set_minWert(0);
		m_pFortschritt->set_ueberschrift(st);
		m_pFortschritt->start();
	}

	if (modus == save)
	{
		//Tabelle öffnen
		openTable(vk3liste);


		MxIterator iter(*liste);
		while (iter.next())
		{			
			Werkauftrag* pWerkauftrag = dynamic_cast<Werkauftrag*>(liste->getAt(iter));
			if (!pWerkauftrag)
				continue;

			waidFromEl = pWerkauftrag->get_waid();
			
			//Fortschritt
			if (m_pFortschritt)
			{
				int iProzent = m_pFortschritt->nextStep(iter.getIndex());

				stMeldung.Format("%d%s, Datensatz %d von %d, (WaID = %d)",iProzent,"%",iter.getIndex(),lAnzahl,waidFromEl);
				m_pFortschritt->setAnzeige1(stMeldung);
			}
	
			if (waidFromEl <= 0)
				continue;

			Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(pWerkauftrag);
			ApaxWerkauftrag *apax = dynamic_cast<ApaxWerkauftrag*>(pWerkauftrag);
			if (!rohr && !apax)
				continue;
	
			TRY
			{
				//Werkauftrag setzen, damit mit deleteWerkauftragInTable die Datensätze zur wanr gelöscht werden können
				set_werkauftrag(pWerkauftrag);

				deleteWerkauftragInTable(vk3liste);			

				//Vk3 Liste in Tabelle schreiben
				for (POSITION pos = pWerkauftrag->get_vk3elementeListe().GetHeadPosition(); pos;)
				{
					Element* ele = dynamic_cast<Element*>(pWerkauftrag->get_vk3elementeListe().GetNext(pos));
					XASSERT(ele);
					if (ele)
					{
						vk3liste.addNew();
						waid.data				= werkauftrag->get_waid();
						eleid.data				= ele->get_id();
						artikelnr.data          = ele->get_artikelnr();

						if (ele->isEleRohr())								//Länge bei Rohr sonst Anzahl schreiben
							anzahllaenge.data	= (double)(ele->get_laenge() / 1000.0); //in Meter
						else
							anzahllaenge.data	= (double)ele->get_anzahl();

						dn.data					= ele->get_dn();							
						anschlussdn.data		= ele->get_anschlussDN();
						preis.data				= ele->get_festPreis();
						isvk3abr.data			= ele->get_constpreis();

						vk3liste.update();
					}
				}	


				if (rohr)
				{
					//Pulverliste Liste in Tabelle schreiben
					for (POSITION pos = rohr->get_pulverlaengenListe().GetHeadPosition(); pos;)
					{
						Element* ele = dynamic_cast<Element*>(rohr->get_pulverlaengenListe().GetNext(pos));
						XASSERT(ele);
						if (ele)
						{
							vk3liste.addNew();
							waid.data				= werkauftrag->get_waid();
							eleid.data				= ele->get_id();
							artikelnr.data          = ele->get_artikelnr();
							anzahllaenge.data		= (double)(ele->get_laenge() / 1000.0);		//Länge in Metern schreiben
							dn.data					= ele->get_dn();							
							anschlussdn.data		= ele->get_anschlussDN();
							preis.data				= ele->get_festPreis();
							isvk3abr.data			= ele->get_constpreis();

							vk3liste.update();
						}
					}

					//Passlaengen in Db schreiben
					for (POSITION pos = rohr->get_passlaengenListe().GetHeadPosition(); pos;)
					{
						Element* ele = dynamic_cast<Element*>(rohr->get_passlaengenListe().GetNext(pos));
						XASSERT(ele);
						if (ele)
						{
							vk3liste.addNew();
							waid.data				= werkauftrag->get_waid();
							eleid.data				= ele->get_id();
							artikelnr.data          = ele->get_artikelnr();
							anzahllaenge.data		= (double)ele->get_anzahl();			//Anzahl schreiben
							dn.data					= ele->get_dn();							
							anschlussdn.data		= ele->get_anschlussDN();
							preis.data				= ele->get_festPreis();
							isvk3abr.data			= ele->get_constpreis();

							vk3liste.update();
						}
			
					}
				}
			}
			CATCH(CDBException, e)
			{
				e->ReportError();
				if (m_pFortschritt)
					m_pFortschritt->ende();

				return false;
			}
			END_CATCH
		}
	}
	if (m_pFortschritt)
		m_pFortschritt->ende();
	return true;
}





void Server::set_Fortschritt(Fortschrittanzeige* pFortschritt)
{
	m_pFortschritt = pFortschritt;
}



bool Server::updateCadWerkauftrag()
{
	if ( modus != load || !werkauftrag)
		return false;

	CadWerkauftrag* cadw = (CadWerkauftrag*)werkauftrag;
	ConvCad2Xml cnv(dbhandle);
	cadw->set_xml_modell(cnv.loadxml(cadw->get_waid()));

	return cadw->get_xml_modell().length() ? true : false;
}


bool Server::updateLiefertermin()
{
	Leitkarte& leitkarte = werkauftrag->get_leitkarte();
	TRY
	{
		MXTable wa(dbhandle, "werkauftrag", true);
		DECLARE_COL(Long, waid, wa);
		DECLARE_COL(Date, wunschliefertermin, wa);

		CString select;			
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);

		if (wa.get_count() != 1)
		{
			CString str; str = getStammdaten().getText("IDS_INTERNER_FEHLER").c_str();
			AfxMessageBox(str);
			return false;
		}

		if (modus == save)
		{
			wa.edit();
			wunschliefertermin.set_datum(leitkarte.get_wunschliefertermin());			
			wa.update();
		}
		else
		{
			leitkarte.set_wunschliefertermin(wunschliefertermin.get_datum());
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


bool Server::updateSdNummer()
{
	
	Leitkarte& leitkarte = werkauftrag->get_leitkarte();
	TRY
	{
		MXTable wa(dbhandle, "werkauftrag", true);

		DECLARE_COL(Long, waid, wa);
		DECLARE_COL(BigInt, sdauftragsnr, wa);
		DECLARE_COL(Long, sdpositionsnr, wa);

		CString select;			
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);

		if (wa.get_count() != 1)
		{
			CString str; str = getStammdaten().getText("IDS_INTERNER_FEHLER").c_str();
			AfxMessageBox(str);
			return false;
		}

		if (modus == save)
		{
			wa.edit();
		
			sdauftragsnr.data  = leitkarte.get_sdAuftragsnr();
			sdpositionsnr.data  = leitkarte.get_sdPositionsnr();	
			
			wa.update();
		}
		else
		{
			leitkarte.set_sdAuftragsnr(sdauftragsnr.data);
			leitkarte.set_sdPositionsnr(sdpositionsnr.data);
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


bool Server::updateKcAuftragsdaten()
{
	
	Leitkarte& leitkarte = werkauftrag->get_leitkarte();
	MXTable wa(dbhandle, "werkauftrag");
	
	DECLARE_COL(Long, waid, wa);
	DECLARE_COL(Int, nl, wa);
	DECLARE_COL(String, pspelement, wa);
	DECLARE_COL(Long, sdpositionsnr, wa);
	DECLARE_COL(BigInt, kcauftragsnr, wa);
	DECLARE_COL(Long, kcposnr, wa);
	DECLARE_COL(Date, wunschliefertermin, wa);
	
	CString select;
	
	TRY
	{
		select.Format("waid = %i", werkauftrag->get_waid());
		wa.load((LPCSTR)select, (LPCSTR)"", true);
		if (wa.get_count() != 1) 
			return false;
	
		if (modus == save)
		{
			wa.edit();
			
			nl.data  = leitkarte.get_prj_nl();
			kcauftragsnr.data = _atoi64(leitkarte.get_kcAuftragsnr());
			sdpositionsnr.data = leitkarte.get_sdPositionsnr();
			kcposnr.data = leitkarte.get_kcPositionsnr();
			*pspelement.data  = leitkarte.get_prj_pspElement();
			wunschliefertermin.set_datum(leitkarte.get_wunschliefertermin());
	
			wa.update();
		}
	}
	CATCH(CDBException, e)
	{ 
		// Schwerwiegender Fehler aufgetreten, Daten sind u.U. korrupt
		AfxMessageBox(e->m_strError, MB_OK | MB_ICONERROR);
		return false;
	}
	END_CATCH
	return true;
}