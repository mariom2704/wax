#include "stdafx.h"


#include ".\dbexcelueberagberobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DbExcelUeberagbeRobby::DbExcelUeberagbeRobby(void)
{
	m_anzahlSpalten = 0;
	
	//Tabellenname
	m_pTable = "Tabelle1";

}

DbExcelUeberagbeRobby::~DbExcelUeberagbeRobby(void)
{
}


/**
Spaltennamen, Dateinamen setzen und Exceldatei initialisieren und öffnen
*/
bool DbExcelUeberagbeRobby::initDatei(const CString &datei,const CStringArray &spalten)
{
	m_arSpalten.Copy(spalten);
	m_anzahlSpalten = spalten.GetSize();
	m_datei = datei;


	//Prüfen ob dll geladen ist
	if (!m_excel.isLoaded()) 
		return false;


	convertIntoChar(m_arSpalten,m_pSpalten);

	int type [] = { 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, 
		ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT, ExcelODBC::EXCELTEXT 
	};


	//vorhandene Exceldatei evt. löschen
	CFile file;
	if (file.Open(m_datei, CFile::modeRead | CFile::shareDenyNone)) 
	{
		file.Close();
		DeleteFile(m_datei);
	}


	//Exceldatei initialisieren
	m_excel.ExcelTableSetVar(m_datei.GetBuffer(),m_pTable);
	m_excel.ExcelTableSetColumnNames(m_pSpalten, m_anzahlSpalten);
	m_excel.ExcelTableSetColumnType(m_anzahlSpalten, type);
	m_excel.ExcelTableCreateTable();
	m_excel.ExcelTableOpenTable(m_datei.GetBuffer(),m_pTable);

	return true;
}

///Zeile initialisieren
void DbExcelUeberagbeRobby::initNewZeile()
{
	m_arZeile.RemoveAll();

	for (int i=0; i<m_anzahlSpalten; i++)
		m_arZeile.Add(".");
}

///Wert der Zeile hinzufügen
void DbExcelUeberagbeRobby::appendFeld(int spaltenNr,const CString &wert)
{
	if (m_arZeile.GetSize() <= spaltenNr)
		return;	

	m_arZeile[spaltenNr] = wert;
}

///Integer Wert als String in die Zeile schreiben
void DbExcelUeberagbeRobby::appendFeldIntToString(int spaltenNr,int wert)
{
	CString stTemp;
	stTemp.Format("%d",wert);
	
	appendFeld(spaltenNr,stTemp);
}

///Zeile in Exceldatei schreiben
void DbExcelUeberagbeRobby::writeZeile()
{
	convertIntoChar(m_arZeile,m_pZeile);
	m_excel.ExcelTableInsertInto(m_pZeile);
}


void DbExcelUeberagbeRobby::convertIntoChar(CStringArray &ar,char *pListe[])
{
	if (ar.GetSize() != m_anzahlSpalten)
		return;

	for (int i=0; i<m_anzahlSpalten; i++)
	{
		pListe[i] = ar.ElementAt(i).GetBuffer();
	}
	//pListe[m_anzahlSpalten] = NULL;

}

void DbExcelUeberagbeRobby::closeDatei()
{
	m_excel.ExcelTableCloseTable(m_datei.GetBuffer(), m_pTable);
}


///Kopfzeile einfügen
void DbExcelUeberagbeRobby::appendKopfZeile(const CString &spalte0,const CString &spalte1,
											const CString &spalte3,const CString &spalte4,
											const CString &spalte6,const CString &spalte7,
											const CString &spalte9,
											int spalte20)
{
	//Neue Zeile initialisieren
	initNewZeile();

	//Felder der Zeile füllen
	appendFeld(0,spalte0);
	appendFeld(1,spalte1);

	appendFeld(3,spalte3);
	appendFeld(4,spalte4);

	appendFeld(6,spalte6);
	appendFeld(7,spalte7);

	appendFeld(9,spalte9);

	appendFeldIntToString(20,spalte20); 

	//Zeile in Exceldatei schreiben
	writeZeile();
}

///Zeile für die Positionen einfügen
void DbExcelUeberagbeRobby::appendPosZeile(const CString &posNr,const CString &avNachbearbeitung,int anzPos,
						const CString &rohrAnfang,const CString &rohrEnde,
						int gesLaenge,int anzMuffen,
						int muffenAbst1,int muffenAbst2,int muffenAbst3,int muffenAbst4,int muffenAbst5,
						int muffenAbst6,int muffenAbst7,int muffenAbst8,int muffenAbst9,int muffenAbst10,
						int restMass,const CString &kappenfarbe,
						 double zeit, int counter)
{
	CString stTemp;

	//Neue Zeile initialisieren
	initNewZeile();

	//Felder der Zeile füllen
	appendFeld(0,posNr);
	appendFeld(1,avNachbearbeitung);
	appendFeldIntToString(2,anzPos);

	appendFeld(4,rohrAnfang); 
	appendFeld(5,rohrEnde);
	appendFeldIntToString(6,gesLaenge);
	appendFeldIntToString(7,anzMuffen);

	if (muffenAbst1 > 0)
		appendFeldIntToString(8,muffenAbst1);
	if (muffenAbst2 > 0)
		appendFeldIntToString(9,muffenAbst2);
	if (muffenAbst3 > 0)
		appendFeldIntToString(10,muffenAbst3);
	if (muffenAbst4 > 0)
		appendFeldIntToString(11,muffenAbst4);
	if (muffenAbst5 > 0)
		appendFeldIntToString(12,muffenAbst5);
	if (muffenAbst6 > 0)
		appendFeldIntToString(13,muffenAbst6);
	if (muffenAbst7 > 0)
		appendFeldIntToString(14,muffenAbst7);
	if (muffenAbst8 > 0)
		appendFeldIntToString(15,muffenAbst8);
	if (muffenAbst9 > 0)
		appendFeldIntToString(16,muffenAbst9);
	if (muffenAbst10 > 0)
		appendFeldIntToString(17,muffenAbst10);

    appendFeldIntToString(18,restMass);

	appendFeldIntToString(20,counter);

	appendFeld(21,kappenfarbe);

	CString strZeit;
	strZeit.Format("%f", zeit);
	int iWo = strZeit.Find(".");
	if(iWo > -1)
	{
		strZeit = strZeit.Mid(0, iWo+4);
	}
	appendFeld(26, strZeit);

	//Zeile in Exceldatei schreiben
	writeZeile();
}
