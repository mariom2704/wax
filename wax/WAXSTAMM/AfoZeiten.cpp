#include "stdafx.h"


#include <mxodbc\mxodbc.h>

AfoZeiten::AfoZeiten(): StammDat("arbeitsfolgen_zeiten", "", "afoid, elemid, dn")
{

}

void AfoZeiten::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	

	DECLARE_COL(Int,	fertigungsstaette,	tbl);
	DECLARE_COL(String,	arbeitsplatz,		tbl);
	DECLARE_COL(Int,	produkttyp,			tbl);
	DECLARE_COL(Int,	elemid,				tbl);
	DECLARE_COL(Int,	afoid,				tbl);
	DECLARE_COL(Int,	dn,					tbl);
	DECLARE_COL(Int,	dnvl_von,			tbl);
	DECLARE_COL(Int,	dnvl_bis,			tbl);
	DECLARE_COL(Int,	laenge_von,			tbl);
	DECLARE_COL(Int,	laenge_bis,			tbl);

	DECLARE_COL(Int,	ElementAnzahlVon,	tbl);
	DECLARE_COL(Int,	ElementAnzahlBis,	tbl);
	DECLARE_COL(Int,	ElementAnzahlId,	tbl);
	DECLARE_COL(Double,	ElementAnzahlDnVon,	tbl);
	DECLARE_COL(Double,	ElementAnzahlDnBis,	tbl);
	DECLARE_COL(Bool,	Anlage,				tbl);

	DECLARE_COL(Int,	winkel_von,			tbl);
	DECLARE_COL(Int,	winkel_bis,			tbl);
	DECLARE_COL(Double, zeit,				tbl);
	DECLARE_COL(Double, zinkzuschlag,		tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new AfoZeiten::data;
		thedata->fertigungsstaette		= fertigungsstaette.data;
		thedata->arbeitsplatz			= *arbeitsplatz.data;
		thedata->produkttyp				= produkttyp.data;
		thedata->elemid					= elemid.data;
		thedata->afoid					= afoid.data;
		thedata->dn						= dn.data;
		thedata->dnvl_von				= dnvl_von.data;
		thedata->dnvl_bis				= dnvl_bis.data;
		thedata->laenge_von				= laenge_von.data;
		thedata->laenge_bis				= laenge_bis.data;

		thedata->ElementAnzahlVon		= ElementAnzahlVon.data;
		thedata->ElementAnzahlBis		= ElementAnzahlBis.data;
		thedata->ElementAnzahlId		= ElementAnzahlId.data;
		thedata->ElementAnzahlDnVon		= ElementAnzahlDnVon.data;
		thedata->ElementAnzahlDnBis		= ElementAnzahlDnBis.data;
		thedata->Anlage					= Anlage.data;

		thedata->winkel_von				= winkel_von.data;
		thedata->winkel_bis				= winkel_bis.data;
		thedata->zeit					= zeit.data;
		thedata->zinkzuschlag			= zinkzuschlag.data;
		

		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

AfoZeiten::data::data() : zeit(0.0), zinkzuschlag(0.0) 
{

}

const int AfoZeiten::data::get_afoid() const
{
	return afoid;
}

const int AfoZeiten::data::get_dn() const
{
	return dn;
}

const int AfoZeiten::data::get_elemid() const
{
	return elemid;
}

const int AfoZeiten::data::get_laenge_bis() const
{
	return laenge_bis;
}

const int AfoZeiten::data::get_laenge_von() const
{
	return laenge_von;
}

const double AfoZeiten::data::get_zeit() const
{
	return zeit;
}

const double AfoZeiten::data::get_zinkzuschlag() const
{
	return zinkzuschlag;
}

const int AfoZeiten::data::get_dnvl_von() const
{
	return dnvl_von;
}

const int AfoZeiten::data::get_dnvl_bis() const
{
	return dnvl_bis;
}



const int AfoZeiten::data::get_winkel_bis() const
{
	return winkel_bis;
}

const int AfoZeiten::data::get_winkel_von() const
{
	return winkel_von;
}


