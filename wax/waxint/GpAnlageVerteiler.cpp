#include "stdafx.h"


#include "resource.h"
#include "GpAnlageVerteiler.h"


GpAnlageVerteiler::GpAnlageVerteiler(MxCObList *auftraege)
	: GpAnlageSaegeNut(auftraege)
{
	fieldDummy = ".";
}

GpAnlageVerteiler::~GpAnlageVerteiler()
{
}



bool GpAnlageVerteiler::createVerteilerXls(bool bDir_verteiler_free)
{
	//Werte erzeugen
	if (!createWerte())
		return false;

	//Exceldatei schreiben
	if (!createExcelDatei())
		return false;

	if (!writeLeitkarte())
		return false;

	if (!writeVerteilerdaten())
		return false;

	if (!writeRohrdaten())
		return false;


	Werkauftrag *werkauftrag = getFirstAuftrag();
	if (!werkauftrag)
		return false;

	if(bDir_verteiler_free)
	{
		MxString stNull = "000000";
		MxString stWanr = MxString("{0}").arg(werkauftrag->get_status().get_wanr());
		if (stWanr.length() < stNull.length())
			stWanr = MxString("{0}{1}").arg(stNull.left(stNull.length() - stWanr.length())).arg(stWanr);
				
		
		MxString pfad = WaxSys::para_str(DIR_VERTEILER_FREE);
		MxString strDateidestination = MxString("{0}{1}.xls").arg(pfad).arg(stWanr);

		CopyFile(datei, strDateidestination, FALSE );
	}	
	
	return true;
}

/**
Exceldatei erzeugen
*/
bool GpAnlageVerteiler::createExcelDatei()
{
	Werkauftrag *werkauftrag = getFirstAuftrag();
	if (!werkauftrag)
		return false;

	//Dateiname
	MxString stNull = "000000";
	MxString stWanr = MxString("{0}").arg(werkauftrag->get_status().get_wanr());
	if (stWanr.length() < stNull.length())
		stWanr = MxString("{0}{1}").arg(stNull.left(stNull.length() - stWanr.length())).arg(stWanr);
			
	
	MxString pfad = WaxSys::para_str(DIR_VERTEILER_FREE_USB);
	datei = MxString("{0}{1}.xls").arg(pfad).arg(stWanr);
	

	CFile file;
	if (file.Open(datei.getString(), CFile::modeRead | CFile::shareDenyNone)) 
	{
		file.Close();

		if (AfxMessageBox(getStammdaten().getText("IDS_VERT_TAB_LOESCHEN").c_str(), MB_YESNO | MB_ICONQUESTION) != IDYES)
			return false;

		::DeleteFile(datei.getString());
	}


	return true;
}
	
/**
Tab Leitkarte schreiben
*/
bool GpAnlageVerteiler::writeLeitkarte()
{		
	//Tab Leitkarte anlegen
	MxString table = "Leitkarte";
	char* columns[21] = { 
		"WA_Nr",			"PSP_Element",      "SD_Aufnr",		     "SD_Posnr",		"SD_Matnr", 
		"Bauabschnitt", 	"Auftrbez",         "PLZ",		         "Ort",				"NL",		
		"NLSachbearbeiter",	"ODSachbearbeiter", "Erstellungsdatum",	 "Freigabedatum",	"Bearbeitungsdatum", 
		"Lieferdatum",		"MB_Datum",			"Geprüft",			 "Abgearbeitet",	"Geändert",    
		NULL 
	};
	int type [] = { 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT,	ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT
	};
	int quantity = 20;	
	createTab(excel, datei, table, columns, type, quantity);


	//Werte
	MxString wanr = MxString("{0}").arg(kopfData.wanr);
	MxString sdNr = MxString("{0}").arg((double)kopfData.sdAuftrag,0);
	MxString sdMatnr = MxString("{0}").arg(kopfData.sdMaterialNr);
	MxString sdPos = MxString("{0}").arg(kopfData.sdPos);

	char *values[21];
	values[ 0] = wanr.getString();
	values[ 1] = fieldDummy.getString();
	values[ 2] = sdNr.getString();
	values[ 3] = sdPos.getString();
	values[ 4] = sdMatnr.getString();
	values[ 5] = fieldDummy.getString();
	values[ 6] = fieldDummy.getString();
	values[ 7] = fieldDummy.getString();
	values[ 8] = kopfData.ort.getString();
	values[ 9] = fieldDummy.getString();
	values[10] = fieldDummy.getString();
	values[11] = fieldDummy.getString();
	values[12] = fieldDummy.getString();
	values[13] = fieldDummy.getString();
	values[14] = fieldDummy.getString();
	values[15] = fieldDummy.getString();
	values[16] = fieldDummy.getString();
	values[17] = fieldDummy.getString();
	values[18] = fieldDummy.getString();
	values[19] = fieldDummy.getString();

	// Table mit Daten fuellen			
	writeRow(excel, datei, table, values);

	return true;
}

/**
Exceltab Verteilerdaten erstellen. Dabei die Sortierung der Sägenutanlage beachten. Die
Liste saegeNutRohrListe ist sortiert.
*/
bool GpAnlageVerteiler::writeVerteilerdaten()
{
	//Tab erzeugen
	MxString table = "Verteilerdaten";
	char* columns[11] = { 
		"PosNr", "Anzahl", "Länge", "DN", "DIN", "Kfarbe", "AnzUpos", 
		"Geprüft", "Abgearbeitet", "Geändert",  NULL    
	};
	int type [] = { 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT
	};
	int quantity = 10;	
	createTab(excel, datei, table, columns, type, quantity);


	char *values[11];
	MxString posnr,anzahl,laenge,dn,din,anzUpos;
	//Daten schreiben

	//Sortierte Liste als Grundlage nutzen
	list<DbAnlage::saegeNutRohre>::iterator iterNr = saegeNutRohrListe.begin();
	while (iterNr != saegeNutRohrListe.end())
	{
		list<DbAnlage::verteilerRohre>::iterator iterVr = verteilerRohrListe.begin();
		while (iterVr != verteilerRohrListe.end())
		{
			//Datensatz suchen
			if (iterNr->waid != iterVr->waid)
			{
				iterVr++;
				continue;
			}

			posnr = MxString("{0}").arg(iterVr->posNr);
			anzahl = MxString("{0}").arg(iterVr->anzahl);
			laenge = MxString("{0}").arg(iterVr->laenge);
			dn = MxString("{0}").arg(iterVr->dn);
			din = MxString("{0}").arg(iterVr->din);
			anzUpos = MxString("{0}").arg(iterVr->anzUpos);

			values[0] = posnr.getString();
			values[1] = anzahl.getString();
			values[2] = laenge.getString();
			values[3] = dn.getString();
			values[4] = din.getString();
			values[5] = iterVr->kFarbe.getString();
			values[6] = anzUpos.getString();
			values[7] = fieldDummy.getString();
			values[8] = fieldDummy.getString();
			values[9] = fieldDummy.getString();

			writeRow(excel, datei, table, values);
			
			break;
		}

		iterNr++;
	}

	return true;
}

bool GpAnlageVerteiler::writeRohrdaten()
{
	//Tab anlegen
	MxString table = "Rohrdaten";
	char* columns[23] = { 
		"PosNr",    "UposNr",       "LfdNr",     "AnzRohre",  "AnzElemente", 
		"BeschrLi", "BeschrRe",     "DN",        "DIN",       "Länge", 
		"Anfang",   "Ende",         "ElementID", "ElementDN", "AbsAbst",     
		"Winkel",   "AnzSoll",      "AnzIst",    "Geprüft",   "Abgearbeitet", 
		"Geändert", "ElementLänge",	NULL    
	};
	int type [] = { 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT
	};
	int quantity = 22;	
	createTab(excel, datei, table, columns, type, quantity);

	char *values[23];
	MxString posnr,uposnr,lfdnr,anzrohre,anzelemente,dn,din,laenge,eleid,absabs,winkel,anzsoll,elelaenge,eledn,
		     beschLi,beschRe,anfang,ende,dummyPos;

	//Daten schreiben

	//Sortierte Liste als Grundlage nutzen
	list<DbAnlage::saegeNutRohre>::iterator iterNr = saegeNutRohrListeLegacy.begin();
	while (iterNr != saegeNutRohrListeLegacy.end())
	{
		int count = 0;
		bool isSet = false;
		while(true)
		{
			//Datensatz suchen
			bool found = false;			
			list<DbAnlage::verteilerNutAbgang>::iterator iterVn = verteilerNutAbgangListe.begin();	
			while (iterVn != verteilerNutAbgangListe.end())
			{	
				if (iterNr->waid == iterVn->waid && iterVn->counter == count + 1)
				{
					found = true;
					break;
				}

				iterVn++;
			}
		
			if (!found)
				break;

			//bei neuer Position Schleife verlassen, damit die gleiche Sortierung wie in saegeNutRohrListe erreicht wird
			if (count > 0 && posnr != iterVn->posNr)
				break;

			if (count == 0)
			{
				posnr = dummyPos = iterVn->posNr;
				uposnr = MxString("{0}").arg(iterVn->uPosNr);
				lfdnr = "1"; 
				anzrohre = MxString("{0}").arg(iterVn->anzRohre);
				anzelemente  = MxString("{0}").arg(iterVn->anzElemente);
				dn  = MxString("{0}").arg(iterVn->dn);
				din  = MxString("{0}").arg(iterVn->din);	
				beschLi = iterVn->beschrLi.getString();
				beschRe = iterVn->beschrRe.getString();
				laenge  = MxString("{0}").arg(iterVn->laenge);
				anfang = iterVn->anfang.getString();
				ende = iterVn->ende.getString();
				anzsoll  = MxString("{0}").arg(iterVn->anzSoll);
			}
			else
			{
				dummyPos= uposnr= lfdnr= anzrohre= anzelemente= dn= din= beschLi= beschRe= laenge= anfang= ende= anzsoll= fieldDummy;
			}
		
			eleid  = MxString("{0}").arg(iterVn->elementId);
			absabs  = MxString("{0}").arg(iterVn->absAbst);
			winkel  = MxString("{0}").arg(iterVn->winkel);
			elelaenge  = MxString("{0}").arg(iterVn->elementLaenge);
			eledn = MxString("{0}").arg(iterVn->elementDn);

			values[0] = dummyPos.getString();
			values[1] = uposnr.getString();
			values[2] = lfdnr.getString();
			values[3] = anzrohre.getString();
			values[4] = anzelemente.getString();
			values[5] = beschLi.getString();
			values[6] = beschRe.getString();
			values[7] = dn.getString();
			values[8] = din.getString();
			values[9] = laenge.getString();
			values[10] = anfang.getString();
			values[11] = ende.getString();
			values[12] = eleid.getString();
			values[13] = eledn.getString();
			values[14] = absabs.getString();
			values[15] = winkel.getString();
			values[16] = anzsoll.getString();
			values[17] = fieldDummy.getString();
			values[18] = fieldDummy.getString();
			values[19] = fieldDummy.getString();
			values[20] = fieldDummy.getString();
			values[21] = elelaenge.getString();
			
			isSet = true;
			writeRow(excel, datei, table, values);

			count++;

		}
		
		//Leerzeilen
		if (isSet)
			writeDummyZeile(Verteiler::A_MAX_ANZAHL_STDELEMENTE_PROROHR - count, excel, datei, table, values, quantity); 

		iterNr++;
	}



	return true;
}

bool GpAnlageVerteiler::writeDummyZeile(int anzahl, ExcelODBC &excel, const MxString &datei, const MxString &table, char* values[], int anzValue)
{
	//Dummywerte setzen
	for (int i=0; i<anzValue; i++)
	{
		values[i] = fieldDummy.getString();
	}

	//Werte schreiben
	for (int k=0; k<anzahl; k++)
	{
		writeRow(excel, datei, table, values);
	}

	return true;
}

bool GpAnlageVerteiler::createTab(ExcelODBC &excel, const MxString &datei, const MxString &table, char* columns[], int type [], int anzahlSpalten)
{
	excel.ExcelTableSetVar(datei.getString(), table.getString());
	excel.ExcelTableSetColumnNames(columns, anzahlSpalten);
	excel.ExcelTableSetColumnType(anzahlSpalten, type);
	excel.ExcelTableCreateTable();
	excel.ExcelTableCloseTable(datei.getString(), table.getString());
	return true;
}


bool GpAnlageVerteiler::writeRow(ExcelODBC &excel, const MxString &datei, const MxString &table, char* values[])
{
	excel.ExcelTableOpenTable(datei.getString(), table.getString());
	excel.ExcelTableInsertInto(values);
	excel.ExcelTableCloseTable(datei.getString(), table.getString());
	return true;
}