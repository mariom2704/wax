#include "stdafx.h"


#include <afxcoll.h>

#include "Rohrleitung.h"
#include "Strangrohr.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Standardkonstruktor
Arbeitsplan::Arbeitsplan():m_pRohr(NULL)
{
	m_bKumulieren=true;

	minderMengenZeit = 0.3001;
	minderMengenZeitzuschlag = 0.25;

	
}

//Destruktor
Arbeitsplan::~Arbeitsplan()
{
	clean();
}

//Kopierfunktion	
void Arbeitsplan::copy(const Arbeitsplan& other)
{
	// Arbeitsplan kopieren
	clean();

	const CObList& afolist = other.get_arbeitsfolgen();
	for (POSITION pos = afolist.GetHeadPosition(); pos != NULL;)	
	{
		const Arbeitsfolge *arbeitsfolge = dynamic_cast<const Arbeitsfolge*>(afolist.GetNext(pos));
		XASSERT(arbeitsfolge);
		if (arbeitsfolge)
		{
			Arbeitsfolge* afo = new Arbeitsfolge;
			afo->copy(*arbeitsfolge);
			arbeitsfolgen.AddTail(afo);
		}
	}
}



CString Arbeitsplan::get_abpSchweissVorrichtplatz()//"32000"
{
	return "32000";
}

void Arbeitsplan::merge(const Arbeitsplan& other)
{
	/**
	Addiert zwei Arbeitspläne
	*/
	
	
	const CObList& afolist = other.arbeitsfolgen;
	for (POSITION pos = afolist.GetHeadPosition(); pos != NULL;)	
	{
		const Arbeitsfolge *arbeitsfolge = dynamic_cast<const Arbeitsfolge*>(afolist.GetNext(pos));
		XASSERT(arbeitsfolge);
		if (arbeitsfolge)
		{
			POSITION p;
			for (p = arbeitsfolgen.GetHeadPosition(); p != NULL;)
			{
				Arbeitsfolge *af = dynamic_cast<Arbeitsfolge*>(arbeitsfolgen.GetNext(p));
				XASSERT(af);
				if (af)
				{
					if (af->get_id() == arbeitsfolge->get_id() && af->get_arbpl() == arbeitsfolge->get_arbpl())
					{
						if (af->merge(*arbeitsfolge))
						{
							p--;
							break;
						}
					}
				}
			}
			// Arbeitsfolge konnte nicht gemerged werden, also ans Ende hängen
			if (!p)
				arbeitsfolgen.AddTail(new Arbeitsfolge(*arbeitsfolge));
		}
	}
}

//Speichern und Laden der Daten
void Arbeitsplan::serialize(BinaryArchive& ar)
{
	if (ar.is_storing())
	{
		for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos;)
		{
			ar << (int)1;
			Arbeitsfolge *af  = (Arbeitsfolge*)get_arbeitsfolgen().GetNext(pos);
			af->serialize(ar);
		}
		ar << (int)0;
	}
	else
	{
		clean();
		int dummy;
		ar >> dummy;
		while (dummy)
		{
			Arbeitsfolge *af = new Arbeitsfolge;
			af->serialize(ar);
			get_arbeitsfolgen().AddTail((CObject*)af);
			ar >> dummy;
		}
	}
}



//Direkter Zugriff auf die Arbeitsfolgen
MxCObList& Arbeitsplan::get_arbeitsfolgen() 
{
	return arbeitsfolgen;
}

const MxCObList& Arbeitsplan::get_arbeitsfolgen() const
{
	return arbeitsfolgen;
}

//Einfügen einer Standardarbeitsfolge. Die restlichen Werte werden den 
//Stammdaten entnommen.
void Arbeitsplan::addAfo(Arbeitsfolge *af)
{
	// Prüfen, ob so eine Arbeitsfolge bereits im Arbeitsplan vorhanden ist. Wenn ja
	// wird die Anzahl und die Zeit dazugezählt.

	if (m_bKumulieren)
	{
		for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos; )
		{
			Arbeitsfolge *afx = (Arbeitsfolge *)get_arbeitsfolgen().GetNext(pos);
			if (afx->get_id() == af->get_id() && afx->get_arbpl() == af->get_arbpl() )
			{
				afx->set_anz(afx->get_anz() + af->get_anz());
				afx->set_zeit(afx->get_zeit() + af->get_zeit());
				afx->set_dn(af->get_dn());

				delete af;
				return;
			}
		}
	}

	get_arbeitsfolgen().AddTail(af);
}

//Liefert true zurück, wenn die Liste mit Arbeitsfolgen leer ist.
bool Arbeitsplan::isEmpty() const
{
	return arbeitsfolgen.IsEmpty() ? true : false;
}

//Sortiert die Afos nach Codenr.
void Arbeitsplan::sortAfos(MxCObList &afoliste)
{
	POSITION pos, nextpos; 
   
	for (int i=0; i < afoliste.GetCount()-1; i++)	
	{
		pos = afoliste.FindIndex(i);
		Arbeitsfolge *first = dynamic_cast<Arbeitsfolge *>(afoliste.GetAt(pos));
		nextpos = afoliste.FindIndex(i+1);
		Arbeitsfolge *second = dynamic_cast<Arbeitsfolge *>(afoliste.GetAt(nextpos));

		XASSERT(first && second);
		if (!first || !second)
			return;
		if (first->get_nr() > second->get_nr())
		{	
			// Arbeitsfolgen werden vertauscht
			afoliste.SetAt(nextpos, first);
			afoliste.SetAt(pos, second);
			i = -1; // Wieder von vorn beginnen
		}
	}
}

//Sortiert die Afos nach Codenr.
void Arbeitsplan::sortAfos()
{
	sortAfos(arbeitsfolgen);
}

// Fügt die Arbeitsfolge 99 (Übergabe an Produktion) hinzu
void Arbeitsplan::addAfo_Produktion()
{
	get_arbeitsfolgen().AddTail(new Arbeitsfolge(99));
}

// Fügt die Arbeitsfolge 8 (Rüstzeiten) hinzu
void Arbeitsplan::addAfo_Ruesten(int count)
{
	const int idruest = 8;
	Arbeitsfolge *af = new Arbeitsfolge(idruest);

	af->set_anz(count);
	// Afo erzeugen mit Zeit laut Stammdaten (0.5)
	AfoZeiten::data *z = NULL;
	POSITION pos;
	for (pos = getStammdaten().get_afozeiten().GetHeadPosition(); pos;)
	{
		z = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(pos);
		if (z->get_afoid() == idruest && z->get_dn() == 0)
			break;
	}
	af->set_zeit(z->get_zeit());

	// jedes weitere Umruesten laut Stammdaten bei dn = 25 (0.16) dazu
	if (count > 1)
	{
		for (pos = getStammdaten().get_afozeiten().GetHeadPosition(); pos;)
		{
			z = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(pos);
			if (z->get_afoid() == idruest && z->get_dn() == 25)
				break;
		}
		af->set_zeit(af->get_zeit() + (z->get_zeit() * (count-1)));
	}
	get_arbeitsfolgen().AddTail(af);
}

void Arbeitsplan::addAfos(CArray<int,int> * afolist, CArray<int,int> * dnlist,  const Beschichtung &beschichtung, int iFertigungsstätte, CString strArbeitsplatz, int iProduktTyp )
{
	for (int i = 0; i < afolist->GetSize(); i++)
	{
		if (!afolist->GetAt(i))
			continue;
		Arbeitsfolge *af = new Arbeitsfolge(afolist->GetAt(i));
		
		if(iFertigungsstätte == Werkauftrag::MAGDEBURG)
		{
			af->set_arbpl("99000");
			
			af->set_kostst(af->getKostst(af->get_arbpl()));
			af->set_ppstd(af->getTarif(af->get_kostst()));
			strArbeitsplatz = af->get_arbpl();
			//af->set_typ(0);
		}
		af->set_anz(1);
		af->set_zeit(getAfoZeit(af->get_id(), dnlist->GetAt(i), beschichtung, iFertigungsstätte, strArbeitsplatz, iProduktTyp));
		af->set_dn(dnlist->GetAt(i));
		//af->set_festpreis(getAfoFestpreis(af->get_id(),dnlist->GetAt(i)));

		addAfo(af);
	}
}

double Arbeitsplan::getAfoZeit(int id, int dn, const Beschichtung &beschichtung, int iFertigungsstätte, CString strArbeitsplatz, int iProduktTyp)
{
	// Zeit fuer Afo ermitteln	
	AfoZeiten::data *az = NULL;
	for (POSITION azpos = getStammdaten().get_afozeiten().GetHeadPosition(); azpos;)
	{
		az = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(azpos);



		if (az->get_Fertigungsstaette() == iFertigungsstätte && az->get_Arbeitsplatz() == strArbeitsplatz && az->get_produktTyp() == iProduktTyp && az->get_afoid() == id && az->get_dn() == dn) 
		{
			//Wenn verzinkt, dann prozentualer Aufschlag 
			if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
				return (az->get_zeit() * (1+az->get_zinkzuschlag()/100));
			return az->get_zeit();
		}
	}
	TRACE("Afozeit für Arbeitsfolge %d und DN=%d konnte nicht ermittelt werden\n", id, dn);
	return 0.0;
}



void Arbeitsplan::getAfoZeit(Material *mat, Arbeitsfolge *afo)
{
	// Zeit fuer Afo ermitteln	
	ProfileSaegezeiten::data *pz = NULL;
	for (POSITION pos = getStammdaten().get_profilesaegezeiten().GetHeadPosition(); pos;)
	{
		pz = (ProfileSaegezeiten::data *)getStammdaten().get_profilesaegezeiten().GetNext(pos);

		if ((pz->get_code() == afo->get_id()) && (pz->get_typ()			== mat->getProfiltyp()) 
											  && (pz->get_ende()		== mat->getProfilende())
											  && (pz->get_laengeVon()	<= mat->get_mass1() || pz->get_laengeVon() == 0)
											  && (pz->get_laengeBis()	>= mat->get_mass1() || pz->get_laengeBis() == 0)) 
		{
			afo->set_ruestzeit(pz->get_ruestzeit());
			afo->set_zeit(pz->get_zeit()*afo->get_anz());
			return;
		}
	}
	TRACE("Afozeit für Arbeitsfolge %d und Profiltyp=%s konnte nicht ermittelt werden\n", afo->get_id(), mat->getProfiltyp());
}

// Fügt alle einem Element zugeordneten Afos hinzu, afofirst und afolast 
// bestimmen welche arbeitsfolgen tatsaechlich genommen werden
void Arbeitsplan::addAfos(int count, Element *ele, const Beschichtung beschichtung, int iFertigungsstätte, CString strArbeitsplatz, int iProduktTyp, int afofirst, int afolast)
{
	CArray<int, int> afolst;
	TRACE("Bauelementeafos für %s id=%i\n", ele->get_bezeichnung(), ele->get_id());
	ele->getAfos(iProduktTyp, afolst);
	
	int radius = 0;
	if (ele->get_id() == ROHR)
	{
		AbzugRadius ar;
		radius = ar.getRadius(ele->get_dn(), ele->get_anschlussDN());
	}
	
	if (afolst.GetSize() == 0)
		return;

	for (int i = afofirst; i < afolast; i++)
	{
		if (!afolst.GetAt(i))
			continue;
		
		// Klinken nur, wenn Radius > 0 
		if (afolst.GetAt(i) == 9 && radius == 0)
			continue;
		
		Arbeitsfolge *af = new Arbeitsfolge(afolst.GetAt(i));
		TRACE("AfoNr: %i\n", af->get_id());
		af->set_dn(ele->get_dn());
		af->set_anz(count);
		af->set_zeit(af->get_zeit() + getAfoZeit(count, ele, af->get_id(), beschichtung,  iFertigungsstätte,  strArbeitsplatz,  iProduktTyp));
		//af->set_festpreis(getAfoFestpreis(af->get_id(),af->get_dn()));
		COIIVerteiler* pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(m_pRohr);
		if(pCOIIVerteiler)
		{
			pCOIIVerteiler->SchweissUHeftFaktorenhinzufuegen(af);
		}
		addAfo(af);
	}
}

//Hier wird die Zeit fuer die Afo ermittelt
double Arbeitsplan::getAfoZeit(int count, Element *ele, int afoid, const Beschichtung beschichtung, int iFertigungsstätte, CString strArbeitsplatz,  int iProduktTyp)
{
	if( iProduktTyp == Rohrleitung::WASSER)
	{
		return getAfoZeitWasser( count, ele,  afoid, iFertigungsstätte, strArbeitsplatz, beschichtung);
	}
	else
	{
		return getAfoZeitCOII( count, ele,  afoid, beschichtung);
	}
}





double Arbeitsplan::getAfoZeitCOII(int count, Element *ele, int afoid, const Beschichtung beschichtung)
{
	// Zeit fuer Afo ermitteln	
	AfoZeiten::data *az = NULL;

	for (POSITION azpos = getStammdaten().get_afozeiten().GetHeadPosition(); azpos;)
	{
		
		az = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(azpos);
		
		if(Rohrleitung::GAS ==  az->get_produktTyp())
		{
			// bei Rohrstutzen saegen muss die eigene Laenge genommen werden
			// Todo: die Schnittlaenge nehmen

			if ((ele->get_id() == ROHR || ele->get_id() == STDSTUTZEN) && (afoid == 2 || afoid == 3 || afoid == 11 || afoid == 13))
			{
				if ((az->get_elemid()     == ele->get_id() || az->get_elemid() == 0) 
				 && (az->get_afoid()       == afoid)
				 && (az->get_dn()         == ele->get_dn()) 
				 && (az->get_laenge_von() <= ele->get_laenge()      || az->get_laenge_von() == 0)
				 && (az->get_laenge_bis() >= ele->get_laenge()      || az->get_laenge_bis() == 0)
				 && (az->get_dnvl_von()   <= ele->get_anschlussDN() || az->get_dnvl_von()   == 0)
				 && (az->get_dnvl_bis()   >= ele->get_anschlussDN() || az->get_dnvl_bis()   == 0))
				{
					//Wenn verzinkt, dann prozentualer Aufschlag 
					if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
						return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
					return az->get_zeit() * count;
				}
			}
			else if (ele->get_id() == KNICK)
			{
				if ((az->get_elemid()     == ele->get_id() || az->get_elemid() == 0) 
				 && (az->get_afoid()       == afoid)
				 && (az->get_dn()         == ele->get_anschlussDN()) 
				 && (az->get_winkel_von() <= (int)ele->get_durchmesser() || az->get_winkel_von()  == 0)
				 && (az->get_winkel_bis() > (int)ele->get_durchmesser() || az->get_winkel_bis()   == 0))
				{
					//Wenn verzinkt, dann prozentualer Aufschlag 
					if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
						return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
					return az->get_zeit() * count;
				}
			}
			// ansonsten die Rohrlaenge
			else
			{
				if ((az->get_elemid() == ele->get_id() || az->get_elemid() == 0) && (az->get_afoid() == afoid))
				{
					if(dynamic_cast<Strangrohr*>(m_pRohr) && (Arbeitsfolge::eSTRANGROHR_STRASSE == afoid))
					{
						
						 if((az->get_dn() == ele->get_dn() || az->get_dn() == 0)
							 
							 && (az->get_laenge_von() <= (dynamic_cast<Strangrohr*>(m_pRohr))->get_laenge()  || az->get_laenge_von() == 0)
						
							&& (az->get_laenge_bis() >= (dynamic_cast<Strangrohr*>(m_pRohr))->get_laenge()  || az->get_laenge_bis() == 0)
						
							&& (az->get_dnvl_bis()   == ele->get_anschlussDN() || az->get_dnvl_bis()   == 0)

							&& ( az->get_element_anzahl_von() < (dynamic_cast<Strangrohr*>(m_pRohr))->getAnzahlMuffen() && (dynamic_cast<Strangrohr*>(m_pRohr))->getAnzahlMuffen() <= az->get_element_anzahl_bis() ||
								az->get_element_anzahl_von() < (dynamic_cast<Strangrohr*>(m_pRohr))->getAnzahlMuffen() &&  az->get_element_anzahl_bis() == 0  ||
								az->get_element_anzahl_von() == 0 &&  az->get_element_anzahl_bis() == 0 )

							&& ( az->get_element_anzahl_Id() == ele->get_ElementAnzahlid() || az->get_element_anzahl_Id() == 0 || az->get_element_anzahl_Id() == -1 )

							&& ( az->get_element_anzahl_DnVon() <  ele->get_ElementAnzahldnMuffeOrDBohrung() && ele->get_ElementAnzahldnMuffeOrDBohrung() <= az->get_element_anzahl_DnBis()  || az->get_element_anzahl_DnVon() == 0 && az->get_element_anzahl_DnBis() == 0 )
					
							&& (az->get_makeAnlage() == ele->get_makeAnlage())
						 )
						{	
							//Wenn verzinkt, dann prozentualer Aufschlag 
							if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
								return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
							return az->get_zeit() * count;
						}
					}
					else
					{
							if((az->get_dn() == ele->get_dn()) 
						 && (az->get_laenge_von() <= ele->get_rohrLaenge()  || az->get_laenge_von() == 0)
						 && (az->get_laenge_bis() >= ele->get_rohrLaenge()  || az->get_laenge_bis() == 0)
						 && (az->get_dnvl_von()   <= ele->get_anschlussDN() || az->get_dnvl_von()   == 0)
						 && (az->get_dnvl_bis()   >= ele->get_anschlussDN() || az->get_dnvl_bis()   == 0)
						 && (az->get_makeAnlage() == ele->get_makeAnlage()  || az->get_makeAnlage() == 0))
						{
							//Wenn verzinkt, dann prozentualer Aufschlag 
							if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
								return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
							return az->get_zeit() * count;
						}
					}
				}
			}
		}
	}
	TRACE("Afozeit für Elementid %i und Arbeitsfolge %i konnte nicht ermittelt werden\n", ele->get_id(), afoid);
 	return 0.0;
}




double Arbeitsplan::getAfoZeitWasser(int count, Element *ele, int afoid, int iFertigungsstätte, CString strArbeitsplatz, Beschichtung beschichtung)
{
	// Zeit fuer Afo ermitteln	
	AfoZeiten::data *az = NULL;

	for (POSITION azpos = getStammdaten().get_afozeiten().GetHeadPosition(); azpos;)
	{
		az = (AfoZeiten::data *)getStammdaten().get_afozeiten().GetNext(azpos);

		// bei Rohrstutzen saegen muss die eigene Laenge genommen werden
		// Todo: die Schnittlaenge nehmen
		if ((ele->get_id() == ROHR || ele->get_id() == STDSTUTZEN) && (afoid == 2 || afoid == 3 || afoid == 11 || afoid == 13))
		{
			if ((az->get_elemid()     == ele->get_id() || az->get_elemid() == 0) 
			 && (az->get_afoid()       == afoid)
			 && (az->get_dn()         == ele->get_dn())
			 
			 &&	(az->get_Fertigungsstaette() == iFertigungsstätte)
			 
			 && (az->get_Arbeitsplatz() == strArbeitsplatz)
			
			 && (az->get_laenge_von() <= ele->get_laenge()      || az->get_laenge_von() == 0)
			 && (az->get_laenge_bis() >= ele->get_laenge()      || az->get_laenge_bis() == 0)
			 && (az->get_dnvl_von()   <= ele->get_anschlussDN() || az->get_dnvl_von()   == 0)
			 && (az->get_dnvl_bis()   >= ele->get_anschlussDN() || az->get_dnvl_bis()   == 0))
			{
				//Wenn verzinkt, dann prozentualer Aufschlag 
				if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
					return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
				return az->get_zeit() * count;
			}
		}
		else if (ele->get_id() == KNICK)
		{
			if ((az->get_elemid()     == ele->get_id() || az->get_elemid() == 0) 
			 && (az->get_afoid()       == afoid)
			
			 && (az->get_dn()         == ele->get_anschlussDN()) 

			 &&	(az->get_Fertigungsstaette() == iFertigungsstätte)
			 && (az->get_Arbeitsplatz() == strArbeitsplatz)

			 && (az->get_winkel_von() < (int)ele->get_durchmesser() || az->get_winkel_von()  == 0)
			 && (az->get_winkel_bis() >= (int)ele->get_durchmesser() || az->get_winkel_bis()   == 0))
			{
				//Wenn verzinkt, dann prozentualer Aufschlag 
				if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
					return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
				return az->get_zeit() * count;
			}
		}
		// ansonsten die Rohrlaenge  
		else
		{
			if ((az->get_elemid() == ele->get_id() || az->get_elemid() == 0) && (az->get_afoid() == afoid))
			{
				if(dynamic_cast<Strangrohr*>(m_pRohr) && (Arbeitsfolge::eSTRANGROHR_STRASSE == afoid))
				{
					
					 if((az->get_dn() == ele->get_dn() || az->get_dn() == 0)
						 
						 && (az->get_laenge_von() <= (dynamic_cast<Strangrohr*>(m_pRohr))->get_laenge()  || az->get_laenge_von() == 0)
					
						&& (az->get_laenge_bis() >= (dynamic_cast<Strangrohr*>(m_pRohr))->get_laenge()  || az->get_laenge_bis() == 0)
					
						&& (az->get_dnvl_bis()   == ele->get_anschlussDN() || az->get_dnvl_bis()   == 0)
						
						&&	(az->get_Fertigungsstaette() == iFertigungsstätte)
						&& (az->get_Arbeitsplatz() == strArbeitsplatz)
						
						&& ( az->get_element_anzahl_von() < (dynamic_cast<Strangrohr*>(m_pRohr))->getAnzahlMuffen() && (dynamic_cast<Strangrohr*>(m_pRohr))->getAnzahlMuffen() <= az->get_element_anzahl_bis() ||
							az->get_element_anzahl_von() < (dynamic_cast<Strangrohr*>(m_pRohr))->getAnzahlMuffen() &&  az->get_element_anzahl_bis() == 0  ||
							az->get_element_anzahl_von() == 0 &&  az->get_element_anzahl_bis() == 0 )

						&& ( az->get_element_anzahl_Id() == ele->get_ElementAnzahlid() || az->get_element_anzahl_Id() == 0 || az->get_element_anzahl_Id() == -1 )

						&& ( az->get_element_anzahl_DnVon() <  ele->get_ElementAnzahldnMuffeOrDBohrung() && ele->get_ElementAnzahldnMuffeOrDBohrung() <= az->get_element_anzahl_DnBis()  || az->get_element_anzahl_DnVon() == 0 && az->get_element_anzahl_DnBis() == 0 )
				
						&& (az->get_makeAnlage() == ele->get_makeAnlage())
					 )
					{	
						//Wenn verzinkt, dann prozentualer Aufschlag 
						if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
							return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
						return az->get_zeit() * count;
					}
				}
				else
				{
						if((az->get_dn() == ele->get_dn()) 
					 && (az->get_laenge_von() <= ele->get_rohrLaenge()  || az->get_laenge_von() == 0)
					 && (az->get_laenge_bis() >= ele->get_rohrLaenge()  || az->get_laenge_bis() == 0)
					 &&	(az->get_Fertigungsstaette() == iFertigungsstätte)
					 && (az->get_Arbeitsplatz() == strArbeitsplatz)
					 && (az->get_dnvl_von()   <= ele->get_anschlussDN() || az->get_dnvl_von()   == 0)
					 && (az->get_dnvl_bis()   >= ele->get_anschlussDN() || az->get_dnvl_bis()   == 0)
					 && (az->get_makeAnlage() == ele->get_makeAnlage()  || az->get_makeAnlage() == 0))
					{
						//Wenn verzinkt, dann prozentualer Aufschlag 

						if (beschichtung.isVerzinkt() && az->get_zinkzuschlag() > 0.0)
							return (az->get_zeit() * (1+az->get_zinkzuschlag()/100) * count);
						return az->get_zeit() * count;
					}
				}
			}
		}
	}
	TRACE("Afozeit für Elementid %i und Arbeitsfolge %i konnte nicht ermittelt werden\n", ele->get_id(), afoid);
 	return 0.0;
}

//Fügt eine Arbeitsfolge fürs Beschichten ein. Bei Pulvern ist dies die Nr. 
//27.
void Arbeitsplan::addAfo_Beschichtung( int anzahl, const Beschichtung & beschichtung, double zeitPulvern, double zeitGrundieren, double zeitPutzen)
{
	UNREFERENCED_PARAMETER(zeitPutzen);

	int afonr[5] = { 0,0,0,0,0 };

	Beschichtungen::data *besch = NULL;
	for (POSITION pos = getStammdaten().get_beschichtungen().GetHeadPosition(); pos;)
	{
		besch = (Beschichtungen::data *)getStammdaten().get_beschichtungen().GetNext(pos);

		if (beschichtung.get_typ() == besch->get_index()) 
		{
			afonr[0] = besch->get_afo1();              
			afonr[1] = besch->get_afo2();             
			afonr[2] = besch->get_afo3();             
			afonr[3] = besch->get_afo4();             
			afonr[4] = besch->get_afo5();             
			break;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (!afonr[i])
			continue;
		Arbeitsfolge *afo = new Arbeitsfolge(afonr[i]);
		afo->set_anz(anzahl);

		if (beschichtung.isGrundiert())
			afo->set_zeit(zeitGrundieren);		
		else if (beschichtung.get_typ() == Beschichtung::ROT_GEPULV ||
			beschichtung.get_typ() == Beschichtung::WEISS_GEPULV)
		{
			afo->set_zeit(zeitPulvern);
			CString afotxt(afo->get_txt());
			if (afotxt.Find("%s") >= 0)
			{	// Beschichtungsstring zusammenbasteln 
				CString txt, str;
				afotxt.Replace(", RAL %i", "");
				str.Format("\"%s\"", beschichtung.get_bezeichnung());
				txt.Format(afotxt, str);
				afo->set_txt(txt);
			}
		}
		else if (beschichtung.get_ral() > 0)
		{
			afo->set_zeit(zeitPulvern);
		
			CString afotxt(afo->get_txt());
		
			// Farbe suchen in Raltabelle
			Ral ral;
			CString farbe = ral.getBezeichnung(beschichtung.get_ral());
			if (afotxt.Find("%s") >= 0)
			{	// Beschichtungsstring zusammenbasteln 
				CString txt;
				txt.Format(afotxt, farbe, beschichtung.get_ral());
				afo->set_txt(txt);
			}
		}
		// sonst schwarz
		addAfo(afo);
	}
}

//Fügt die Arbeitsfolge 41 oder 42 (verzinkt) ein (Bündeln und anliefern). 
//Der Unterschied der beiden Arbeitsfolgen ist nur der Arbeitsplatz.
void Arbeitsplan::addAfo_Anlieferung(bool verzinkt, bool pulveranlage)
{
	Arbeitsfolge *afanliefer = new Arbeitsfolge(pulveranlage ? 40 : verzinkt ? 42 : 41);
	addAfo(afanliefer);
}

	
void Arbeitsplan::clean()
{
	arbeitsfolgen.deleteAllElemente();
}

//Initialisiert den Text der Arbeitsfolgen 27/40/41/67, bzw. setzt den 
//Standardtext wenn setDefault true ist.
void Arbeitsplan::updateSpecialAF(Arbeitsfolge* af, const Beschichtung &beschichtung, double gewicht, int maxlaenge, int wanr)
{
	CString txt;
	/*
	Ist af gleich NULL, wird die gesamte Liste der Arbeitsfolgen durchgearbeitet
	*/
	bool process_all = af ? false : true;
	for (POSITION pos = arbeitsfolgen.GetHeadPosition(); pos; )
	{
		if (process_all)
		{
			af = dynamic_cast<Arbeitsfolge*>(arbeitsfolgen.GetNext(pos));
		}
		else
			pos = NULL;
		XASSERT(af);
		if (!af)
			return;

		if (af->get_isTextGeandert())
			continue;

		// bei Pulvern den Text zusammenbasteln
		if (af->get_id() == 27)
		{
			if (beschichtung.get_typ() == Beschichtung::ROT_GEPULV ||
				beschichtung.get_typ() == Beschichtung::WEISS_GEPULV)
			{
				CString afotxt(af->get_originalTxt());
				if (afotxt.Find("%s") >= 0)
				{	// Beschichtungsstring zusammenbasteln 
					CString txt, str;
					afotxt.Replace(", RAL %i", "");
					str.Format("\"%s\"", beschichtung.get_bezeichnung());
					txt.Format(afotxt, str);
					af->set_txt(txt);
				}
			}
			else
			{
				// Farbe suchen in Raltabelle
				Ral ral;
				CString farbe = ral.getBezeichnung(beschichtung.get_ral());

				if (af->get_originalTxt().Find("%s") >= 0)
				{
					txt.Format(af->get_originalTxt(), farbe, beschichtung.get_ral());
					af->set_txt(txt);
				}
			}
		}
		else if (af->get_id() == 67)
		{
			int ral = 0;
			if (beschichtung.isGepulvert())
			{
				if (beschichtung.get_typ() == Beschichtung::ROT_GEPULV)
					ral = 3000;
				else if	(beschichtung.get_typ() == Beschichtung::WEISS_GEPULV) 
					ral = 9010;
				else 
					ral = beschichtung.get_ral();
				txt.Format(af->get_originalTxt(), ral, wanr);
			}
			else
			{
				CString afotxt = af->get_originalTxt();
				afotxt.Replace(", RAL %i", "");
				txt.Format(afotxt, wanr);
			}
			af->set_txt(txt); 
		}
		else if (af->get_id() == 40 || af->get_id() == 42 || af->get_id() == 41)
		{	
			// Afo
			if (af->get_originalTxt().Find("%i") >= 0)
			{
				txt.Format(af->get_originalTxt(), (int)gewicht, (float)maxlaenge/1000);
				af->set_txt(txt);
			}
		}
	}
}

double Arbeitsplan::getZeit(bool gesamt)
{
	double zeit = 0;

	// gesamt = true: alles
	// gesamt = false: ohne Pulvern 
	for (POSITION pos = arbeitsfolgen.GetHeadPosition(); pos;)	
	{
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge *>(arbeitsfolgen.GetNext(pos));
		XASSERT(afo);
		if (!afo)
			break;

		if (!gesamt && afo->get_id() == 27)
			 continue;
	
		zeit += afo->get_zeit() + afo->get_ruestzeit();
	}
	return zeit;
}

double Arbeitsplan::getZeitArbeitsplatz(CString arbeitsplatz)
{
	double zeit = 0;

	for (POSITION pos = arbeitsfolgen.GetHeadPosition(); pos;)	
	{
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge *>(arbeitsfolgen.GetNext(pos));
		XASSERT(afo);
		if (!afo)
			break;

		if (afo->get_arbpl() == arbeitsplatz)
		{
			zeit += afo->get_zeit() + afo->get_ruestzeit();
		}
	}
	return zeit;
}

double Arbeitsplan::getZeitKostenstelle(int kostenstelle)
{
	double zeit = 0;

	for (POSITION pos = arbeitsfolgen.GetHeadPosition(); pos;)	
	{
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge *>(arbeitsfolgen.GetNext(pos));
		XASSERT(afo);
		if (!afo)
			break;

		if (afo->get_kostst() == kostenstelle)
		{
			zeit += afo->get_zeit() + afo->get_ruestzeit();
		}
	}
	return zeit;
}

///Hat Arbeitsplan Afo mit angegebener ID
bool Arbeitsplan::hasAfo(int id)
{ 
	for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if (!afo)
			break;

		if (afo->get_id() == id)
			return true;
	}
	return false;
}


///Returns an Arbeitsfolge
Arbeitsfolge* Arbeitsplan::getAfo(Arbeitsfolge::ÂRBEITSFOLGENTYP id)
{ 
	for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if (!afo)
			break;

		if (afo->get_id() == id)
			return afo;
	}
	return NULL;
}

///Hat Arbeitsplan Afo fuer angegebenen Arbeitsplatz
bool Arbeitsplan::hasArbeitsplatz(CString arbeitsplatz)
{ 
	for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if (!afo)
			break;
			
		if (afo->get_arbpl() == arbeitsplatz)
			return true;
	}
	return false;
}

void Arbeitsplan::setSap(int id, bool sap)
{
	for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if(!afo)
			break;
		
		if (afo->get_id() == id)
			afo->set_sap(sap);
	}
}

///Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16/17/54 gesetzt 
void Arbeitsplan::setKranzeit(double zeit, bool mitVerlaengerung)
{
	bool setzeit = false;

	for (POSITION pos = get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if(!afo)
			break;

		if (mitVerlaengerung)
		{
			if (afo->get_id() == 16 && zeit > 0.0)
			{
				setzeit = true;
				afo->set_ruestzeit(zeit);
			}
		}
		else
		{
			if (afo->get_id() == 17 || afo->get_id() == 54 || afo->get_id() == 16)
			{
				setzeit = true;
				afo->set_ruestzeit(zeit);
			}
		}
	}

}


///Afos fuer Schweisser zusammenfassen 
void Arbeitsplan::mergeSchweissafos()
{
	// Afo 54 auf Afo 17 aufschlagen, macht beides der Schweisser (dann nur 1 Lohnschein)
	Arbeitsfolge *afo54 = NULL, *afo17 = NULL;
	int count = 0, index = 0;

	for (POSITION posi = arbeitsfolgen.GetHeadPosition(); posi;)
	{
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge *>(arbeitsfolgen.GetNext(posi));
		XASSERT(afo);
		if (!afo)
			break;
		
		if (afo->get_id() == 54)
		{
			afo54 = afo;
			index = count; 
		}
		if (afo->get_id() == 17)
			afo17 = afo;

		count++;
	}
	if (afo54 && afo17)
	{
		afo17->set_zeit(afo17->get_zeit()+afo54->get_zeit());
		delete afo54;
		arbeitsfolgen.RemoveAt(arbeitsfolgen.FindIndex(index));
	}
}

/**
Mindermenge setzen. Wenn die Zeit bei den Afos 16 zusammen < 0.3001 ist, wird 
der Zuschlag gesetzt.

In der Programmierung darauf achten, dass er nur einmal gesetzt wird. (da Mindermengenzeit > MindermengenZeitzuschlag ist!)
*/
void Arbeitsplan::setMindermenge()
{
	
	Arbeitsfolge *afo16 = NULL;
				 

	double gesamtZeit = 0;
	
	for (POSITION posi = arbeitsfolgen.GetHeadPosition(); posi;)
	{
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge *>(arbeitsfolgen.GetNext(posi));
		XASSERT(afo);
		if (!afo)
			break;
		
		if (afo->get_id() == 16)
			afo16 = afo; //Afo 16 merken

		if (afo->get_id() == 16)
			gesamtZeit += afo->get_zeit();
	}

	//wenn Zeit kleiner Mindermengenzeit, dann Zuschlag setzen
	if (gesamtZeit < get_minderMengenZeit())
	{
		if (afo16)
			afo16->set_zeit(afo16->get_zeit() + get_minderMengenZeitzuschlag());
	}

	
}

///Fuegt einem Material zugeordnete Afo hinzu 
void Arbeitsplan::addAfos(Material *mat)
{
	CUIntArray afonr;
	mat->getAfos(afonr);
	
	for (int i=0; i<afonr.GetSize(); i++)
	{
		Arbeitsfolge *af = new Arbeitsfolge(afonr.GetAt(i));
		af->set_anz(mat->get_anzahl());
		getAfoZeit(mat, af);
		addAfo(af);
	}
}

/**
wenn m_bKumulieren auf false gesetzt wird, werden die Afos beim Einfügen in den Arbeitsplan nicht mehr
automatisch kumuliert. 
Standard ist m_bKumulieren=true
*/
void Arbeitsplan::set_kumulieren(bool value)
{
	m_bKumulieren = value;
}

///Ab der Mindermengenzuschlag berechnet wird
double Arbeitsplan::get_minderMengenZeit()
{
	return minderMengenZeit;
}

///Zuschlag der berechnet wird, wenn die Mindermengenzeit bestimmten Wert hat.
double Arbeitsplan::get_minderMengenZeitzuschlag()
{
	return minderMengenZeitzuschlag;
}


#ifdef _DEBUG
//Ausgabe der Elemente in der Liste
void Arbeitsplan::Dump(CDumpContext &dc) const
{
	//Werte von elementList ausgeben
	dc << "\n\nArbeitsplan: ";

	dc << arbeitsfolgen;
}

#endif