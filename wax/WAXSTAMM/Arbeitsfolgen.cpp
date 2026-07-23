#include "stdafx.h"

#include "resource.h"


#include <mxodbc\mxodbc.h>


Arbeitsfolgen::Arbeitsfolgen() : StammDat("arbeitsfolgen", "", "code")
{

}

void Arbeitsfolgen::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int,	code,				tbl);
	DECLARE_COL_LANGUAGE_WAX(String,  txt, tbl);
	DECLARE_COL(String,	arbeitsplatz,		tbl);
	DECLARE_COL(Int,	posnr,				tbl);
	DECLARE_COL(Bool,	wasser,				tbl);
	DECLARE_COL(Bool,	co2tikko,			tbl);
	DECLARE_COL(Bool,	co2,				tbl);
	DECLARE_COL(Bool,	argon,				tbl);
	DECLARE_COL(Bool,	halter,				tbl);
	DECLARE_COL(Bool,	allgemein,			tbl);
	DECLARE_COL(Bool,	co2hdverteiler_dn65,tbl);
	DECLARE_COL(Bool,	co2hdverteiler_dn80,tbl);
	DECLARE_COL(Bool,	verteiler,			tbl);
	DECLARE_COL(Bool,	gewinderohr,		tbl);
	DECLARE_COL(Bool,	gewinderohrHD,		tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new Arbeitsfolgen::data;
		thedata->code			= code.data;
		thedata->txt			= *txt.data;
		thedata->arbeitsplatz	= *arbeitsplatz.data;
		thedata->posnr			= posnr.data;
		thedata->wasser			= wasser.data ? true:false;
		thedata->co2tikko		= co2tikko.data ? true:false;
		thedata->co2			= co2.data ? true:false;
		thedata->argon			= argon.data ? true:false;		
		thedata->halter			= halter.data ? true:false;
		thedata->allgemein		= allgemein.data ? true:false;
		thedata->co2hdverteiler_dn65 = co2hdverteiler_dn65.data ? true:false;
		thedata->co2hdverteiler_dn80 = co2hdverteiler_dn80.data ? true:false;
		thedata->gewinderohr	= gewinderohr.data ? true:false;
		thedata->gewinderohrhd	= gewinderohrHD.data ? true:false;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

Arbeitsfolgen::data::data() : wasser(false), co2tikko(false), co2(false), argon(false), halter(false), allgemein(false), co2hdverteiler_dn65(false), co2hdverteiler_dn80(false), gewinderohrhd(false), gewinderohr(false), posnr(0)
{

}


bool Arbeitsfolgen::data::get_allgemein() const
{
	return allgemein;
}

int Arbeitsfolgen::data::get_posnr() const
{
	return posnr;
}

bool Arbeitsfolgen::data::get_argon() const
{
	return argon;
}

bool Arbeitsfolgen::data::get_co2() const
{
	return co2;
}

bool Arbeitsfolgen::data::get_co2hdverteiler_dn65() const
{
	return co2hdverteiler_dn65;
}

bool Arbeitsfolgen::data::get_co2hdverteiler_dn80() const
{
	return co2hdverteiler_dn80;
}

bool Arbeitsfolgen::data::get_co2tikko() const
{
	return co2tikko;
}

int Arbeitsfolgen::data::get_code() const
{
	return code;
}

bool Arbeitsfolgen::data::get_halter() const
{
	return halter;
}

const CString& Arbeitsfolgen::data::get_txt() const
{
	return txt;
}

bool Arbeitsfolgen::data::get_wasser() const
{
	return wasser;
}

bool Arbeitsfolgen::data::get_gewinderohr() const
{
	return gewinderohr;
}

bool Arbeitsfolgen::data::get_gewinderohrhd() const
{
	return gewinderohrhd;
}

const CString Arbeitsfolgen::data::get_arbeitsplatz() const
{
	return arbeitsplatz;
}

