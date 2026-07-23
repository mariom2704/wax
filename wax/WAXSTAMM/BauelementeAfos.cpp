#include "stdafx.h"


#include <mxodbc\mxodbc.h>


BauelementeAfos::BauelementeAfos(): StammDat("bauelemente_arbeitsfolgen", "", "")
{
}

void BauelementeAfos::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	

	DECLARE_COL(Int, produkttyp,	tbl);
	DECLARE_COL(Int, elemid,	tbl);
	DECLARE_COL(Int, code1,		tbl);
	DECLARE_COL(Int, code2,		tbl);
	DECLARE_COL(Int, code3,		tbl);
	DECLARE_COL(Int, code4,		tbl);
	DECLARE_COL(Int, dn,		tbl);
	DECLARE_COL(Int, winkel,	tbl);
	DECLARE_COL(Int, laenge_von,tbl);
	DECLARE_COL(Int, laenge_bis,tbl);
	DECLARE_COL(Int, dnvl_von,  tbl);
	DECLARE_COL(Int, dnvl_bis,  tbl);
	DECLARE_COL(Bool,isAnlage,  tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new BauelementeAfos::data;
		thedata->produkttyp	= produkttyp.data;
		thedata->elemid		= elemid.data;
		thedata->code1		= code1.data;
		thedata->code2		= code2.data;
		thedata->code3		= code3.data;
		thedata->code4		= code4.data;
		thedata->dn			= dn.data;
		thedata->winkel		= winkel.data;
		thedata->laenge_von = laenge_von.data;
		thedata->laenge_bis = laenge_bis.data;
		thedata->dnvl_von   = dnvl_von.data;
		thedata->dnvl_bis   = dnvl_bis.data;
		thedata->isAnlage   = isAnlage.data ? true : false;

		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

BauelementeAfos::data::data() : code1(0), code2(0), code3(0), code4(0)
{

}

const int BauelementeAfos::data::get_code1() const
{
	return code1;
}

const int BauelementeAfos::data::get_code2() const
{
	return code2;
}


const int BauelementeAfos::data::get_produktTyp() const
{
	return produkttyp;
}

const int BauelementeAfos::data::get_code3() const
{
	return code3;
}

const int BauelementeAfos::data::get_code4() const
{
	return code4;
}

const int BauelementeAfos::data::get_dn() const
{
	return dn;
}

const int BauelementeAfos::data::get_elemid() const
{
	return elemid;
}

const int BauelementeAfos::data::get_laenge_bis() const
{
	return laenge_bis;
}

const int BauelementeAfos::data::get_laenge_von() const
{
	return laenge_von;
}


const int BauelementeAfos::data::get_dnvl_bis() const
{
	return dnvl_bis;
}

const int BauelementeAfos::data::get_dnvl_von() const
{
	return dnvl_von;
}

const int BauelementeAfos::data::get_winkel() const
{
	return winkel;
}


const bool BauelementeAfos::data::get_isAnlage() const
{
	return isAnlage;
}

