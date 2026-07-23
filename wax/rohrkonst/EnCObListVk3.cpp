#include "stdafx.h"

#include ".\encoblistvk3.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



EnCObListVk3::EnCObListVk3(void) 
{
	auto_delete = false;
	Rortype= eRohrTypeUknownRohr;
}

EnCObListVk3::~EnCObListVk3(void)
{

}


bool EnCObListVk3::copy( EnCObListVk3& other)
{
	EnCObListModell::copy(other);
	Rortype = other.GetRohrType();

	return true;
};

///Vk3 Preise aus den Stammdaten lesen
int EnCObListVk3::setVk3Preise(bool verzinkt, int Rohrlaenge,  int AnzahlElemente, int ElementLaenge)
{
	
	// Preis berechnen
	int iPreiswertigkeit = 0;	
	
	for(POSITION pos = GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if(GetRohrType() == ROHR_Q16 || GetRohrType() == ROHR_Q18 || GetRohrType() == ROHR_Q19)
		{		
			CString strKurztext = "";

			
			if( ele->get_id() == ROHRNIPPEL_COII || ele->get_id() == ROHR )
			{
				strKurztext = ele->getBezeichnung();
				ele->initStammdatenVK3Ele(ele->get_dn(), ele->get_anschlussDN(),  GetRohrType(), 0, 0, 0, ele->get_id(), 0, 0, false);
				ele->set_kurztext(strKurztext);
			}
			else
			{
				ele->initStammdatenVK3Ele(ele->get_dn(), ele->get_anschlussDN(),  GetRohrType(), 0, 0, 0, ele->get_id(), 0, 0, false);
			}
		}
		else if(GetRohrType() == eRohrTypeStrangRohr)
			 {		
				if(ele->get_makeAnlage())
				{					
					int ilocalPreiswertigkeit = ele->initStammdatenVK3Ele(ele->get_dn(), ele->get_anschlussDN(), ele->get_id(), verzinkt, Rohrlaenge, AnzahlElemente, ele->get_ElementAnzahlid(), ele->get_ElementAnzahldnMuffeOrDBohrung(), ElementLaenge, true); 	
					if(ilocalPreiswertigkeit > 0)
					{
						iPreiswertigkeit = ilocalPreiswertigkeit;
					}
				}
				else if(ele->get_id() == 51 && (ele->get_dn() == 65 || ele->get_dn() == 80) && (ele->get_ElementAnzahldnMuffeOrDBohrung() == 20 ||  ele->get_ElementAnzahldnMuffeOrDBohrung() == 25))
				{					
					int ilocalPreiswertigkeit = ele->initStammdatenVK3Ele(0, ele->get_anschlussDN(), ele->get_id(), verzinkt, 0, 0, ele->get_ElementAnzahlid(), ele->get_ElementAnzahldnMuffeOrDBohrung(), 0, ele->get_makeAnlage()); 	
					if(ilocalPreiswertigkeit > 0)
					{
						iPreiswertigkeit = ilocalPreiswertigkeit;
					}
				}else
				{
					int ilocalPreiswertigkeit = ele->initStammdatenVK3Ele(ele->get_dn(), ele->get_anschlussDN(), ele->get_id(), verzinkt, 0, 0, 0, 0, 0, 0);
					if(ilocalPreiswertigkeit > 0)
					{
						iPreiswertigkeit = ilocalPreiswertigkeit;
					}
				}
			 }
			 else
			 {
				int ilocalPreiswertigkeit = ele->initStammdatenVK3Ele(ele->get_dn(), ele->get_anschlussDN(), ele->get_id(), verzinkt, 0, 0, 0, 0, 0, 0);
				if(ilocalPreiswertigkeit > 0)
				{
					iPreiswertigkeit = ilocalPreiswertigkeit;
				}
			 }
	}
	return iPreiswertigkeit;
}


///Liste kumulieren, gleiche Elemente zusammenfassen
bool EnCObListVk3::kumulieren()
{
	EnCObListVk3 templist;

	//Kopie der Liste 
	templist.copy(*this/*,true,true*/);



	//vk3liste löschen
	deleteAllElemente();

	//Templiste durchlaufen und vk3liste neu aufbauen
	for (POSITION pos = templist.GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(templist.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		POSITION upos;
		// Element schon in vk3, dann Anzahl addieren
		for (upos = GetHeadPosition(); upos;)
		{
			Element *elem = dynamic_cast<Element*>(GetNext(upos));
			XASSERT(elem);
			if (!elem)
				break;

			if (elem->get_id() == ele->get_id() && elem->get_anschlussDN() == ele->get_anschlussDN()
												&& elem->get_dn()		   == ele->get_dn())
			{
				if (elem->get_id() != GRFITTING)
				{
					if (elem->isEleVK3Rohr() || elem->isElePulvern())
						elem->set_laenge(elem->get_laenge() + ele->get_laenge());
					else
					{
						if(elem->get_makeAnlage() && ele->get_makeAnlage() && elem->get_idCounterVk3Preis() == ele->get_idCounterVk3Preis())
						{
							elem->set_anzahl(elem->get_anzahl() + ele->get_anzahl());
						}
						else
						{
							if(!elem->get_makeAnlage() && !ele->get_makeAnlage() && elem->get_idCounterVk3Preis() == ele->get_idCounterVk3Preis())
							{
								elem->set_anzahl(elem->get_anzahl() + ele->get_anzahl());
							}
							else
							{
								continue;;
							}
						}
					}			
				}
				else if (elem->get_artikelnr() == ele->get_artikelnr()) //Fittinge auf Artikelebene kumulieren
				{
					elem->set_anzahl(elem->get_anzahl() + ele->get_anzahl());
				}
				else
					continue;

				upos--;
				break;
			}
		}

		
		
		if(upos == NULL)   
		{
			Element *e = new Element;
			e->copy(*ele);
			if (ele->isEleVK3Rohr())
				e->set_laenge(e->get_laenge());
			else
				e->set_anzahl(e->get_anzahl());
			AddTail(e);
		}
		
	}


	//Speicher freigeben
	templist.deleteAllElemente();


	return true;
}



bool EnCObListVk3::setMenge(int menge)
{

	//Templiste durchlaufen und vk3liste neu aufbauen
	for (POSITION pos = GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->isEleVK3Rohr() || ele->isElePulvern())
			ele->set_laenge(ele->get_laenge() * menge);
		else
			ele->set_anzahl(ele->get_anzahl() * menge);
	}

	return true;
}


double EnCObListVk3::getGesamtpreis()
{
	double preisGes = 0,
		   preisEl;

	for (POSITION pos = GetHeadPosition(); pos;)
	{
		Element *ele = dynamic_cast<Element*>(GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		preisEl = ele->get_festPreis();

		if (ele->isEleVK3Rohr() || ele->isElePulvern())
			preisGes += preisEl * (double)ele->get_laenge() / 1000.0;
		else
			preisGes += (double)ele->get_anzahl()  * preisEl;
	}


	return preisGes;
}