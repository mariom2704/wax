
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_SAP_3CD91A0B00AE_INCLUDED
#define _INC_SAP_3CD91A0B00AE_INCLUDED

#include "..\waxprj\Ausgaben.h"

class WAXINT Sap : 
public Ausgaben  
{
public:
	enum SAPAKTIONEN 
	{
		WAXTOSAP = 1,

		SAPTOWAX_FILE = 2,

		SAPTOWAX_ALL = 3,

		DELETEWAXFROMSAP = 4
	};

	enum WRITEFILE 
	{
		FILEERROR = 1,

		ADDTOFILE = 2,

		NEWFILE = 3
	};

private:

	double getPreis();

	bool getText(const CString &text, int offset, int &pos, CString &result, bool trim = true);

	bool createOK;

	CStringArray wanummer; 

	//Gesamtliste Rohre
	CObList rohrlist;

	CObList rohrstutlist;

	CObList rohrtemplist;

	CObList rohrendlist;

	CStringArray posNr;

	CStringArray hinweis;

	CArray<int,int> anzahl;

	CArray<int,int> dnvl;

	CArray<int,int> laenge;

	CArray<double,double> gewicht;

	void setLeitkarte(CString wa);

	CString viewSapfile(CString fname);

	CString readSapfile(CString fname);

	void createRohrdata();

	void createRohrstutzendata();

	void setRohrdata();
	
	void setRohrtemplist();
	
	void setRohrendlist();

	void setRohrstutzenlaenge();

	void setRohreNeu();
	
	void setRohrstutzenNeu();

	int createSapdata();
	
	bool createSapfile(CString fname, double &preis);

	///Schreibt die Daten eines Auftrages in die Saptabelle 
	void updateSapTabelle(double price);

	void deleteWaxFromSap();

	bool readAuftraege(CString fn);

	int deleteAddAuftrag(CString fn, CString txt, CString wan);

	void addAuftrag(CString fn, CString txt);

	const CString format_arbeitsfolgen(CObList& afos, const CString& wanummer);

public:
	bool writeFile(CString fn, CString txt, CString wan);

	Sap();

	virtual ~Sap();

	virtual void initAusgaben();

	virtual const CStringArray& makeAusgabe(int id = 0);

	// Liefert den Namen (fuer Menueeintrag) fuer eine Ausgabe.
	virtual CString getName();

	inline const bool get_createOK() const; 

protected:
};

inline const bool Sap::get_createOK() const
{
	return createOK;
}

#endif /* _INC_SAP_3CD91A0B00AE_INCLUDED */
