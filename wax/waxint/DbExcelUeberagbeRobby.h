#pragma once
//#include "..\waxprj\dbbase.h"
#include "ExcelODBC.h"

/**
Excelschnittstellendatei für die Schweißstrassen erstellen
*/
class DbExcelUeberagbeRobby : public WaxBase
{
	
public:

	enum
	{
		eZEILEN_SPALTEN = 27,
	};

	DbExcelUeberagbeRobby(void);
	virtual ~DbExcelUeberagbeRobby(void);

	bool initDatei(const CString &datei,const CStringArray &spalten);	//Exceldatei Spaltennamen setzen

	void initNewZeile();									//neue Zeile initialisieren
	void appendFeld(int spaltenNr,const CString &wert);		//Werte in die Zeile schreiben
	void appendFeldIntToString(int spaltenNr,int wert);		//Integer Wert als String in die Zeile schreiben
	void writeZeile();										//Zeile in Exceldatei schreiben

	void closeDatei();										//Exceldatei schließen


	void appendKopfZeile(const CString &spalte0,const CString &spalte1,
						 const CString &spalte3,const CString &spalte4,
						 const CString &spalte6,const CString &spalte7,
						 const CString &spalte9,
						 int spalte20);

	void appendPosZeile(const CString &posNr,const CString &avNachbearbeitung,int anzPos,
						const CString &rohrAnfang,const CString &rohrEnde,
						int gesLaenge,int anzMuffen,
						int muffenAbst1,int muffenAbst2,int muffenAbst3,int muffenAbst4,int muffenAbst5,
						int muffenAbst6,int muffenAbst7,int muffenAbst8,int muffenAbst9,int muffenAbst10,
						int restMass,const CString &kappenfarbe,
						double zeit, int counter);

private:
	CStringArray m_arSpalten,
				 m_arZeile;

	CString		 m_datei;

	int			 m_anzahlSpalten;

	char*		 m_pTable;

	ExcelODBC	 m_excel;


	void convertIntoChar(CStringArray &ar,char *pListe[]);

	

	char  *m_pSpalten[eZEILEN_SPALTEN],
		  *m_pZeile[eZEILEN_SPALTEN];
};
