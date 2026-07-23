#include "stdafx.h"

#include <mxodbc\mxodbc.h>



#include "FormstueckeUndFittinge.h"


FormstueckeUndFittinge::FormstueckeUndFittinge()
: StammDat("", "", "")
{
	
}


double FormstueckeUndFittinge::getPreis(int nr, FormstueckeUndFittinge::PREISART preisart)
{
	for (POSITION apos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); apos;)
	{
		FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(apos);
		if (f->get_artikelnr() == nr)
		{
			switch (preisart)
			{
			case FormstueckeUndFittinge::festPreis:
				return f->get_festPreis();
			case FormstueckeUndFittinge::sapPreis:
				return f->get_sapPreis();
			case FormstueckeUndFittinge::pulverPreis:
				return f->get_pulverpreis();
			default:
				return 0;
			}	
		}
	}
	return 0;
}

CString FormstueckeUndFittinge::getKurztext(int nr)
{
	for (POSITION apos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); apos;)
	{
		FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(apos);
		if (f->get_artikelnr() == nr)
			return f->get_kurztext();
	}
	return "";
}

CString FormstueckeUndFittinge::data::setKurztext(int elemid, CString kt, int gruppenid, int dn1, int dn2, int dn3, int dn4, 
											CString iso, CString gf, bool schwarz, bool neunzigbar)
{
	if (elemid == 42)
		return kt;

	int textformat = WaxSys::para_int(TEXTFORMAT);
	CString trennzeichen = WaxSys::para_str(TRENNZEICHEN);
	bool dn2zoll = (WaxSys::para_int(DN2ZOLL) == 0) ? false : true;

	// short form
	//if (textform == "S")

	if (textformat == 0 && elemid == 44)
	{
		switch (gruppenid)
		{
		case ANBOHRSCHELLE:
			kt = "AS";
			break;
		case BOGEN:
			kt = "B";
			break;
		case DOPPELNIPPEL:
			kt = "DN";
			break;
		case ENDKAPPE:
			kt = "EK";
			break;
		case KAPPE:
			kt = "KP";
			break;
		case KREUZ_STUECK:
			kt = "K";
			break;
		case KUPPLUNG:
			kt = "KU";
			break;
		case F_MUFFE:
			kt = "M";
			break;
		case RED_KUPPLUNG:
			kt = "RKU";
			break;
		case RED_MUFFE:
			kt = "RM";
			break;
		case RED_NIPPEL:
			kt = "RN";
			break;
		case REDUZIERSTUECK:
			kt = "RS";
			break;
		case STOPFEN:
			kt = "ST";
			break;
		case T_STUECK:
			kt = "T";
			break;
		case VERLAENGERUNG:
			kt = "VL";
			break;
		case VERSCHRAUBUNG:
			kt = "VS";
			break;
		case WINKEL:
			kt = "W";
			break;
		case WINKEL_30:
			kt = "W_30";
			break;
		case WINKEL_A1_45:
			kt = "W_A1_45";
			break;
		case WINKEL_A4:
			kt = "W_A4";
			break;
		case WINKELVERSCHRAUBUNG:
			kt = "WVS";
			break;
		}
	}

	// Nennweite
	if (dn2zoll)
	{
		if (dn1)
			kt += " " + WaxStamm::dn2zoll(dn1);
		if (dn2)
			kt += trennzeichen + WaxStamm::dn2zoll(dn2);
		if (dn3)
			kt += trennzeichen + WaxStamm::dn2zoll(dn3);
		if (dn4)
			kt += trennzeichen + WaxStamm::dn2zoll(dn4);
	}
	else
	{
		CString txt;
		if (dn1)
		{
			txt.Format(" %d", dn1);
			kt += txt;
		}
		if (dn2)
		{
			txt.Format("%s%d", trennzeichen, dn2); 
			kt += txt;
		}
		if (dn3)
		{
			txt.Format("%s%d", trennzeichen, dn3); 
			kt += txt;
		}
		if (dn4)
		{
			txt.Format("%s%d", trennzeichen, dn4); 
			kt += txt;
		}
	}

	// long form
	//if (textform == "L")
	if (textformat == 2 || elemid == 43)
	{
		if (iso.GetLength() || gf.GetLength())
		{
			kt += " (";
			if (iso.GetLength())
				kt += iso + "/";

			if (gf.GetLength())
				kt += "GF" + gf;
			kt += ")";
		}
		if (gruppenid != KAPPE || gf.GetLength() != 0)
		{
			CString str;
			if (schwarz)
			{
				str = getStammdaten().getText("IDS_SCHWARZ").c_str();
				kt += " " + str;
			}
			else
			{
				str = getStammdaten().getText("IDS_VERZINKT").c_str();
				kt += str; 
			}
			if (neunzigbar)
			{
				str = getStammdaten().getText("IDS_90BAR").c_str();
				kt += str;
			}
		}
	}
	// middle form
	//else if (textform == "M")
	else if (textformat == 1)
	{
		if (gf.GetLength())
		{
			kt += " (";
			kt += "GF" + gf;
			kt += ")";
		}
		if (neunzigbar)
		{
			CString str; str = getStammdaten().getText("IDS_90BAR").c_str();
			kt += str;
		}
	}
	// short form
	//else if (textform == "S")
	else if (textformat == 0)
	{
		if (gf.GetLength())
		{
			kt += " (";
			kt += gf;
			kt += ")";
		}
	}
	return kt;
}

//Laden der Tabellendaten
void FormstueckeUndFittinge::load(const string& db)
{
	double toleranz = 0.000001;

	// Tabelle initialisieren
	MXTable fufitt(get_database(db), "tmpTable");
	
	// Tabellenfelder definieren
	DECLARE_COL(Int, elemid, fufitt);
	DECLARE_COL(Int, gruppenid, fufitt);
	DECLARE_COL(String, gruppe_deutsch, fufitt);
	DECLARE_COL(String, kurztext_deutsch, fufitt);

	DECLARE_COL(Long, artikelnr, fufitt);
	DECLARE_COL(Double, SapPreis, fufitt);
	DECLARE_COL(Double, FestPreis, fufitt);
	DECLARE_COL(Double, Gewicht, fufitt);
	DECLARE_COL(Double, pulverpreis, fufitt);	
	DECLARE_COL(Bool, einkauf, fufitt);
	DECLARE_COL(Int, dn1, fufitt);
	DECLARE_COL(Int, dn2, fufitt);
	DECLARE_COL(Int, dn3, fufitt);
	DECLARE_COL(Int, dn4, fufitt);
	DECLARE_COL(Int, zmass1, fufitt);
	DECLARE_COL(Int, zmass2, fufitt);
	DECLARE_COL(Int, zmass3, fufitt);

	DECLARE_COL(Int, laenge, fufitt);
	DECLARE_COL(String, gfzeichen, fufitt);
	DECLARE_COL(String, iso, fufitt);
	DECLARE_COL(Bool, schwarz, fufitt);
	DECLARE_COL(Bool, wasser_sprinkler, fufitt);
	DECLARE_COL(Bool, neunzigbar, fufitt);
	DECLARE_COL(Bool, halbzeug, fufitt);


	string sql = "SELECT f.elemid, f.gruppenid, f.Gruppe" + WaxSys::getLanguage() + ", f.Kurztext" + WaxSys::getLanguage() + ", ";
	sql +=		 "f.artikelnr, a.SapPreis, a.FestPreis, a.Gewicht, f.pulverpreis, f.einkauf, f.dn1, f.dn2, f.dn3, f.dn4, f.zmass1, f.zmass2, f.zmass3, ";
	sql +=		 "f.laenge, f.gfzeichen, f.iso, f.schwarz, f.wasser_sprinkler, f.neunzigbar, f.halbzeug ";
    sql +=       "FROM formstuecke_und_fittinge f LEFT JOIN artikelstamm a ON f.artikelnr = a.Artikelnummer ";
	sql +=       "order by f.elemid, f.gruppenid, f.dn1, f.dn2, f.dn3, f.dn4, f.gfzeichen";

	fufitt.set_sql(sql);

	fufitt.load();

	lst.RemoveAll();
	
	while (!fufitt.isEof())
	{
		data* thedata = new FormstueckeUndFittinge::data;
		thedata->elem_ID = elemid.data;
		thedata->gruppenid = gruppenid.data;
		thedata->gruppe = *gruppe_deutsch.data;
		thedata->artikelnr = artikelnr.data;
		thedata->gewicht = Gewicht.data;
		if (thedata->gewicht < toleranz)
			thedata->gewicht = 0;


		if (FestPreis.data < toleranz)
		{
			thedata->festPreis = SapPreis.data;
			if (thedata->festPreis < toleranz)
				thedata->festPreis = 0;
		}
		else
			thedata->festPreis = FestPreis.data;

		thedata->sapPreis = SapPreis.data;
		
		thedata->pulverpreis = pulverpreis.data;
		thedata->einkauf = einkauf.data ? true : false;
		thedata->halbzeug = halbzeug.data ? true : false;

		// -1 (Datenbank NULL) entspricht DN0
		XASSERT(dn1.data <= 300 && dn2.data <= 300 && dn3.data <= 300 && dn4.data <= 300);
		XASSERT(dn1.data > -1 && dn2.data > -1 && dn3.data > -1 && dn4.data > -1);

		thedata->dn1 = dn1.data > 0 ? dn1.data : 0;
		thedata->dn2 = dn2.data > 0 ? dn2.data : 0;
		thedata->dn3 = dn3.data > 0 ? dn3.data : 0;
		thedata->dn4 = dn4.data > 0 ? dn4.data : 0;
		
		thedata->zmass1 = zmass1.data;
		thedata->zmass2 = zmass2.data;
		thedata->zmass3 = zmass3.data;
		thedata->laenge = laenge.data;

		thedata->gfzeichen = *gfzeichen.data;
		thedata->iso = *iso.data;
		thedata->schwarz = schwarz.data ? true : false;
		thedata->neunzigbar = neunzigbar.data ? true : false;
		thedata->wasser_sprinkler = wasser_sprinkler.data ? true : false;
		
		CString kt(*kurztext_deutsch.data);
		thedata->kurztext =	thedata->setKurztext(thedata->elem_ID, kt, thedata->get_gruppenid(), 
										thedata->get_dn1(),    thedata->get_dn2(),
										thedata->get_dn3(),    thedata->get_dn4(), 
										thedata->get_iso(),	   thedata->get_gfzeichen(), 
										thedata->get_schwarz(), thedata->get_neunzigbar());

		lst.AddTail(thedata);
		fufitt.moveNext();
	}
}

FormstueckeUndFittinge::data::data() : festPreis(0.0), sapPreis(0.0),gewicht(0.0), artikelnr(0), elem_ID(0), dn4(0), dn1(0), schwarz(false), neunzigbar(false), wasser_sprinkler(false)
{

}

 int FormstueckeUndFittinge::data::get_artikelnr() const
{
	return artikelnr;
}

const CString& FormstueckeUndFittinge::data::get_gruppe() const
{
	return gruppe;
}

 double FormstueckeUndFittinge::data::get_gewicht() const
{
	return gewicht;
}

const CString& FormstueckeUndFittinge::data::get_kurztext() const
{
	return kurztext;
}

double FormstueckeUndFittinge::data::get_festPreis() const
{
	return festPreis;
}

double FormstueckeUndFittinge::data::get_sapPreis() const
{
	return sapPreis;
}


double FormstueckeUndFittinge::data::get_pulverpreis() const
{
	return pulverpreis;
}

int FormstueckeUndFittinge::data::get_elemid() const
{
	return elem_ID;
}

const CString& FormstueckeUndFittinge::data::get_iso() const
{
	return iso;
}

const CString& FormstueckeUndFittinge::data::get_gfzeichen() const
{
	return gfzeichen;
}

int FormstueckeUndFittinge::data::get_dn1() const
{
	return dn1;
}

int FormstueckeUndFittinge::data::get_dn2() const
{
	return dn2;
}

int FormstueckeUndFittinge::data::get_dn3() const
{
	return dn3;
}

int FormstueckeUndFittinge::data::get_dn4() const
{
	return dn4;
}

bool FormstueckeUndFittinge::data::get_schwarz() const
{
	return schwarz;
}

bool FormstueckeUndFittinge::data::get_neunzigbar() const
{
	return neunzigbar;
}

const bool FormstueckeUndFittinge::data::get_wasser_sprinkler() const
{
	return wasser_sprinkler;
}

const bool FormstueckeUndFittinge::data::get_einkauf() const
{
	return einkauf;
}


const bool FormstueckeUndFittinge::data::get_halbzeug() const
{
	return halbzeug;
}

const int FormstueckeUndFittinge::data::get_gruppenid() const
{
	return gruppenid;
}

const int FormstueckeUndFittinge::data::get_zmass1() const
{
	return zmass1;
}

const int FormstueckeUndFittinge::data::get_zmass2() const
{
	return zmass2;
}

const int FormstueckeUndFittinge::data::get_zmass3() const
{
	return zmass3;
}

const int FormstueckeUndFittinge::data::get_laenge() const
{
	return laenge;
}
