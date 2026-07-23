#include "stdafx.h"

#include "resource.h"


#include <mxodbc\mxodbc.h>

Arbeitsplaetze::Arbeitsplaetze(): StammDat("arbeitsplaetze", "", "nr")
{
}

void Arbeitsplaetze::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(String, nr, tbl);
	DECLARE_COL_LANGUAGE_WAX(String, bezeichnung, tbl);
	DECLARE_COL(Long, kostenstelle,tbl);
	DECLARE_COL(Double, ruestzeit, tbl);
	DECLARE_COL(Int, typ, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new Arbeitsplaetze::data;
		thedata->nr	= *nr.data;
		thedata->bezeichnung = *bezeichnung.data;
		thedata->kostenstelle = kostenstelle.data;
		thedata->ruestzeit = ruestzeit.data;
		thedata->typ = typ.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

Arbeitsplaetze::data::data() : kostenstelle(0), bezeichnung(""), nr(""), ruestzeit(0.0), typ(0)
{

}

const CString& Arbeitsplaetze::data::get_bezeichnung() const
{
	return bezeichnung;
}

const long Arbeitsplaetze::data::get_kostenstelle() const
{
	return kostenstelle;
}

const CString Arbeitsplaetze::data::get_nr() const
{
	return nr;
}


const int Arbeitsplaetze::data::get_typ() const
{
	return typ;
}



const double Arbeitsplaetze::data::get_ruestzeit() const
{
	return ruestzeit;
}

double Arbeitsplaetze::getRuestzeit(CString nr)
{
	Arbeitsplaetze::data *arb = NULL;
	for (POSITION apos = getStammdaten().get_arbeitsplaetze().GetHeadPosition(); apos;)
	{
		arb = (Arbeitsplaetze::data *)getStammdaten().get_arbeitsplaetze().GetNext(apos);
		if (arb->get_nr() == nr)
			return arb->get_ruestzeit();
	}
	return 0;
}

long Arbeitsplaetze::getKostenstelle(CString nr)
{
	Arbeitsplaetze::data *arb = NULL;
	for (POSITION apos = getStammdaten().get_arbeitsplaetze().GetHeadPosition(); apos;)
	{
		arb = (Arbeitsplaetze::data *)getStammdaten().get_arbeitsplaetze().GetNext(apos);
		if (arb->get_nr() == nr)
			return arb->get_kostenstelle();
	}
	return 0;
}

CString Arbeitsplaetze::getBezeichnung(CString nr)
{
	Arbeitsplaetze::data *arb = NULL;
	for (POSITION apos = getStammdaten().get_arbeitsplaetze().GetHeadPosition(); apos;)
	{
		arb = (Arbeitsplaetze::data *)getStammdaten().get_arbeitsplaetze().GetNext(apos);
		if (arb->get_nr() == nr)
			return arb->get_bezeichnung();
	}
	CString str; str = getStammdaten().getText("IDS_KEINE_BEZEICHNUNG").c_str();
	return str;
}


int Arbeitsplaetze::getTyp(CString nr)
{
	Arbeitsplaetze::data *arb = NULL;
	for (POSITION apos = getStammdaten().get_arbeitsplaetze().GetHeadPosition(); apos;)
	{
		arb = (Arbeitsplaetze::data *)getStammdaten().get_arbeitsplaetze().GetNext(apos);
		if (arb->get_nr() == nr)
			return arb->get_typ();
	}
	return 0;
}