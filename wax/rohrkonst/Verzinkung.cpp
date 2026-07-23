#include "stdafx.h"
#include "verzinkung.h"
#include "Rohrleitung.h"
#include "Arbeitsplan.h"
#include "Arbeitsfolge.h"
#include "Verteiler.h"

#if 0

#include "..\waxstamm\waxstamm_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

USE_ELEMENTS


///Standardkonstruktor
Verzinkung::Verzinkung() :
preisVerzinkung(0.0), preisZinkzuschlag(0.0), preisEnergiezuschlag(0.0), 
preisTransportversicherung(0.0), preisPutzen(0.0), preis(0.0),
zeitPutzen(0.0), zeitElemente(0.0), zeitStdElemente(0.0), zeitGewElemente(0.0), zeitInnen(0.0), zeitAussen(0.0)
{
}


///Verzinkung nach Rohrleitung erstellen
Verzinkung::Verzinkung(Rohrleitung* rohrleitung) :  
preisVerzinkung(0.0), preisZinkzuschlag(0.0), preisEnergiezuschlag(0.0), 
preisTransportversicherung(0.0), preisPutzen(0.0), preis(0.0),
zeitPutzen(0.0), zeitElemente(0.0), zeitStdElemente(0.0), zeitGewElemente(0.0), zeitInnen(0.0), zeitAussen(0.0)
{
	rohr = rohrleitung;
	stmdat = NULL;
	init_stmdat(rohr->get_preis().get_nrVerzinkerei());
}

///Destruktor
Verzinkung::~Verzinkung()
{
	clear_map(emap);
	clear_map(smap);
	clear_map(gmap);
}

///Loescht die Objekte und die Pointer aus der Map
void Verzinkung::clear_map(ElementMap& map)
{
	ElementMap::iterator it;
	for (it = map.begin(); it != map.end(); it++)
		delete it->first;
	map.clear();
}


bool Verzinkung::init_stmdat(int nr)
{
	for (POSITION pos = getStammdaten().get_verzinkerei().GetHeadPosition(); pos;)
	{
		Verzinkerei::data *v = (Verzinkerei::data *)getStammdaten().get_verzinkerei().GetNext(pos);

		if (v->get_nr() == nr)
		{
			stmdat = v;
			return true;
		}
	}
	XASSERT(stmdat != NULL);
	return false;
}

void Verzinkung::set_rohr(Rohrleitung* value)
{
	rohr = value;
	init_stmdat(rohr->get_preis().get_nrVerzinkerei());
}

/******************************* Preis fuers Verzinken ***********************************/

///Liefert den Gesamtpreis
double Verzinkung::getPreis()
{
	getPutzzeit();

	// Preis nach Gewicht
	preis = getPreisGewicht();

	// Zinkzuschlag
	preis += getPreisZinkzuschlag();

	// Preis Putzen
	preis += getPreisPutzen();

	// Energiezuschlag
	getPreisEnergiezuschlag();
	
	// Transportversicherung
	getPreisTransportversicherung();
	
	preis += preisZinkzuschlag + preisTransportversicherung;
	
	return preis;
}

///Liefert Preis nach Gewicht
double Verzinkung::getPreisGewicht()
{
	if (rohr->isRohrGlatt())
		preisVerzinkung = rohr->getGesGewicht() * stmdat->get_verzinkungskosten2();
	else
		preisVerzinkung = rohr->getGesGewicht() * stmdat->get_verzinkungskosten1();

	return preisVerzinkung; 
}

///Liefert Preis für Zinkzuschlag
double Verzinkung::getPreisZinkzuschlag()
{
	preisZinkzuschlag = preis * stmdat->get_zinkzuschlag()/100;
	return preisZinkzuschlag;
}

///Liefert Preis fuers Putzen
double Verzinkung::getPreisPutzen()
{
	preisPutzen = get_zeitPutzen() * stmdat->get_stundensatzPutzen();
	return preisPutzen;
}

///Liefert Preis für Energiezuschlag
double Verzinkung::getPreisEnergiezuschlag()
{	
	preisEnergiezuschlag = preis * stmdat->get_energiezuschlag()/100;
	return preisEnergiezuschlag;
}

///Liefert Preis für Transportversicherung
double Verzinkung::getPreisTransportversicherung()
{
	preisTransportversicherung = preis * stmdat->get_transportversicherung()/100;
	return preisTransportversicherung;
}

///Liefert die Zeit fuers Putzen aus dem Arbeitsplan
double Verzinkung::getZeit()
{
	CObList& arbeitsfolgen = rohr->get_arbeit().get_arbeitsfolgen();
	for (POSITION pos = arbeitsfolgen.GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo = (Arbeitsfolge *)arbeitsfolgen.GetNext(pos);
		XASSERT(afo);
		if (!afo)
			break;

		if (afo->get_id() == 34)
			return afo->get_zeit();
	}
	return 0.0;
}

/******************************* Zeit fuers Putzen ***********************************/

///Ermittelt die Gesamtzeit fuers Putzen 
double Verzinkung::getPutzzeit() 
{
	// Erst Liste mit zu putzenden Elemente fuellen
	insertListe();

	zeitPutzen = 0.0;

	zeitPutzen += getElementZeit();

	zeitPutzen += getPutzzeitInnen();

	zeitPutzen += getPutzzeitAussen();

	return zeitPutzen;
}

///Ermittelt die Elemente zum Putzen
void Verzinkung ::insertListe()
{
	/// Parameterliste als map, Liste mit initialisierten Objekten
	clear_map(emap);
	clear_map(smap);
	clear_map(gmap);

	PutzenZeiten putz;
	double zeit = 0.0;

	CObList &lst = rohr->get_elementList();

	for (POSITION pos = lst.GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(lst.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		zeit = 0.0;
		zeit = putz.getZeit(ele->get_id(), ele->get_dn());

		if (zeit > 0.0)
		{
			Element e(*ele); 
			e.set_anzahl(rohr->get_anzahl());
			insertEle(e, zeit);
		}

		CObList& ulist = ele->get_uelementList();
		for (POSITION upos = ulist.GetHeadPosition(); upos;)
		{
			Element *uele = dynamic_cast<Element*>(ulist.GetNext(upos));
			XASSERT(uele);
			if (!uele)
				break;

			zeit = 0.0;
			zeit = putz.getZeit(uele->get_id(), uele->get_dn());

			if (zeit > 0.0)
			{
				Element ue(*uele); 
				ue.set_anzahl(rohr->get_anzahl());
				insertEle(ue, zeit);
			}

			CObList& uulist = uele->get_uelementList();
			for (POSITION uupos = uulist.GetHeadPosition(); uupos;)
			{
				Element *uuele = dynamic_cast<Element*>(uulist.GetNext(uupos));
				XASSERT(uuele);
				if (!uuele)
					break;

				zeit = 0.0;
				zeit = putz.getZeit(uuele->get_id(), uuele->get_dn());

				if (zeit > 0.0)
				{
					Element uue(*uuele); 
					uue.set_anzahl(rohr->get_anzahl());
					insertEle(uue, zeit);
				}
			}
 		}
	}
}

///Fuegt die Elemente in die Maps ein
void Verzinkung::insertEle(const Element& ele, double zeit)
{
	insert(ele.get_id() == GEW ? gmap : smap, ele, zeit, ele.get_id() == GEW ? 2 : 1); 
	insert(emap, ele, zeit, 0); 
}

///Fuegt die Elemente in die Map ein
void Verzinkung::insert(ElementMap& map, const Element& ele, double zeit, int liste)
{
	ElementMap::iterator it;
	// Ist so ein Element schon drin, dann Anzahl addieren 
	for (it = map.begin(); it != map.end(); it++)
	{
		Element* e = it->first;
		if ((liste == 0 && ele.get_id() == e->get_id() && e->get_dn() == ele.get_dn())
 		||	(liste == 1 && ele.get_id() != GEW && e->get_anschlussDN() == ele.get_anschlussDN())
 		||	(liste == 2 && ele.get_id() == GEW && e->get_anschlussDN() == ele.get_anschlussDN()))
		{
			e->set_anzahl(e->get_anzahl() + ele.get_anzahl());
			return;
		}
	}
	map.insert(make_pair(new Element(ele), zeit));
}

///Ermittelt die Gesamtzeit fuer die Elemente
double Verzinkung::getElementZeit()
{
	zeitElemente = 0.0;
	ElementMap::iterator it;
	for (it = emap.begin(); it != emap.end(); it++)
	{
		zeitElemente += it->second * it->first->get_anzahl();
	}

	
	zeitStdElemente = 0.0;
	for (it = smap.begin(); it != smap.end(); it++)
	{
		zeitStdElemente += it->second * it->first->get_anzahl();
	}

	zeitGewElemente = 0.0;
	for (it = emap.begin(); it != emap.end(); it++)
	{
		zeitGewElemente += it->second * it->first->get_anzahl();
	}
	// Todo: wenn Berechnung nach Standardele+Gew, dann das rein   
	//return zeitStdElemente + zeitGewElemente;
	// und das raus;	
	return zeitElemente;
}

///Ermittelt die Putzzeit fuer den Innenbereich
double Verzinkung::getPutzzeitInnen() 
{
	PutzenZeiten putz;

	zeitInnen = putz.getZeit(ROHR_Q1, rohr->getMaxRohrDN()) * (double)rohr->get_anzahl();
	
	return zeitInnen;
}

///Ermittelt die Putzzeit fuer den Aussenbereich
double Verzinkung::getPutzzeitAussen() 
{
	zeitAussen = 0.0;
	PutzenZeiten putz;
	
	return zeitAussen;
}

/********************** MERGE **********************************/

///Addiert die Verzinkungen
void Verzinkung::merge(const Verzinkung& other)
{
	mergeMaps(other);

	zeitPutzen      += other.get_zeitPutzen();
	zeitElemente    += other.get_zeitElemente();
	zeitStdElemente += other.get_zeitStdElemente();
	zeitGewElemente += other.get_zeitGewElemente();
	zeitInnen       += other.get_zeitInnen();
	zeitAussen      += other.get_zeitAussen();

	preisVerzinkung				+= other.preisVerzinkung;
	preisZinkzuschlag			+= other.preisZinkzuschlag;
	preisPutzen					+= other.preisPutzen;
	preisEnergiezuschlag		+= other.preisEnergiezuschlag;
	preisTransportversicherung	+= other.preisTransportversicherung;
	preis						+= other.preis;
}

///Addiert Listen mit zu putzenden Elementen
void Verzinkung::mergeMaps(const Verzinkung& other)
{
	mergeMap(emap, other.emap, 0); 
	mergeMap(smap, other.smap, 1); 
	mergeMap(gmap, other.gmap, 2); 
}

///Addiert Listen mit zu putzenden Elementen
void Verzinkung::mergeMap(ElementMap& map, ElementMap m, int liste)
{
	ElementMap::iterator it;
	for (it = m.begin(); it != m.end(); it++)
	{
		insert(map, *it->first, it->second, liste);
	}
}

void Verzinkung::add_rohr(Rohrleitung* rohrltg)
{
	if (!stmdat)
		set_rohr(rohrltg); // Damit die Pointer initialisiert sind, auch wenn nur die Gesamtlisten gebraucht werden (siehe Druckliste)
	Verzinkung z(rohrltg);
	z.getPreis();
	merge(z);
}


double Verzinkung::get_preisPutzen() const
{
	return preisPutzen;
}

double Verzinkung::get_preisVerzinkung() const
{
	return preisVerzinkung;
}

double Verzinkung::get_preisZinkzuschlag() const
{
	return preisZinkzuschlag;
}

double Verzinkung::get_preisEnergiezuschlag() const
{
	return preisEnergiezuschlag;
}

double Verzinkung::get_preisTransportversicherung() const
{
	return preisTransportversicherung;
}

double Verzinkung::get_zeitPutzen() const
{
	return zeitPutzen;
}

double Verzinkung::get_zeitElemente() const
{
	return zeitElemente;
}

double Verzinkung::get_zeitStdElemente() const
{
	return zeitStdElemente;
}

double Verzinkung::get_zeitGewElemente() const
{
	return zeitGewElemente;
}

double Verzinkung::get_zeitInnen() const
{
	return zeitInnen;
}

double Verzinkung::get_zeitAussen() const
{
	return zeitAussen;
}

double Verzinkung::get_preis() const
{
	return preis;
}

#endif