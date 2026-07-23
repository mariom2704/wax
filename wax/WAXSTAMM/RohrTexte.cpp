#include "stdafx.h"
#include <mxodbc\mxodbc.h>




RohrTexte::~RohrTexte()
{
}

RohrTexte::RohrTexte() : StammDat("rohrtexte", "", "")
{
}

void RohrTexte::load(const string& db)
{
	// Tabelle initialisieren
	MXTable rohrtexte(get_database(db), get_tbl());
		
	// Tabellenfelder definieren
	DECLARE_COL(Bool, verzinkt, rohrtexte);
	DECLARE_COL(Int, rohrtyp, rohrtexte);
	DECLARE_COL(Bool, vortext, rohrtexte);
	DECLARE_COL(LongString, langtext, rohrtexte);

	//rohrtexte.load(get_select(), get_order());

	string sql = "SELECT verzinkt, rohrtyp, vortext, langtext" + WaxSys::getLanguage() + " FROM rohrtexte";

	rohrtexte.set_sql(sql);

	rohrtexte.load();

	lst.RemoveAll();
	
	while (!rohrtexte.isEof())
	{
		data* thedata = new RohrTexte::data;
		thedata->set_verzinkt(verzinkt.data ? true : false);
		thedata->set_rohrtyp(rohrtyp.data);
		thedata->set_vortext(vortext.data ? true : false);
		thedata->set_langtext(*langtext.data);

		lst.AddTail(thedata);
		rohrtexte.moveNext();
	}
}



const bool RohrTexte::data::get_vortext() const
{
	return vortext;
}

const bool RohrTexte::data::get_verzinkt() const
{
	return verzinkt;
}

const int RohrTexte::data::get_rohrtyp() const
{
	return rohrtyp;
}

const CString& RohrTexte::data::get_langtext() const
{
	return langtext;
}

void RohrTexte::data::set_langtext(const CString& value)
{
	langtext = value;
}

void RohrTexte::data::set_rohrtyp(int value)
{
	rohrtyp = value;
}

void RohrTexte::data::set_verzinkt(bool value)
{
	verzinkt = value;
}

void RohrTexte::data::set_vortext(bool value)
{
	vortext = value;
}

