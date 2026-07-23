#include "stdafx.h"

#include <mxodbc\mxodbc.h>


RohrstutzenBrennzugabe::RohrstutzenBrennzugabe() : StammDat("rohrstutzen_brennzugabe", "", "")
{

}

void RohrstutzenBrennzugabe::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, verteilerDN, tbl);
	DECLARE_COL(Int, stutzenDN, tbl);
	DECLARE_COL(Int, brennzugabe, tbl);
	
	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new RohrstutzenBrennzugabe::data;
		thedata->VerteilerDN = verteilerDN.data;
		thedata->StutzenDN = stutzenDN.data;
		thedata->Brennzugabe = brennzugabe.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

RohrstutzenBrennzugabe::data::data()
{
	VerteilerDN = 0;
	StutzenDN = 0;
	Brennzugabe = 0;
}

