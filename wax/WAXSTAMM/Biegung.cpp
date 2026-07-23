#include "stdafx.h"


#include "Biegung.h"
#include <mxodbc\mxodbc.h>

Biegung::Biegung(void)  : StammDat("biegung", "", "")
{
}


//Laden der Daten aus der Stammdatenbank, Tabelle Bauelemente
void Biegung::load(const string& db)
{

	// Tabelle initialisieren
	MXTable bg(get_database(db), get_tbl());
		
	// Tabellenfelder definieren
	DECLARE_COL(Long, dn, bg);
	DECLARE_COL(Long, rohrId, bg);
	DECLARE_COL(Long, minWinkel, bg);
	DECLARE_COL(Long, maxWinkel, bg);
	DECLARE_COL(Long, minAbst, bg);
	DECLARE_COL(Long, minEleAbst, bg);
	DECLARE_COL(Long, radRolle, bg);

	bg.load();

	lst.RemoveAll();
	
	while (!bg.isEof())
	{
		data* thedata = new Biegung::data;
		thedata->dn = dn.data;
		thedata->rohrId = rohrId.data;
		thedata->maxWinkel = maxWinkel.data;
		thedata->minWinkel = minWinkel.data;
		thedata->minAbst = minAbst.data;
		thedata->minEleAbst = minEleAbst.data;
		thedata->radRolle = radRolle.data;
		
		lst.AddTail(thedata);
		bg.moveNext();
	}
}


Biegung::data::data() : dn(0), rohrId(0), maxWinkel(0), minWinkel(0), minAbst(0), minEleAbst(0), radRolle(0)
{

}

const long Biegung::data::get_dn() const
{
	return dn;
}


const long Biegung::data::get_rohrId() const
{
	return rohrId;
}


const long Biegung::data::get_maxWinkel() const
{
	return maxWinkel;
}


const long Biegung::data::get_minWinkel() const
{
	return minWinkel;
}


const long Biegung::data::get_minAbst() const
{
	return minAbst;
}


const long Biegung::data::get_radRolle() const
{
	return radRolle;
}



const long Biegung::data::get_minEleAbst() const
{
	return minEleAbst;
}

bool Biegung::getAbstaende(long dn, long rohrId, long& minWinkel, long& maxWinkel, long& minAbst, long& minEleAbst)
{
	Biegung::data *bg = NULL;
	for (POSITION apos = getStammdaten().get_biegung().GetHeadPosition(); apos;)
	{
		bg = (Biegung::data *)getStammdaten().get_biegung().GetNext(apos);
		if (bg->get_dn() == dn && bg->get_rohrId() == rohrId)
		{
			maxWinkel = bg->get_maxWinkel();
			minAbst = bg->get_minAbst();
			minEleAbst = bg->get_minEleAbst();
			minWinkel = bg->get_minWinkel();

			return true;
		}
	}

	return false;
}


bool Biegung::getRadRolle(long dn, long rohrId, long& rad)
{
	Biegung::data *bg = NULL;
	for (POSITION apos = getStammdaten().get_biegung().GetHeadPosition(); apos;)
	{
		bg = (Biegung::data *)getStammdaten().get_biegung().GetNext(apos);
		if (bg->get_dn() == dn && bg->get_rohrId() == rohrId)
		{
			rad = bg->get_radRolle();			
			return true;
		}
	}

	return false;
}