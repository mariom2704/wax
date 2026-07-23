#include "stdafx.h"

#include <mxodbc\mxodbc.h>



Ral::Ral() : StammDat("ralfarben", "", "Id")
{
	// Kein Select, Sortierung nach id
}


void Ral::load(const string& db)
{
	// Tabelle initialisieren
	MXTable ral(get_database(db), get_tbl());
	
	// Tabellenfelder definieren
	DECLARE_COL(Int, Id, ral);
	DECLARE_COL_LANGUAGE_WAX(String, bezeichnung, ral);

	ral.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!ral.isEof())
	{
		data* thedata = new Ral::data;
		thedata->id = Id.data;
		thedata->bezeichnung = *bezeichnung.data;

		lst.AddTail(thedata);
		ral.moveNext();
	}
}

Ral::data::data() : id(0)
{

}

const CString& Ral::data::get_bezeichnung() const
{
	return bezeichnung;
}

 int Ral::data::get_id() const
{
	return id;
}

CString Ral::getBezeichnung(int id)
{
	Ral::data* ral = NULL;
	for (POSITION pos = getStammdaten().get_ral().GetHeadPosition(); pos != NULL; )
	{
		ral = (Ral::data* )getStammdaten().get_ral().GetNext(pos);
		if ( ral->get_id() == id)
			return ral->get_bezeichnung();
	}
	CString str; str = getStammdaten().getText("IDS_UNBEKANNT").c_str();
	return str;
}


