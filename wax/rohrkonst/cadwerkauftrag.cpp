#include "stdafx.h"
#include "resource.h"


#include "verteiler.h"
#include "strangrohr.h"
#include "gewinderohr.h"
#include "xml2cadmodell.h"
#include "cadwerkauftrag.h"
#include "waxcadmodell.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

using namespace DOM;
using namespace CadModell;


//Konstruktor
CadWerkauftrag::CadWerkauftrag() : Rohrleitung()
{
	stdbezeichnung = getStammdaten().getText("IDS_CADWERKAUFTRAG").c_str();
	set_stdbemerkung(stdbezeichnung);
	set_cadpos(true);
	gewinderohr = false;
	handelslaenge = false;
	set_dichtmittel(0);
}

//Destruktor
CadWerkauftrag::~CadWerkauftrag()
{
}

//Speichern und Laden der Rohrleitung. Achtung:
void CadWerkauftrag::serialize(BinaryArchive& ar)
{
	Rohrleitung::serialize(ar);
	if (ar.is_storing())
	{
		CString x(xml_modell.c_str());
		ar << x;
	}
	else
	{
		CString x;
		ar >> x;
		set_xml_modell((PCSTR)x);
	}
}

//Hier wird automatisch der Arbeitsplan generiert
void CadWerkauftrag::createAutoArbeit(bool beschichtung)
{
	Rohrleitung* mk_arbeit = NULL;
	
	if (isVerteiler())
		mk_arbeit = new Verteiler;
	else if (isStrangrohr())
		mk_arbeit = new Strangrohr;
	else if (isGewinderohr())
		mk_arbeit = new Gewinderohr;
	
	if (!mk_arbeit)
		return;

	mk_arbeit->copy(*this);
	mk_arbeit->createAutoArbeit(beschichtung);
	get_arbeit().copy(mk_arbeit->get_arbeit());
	
	delete mk_arbeit;
}

//Rohrkonstruktion auf korrekte Werte checken
CString CadWerkauftrag::checkPlausis()
{
	CString ret = Rohrleitung::checkPlausis();
	return "";
}

void CadWerkauftrag::create_elementarray()
{
	WaxCadModell celist;

	Xml2CadModell xml2cad;

	
	
	
	celist.set_sdmaterialnr(get_leitkarte().get_sdMaterialnummer());

	if (xml2cad.convert(get_xml_modell(), celist))
	{
		gewinderohr = celist.get_sdmaterialnr() == Leitkarte::GewinderohreGepulvert || celist.get_sdmaterialnr() == Leitkarte::GewinderohreVerzinkt;
		
		init_element_array(celist);

		if (get_elementArray().GetSize() > 1)
		{
			Element* eanf = get_elementArray().get_element_at(0);
			Element* eend = get_elementArray().get_element_at(get_elementArray().GetSize()-1);
			if (eanf && eend)
			{
				if (!eanf->get_makeAnlage() && eend->get_makeAnlage())
					elementeingabe_invertieren();
			}
		}

		int iBogen = -1;
		int iMaschmuffe = -1;
		for(int i=0; i < get_elementArray().GetSize(); i++)
		{	
			Element* eanf = get_elementArray().get_element_at(i);
			if(eanf->get_id() == ELEMENT_BOGEN || eanf->get_id() == BOG2S)
			{
				iBogen = i;
			}

			if(eanf->get_makeAnlage())
			{
				iMaschmuffe = i;
			}
				
		}

		if (iBogen > -1 && iBogen < iMaschmuffe)
		{				
			elementeingabe_invertieren();
		}	

		scan_dnvl(celist, elementArray);

		set_rohrKFarbe(celist.kappenfarbe);
	}
	init_infos(celist); // Fehler als Fertigungshinweis eintragen
	// SD Materialnummer kann in XML geändert worden sein
	get_leitkarte().set_sdMaterialnummer(celist.get_sdmaterialnr());
}

void CadWerkauftrag::scan_dnvl(const WaxCadModell& celist, const EnContainerRohrElemente& eleArray)
{
	// DN des Rohres setzen als Anschlussdn des ersten Elements
	Element* e = eleArray.get_element_at(0);
	if (e)
		set_dnvl(e->get_anschlussDN());
	else // oder wenn keine Elemente vorhanden sind als DN des ersten Rohrstückes
	{
		vector<CadModell::CadElement>::const_iterator it = celist.begin();
		for (;it != celist.end(); it++)
		{
			if (it->typrohr.dn == 0)
				continue;
			set_dnvl(it->typrohr.dn);
			break;
		}
		if (celist.empty() || it == celist.end())
			getErrorHandler().appendError(MxErrorHandling::MxError("Konversion fehlgeschlagen", 0, MxErrorHandling::MxError::Critical));
	}
}


//Rohrkonstruktion erzeugen
void CadWerkauftrag::createModell()
{

	// Einlesen des XML und erzeugen einer Liste mit CAD Elementen
	create_elementarray();

	Rohrleitung::createModell(); // Für die bekloppte Sap Liste

	// Loeschen aller Elemente aus der Liste
	elementList.deleteAllElemente();
			
	if (get_laenge() > 0) 
	{
		if (!isGewinderohr())
		{
			// Laengen für Anfang/Ende/Verb. setzen 
			setAnfaEndeVerbLaenge();
	
			// Berechnen der Rohrstuecke
			setAutoRohrlaengen();
		}
		if (isGewinderohr())
		{
			Gewinderohr gw(elementArray, get_rohrAnfang(), get_rohrEnde());
	
			if (!gw.makeGewinderohr(elementList, getErrorHandler()))
				return;

			gw.auto_set_kaltbandgeschweisst(); 
			gw.setRohrquali(); // Vor Aufruf dieser Methode muss kaltbandgeschweisst gesetzt sein!
		}
		else// Elementliste fuer Rohrkonstruktion generieren
		{
			if (!makeRohre(elementList))
				return; 
		}
		
		// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen 
		rohreBeschriften(elementList);

		// Bei Rohrstuecken setzen ob mit od. ohne Rohrstutzen (fuers Pulvern)
		setRohrMitStut();
		calc_laengeProNr();		
	}
	delete_stueckliste();
	create_stueckliste();
}


void CadWerkauftrag::init_element_array(WaxCadModell& lst)
{
	/**
	Ruft das WaxCadModell zur Erzeugung des Elementarrays auf
	*/
	elementArray.deleteAll();

	int len = 0, 
		anfid = 0, 
		endid = 0, 
		rohrquali = 0;

	m_bTwisted = false;
	len = lst.create_waxstrang(anfid, endid, rohrquali, elementArray, gewinderohr, &m_bTwisted );

	set_rohrAnfang(anfid);
	
	set_rohrEnde(endid);

	if(gewinderohr)
	{
		set_rohrVerb(GEW);
		if(endid == GRFITTING)
			set_rohrEnde(GLATT);
	}

	set_laenge(len);
	set_rohrQuali(rohrquali);
	set_dichtmittel(lst.dichtmittel);
	
	int unused = 0;
	elementArray.sortElements(unused);

	if (gewinderohr)
	{
		for (int i = 0; i < elementArray.GetCount(); i++)
		{
			Element* e = elementArray.get_element_at(i);
			if (e )
				e->set_abstAbs(e->get_abstRel());
		}//EnContainerRohrElemente
	}

	TRACE("Anzahl Elemente in lst = %i\n", lst.size());
}

void CadWerkauftrag::init_infos(const WaxCadModell& lst)
{
	// Daten aus der Tabelle Werkauftrag werden beim Einlesen der DB gesetzt,
	// können aber auch über die Grafikerzeugung gesetzt werden. 
	
	if (lst.ferthinweis.length())
	{
		if(isGewinderohr() && m_bTwisted)
		{
			string rohrgedreht = "\n\nRohr wurde gedreht";
			rohrgedreht = rohrgedreht + lst.ferthinweis;
			set_fertigungshinweis(rohrgedreht.c_str());
		}
		else
		{
			set_fertigungshinweis(lst.ferthinweis.c_str());
		}
	}
	else
	{
		if(isGewinderohr() && m_bTwisted)
			set_fertigungshinweis("\n\nRohr wurde gedreht");
	}

	if (lst.posnr.length())
		set_posNr(lst.posnr.c_str());

	if (lst.anzahl)
		set_anzahl(lst.anzahl);

	dichtmittel = lst.dichtmittel;
	handelslaenge = lst.isHandelslaenge;
	m_bUeberlaenge = lst.m_MitUeberlaenge;
	m_IsApaxBauteil = lst.m_IsApaxBauteil;
}


void CadWerkauftrag::check_elementArray()
{
	///Kein Aufruf der Funktion in der Basisklasse, da CadWerkauftrag keine Abgaenge hat
}



bool CadWerkauftrag::createVk3Liste()
{
	return true;
}

const CString CadWerkauftrag::getBemassungstext() const
{
	return "";
}



void CadWerkauftrag::set_xml_modell(const string& xml) 
{ 
	xml_modell = xml; 
	createModell();

	//Sonderkosten für das Bauteil ermitteln
	this->get_preis().calcAutoSoko(true);
}

bool CadWerkauftrag::isGewinderohr() const
{
	return gewinderohr || Rohrleitung::isGewinderohr();
}

bool CadWerkauftrag::isHandelslaenge() const 
{ 
	return handelslaenge;
}


void CadWerkauftrag::set_dichtmittel(int dm)
{
	dichtmittel = dm;
}

int CadWerkauftrag::get_dichtmittel() const
{
	return dichtmittel;
}


bool CadWerkauftrag::isVerteiler() const
{
	if (isSdMaterialNrVerteiler())
		return true;

	return false;
}
bool CadWerkauftrag::isStrangrohr() const
{
	if (isSdMaterialNrStrangrohr())
		return true;

	return false;
}