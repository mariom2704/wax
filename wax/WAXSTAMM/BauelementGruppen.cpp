#include "stdafx.h"



#include <mxodbc\mxodbc.h>


BauelementGruppen::BauelementGruppen() : StammDat("bauelementgruppen", "", "")
{
	
}

void BauelementGruppen::load(const string& db)
{
	MXTable bgrp(get_database(db), get_tbl());

	DECLARE_COL(Long, elemid, bgrp);
	DECLARE_COL_LANGUAGE_WAX(String, bezeichnung, bgrp);
	DECLARE_COL_LANGUAGE_WAX_EXT(String, Kurz, bgrp);
	DECLARE_COL(Bool, kann_dn2, bgrp);
	DECLARE_COL(Bool, kann_laenge, bgrp);
	DECLARE_COL(Bool, kann_winkel, bgrp);

	bgrp.load(get_select(), get_order());

	lst.RemoveAll();

	
	while (!bgrp.isEof())
	{
		data* thedata = new BauelementGruppen::data;
		thedata->elem_ID = elemid.data;
		thedata->bezeichnung = *bezeichnung.data;
		thedata->kurzbezeichnung = *Kurz.data;
		thedata->kannDN2 = kann_dn2.data ? true : false;
		thedata->kannLaenge = kann_laenge.data  ? true : false;
		thedata->kannWinkel = kann_winkel.data  ? true : false;
		lst.AddTail(thedata);
		bgrp.moveNext();
	}
}

BauelementGruppen::data::data() : elem_ID(0), kannDN2(false), kannLaenge(false), kannWinkel(false)
{
}

int BauelementGruppen::data::get_elem_ID() const
{
	return elem_ID;
}

const CString& BauelementGruppen::data::get_bezeichnung() const
{
	return bezeichnung;
}


const CString& BauelementGruppen::data::get_kurzbezeichnung() const
{
	return kurzbezeichnung;
}

bool BauelementGruppen::data::get_kannDN2() const
{
	return kannDN2;
}

bool BauelementGruppen::data::get_kannLaenge() const
{
	return kannLaenge;
}

bool BauelementGruppen::data::get_kannWinkel() const
{
	return kannWinkel;
}

const CString BauelementGruppen::getBezeichnung(int id) const
{
	CString str; 
	if (!id)
	{
		str = getStammdaten().getText("IDS_GLATT").c_str();
		return str;
	}
	
	for (POSITION pos = getStammdaten().get_bauelementgruppen().GetHeadPosition(); pos;)
	{
		const BauelementGruppen::data *f = (const BauelementGruppen::data *)getStammdaten().get_bauelementgruppen().GetNext(pos);
		if (f->get_elem_ID() == id)
			return f->get_bezeichnung();
	}
	str = getStammdaten().getText("IDS_UNBEKANNT").c_str();
	return str;
}



const CString BauelementGruppen::getKurzbezeichnung(int id) const
{
	CString str; 

	if (!id)
	{
		str = getStammdaten().getText("IDS_GLATT").c_str();
		return str;
	}
	
	
	for (POSITION pos = getStammdaten().get_bauelementgruppen().GetHeadPosition(); pos;)
	{
		const BauelementGruppen::data *f = (const BauelementGruppen::data *)getStammdaten().get_bauelementgruppen().GetNext(pos);
		if (f->get_elem_ID() == id)
			return f->get_kurzbezeichnung();
	}
	str = getStammdaten().getText("IDS_UNBEKANNT").c_str();
	return str;
}
