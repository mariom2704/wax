// Copyright (C) 2001 Minimax GmbH

#include "stdafx.h"

#include <mxodbc\mxodbc.h>
#include "resource.h"



Kostenstellen::Kostenstellen() : StammDat("kostenstellen", "", "nr")
{

}

void Kostenstellen::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, id, tbl);
	DECLARE_COL(Long, nr, tbl);
	DECLARE_COL_LANGUAGE_WAX(String, bezeichnung, tbl);
	DECLARE_COL(Double, tarif, tbl);
	DECLARE_COL(Double, zuschlag, tbl);
	DECLARE_COL(Double, grenztarif, tbl);
	DECLARE_COL(Double, grenzZuschlag, tbl);


	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new Kostenstellen::data;
		thedata->id	= id.data;
		thedata->nr	= nr.data;
		thedata->bezeichnung = *bezeichnung.data;
		thedata->tarif = tarif.data;
		thedata->zuschlag = zuschlag.data;
		thedata->grenztarif = grenztarif.data;
		thedata->grenzZuschlag = grenzZuschlag.data;
			
		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

Kostenstellen::data::data() : tarif(0.0)
{

}

const CString& Kostenstellen::data::get_bezeichnung() const
{
	return bezeichnung;
}

const double Kostenstellen::data::get_tarif() const
{
	return tarif;
}


const double Kostenstellen::data::get_grenztarif() const
{
	return grenztarif;
}


const double Kostenstellen::data::get_zuschlag() const
{
	return zuschlag;
}



const double Kostenstellen::data::get_grenzZuschlag() const
{
	return grenzZuschlag;
}

const long Kostenstellen::data::get_nr() const
{
	return nr;
}

const int Kostenstellen::data::get_id() const
{
	return id;
}

long Kostenstellen::getNr(int id)
{
	Kostenstellen::data *k = NULL;
	for (POSITION apos = getStammdaten().get_kostenstellen().GetHeadPosition(); apos;)
	{
		k = (Kostenstellen::data *)getStammdaten().get_kostenstellen().GetNext(apos);
		if (k->get_id() == id)
			return k->get_nr();
	}
	return 0;
}

double Kostenstellen::getTarif(long nr)
{
	Kostenstellen::data *k = NULL;
	for (POSITION apos = getStammdaten().get_kostenstellen().GetHeadPosition(); apos;)
	{
		k = (Kostenstellen::data *)getStammdaten().get_kostenstellen().GetNext(apos);
		if (k->get_nr() == nr)
			return k->get_tarif();
	}
	return 0.0;
}


double Kostenstellen::getGrenztarif(long nr)
{
	Kostenstellen::data *k = NULL;
	for (POSITION apos = getStammdaten().get_kostenstellen().GetHeadPosition(); apos;)
	{
		k = (Kostenstellen::data *)getStammdaten().get_kostenstellen().GetNext(apos);
		if (k->get_nr() == nr)
			return k->get_grenztarif();
	}
	return 0.0;
}


double Kostenstellen::getZuschlag(long nr)
{
	Kostenstellen::data *k = NULL;
	for (POSITION apos = getStammdaten().get_kostenstellen().GetHeadPosition(); apos;)
	{
		k = (Kostenstellen::data *)getStammdaten().get_kostenstellen().GetNext(apos);
		if (k->get_nr() == nr)
			return k->get_zuschlag();
	}
	return 0.0;
}


double Kostenstellen::getGrenzZuschlag(long nr)
{
	Kostenstellen::data *k = NULL;
	for (POSITION apos = getStammdaten().get_kostenstellen().GetHeadPosition(); apos;)
	{
		k = (Kostenstellen::data *)getStammdaten().get_kostenstellen().GetNext(apos);
		if (k->get_nr() == nr)
			return k->get_grenzZuschlag();
	}
	return 0.0;
}