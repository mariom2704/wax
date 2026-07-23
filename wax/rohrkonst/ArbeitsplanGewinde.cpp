#include "stdafx.h"


#include "..\waxstamm\FormstueckeUndFittinge.h"
#include "RohrlisteAuftrag.h"
	
	#include "arbeitsplanGewinde.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

ArbeitsplanGewinde::ArbeitsplanGewinde()
{
	m_pArbeitsplan  = NULL;
	m_pBeschichtung = NULL;
	m_pModell       = NULL;

	m_rohrtyp		= TYPNOTSET;
	m_dichtmittel	= DICHTMITTELNOTSET;
	m_bUseArbeitsplanForVk3 = false;

}

ArbeitsplanGewinde::~ArbeitsplanGewinde()
{
}

/**
notwendigen Werte für die Ermittlung der Afos setzen.

Hinweis:
Zur Zeit wird noch der Rohrtyp übergeben, damit für nur diesen Typ die Afos erzeugt werden. Zukünftig sollten
automatisch allein aus dem Modell die richtigen Afos erzeugt werden.
*/
void ArbeitsplanGewinde::set_Werte(Werkauftrag* pRohr, ArbeitsplanGewinde::ROHRTYP typ,Beschichtung &be,EnCObListModell &modell, bool bUseArbeitsplanForVk3)
{
	m_pBeschichtung = &be;
	m_pModell = &modell;
	m_pGewinderohr = pRohr;
	m_bUseArbeitsplanForVk3 = bUseArbeitsplanForVk3;


	m_rohrtyp = typ;
}

///Gewinderohrwerte setzen
void ArbeitsplanGewinde::set_Gewinderohrwerte(ArbeitsplanGewinde::DICHTMITTEL dichtmittel)
{
	m_dichtmittel = dichtmittel;
}



/**
Prüfen ob Arbeitsplan, Beschichtung und Modell gesetzt sind
*/
bool ArbeitsplanGewinde::isSet()
{
	if (!m_pArbeitsplan)
		return false;

	if (!m_pBeschichtung)
		return false;

	if (!m_pModell)
		return false;

	return true;
}


/**
Arbeitsplan für das übergebene Modell erzeugen
*/
bool ArbeitsplanGewinde::createArbeitsplan(Arbeitsplan &ap)
{

	//Referenz auf Membervariable setzen
	m_pArbeitsplan = &ap;

	if (!isSet())
		return false;


	bool bReturn = true;

	//Allgemeine Afos setzen
	//..

	//Anhand des Rohrtyps die Afos erzeugen
/*	Hinweis:
	Zur Zeit wird noch der Rohrtyp übergeben, damit für nur diesen Typ die Afos erzeugt werden. Zukünftig sollten
	automatisch allein aus dem Modell die richtigen Afos erzeugt werden.
*/
	switch (m_rohrtyp)
	{
	case GEWINDEROHR:
		bReturn = createArbeitsplanGewinderohr();
		break;
	}



	//Afos sortieren
	/*
#ifdef _DEBUG
	afxDump << m_pArbeitsplan->get_arbeitsfolgen();
#endif
	*/
	m_pArbeitsplan->sortAfos(m_pArbeitsplan->get_arbeitsfolgen());

	return bReturn;
}


/**
Afos im Arbeitsplan mit Menge des Bauteils multiplzieren.
Ausnahme ist die Afo für Pulvern (da diese schon Menge der Bauteile beinhaltet).
*/
bool ArbeitsplanGewinde::setGesamtAnzahl(int anzahl)
{
	const int AFO_PULVERN = 27;
	const int AFO_GRUNDIEREN = 21;


	if (!isSet())
		return false;

	if (anzahl <= 0)
		return false;

	if (anzahl == 1)
		return true;

	
	for (POSITION pos = m_pArbeitsplan->get_arbeitsfolgen().GetHeadPosition(); pos != NULL;)
	{
		Arbeitsfolge* afo = (Arbeitsfolge*)m_pArbeitsplan->get_arbeitsfolgen().GetNext(pos);
		if (afo->get_id() != AFO_PULVERN  && afo->get_id() != AFO_GRUNDIEREN)
		{
			afo->set_anz(afo->get_anz() * anzahl);
			afo->set_zeit(afo->get_zeit() * anzahl);
		}
	}

	return true;
}






/**
Arbeitsplan für Gewinderrohr erstellen
*/
bool ArbeitsplanGewinde::createArbeitsplanGewinderohr()
{
	bool bReturn = true;
	
	//Prüfen ob notwendige Werte gesetzt sind
	if (!isSet())
		return false;

	if (m_dichtmittel == DICHTMITTELNOTSET)
		return false;
	
	//Arbeitsfolgen bestimmen

	if (!setAfo_RohrSaegen_GewindeSchneiden())
		bReturn = false;

	if (!setAfo_RohrleitungMitFittingAufdichten())
		bReturn = false;

	if (!setAfo_FittingAufFitting_InRohrachse())
		bReturn = false;

	if (!setAfo_FittingAufFitting_Abgang())
		bReturn = false;

	if (!setAfo_VNutEinrollenGewinderohr())
		bReturn = false;


	return bReturn;
}



void ArbeitsplanGewinde::FilterFertigugAfos(int iAnzahl)
{
	
	if(m_pGewinderohr->Get_Fertigungsstaette()== Werkauftrag::MAGDEBURG)
	{

		Arbeitsfolge *afx = new Arbeitsfolge (68); 
		const VK3Preise& vk3 = getStammdaten().get_vk3preise();
		for (POSITION pos = m_pArbeitsplan->get_arbeitsfolgen().GetHeadPosition(); pos; )
		{
			Arbeitsfolge *af = (Arbeitsfolge *)m_pArbeitsplan->get_arbeitsfolgen().GetNext(pos);
			
			int tg = af->get_id();
			
			if(tg != 99  && tg != 42) //Übergabe Produktion Bündeln
			{
				afx->set_zeit(afx->get_zeit() + af->get_zeit() * iAnzahl);
			//	af->set_typ(0);
			//	af->calc_preis();
				const VK3Preise::data *v = vk3.query(Werkauftrag::MAGDEBURG, af->get_id(), 0, af->get_dn(), 0, 0, 0, 0, 0, 0);
		
				if(v != NULL)
				{
					afx->set_preis(afx->get_preis() + v->get_preis() * af->get_anz() * iAnzahl);
				}
				
			}	
		}
		
		m_pArbeitsplan->get_arbeitsfolgen().RemoveAll();
		afx->set_arbpl("99000");
		m_pArbeitsplan->get_arbeitsfolgen().AddTail(afx);

		afx = new Arbeitsfolge (42);
		afx->set_arbpl("32999");
		m_pArbeitsplan->get_arbeitsfolgen().AddTail(afx);

		afx = new Arbeitsfolge (99);
		m_pArbeitsplan->get_arbeitsfolgen().AddTail(afx);

		m_pArbeitsplan->sortAfos();
	}

}


/**
Rohrmodell auf Arbeitsfolgen 4000, 4010, 4200 und 4210 prüfen
und bei Bedarf in Arbeitsplan schreiben
*/
bool ArbeitsplanGewinde::setAfo_RohrSaegen_GewindeSchneiden()
{
	const int AFO_ROHR_SAEGEN		= 4000;
	const int AFO_GEWINDESCHNEIDEN	= 4010;

	CArray<int, int> afolist;
	CArray<int, int> dnlist;
	

	Element *pElement	= NULL,
		    *pElPrev	= NULL,
			*pElNext	= NULL;


	if (!isSet())
		return false;


	//Rohre durchlaufen und bei jedem Rohr auf Afo prüfen, da Rohre unterschiedliche DN haben können 
	MxCObListIterator iter(*m_pModell);
	while (iter.next())
	{
		pElement = dynamic_cast<Element*>(m_pModell->getAt(iter));
		XASSERT(pElement);
		if (!pElement)
			continue;

		if (!pElement->isEleRohr()) 
			continue;

		if(m_bUseArbeitsplanForVk3 || m_pGewinderohr->Get_Fertigungsstaette()== Werkauftrag::MAGDEBURG)
		{
			//Afo ROHR_SAEGEN bestimmen
			afolist.Add(AFO_ROHR_SAEGEN + getOffsetAfoGewinderohr());
			dnlist.Add(pElement->get_dn());
		}



	//Afo GEWINDESCHNEIDEN bestimmen
		//Element vor dem Rohr holen und prüfen ob Gewinde schneiden Afo genommen werden muß
		pElPrev = dynamic_cast<Element*>(m_pModell->getAt(iter - 1));
		if (pElPrev)
		{
			if (pElPrev->get_id() == GEW || pElPrev->get_id() == GRFITTING)
			{
				afolist.Add(AFO_GEWINDESCHNEIDEN + getOffsetAfoGewinderohr());
				dnlist.Add(pElement->get_dn());
			}
		}

		//Element nach dem Rohr holen und prüfen ob Gewinde schneiden Afo genommen werden muß
		pElNext = dynamic_cast<Element*>(m_pModell->getAt(iter + 1));
		if (pElNext)
		{
			if (pElNext->get_id() == GEW || pElNext->get_id() == GRFITTING)
			{
				afolist.Add(AFO_GEWINDESCHNEIDEN + getOffsetAfoGewinderohr());
				dnlist.Add(pElement->get_dn());
			}
		}
	}

	//Liste mit Afos in den Arbeitsplan einfügen
	int iProduktionsstätte = Werkauftrag::OLDESLOE;
	if (!afolist.IsEmpty())
	{
		
		if(m_pGewinderohr->Get_Fertigungsstaette() ==  Werkauftrag::MAGDEBURG)
		{
			iProduktionsstätte = Werkauftrag::MAGDEBURG;
		}
		m_pArbeitsplan->addAfos(&afolist,&dnlist, *m_pBeschichtung, iProduktionsstätte, "0", Rohrleitung::WASSER);
		
	}

	return true;
}


void ArbeitsplanGewinde::createExtraAutoArbeitSaegen(RohrlisteAuftrag* pRohrlisteAuftrag)
{


	const int AFO_ROHR_SAEGEN = 4000;

	for (POSITION pos = pRohrlisteAuftrag->zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.GetHeadPosition(); pos != NULL;)	
	{
		Element* pElement = dynamic_cast<Element*>( pRohrlisteAuftrag->zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.GetNext(pos));
		XASSERT(pElement);
		if (!pElement)
			break;

		if (pElement->isEleRohr()) 
		{

			Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
					
			if (pNewAfo)
			{
				int anzahl = pElement->get_anzahl();;
				pNewAfo->set_anz(anzahl);
				if(m_pGewinderohr->Get_Fertigungsstaette() == Werkauftrag::OLDESLOE)
				{
					pNewAfo->set_arbpl("38030");	
					pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
					pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
					Arbeitsplaetze arb;
					pNewAfo->set_ruestzeit(arb.getRuestzeit(pNewAfo->get_arbpl()));

					pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),pElement->get_dn(), *m_pBeschichtung, m_pGewinderohr->Get_Fertigungsstaette(),  pNewAfo->get_arbpl(), Rohrleitung::WASSER));
					pNewAfo->set_dn(pElement->get_dn());
					
					m_pArbeitsplan->addAfo(pNewAfo);
				}
			}
		}
	}
	

	for (POSITION pos = pRohrlisteAuftrag->zuschnittliste_Gewinde_ab10Stueck_32070.GetHeadPosition(); pos != NULL;)	
	{
		Element* pElement = dynamic_cast<Element*>( pRohrlisteAuftrag->zuschnittliste_Gewinde_ab10Stueck_32070.GetNext(pos));
		XASSERT(pElement);
		if (!pElement)
			break;
		if (pElement->isEleRohr()) 
		{
			Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
		
			if (pNewAfo)
			{
				int anzahl = pElement->get_anzahl();;
				pNewAfo->set_anz(anzahl);
				if(m_pGewinderohr->Get_Fertigungsstaette() == Werkauftrag::OLDESLOE)
				{
					pNewAfo->set_arbpl("32070");	
					pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
					pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
					Arbeitsplaetze arb;
					pNewAfo->set_ruestzeit(arb.getRuestzeit(pNewAfo->get_arbpl()));
			
					pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),pElement->get_dn(), *m_pBeschichtung, m_pGewinderohr->Get_Fertigungsstaette(), pNewAfo->get_arbpl(), Rohrleitung::WASSER));
					pNewAfo->set_dn(pElement->get_dn());
					m_pArbeitsplan->addAfo(pNewAfo);
				}
			}
		}
	}

	for (POSITION pos = pRohrlisteAuftrag->zuschnittliste_Gewinde_bis10Stueck_31010.GetHeadPosition(); pos != NULL;)	
	{
		Element* pElement = dynamic_cast<Element*>( pRohrlisteAuftrag->zuschnittliste_Gewinde_bis10Stueck_31010.GetNext(pos));
		XASSERT(pElement);
		if (!pElement)
			break;
		if (pElement->isEleRohr()) 
		{
			Arbeitsfolge* pNewAfo = new Arbeitsfolge(AFO_ROHR_SAEGEN);
		
			if (pNewAfo)
			{
				int anzahl = pElement->get_anzahl();;
				pNewAfo->set_anz(anzahl);
				if(m_pGewinderohr->Get_Fertigungsstaette() == Werkauftrag::OLDESLOE)
				{
					pNewAfo->set_arbpl("31010");	
					pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
					pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
					Arbeitsplaetze arb;
					pNewAfo->set_ruestzeit(arb.getRuestzeit(pNewAfo->get_arbpl()));
			
					pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),pElement->get_dn(), *m_pBeschichtung, m_pGewinderohr->Get_Fertigungsstaette(),  pNewAfo->get_arbpl(), Rohrleitung::WASSER));
					pNewAfo->set_dn(pElement->get_dn());
					m_pArbeitsplan->addAfo(pNewAfo);
				}
			}
		}
	}
	m_pArbeitsplan->sortAfos();
};

/**
Rohrmodell auf Arbeitsfolgen 4020-4070 und 4220-4270 prüfen
und bei Bedarf in Arbeitsplan schreiben
*/
bool ArbeitsplanGewinde::setAfo_RohrleitungMitFittingAufdichten()
{
	const int AFO_ROHR_1_FITTING	= 4020;
	const int AFO_ROHR_2_FITTINGE	= 4030;
	const int AFO_ROHR_3_FITTINGE	= 4040;
	const int AFO_ROHR_4_FITTINGE	= 4050;
	const int AFO_ROHR_5_FITTINGE	= 4060;
	const int AFO_ROHR_6_FITTINGE	= 4070;

	int dn = 0,
		anzahlFittinge = 0;

	Arbeitsfolge *pNewAfo = NULL;

	Element *pElement  = NULL,
		    *pElPrev   = NULL,
			*pElNext   = NULL;


	if (!isSet())
		return false;


	//Rohr mit kleinster Nennweite ermitteln
	dn = m_pModell->getMinDn(true);

	//Fitting zählen, die min. auf einer Seite eine Verbindung zu einem Rohr haben
	MxCObListIterator iter(*m_pModell);
	while (iter.next())
	{
		pElement = dynamic_cast<Element*>(m_pModell->getAt(iter));
		XASSERT(pElement);
		if (!pElement)
			break;

		if (pElement->get_id() != GRFITTING) 
			continue;

		//Element vor und nach dem Fitting halen
		pElPrev = dynamic_cast<Element*>(m_pModell->getAt(iter - 1));
		pElNext = dynamic_cast<Element*>(m_pModell->getAt(iter + 1));
	
		//wenn eines der beiden Elemente ein Rohr ist, Counter hochsetzen
		if (pElPrev && pElPrev->isEleRohr()){
			anzahlFittinge++;
		}
		else if (pElNext && pElNext->isEleRohr()){
			anzahlFittinge++;
		}
	}


	
	//Anhand der Anzahl der Fitting die Afo bestimmen
	switch (anzahlFittinge)
	{
	case 1:
		pNewAfo = new Arbeitsfolge(AFO_ROHR_1_FITTING + getOffsetAfoGewinderohr());
		break;
	case 2:
		pNewAfo = new Arbeitsfolge(AFO_ROHR_2_FITTINGE + getOffsetAfoGewinderohr());
		break;
	case 3:
		pNewAfo = new Arbeitsfolge(AFO_ROHR_3_FITTINGE + getOffsetAfoGewinderohr());
		break;
	case 4:
		pNewAfo = new Arbeitsfolge(AFO_ROHR_4_FITTINGE + getOffsetAfoGewinderohr());
		break;
	case 5:
		pNewAfo = new Arbeitsfolge(AFO_ROHR_5_FITTINGE + getOffsetAfoGewinderohr());
		break;
	case 6:
		pNewAfo = new Arbeitsfolge(AFO_ROHR_6_FITTINGE + getOffsetAfoGewinderohr());
		break;

	default:
		pNewAfo = NULL;
		break;
	}


	//Afo in Arbeitsplan schreiben
	if (pNewAfo)
	{
		CString strArbeitsplatz = "0";
		int anzahl = 1;
		pNewAfo->set_anz(anzahl);
		int iFertigungsstätte = Werkauftrag::OLDESLOE;
		if(m_pGewinderohr->Get_Fertigungsstaette() == Werkauftrag::MAGDEBURG)
		{
			iFertigungsstätte = Werkauftrag::MAGDEBURG;
			pNewAfo->set_arbpl("99000");
			
			pNewAfo->set_kostst(pNewAfo->getKostst(pNewAfo->get_arbpl()));
			pNewAfo->set_ppstd(pNewAfo->getTarif(pNewAfo->get_kostst()));
			strArbeitsplatz = pNewAfo->get_arbpl();
		}
		pNewAfo->set_zeit(anzahl * m_pArbeitsplan->getAfoZeit(pNewAfo->get_id(),dn, *m_pBeschichtung, iFertigungsstätte, strArbeitsplatz, Rohrleitung::WASSER));
		pNewAfo->set_dn(dn);
		m_pArbeitsplan->addAfo(pNewAfo);
	}

	return true;
}

/**
Rohrmodell auf Arbeitsfolgen 4090,4080, 4280 und 4290 prüfen
und bei Bedarf in Arbeitsplan schreiben

Variante wenn T-Stück oder Kreuzstück Unterelemente hat.
*/
bool ArbeitsplanGewinde::setAfo_FittingAufFitting_Abgang()
{
	const int AFO_FITTING_AUF_FITTING = 4090;
	const int AFO_REDNIPPEL			  = 4080;
	
	CArray<int, int> afolist;
	CArray<int, int> dnlist;
	

	Element *pElement	= NULL,
		    *pUnterEl   = NULL;

   
	if (!isSet())
		return false;

	//in der Liste Fittinge mit Rednippeln suchen
	MxCObListIterator iter(*m_pModell);
	while (iter.next())
	{
		pElement = dynamic_cast<Element*>(m_pModell->getAt(iter));
		XASSERT(pElement);
		if (!pElement)
			break;

		//prüfen ob Fitting
		if (pElement->get_id() != GRFITTING) 
			continue;

		//prüfen ob Fitting Unterelemente hat
		EnCObListModell &unterElementList = pElement->get_uelementList();
		if (unterElementList.isEmpty())
			continue;

		//Unterelemente abarbeiten
		MxCObListIterator iterUe(unterElementList);
		while (iterUe.next())
		{
			pUnterEl = dynamic_cast<Element*>(m_pModell->getAt(iterUe));
			XASSERT(pUnterEl);
			if (!pUnterEl)
				break;

			//prüfen ob Fitting
			if (pElement->get_id() != GRFITTING) 
				continue;


			//Element hinzufügen
			if (pUnterEl->get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL)
					afolist.Add(AFO_REDNIPPEL + getOffsetAfoGewinderohr());
				else
					afolist.Add(AFO_FITTING_AUF_FITTING + getOffsetAfoGewinderohr());

			dnlist.Add(pUnterEl->get_dn1());
		}
	}


	//Liste mit Afos in den Arbeitsplan einfügen
	if (!afolist.IsEmpty())
	{
		int iFertigungsstätte = Werkauftrag::OLDESLOE;
		if(m_pGewinderohr->Get_Fertigungsstaette() ==  Werkauftrag::MAGDEBURG)
		{
			iFertigungsstätte = Werkauftrag::MAGDEBURG;
		}
		m_pArbeitsplan->addAfos(&afolist,&dnlist,  *m_pBeschichtung,  iFertigungsstätte, "0", Rohrleitung::WASSER);
	}

	return true;
}

/**
Rohrmodell auf Arbeitsfolgen 4090,4080, 4280 und 4290 prüfen
und bei Bedarf in Arbeitsplan schreiben

Wenn im Rohrstrang zwei Fittinge nebeneinander sitzen
*/
bool ArbeitsplanGewinde::setAfo_FittingAufFitting_InRohrachse()
{
	const int AFO_FITTING_AUF_FITTING = 4090;
	const int AFO_REDNIPPEL			  = 4080;
	
	CArray<int, int> afolist;
	CArray<int, int> dnlist;

	Element *pElement	= NULL,
			*pElNext	= NULL;


	if (!isSet())
		return false;

	//prüfen ob zwei Fittinge direkt aneinander liegen
	MxCObListIterator iter(*m_pModell);
	while (iter.next())
	{
		pElement = dynamic_cast<Element*>(m_pModell->getAt(iter));
		XASSERT(pElement);
		if (!pElement)
			break;

		if (pElement->get_id() != GRFITTING) 
			continue;

		//prüfen ob Nächstes Element auch Fitting ist
		pElNext = dynamic_cast<Element*>(m_pModell->getAt(iter + 1));
		if (pElNext)
		{
			if (pElNext->get_id() == GRFITTING)
			{
				if (pElNext->get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL)
					afolist.Add(AFO_REDNIPPEL + getOffsetAfoGewinderohr());
				else
					afolist.Add(AFO_FITTING_AUF_FITTING + getOffsetAfoGewinderohr());

				dnlist.Add(pElement->get_dn());
			}
		}	
	}

	//Liste mit Afos in den Arbeitsplan einfügen
	if (!afolist.IsEmpty())
	{	
		int iFertigungsstätte = Werkauftrag::OLDESLOE;
		if(m_pGewinderohr->Get_Fertigungsstaette() == Werkauftrag::MAGDEBURG)
		{
			iFertigungsstätte = Werkauftrag::MAGDEBURG;
		}
		m_pArbeitsplan->addAfos(&afolist,&dnlist,  *m_pBeschichtung, iFertigungsstätte, "0", Rohrleitung::WASSER);
	}

	return true;
}

/**
Prüfen auf Afo 4015 und 4215 (V-Nuten bei Gewinderohr einrollen)
*/
bool ArbeitsplanGewinde::setAfo_VNutEinrollenGewinderohr()			
{
	const int AFO_VNUT_EINROLLEN = 4015;

	if (!isSet())
		return false;

	CArray<int, int> afolist;
	CArray<int, int> dnlist;
	

	Element *pElement = NULL;

	//Nut am bauteilanfang
	MxCObListIterator iter(*m_pModell);
	if (iter.first())
	{
		pElement = dynamic_cast<Element*>(m_pModell->getAt(iter));
		XASSERT(pElement);
		if (!pElement)
			return false;

		if (pElement->get_id() == NUT)
		{
			afolist.Add(AFO_VNUT_EINROLLEN + getOffsetAfoGewinderohr());
			dnlist.Add(pElement->get_dn());
		}
	}


	//Nut am Bauteilende
	if (m_pModell->getSize() > 2)
	{
		if (iter.last())
		{
			pElement = dynamic_cast<Element*>(m_pModell->getAt(iter));
			XASSERT(pElement);
			if (!pElement)
				return false;

			if (pElement->get_id() == NUT)
			{
				afolist.Add(AFO_VNUT_EINROLLEN + getOffsetAfoGewinderohr());
				dnlist.Add(pElement->get_dn());
			}
		}
	}

	//Liste mit Afos in den Arbeitsplan einfügen
	if (!afolist.IsEmpty())
	{
		int iFertigungsstätte = Werkauftrag::OLDESLOE;
		if(m_pGewinderohr->Get_Fertigungsstaette() == Werkauftrag::MAGDEBURG)
		{
			iFertigungsstätte = Werkauftrag::MAGDEBURG;
		}
		m_pArbeitsplan->addAfos(&afolist,&dnlist,  *m_pBeschichtung, iFertigungsstätte, "0", Rohrleitung::WASSER);
	}

	return true;
}


/**
Offset bei Gewinderohr Afos bei Dichtmittel Hanf+Kitt
Afos ab 4200.
*/
int ArbeitsplanGewinde::getOffsetAfoGewinderohr()
{
	const int OFFSET_AFO_GEWINDEROHR_DICHTMITTEL_HANF = 200;

	
	//Offset für Gewinderohr Afos ab 4200
	if (m_dichtmittel == HANF || 
		m_dichtmittel == SYNTHESOL || 
		m_dichtmittel == TEFLON ||
		m_dichtmittel == NEOFERMIT)
	{
		return OFFSET_AFO_GEWINDEROHR_DICHTMITTEL_HANF;
	}

	return 0;
}