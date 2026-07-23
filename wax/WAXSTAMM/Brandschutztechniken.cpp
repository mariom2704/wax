#include "stdafx.h"

#include <mxodbc\mxodbc.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



Brandschutztechniken::Brandschutztechniken()
: StammDat("brandschutztechniken", "BezeichnungDeutsch like '%'", "Nummer")
{
	
}

void Brandschutztechniken::load(const string& db)
{
	// Tabelle initialisieren
	MXTable Brandschutztech(get_database(db), get_tbl());
		
	// Tabellenfelder definieren
	DECLARE_COL(Int, Nummer, Brandschutztech);
	DECLARE_COL(String, BezeichnungDeutsch, Brandschutztech);
	DECLARE_COL(String, BezeichnungEnglisch, Brandschutztech);


	Brandschutztech.load(get_select(), get_order());

	lst.RemoveAll();
	
	while (!Brandschutztech.isEof())
	{
		data* thedata = new Brandschutztechniken::data;
		thedata->Nummer					=	Nummer.data;
		thedata->BezeichnungDeutsch		=	*BezeichnungDeutsch.data;
		thedata->BezeichnungEnglisch	=	*BezeichnungEnglisch.data;

		lst.AddTail(thedata);
		Brandschutztech.moveNext();
	}
}

 int Brandschutztechniken::data::get_Nummer() const
{
	return Nummer;
}

const CString& Brandschutztechniken::data::get_bezeichnung() const
{
	return BezeichnungDeutsch;
}




Brandschutztechniken::data::data() : Nummer(0) 
{

}



