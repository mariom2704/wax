#include "stdafx.h"
#include "Rohrleitung.h"


		#include "zinkkosten.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

#define putzen (get_putzkosten(STDELEMENT) + get_putzkosten(AUSSENGEW))

ZinkKosten::ZinkKosten(const Verzinkung::data* z, const Rohrleitung& r) : rohr(&r), verzinkerei(z)
{
	gesamt = grund = zuschlag = gewicht = vkzuschlag = 0.0;

	if (rohr)
		gewicht = rohr->getGesGewicht();
	if (verzinkerei)
	{
		scan_stueckliste();
		calc();
	}
}

void ZinkKosten::SetGesamtVerzinkerei(const Verzinkung::data* z)
{
	verzinkerei = z;
}
ZinkKosten::ZinkKosten(): rohr(NULL), verzinkerei(NULL)
{
	gesamt = grund = zuschlag = gewicht = vkzuschlag = 0.0;
}

void ZinkKosten::calc()
{
	calc_grund();
	calc_zuschlag();

	calc_vkzuschlag();
	gesamt = get_grund() + get_zuschlag() + putzen + get_vkzuschlag();
}


void ZinkKosten::calc_vkzuschlag()
{
	vkzuschlag = (get_grund() + get_zuschlag() + putzen) * verzinkerei->get_vkzuschlag() / 100.0;
	
}

void ZinkKosten::calc_grund()
{
	grund = get_gewicht() * verzinkerei->get_euro_pro_kg();
}

void ZinkKosten::calc_zuschlag()
{
	zuschlag = grund * verzinkerei->get_zinkpreisausgleich() / 100.0; 
}


/// Zusammenfassen zweier Rohrleitungen, es werden die einzelnen Kosten addiert
void ZinkKosten::add(const ZinkKosten& other)
{
	gesamt += other.get_gesamt();
	grund += other.get_grund();
	zuschlag += other.get_zuschlag();
	vkzuschlag += other.get_vkzuschlag();
	
	for (vector<verzele>::const_iterator it = other.elelist.begin(); it != other.elelist.end(); it++)
		elelist.push_back(*it);

	gewicht += other.get_gewicht();
	added++; 
}

/**
Durchsucht die Stückliste nach Rohrelementen, die geputzt werden müssen. 
Initialisiert die Elementliste.
*//*
Änderung 22.05.06, Jacobs: 
	- Bei der Ermittlung von Zeiten für Ausseng. bei den Elementen im Array muß der DN des Elementes genommen werden,
	  bei Standardelementen bezieht sich der DN Wert auf das Hauptrohr
	- Scheiben und Boden sind keine Standardelemente
*/
void ZinkKosten::scan_stueckliste()
{
	
	if (!rohr)
		return;
	elelist.clear();
	scan_stueckliste(rohr->get_elementList());
}



void ZinkKosten::scan_stueckliste(const EnCObListModell& liste)
{
	for (POSITION epos = liste.GetHeadPosition(); epos != NULL;)	
	{
		const Element *hele = dynamic_cast<const Element*>(liste.GetNext(epos));
		XASSERT(hele);
		if (!hele)
			continue;

		if (!hele)
			continue;

		if (hele->isEleRohr() || hele->get_id() == ROHR)
		{
			if (hele->get_uelementList().getSize() > 0)
				scan_stueckliste(hele->get_uelementList());
			
		}
		else
		{
			if(!ChangeVirtualAssembledElements(hele))
			{
				
				//Bei Außengewinde wird Gewinde von Element genommen.
				if (hele->get_id() == GEW || hele->get_id() == GEWINDE_KAPPE_COII  || hele->get_id() == GEWINDE_KAPPE_TYP_D || hele->get_id() == GWRST)
					add_ele(AUSSENGEW, hele->get_anzahl()*rohr->get_anzahl(), hele->get_dn());
				//Bei welchen Teilen Kosten berechnet werden
				else if (hele->isEleAbgang() ||
						 (hele->isEleAbgEnde() && hele->get_id() != GLATT) ||
						 hele->isEleFlansch())
					add_ele(STDELEMENT, hele->get_anzahl()*rohr->get_anzahl(), hele->get_dn());

				TRACE("Element: %s\n", hele->getBezeichnung());
			}
		}
	}
}


bool ZinkKosten::ChangeVirtualAssembledElements(const Element* element)
{

	if(GEGENFLANSCH_GEWINDE_COII == element->get_id() || GEGENFLANSCH_GEWINDE_MUFFE_COII == element->get_id())
	{
		Element elementu(FLANSCH_COII, 0, element->get_dn(), element->get_dn());
		Assembleelement(&elementu);
		
		Element elementuuuu(GEW, 0, element->get_dn(), element->get_dn());
		Assembleelement(&elementuuuu);	

		
		if( GEGENFLANSCH_GEWINDE_MUFFE_COII  == element->get_id())
		{
			Element MuffeElement(MUFFE_COII, 0, 15, 0);
			Assembleelement(&MuffeElement);
		}

		return true;
	}

	if(element->get_id() == ROHRNIPPEL_COII )
	{
		Element GewindeElement(GEW, 0, 50, 50);
		Assembleelement(&GewindeElement);	
		return true;
	}

	return false;			
}

void ZinkKosten::Assembleelement(Element* hele)
{
	//Bei Außengewinde wird Gewinde von Element genommen. 
	if (hele->get_id() == GEW || hele->get_id() == GWRST)
		add_ele(AUSSENGEW, hele->get_anzahl()*rohr->get_anzahl(), hele->get_dn());
	//Bei welchen Teilen Kosten berechnet werden
	else if (hele->isEleAbgang() ||
		     (hele->isEleAbgEnde() && hele->get_id() != GLATT) ||
			 hele->isEleFlansch())
		add_ele(STDELEMENT, hele->get_anzahl()*rohr->get_anzahl(), hele->get_dn());

	TRACE("Element: %s\n", hele->getBezeichnung());
}

void ZinkKosten::add_ele(enum ELE kennung, int anzahl, int dn)
{
	verzele e;
	e.anzahl = anzahl;
	e.dn = dn;
	e.kennung = kennung;
	e.zeit = e.anzahl * get_zeit(e.kennung, e.dn); 
	e.preis = e.zeit / 60.0 * verzinkerei->get_stdsatz();
	elelist.push_back(e);
}

double ZinkKosten::get_zeit(enum ELE elekennung, int dn) const
{
	/**
	Sucht aus der Tabelle putzen_zeiten die Zeit für das Standardelement oder Aussengewinde, entsprechend der DN
	*/
	PutzenZeiten::data *pz = NULL;
	for (POSITION pzpos = getStammdaten().get_putzenzeiten().GetHeadPosition(); pzpos;)
	{
		pz = (PutzenZeiten::data *)getStammdaten().get_putzenzeiten().GetNext(pzpos);

		if (pz->get_elemid() == elekennung && pz->get_dn() == dn)				 
			return pz->get_zeit();
	}
	return 0.0;
}


double ZinkKosten::get_euro_pro_kg() const
{
	/**
	Liefert den Stundensatz. Wenn die Zinkkosten nicht aufaddiert wurden
	wird der Wert aus den Stammdaten benutzt, wurden Positionen mit
	add() zusammenaddiert, wird der berechnete mittlere Wert zurückgeliefert
	*/
	if (!added && verzinkerei)
		return verzinkerei->get_euro_pro_kg();
	return 
		gewicht > 0.01 ? grund / gewicht : 0.0;
}


double ZinkKosten::get_stundensatz() const
{
	if (!added && verzinkerei)
		return verzinkerei->get_stdsatz();
	double gesamtzeit = get_putz_zeit(STDELEMENT) + get_putz_zeit(AUSSENGEW);
	double p = putzen;
	return gesamtzeit > 0.01 ? p / gesamtzeit * 60.0 : 0.0;
}


double ZinkKosten::get_zuschlagproz() const
{
	if (!added && verzinkerei)
		return verzinkerei->get_zinkpreisausgleich();
	return grund > 0.01 ? zuschlag / grund * 100.0 : 0.0;
}

string ZinkKosten::get_name() const
{
	string ret;
	if (verzinkerei)
		ret = verzinkerei->get_name();
	return ret;
}


double ZinkKosten::get_putz_zeit(enum ELE ele) const
{
	double putzzeit = 0.0;
	for (vector<verzele>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
	{
		if (it->kennung == ele)
			putzzeit += it->zeit;
	}
	return putzzeit;
}

double ZinkKosten::get_putzkosten(enum ELE ele) const
{
	double putzkosten = 0.0;
	for (vector<verzele>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
	{
		if (it->kennung == ele)
			putzkosten += it->preis;
	}
	return putzkosten;
}

int ZinkKosten::get_element_anzahl(enum ELE ele) const
{
	int eleanz = 0;
	for (vector<verzele>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
	{
		if (it->kennung == ele)
			eleanz += it->anzahl;
	}
	return eleanz;

}


double ZinkKosten::sum_zeit(enum ELE ele, int dn) const
{
	/**
	Bei dn = -1 werden alle Elemente des Typs aufsummiert
	*/
	double zeit = 0.0;
	for (vector<verzele>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
	{
		if (it->kennung == ele && (dn == -1  || it->dn == dn))
			zeit += it->zeit;
	}
	return zeit;
}

int ZinkKosten::sum_anz(enum ELE ele, int dn) const
{	
	/**
	Bei dn = -1 werden alle Elemente des Typs aufsummiert
	*/
	int anz = 0;
	for (vector<verzele>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
	{
		if (it->kennung == ele && (dn == -1 || it->dn == dn))
			anz += it->anzahl;
	}
	return anz;
}

double ZinkKosten::sum_putzkosten() const
{
	double kosten = 0.0;
	for (vector<verzele>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
		kosten += it->preis;
	return kosten;
}


double ZinkKosten::GesamtGrundkosten()
{
	return get_grund() * (1 + verzinkerei->get_gewichtszuschlag()/100); //* 1.06;
}


double ZinkKosten::GesamtGewicht()
{
	return get_gewicht() * (1 + verzinkerei->get_gewichtszuschlag()/100); //* 1.06;
}

double ZinkKosten::GesamtMGK()
{
	return  (GesamtGrundkosten() + GesamtZuschlag() + putzen) * (1 + verzinkerei->get_mgk()/100); //* 1.07;
}

double ZinkKosten::GesamtVKZuschlag()
{
	return  GesamtMGK()* verzinkerei->get_vkzuschlag() / 100;    //* 0.07;
}


double ZinkKosten::GesamtKosten()
{
	 return  GesamtMGK() + GesamtVKZuschlag();
}



double ZinkKosten::GesamtZuschlag()
{
	return GesamtGrundkosten() * 0.2;
}
