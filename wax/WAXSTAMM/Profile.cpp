#include "stdafx.h"

#include <mxodbc\mxodbc.h>


Profile::Profile() : StammDat("profile", "", "typ,a_h")
{
}

void Profile::load(const string& db)
{
	double toleranz = 0.000001;

	MXTable profile(get_database(db), "tmpTable");
	
	DECLARE_COL(Long, elemid, profile);
	DECLARE_COL_LANGUAGE_WAX_EXT(String, Bezeichnung, profile);
	DECLARE_COL(String, typ, profile);
	DECLARE_COL(Long, a_h, profile);
	DECLARE_COL(Long, b, profile);
	DECLARE_COL(Long, s, profile);
	DECLARE_COL(Long, artikelnr, profile);
	DECLARE_COL(Bool, einkauf, profile);
	DECLARE_COL(Bool, schwarz, profile);
	DECLARE_COL(Bool, halbzeug, profile);
	DECLARE_COL(Bool, co2tikko, profile);
	DECLARE_COL(Double, SapPreis, profile);
	DECLARE_COL(Double, FestPreis, profile);
	DECLARE_COL(Double, Gewicht, profile);
	

	string sql = "SELECT p.elemid, a.Bezeichnung" + WaxSys::getLanguage() + ", p.typ, p.a_h, p.b, p.s, p.artikelnr, p.einkauf, p.schwarz, p.halbzeug, p.co2tikko, a.SapPreis, a.FestPreis, a.Gewicht ";
	sql +=	     "FROM profile p LEFT JOIN artikelstamm a ON p.artikelnr = a.Artikelnummer ";
	sql +=		 "order by p.typ, p.a_h, p.b, p.s";

	profile.set_sql(sql);

	profile.load();

	//profile.load(get_select(), get_order());

	lst.RemoveAll();
	while (!profile.isEof())
	{
		data* thedata = new Profile::data;
		thedata->elemid = elemid.data;
		thedata->kurztext = *Bezeichnung.data;
		thedata->typ = *typ.data;
		thedata->a_h = a_h.data;
		thedata->b = b.data;
		thedata->s = s.data;
		thedata->artikelnr = artikelnr.data;
		thedata->einkauf = einkauf.data ? true : false;
		thedata->schwarz = schwarz.data ? true : false;
		thedata->halbzeug = halbzeug.data ? true : false;
		thedata->gewicht = Gewicht.data;
		thedata->co2tikko = co2tikko.data ? true : false;

		if (FestPreis.data < toleranz)
		{
			thedata->festPreis = SapPreis.data;
			if (thedata->festPreis < toleranz)
				thedata->festPreis = 0;
		}
		else
			thedata->festPreis = FestPreis.data;

		thedata->sapPreis = SapPreis.data;

			
		lst.AddTail(thedata);
		profile.moveNext();
	}
}

const int Profile::data::get_elemid() const
{
	return elemid;
}

const CString& Profile::data::get_kurztext() const
{
	return kurztext;
}

const CString& Profile::data::get_typ() const
{
	return typ;
}

const int Profile::data::get_a_h() const
{
	return a_h;
}

const int Profile::data::get_b() const
{
	return b;
}

const int Profile::data::get_s() const
{
	return s;
}

const int Profile::data::get_artikelnr() const
{
	return artikelnr;
}

const bool Profile::data::get_einkauf() const
{
	return einkauf;
}
const double Profile::data::get_festPreis() const
{
	return festPreis;
}

/**
GLD aus Sap
*/
const double Profile::data::get_sapPreis() const
{
	return sapPreis;
}


const bool Profile::data::get_schwarz() const
{
	return schwarz;
}

const bool Profile::data::get_halbzeug() const
{
	return halbzeug;
}

const double Profile::data::get_gewicht() const
{
	return gewicht;
}

const bool Profile::data::get_co2tikko() const
{
	return co2tikko;
}

CString Profile::data::get_text()
{
	CString text, str;
	if (get_typ() == "F")
	{
		str = getStammdaten().getText("IDS_FLACH_PROFIL").c_str();
		text.Format("%s     %ix%i", str, get_a_h(), get_s());
	}
	else if(get_typ() == "L")
	{
		str = getStammdaten().getText("IDS_WINKEL_PROFIL").c_str();
		text.Format("%s     %ix%ix%i", str, get_a_h(), get_b(), get_s());
	}
	else if(get_typ() == "U")
	{
		str = getStammdaten().getText("IDS_U_PROFIL").c_str();
		text.Format("%s     U%i", str, get_a_h());
	}
	return text;
}
