#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_EXCEL_3CF3979E034B_INCLUDED
#define _INC_EXCEL_3CF3979E034B_INCLUDED

#include <afxtempl.h>

#include <afxcoll.h>

#include "ExcelODBC.h"

///Erstellt die Exceldateien zur Ansteuerung der Fertigungsmaschinen
class WAXINT Excel : 
public ExcelODBC  
{
public:
	/// IDs der verschiedenen Aktionen
	enum EXCELAKTIONEN 
	{
		AUFTRAGSTABELLE = 1,
				
		VERTEILERANLAGE_ALT = 4,
		
		STRANGROHRANLAGE_1 = 5,
		
		STRANGROHRANLAGE_2 = 6,
		
		STRANGROHRANLAGE_3 = 7,
		
		STRANGROHRANLAGE_4 = 8,
	
		PREISTEST = 9,

		ZUSAMMENFASSUNG_WERKAUFTRAEGE = 10, 

		SAEGENUTANLAGE = 11,

		VERTEILERANLAGE = 12
	};

	/// Werkauftragstypen
	enum WATYP 
	{
		UNKNOWNX,
		
		VERT,
		
		STRANG,
		
		STATVERT,
	
		GROHR,

		STDVERT,

		HL,

		PPBLEIT,

		PUMPVERT,

		ENTLEIT,

		FORM,

		APAX
	};

public:
	void set_walist(MxCObList* walistLocal);

private:
	vector<Element*> elevector;

	bool checkLaengeStdElemente(Rohrleitung* vert);
	
	bool ckeckElevector(Element* ele);

	


	int currentAction;

	bool neuwa;

	Werkauftrag* werkauftrag;

	MxCObList walist;

	CObList wasublist;

	CObList sammelauftraege;

	CObList rohrlist;

	CWordArray aktionen;

	CArray<double,double> zeitSegment;

	CArray<double,double> zeitPulvern;

	CArray<double,double> zeitStrangrohrStrasse;

	CArray<double, double> zeit; 

	CArray<int,int> watyp;

	CArray<int,int> muffenID;

	

	CArray<int,int> muffenDN;

	CArray<int,int> muffenDN1;

	CArray<int,int> muffenDN2;
	
	CArray<int,int> muffenDN3;
	
	CArray<int,int> muffenDN4;
	
	CArray<int,int> muffenDN5;

	CArray<int,int> muffenAbstand1;

	CArray<int,int> muffenAbstand2;

	CArray<int,int> muffenAbstand3;

	CArray<int,int> muffenAbstand4;

	CArray<int,int> muffenAbstand5;

	CArray<int,int> muffenAbstand6;

	CArray<int,int> muffenAbstand7;

	CArray<int,int> muffenAbstand8;

	CArray<int,int> muffenAbstand9;

	CArray<int,int> muffenAbstand10;

	CArray<int,int> anzahl;

	CArray<int,int> anzRohrstraenge;

	CArray<int,int> anzRohrstuecke;

	CArray<int,int> dnvl;

	CArray<int,int> laenge;

	CArray<int,int> rohrlaengen;
	
	CArray<int,int> rohrQuali;

	CArray<int,int> rohrKFarbe;

	CArray<int,int> rohrAnfang;

	CArray<bool,bool> isdoubledisc;

	CArray<int,int> rohrEnde;

	CArray<int,int> muffenAnzahl;

	

	double ZeitGesamt;

	CArray<int,int> restmass;

	CArray<bool,bool> avnachbearbeitung;

	CStringArray quali;

	CStringArray m_Kolliliste;

	CStringArray UwieGUID;

	CStringArray bearbdat;

	CStringArray wan; 

	CStringArray pspel; 

	CStringArray sdAuf; 

	CStringArray kcAuf; 

	CStringArray sdPos; 

	CStringArray kcPos; 

	CStringArray sdMat; 

	CStringArray bauabschnitt; 

	CStringArray auftrbez; 

	CStringArray plz; 

	CStringArray ort; 

	CStringArray nl; 

	CStringArray nlsachb; 
	
	CStringArray odsachb;
	
	CStringArray erstelldat; 
	
	CStringArray freidat; 
	
	CStringArray liefdat;
	
	CStringArray wliefdat; 

	CStringArray beschichtung;
	
	CStringArray posNr;

	CStringArray beschrL;
	
	CStringArray beschrR;

	CStringArray guidline; 

	CStringArray kappenfarbe;

	CStringArray scheibe;

	int createAuftragsdata();

	bool createZusammenfassungAuftraegeRobbi();

	CString createAuftragstabelle(CString fname);

	int createSaegenutdata();

	bool createVerteilerdata();

	
	int createStrangrohrdata();

	void createStrangrohrtabelle();

	void createLeitkartedata();

	void createRohrdata(int maxAnzStdele = 0);

	void createRohrdataStdvert();

	void createRohrdataStrang(int maxAnzStdele);

	bool setTableLeitkarte(char fname[], bool createTable);

	bool setTableVerteilerdaten(int index, char fname[], bool createTable);
	
	bool setTableRohrdaten(int index, char fname[], bool createTable);

	/// Rohre nach Nennweite & Laenge sortieren
	void sortListe(CObList& liste, bool aufsteigend = false);

	/// Werkauftraege sortieren
	void sortWerkauftraege(int actionid);

	/// Insert Leitkartendaten in Grohr/Saegenut-Datei
	CString insertKopfsatz(Werkauftrag *wa);

	/// Gibt die erste Zeile oder max. 80 Zeichen eines String zurück
	const CString get_first_line_of(const CString&  str) const;

public:

	Excel();

	Excel(Werkauftrag* wa);

	virtual ~Excel();

	const CWordArray& get_aktionen();

	void makeAction(int id);

	void setExcelTableOfficeVersion2010(bool);

	Werkauftrag* get_werkauftrag();

	void initAction();

	CString getName(int id);

	void set_currentAction(int value);

	void set_werkauftrag(Werkauftrag* value);

	/// Kopiert die Liste mit (Unter)Auftraegen
	void set_walist(CObList& value);

	/// Oeffnet das angegebene File mit der Standardprogramm
	void openFile(CString fname, CString text);

	/// Erstellt ein File mit dem angegebenen Text
	bool writeFile(CString fname, CString text);

	/// Ermittelt Innendurchmesser od. Wandstaerke je nach Index
	double get_Stammdaten(int id, int dn, int index);

	void set_wasublist(CObList& value);

	const CObList& get_sammelauftraege() const;

	void set_walist();
};

#endif /* _INC_EXCEL_3CF3979E034B_INCLUDED */
