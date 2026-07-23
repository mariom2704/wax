// Copyright (C) 2001 Minimax GmbH

#include "stdafx.h"


#include <mxodbc\mxodbc.h>

DummyMaterialnummern::DummyMaterialnummern()
: StammDat("dummy_material_nummern", "in_wax_bestellbar", "")
{
}

void DummyMaterialnummern::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, dummy_material_nummer, tbl);
	DECLARE_COL_LANGUAGE_WAX(String, bezeichnung, tbl);
	DECLARE_COL(String, verwendung, tbl);
	DECLARE_COL(String, segment, tbl);
	DECLARE_COL(Int, lieferzeit, tbl);
	DECLARE_COL(String, klasse, tbl);
	DECLARE_COL(String, iPPEClassenName, tbl);
	DECLARE_COL(String, iPPEKurzText, tbl);
	DECLARE_COL(Bool, verzinkt, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata			 = new DummyMaterialnummern::data;
		thedata->nummer			 = dummy_material_nummer.data;
		thedata->bezeichnung	 = *bezeichnung.data;
		thedata->verwendung	 	 = *verwendung.data;
		thedata->lieferzeit		 = lieferzeit.data;
		thedata->segment		 = *segment.data;
		thedata->klasse			 = *klasse.data;
		thedata->iPPEClassenName = *iPPEClassenName.data;
		thedata->iPPEKurzText	 = *iPPEKurzText.data;
		thedata->verzinkt = verzinkt.data ? true : false;

		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

DummyMaterialnummern::data::data() : nummer(0)
{

}

const int DummyMaterialnummern::data::get_nummer() const
{
	return nummer;
}

const CString& DummyMaterialnummern::data::get_iPPEClassName() const
{
	return iPPEClassenName;
}

const CString& DummyMaterialnummern::data::get_iPPEKurzText() const
{
	return iPPEKurzText;
}

const CString& DummyMaterialnummern::data::get_verwendung() const
{
	return verwendung;
}

const CString& DummyMaterialnummern::data::get_bezeichnung() const
{
	return bezeichnung;
}

const int DummyMaterialnummern::data::get_lieferzeit() const
{
	return lieferzeit;
}

const CString& DummyMaterialnummern::data::get_segment() const
{
	return segment;
}

const CString& DummyMaterialnummern::data::get_klasse() const
{
	return klasse;
}

const bool DummyMaterialnummern::data::get_verzinkt() const
{
	return verzinkt;
}
