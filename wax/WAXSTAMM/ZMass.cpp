// Copyright (C) 2001 Minimax GmbH

#include "stdafx.h"

#include <mxodbc\mxodbc.h>




ZMass::~ZMass()
{

}

ZMass::ZMass()
: StammDat("zmass", "", "")
{

}

void ZMass::load(const string& db)
{
	MXTable zmass(get_database(db), get_tbl());

	DECLARE_COL(Int, dn, zmass);
	DECLARE_COL(Int, dnvl, zmass);
	DECLARE_COL(Int, zmass_verteiler, zmass);
	DECLARE_COL(Int, zmass_fitting, zmass);
	DECLARE_COL(Int, zmass_muffe, zmass);

	zmass.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!zmass.isEof())
	{
		data* thedata = new ZMass::data;
		thedata->dn = dn.data;
		thedata->dnvl = dnvl.data;
		thedata->zmass_verteiler = zmass_verteiler.data;
		thedata->zmass_fitting = zmass_fitting.data;
		thedata->zmass_muffe = zmass_muffe.data;
		lst.AddTail(thedata);
		zmass.moveNext();
	}
}

 int ZMass::data::get_dn() const
{
	return dn;
}

 int ZMass::data::get_dnvl() const
{
	return dnvl;
}

 int ZMass::data::get_zmass_verteiler() const
{
	return zmass_verteiler;
}

 int ZMass::data::get_zmass_fitting() const
{
	return zmass_fitting;
}

 int ZMass::data::get_zmass_muffe() const
{
	return zmass_muffe;
}

ZMass::data::data() : dn(0), dnvl(0), zmass_verteiler(0), zmass_fitting(0), zmass_muffe(0)
{
}



int ZMass::get_val(FormstueckeUndFittinge::GRUPPENID  id, int dn, int dnvl) const
{
	for (POSITION pos = lst.GetHeadPosition(); pos;)
	{
		ZMass::data *z = (ZMass::data *)lst.GetNext(pos);
		if (z->get_dn() == dn && z->get_dnvl() == dnvl) 
		{
			// Muffe
			switch (id)
			{
				case FormstueckeUndFittinge::F_MUFFE:
				case FormstueckeUndFittinge::RED_MUFFE:
				case FormstueckeUndFittinge::DOPPELNIPPEL:
				case FormstueckeUndFittinge::RED_NIPPEL:
				case FormstueckeUndFittinge::STOPFEN:
				case FormstueckeUndFittinge::VERSCHRAUBUNG:
				case FormstueckeUndFittinge::WINKELVERSCHRAUBUNG:
				case FormstueckeUndFittinge::KAPPE:
					return z->get_zmass_muffe();
				default:
					return z->get_zmass_fitting();
			}
		}
	}
	return 0;
}

