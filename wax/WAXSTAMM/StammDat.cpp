#include "stdafx.h"
#include <mxodbc\mxodbc.h>


#include "StammDat.h"

DWORD StammDat::db = NULL;

StammDat::~StammDat()
{
	unload();
}


const string& StammDat::get_tbl() const
{
	return tbl;
}

const string& StammDat::get_select() const
{
	return select;
}

const string& StammDat::get_order() const
{
	return order;
}



//Tabelleninitialisierung wird in abgeleiteten Klassen überschrieben
StammDat::StammDat(const string& table, const string& sel, const string& ord    )
{
	tbl = table;
	select = sel;
	order = ord;
}

const CObList& StammDat::get_lst() const
{
	return lst;
}

CObList& StammDat::get_lst()
{
	return lst;
}


void StammDat::load(const string& db)
{
	UNREFERENCED_PARAMETER(db);
	XASSERT(FALSE);
}

//Stammdaten löschen
void StammDat::unload()
{
	for (POSITION pos = lst.GetHeadPosition(); pos;)
		delete lst.GetNext(pos);
	lst.RemoveAll();
}


DWORD StammDat::get_database(const string& name)
{
	if (!db)
	{
		MXDatabase *stammdb = new MXDatabase;
		stammdb->doOpen(name.c_str());
		stammdb->refcounter++; // Damit das Objekt nicht von einem Destruktor von MXTable gelöscht wird
		db = (DWORD)stammdb;
	}
	return db;
}

void StammDat::reset_database()
{
	if (db)
	{
		MXDatabase* stammdb = (MXDatabase*)db;
		delete stammdb;
		db = NULL;
	}
}


void StammDat::set_tbl(const string& tblname)
{
	tbl = tblname;
}