#include "stdafx.h"


#include <mxodbc\mxodbc.h>


AbzugRadius::AbzugRadius() : StammDat("abzug_rohrstutzen_radius_klinken", "", "dn")
{}



void AbzugRadius::load(const string& db)
{
	MXTable tbl(get_database(db), get_tbl());	
	
	DECLARE_COL(Int, dn, tbl);
	DECLARE_COL(Int, dnvl, tbl);
	DECLARE_COL(Int, radius, tbl);
	DECLARE_COL(Int, abzug, tbl);

	tbl.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!tbl.isEof())
	{
		data* thedata = new AbzugRadius::data;
		thedata->dn = dn.data;
		thedata->dnvl = dnvl.data;
		thedata->radius = radius.data;
		thedata->abzug = abzug.data;

		lst.AddTail(thedata);
		tbl.moveNext();
	}
}

AbzugRadius::data::data() : dn(0), dnvl(0), radius(0), abzug(0) 
{

}

int AbzugRadius::data::get_dn() const
{
	return dn;
}

int AbzugRadius::data::get_dnvl() const
{
	return dnvl;
}

int AbzugRadius::data::get_radius() const
{
	return radius;
}

int AbzugRadius::data::get_abzug() const
{
	return abzug;
}

int AbzugRadius::getRadius(int dn, int dnvl)
{
	for (POSITION gpos = getStammdaten().get_abzugradius().GetHeadPosition(); gpos;)
	{
		AbzugRadius::data *g = (AbzugRadius::data *)getStammdaten().get_abzugradius().GetNext(gpos);
		if (g->get_dn() == dn && g->get_dnvl() == dnvl) 
			return g->get_radius();
	}
	return 0;
}

int AbzugRadius::getAbzug(int dn, int dnvl)
{
	for (POSITION gpos = getStammdaten().get_abzugradius().GetHeadPosition(); gpos;)
	{
		AbzugRadius::data *g = (AbzugRadius::data *)getStammdaten().get_abzugradius().GetNext(gpos);
		if (g->get_dn() == dn && g->get_dnvl() == dnvl) 
			return g->get_abzug();
	}
	return 0;
}
