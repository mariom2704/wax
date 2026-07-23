#include "StdAfx.h"
#include ".\befehlreaddb.h"
#include "MxDatenMatrix.h"



BefehlReadDb::BefehlReadDb(void)
{
	m_dbhandle = NULL;
	m_opentab  = NULL;
}

BefehlReadDb::~BefehlReadDb(void)
{
	delete m_opentab;
}


bool BefehlReadDb::start()
{

/*
	if (!openDb("localhost->waxserver"))
		return false;

	
	int artNr = 0;
	m_tabArtikelstamm = "sapmaterialstamm";

	stuecklistewa(artNr);
*/
	//////////////////

	// Accsess DB
	if (!openDb(_T("D:\\work\\Wax\\DATENBANKEN\\WaxStamm.mdb")))
		return false;

	m_tabArtikelstamm = _T("sap_artikelstamm");
	int artNr = 773222;

	sapmaterialstamm(artNr);

	return true;
}

bool BefehlReadDb::startJoin1()
{
	//if (!openDb(_T("d:\\programme\\spanx\\spanxx.mdb")))
	//	return false;

	test_write();

	return true;
}

void BefehlReadDb::test_write()
{
	int iDummy;
	double dDummy;
	CString stDummy;
	
	MXDatabase db;
	db.doOpen(_T("ODBC;DSN=Spanx;"));

	//Tabelle definieren
	MXTable artikel((DWORD_PTR)&db, _T("Password"), false);
	
	DECLARE_COL(String,	Name,			artikel);
	DECLARE_COL(String,	Password,			artikel);
//	DECLARE_COL(String,	Gruppe,			artikel);
	

	int waidNr = 3333;

	CString select = _T("Name = 'SYSTEM'");
TRY
{
	artikel.load(NULL, NULL, false);
	//artikel.addNew();
	artikel.edit();
	*Name.data = "0815";
	*Password.data = "z";
//	*Liste.data = "Z";
	
	artikel.update();
	artikel.close();
}
CATCH(CDBException, e)
{
	e->ReportError();
}
END_CATCH
}


bool BefehlReadDb::startJoin2()
{
	if (!openDb(_T("odwax->waxserver")))
		return false;

	joinAbfrage2();

	return true;
}


bool BefehlReadDb::openDb(const CString &dbName)
{
	if (m_dbhandle)
		return true;
	
	DBConnection con;
	con.set_serverdbentry(dbName);
	CString conStr = con.get_serverdb();


	if (!m_opentab)
		m_opentab = new MXTable(conStr, _T("werkauftrag"));

	m_dbhandle = m_opentab->get_Db();

	if (!m_dbhandle)
		return false;

	return true;
}

bool BefehlReadDb::stuecklistewa(int &artNr)
{
	int iDummy;
	double dDummy;
	CString stDummy;
	
	//Tabelle definieren
	MXTable stueckliste(m_dbhandle, _T("stuecklistewa"), true);
	
	DECLARE_COL(Int,	waid,			stueckliste);
	DECLARE_COL(Int,	elemid,			stueckliste);
	DECLARE_COL(Double,	anzahllaenge,	stueckliste);
	DECLARE_COL(Int,	artikelnr,		stueckliste);
	DECLARE_COL(Int,	dn,				stueckliste);
	DECLARE_COL(Int,	anschlussdn,	stueckliste);
	DECLARE_COL(Double, stueckpreis,	stueckliste);
	DECLARE_COL(String,	bez,			stueckliste);


	int waidNr = 148544;

	CString select;
	select.Format(_T("waid = %i"), waidNr);

	stueckliste.load((LPCTSTR)select, (LPCTSTR)"", false);
	
/*
	int anzahl = stueckliste.get_count();
			
	if (!anzahl)
		return false;
*/		

	for(;!stueckliste.isEof(); stueckliste.moveNext())
	{
		iDummy = waid.data;
		iDummy = elemid.data;
		dDummy = anzahllaenge.data;
		stDummy = *bez.data;

		artNr = artikelnr.data;

		//Artikeldaten lesen
		sapmaterialstamm(artNr);
	}
	return true;
}

bool BefehlReadDb::sapmaterialstamm(int artnr)
{
	//Tabelle definieren
	MXTable artikelstamm(m_dbhandle, (LPCTSTR)m_tabArtikelstamm, false);
	
	DECLARE_COL(String,	materialbe,	artikelstamm);
	DECLARE_COL(String,	me,			artikelstamm);
	DECLARE_COL(Double,	preis,		artikelstamm);


	CString select;
	select.Format(_T("materialnu = %i"), artnr);

	artikelstamm.load((LPCTSTR)select, (LPCTSTR)"", false);
	
	
	/*
	int anzahl = artikelstamm.get_count();
			
	if (!anzahl)
		return false;
	*/

	double dDummy;
	CString stDummy1,stDummy2;

	dDummy   = preis.data;
	stDummy1 = *materialbe.data;
	stDummy2 = *me.data;

	return true;
}

//Sollte nur für kleine Ergebnismengen genommen werden, da das Ergebnis komplett in eine Liste kopiert wird
bool BefehlReadDb::joinAbfrage()
{

	MXTable abfrage(m_dbhandle, _T("status"));

	int waidNr1 = 149837,
		//waidNr2 = 180000; 
		waidNr2 = 149840;

	CString sql;
	sql.Format(_T("SELECT b.stueckpreis, a.bauort,a.bezeichnung,  b.elemid  ")
		       _T("FROM werkauftrag a, stuecklistewa b ")
			   _T("where a.waid = b.waid ")
			   _T("and a.waid > %i and a.waid < %i order by a.waid"), waidNr1,waidNr2);
			  
	MxDatenMatrix result;

/*
	if (!abfrage.executeSQL(sql,result))
		return false;

	int anzahl = result.getAnzahlZeilen();

	
	double dDummy;
	CString stDummy1,stDummy2;
	int    iDummy;

	MxDatenZeile *mxZeile;
	for (mxZeile = result.getStartZeile(); mxZeile; mxZeile = result.getNextZeile())
	{
		//Typen müssen mit Select Statement übereinstimmen!
		mxZeile->getWertAsDouble(0,dDummy);
		mxZeile->getWert(1,stDummy1);
		mxZeile->getWert(2,stDummy2);
		mxZeile->getWertAsInt(3,iDummy);

		TRACE("\n%.2f | %s | %s | %i",dDummy,stDummy1,stDummy2,iDummy);
	}
*/
	return true;
}



//Verarbeitungsgeschwindigkeit ist schneller als joinAbfrage1()
bool BefehlReadDb::joinAbfrage2()
{
	//Tabelle definieren
	MXTable abfrage(m_dbhandle);
	abfrage.set_sql(_T("SELECT a.waid, a.bauort, a.bezeichnung, b.stueckpreis, b.elemid  ")
		       _T("FROM werkauftrag a inner join stuecklistewa b on ")
			   _T("a.waid = b.waid "));
			   
	
	//Felder MÜSSEN mit den Feldern im Sql-Statement übereinstimmen!
	DECLARE_COL(Int,	waid,		 abfrage);
	DECLARE_COL(String,	bauort,		 abfrage);
	DECLARE_COL(String,	bezeichnung, abfrage);
	DECLARE_COL(Double,	stueckpreis, abfrage);
	DECLARE_COL(Int,	elemid,		 abfrage);

	int waidNr1 = 160020,
		waidNr2 = 160040;

	CString sql;
	sql.Format(_T("a.waid > %i and a.waid < %i order by a.waid"), waidNr1,waidNr2);
			  
	TRY
	{
		abfrage.load(sql);
	}
	CATCH(CDBException, e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH
	
	
	
	double dDummy = 0.0;
	CString stDummy1,stDummy2;
	int    iDummy = 0;


	for(;!abfrage.isEof(); abfrage.moveNext())
	{
		
		dDummy   = stueckpreis.data;
		stDummy1 = *bauort.data;
		stDummy2 = *bezeichnung.data;
		iDummy   = elemid.data;

		TRACE(_T("%i | %.2f | %s | %s | %i\n"),waid.data, dDummy,stDummy1,stDummy2,iDummy);

	}

	return true;
}
