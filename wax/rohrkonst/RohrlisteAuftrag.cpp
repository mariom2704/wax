#include "stdafx.h"


#include "elementlist.h"
#include "Gewinderohr.h"
#include "RohrlisteAuftrag.h"

using namespace WaxDruck;


RohrlisteAuftrag::RohrlisteAuftrag(Werkauftrag* w) 
{

	if (!w)
		return;

	
		createSaegeMaschinenGewinderohrlaengen(w);
	
		
		//int limit = 2;
		int counter = 0;
		for (POSITION pos = zuschnittliste_Gewinde_gesamt.GetHeadPosition(); pos != NULL;)	
		{
			Element *element = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetNext(pos));
			XASSERT(element);
			if (!element)
				break;

			if (element->isEleRohr()) 
			{
				if ( 20 <= element->get_anzahl() && 50 <= element->get_laenge() && element->get_laenge() <= 500 ) 
				{
					POSITION p = zuschnittliste_Gewinde_gesamt.FindIndex(counter+1); 
					Element *prev = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(p));
					p = zuschnittliste_Gewinde_gesamt.FindIndex(counter+2); 
					Element *next = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(p));
					XASSERT(prev && next);
					if (!prev || !next)
						break;
					zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.AddTail(element);
					zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.AddTail(prev);
					zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030.AddTail(next);
					
				}else if ( 10 <= element->get_anzahl()) 
					{
						POSITION p = zuschnittliste_Gewinde_gesamt.FindIndex(counter+1); 
						Element *prev = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(p));
						p = zuschnittliste_Gewinde_gesamt.FindIndex(counter+2); 
						Element *next = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(p));
						XASSERT(prev && next);
						if (!prev || !next)
							break;

						zuschnittliste_Gewinde_ab10Stueck_32070.AddTail(element);
						zuschnittliste_Gewinde_ab10Stueck_32070.AddTail(prev);
						zuschnittliste_Gewinde_ab10Stueck_32070.AddTail(next);
						
					}
				
				else if ( 10 > element->get_anzahl()) 
					{
						POSITION p = zuschnittliste_Gewinde_gesamt.FindIndex(counter+1); 
						Element *prev = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(p));
						p = zuschnittliste_Gewinde_gesamt.FindIndex(counter+2); 
						Element *next = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(p));
						XASSERT(prev && next);
						if (!prev || !next)
							break;

						zuschnittliste_Gewinde_bis10Stueck_31010.AddTail(element);
						zuschnittliste_Gewinde_bis10Stueck_31010.AddTail(prev);
						zuschnittliste_Gewinde_bis10Stueck_31010.AddTail(next);
						
					}
					
			}

			counter++;
		 }

	
	
	

}



RohrlisteAuftrag::~RohrlisteAuftrag()
{
	deleteMaschinenlisten();
}



//Löscht die Zuschnittslisten und die Rohrnutenlisten
void RohrlisteAuftrag::deleteMaschinenlisten()
{
	CObList *lst[] = 
	{ 
		&zuschnittliste_Gewinde_gesamt,
		&zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030,
		&zuschnittliste_Gewinde_ab10Stueck_32070,
		&zuschnittliste_Gewinde_bis10Stueck_31010,
		NULL 
	};

	for (int i = 0; lst[i]; i++)
	{
		//for (POSITION p = lst[i]->GetHeadPosition(); p != NULL;)	
		//	delete lst[i]->GetNext(p);
		lst[i]->RemoveAll();
	}
	
}




//Erfast die Daten fuer die Gewinderohrliste
void RohrlisteAuftrag::createSaegeMaschinenGewinderohrlaengen(Werkauftrag* werkauftrag)
{
	
	Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(werkauftrag);
	XASSERT(grohr);
	if (!grohr)
		return;


	zuschnittliste_Gewinde_gesamt.ResetGesamtlaenge();
	zuschnittliste_Gewinde_gesamt.ResetAnzahlRohrstuecke();
	zuschnittliste_Gewinde_gesamt.GetAnzahlFittinge();
	

	ElementList elelst(grohr->get_elementList());
	
	// Abarbeiten der Elementliste
	//int counter = 0;
	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *element = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(element);
		if (!element)
			break;

		if (element->get_id() == GRFITTING)
			zuschnittliste_Gewinde_gesamt.SetAnzahlFittinge(grohr->get_anzahl());
		else if (element->isEleRohr()) 
		{
			// vorheriges und naechstes Element fuer diverse Abfragen
			Element* prevelement = NULL;
			Element* nextelement = NULL;
			prevelement = elelst.get_prev(count);
			nextelement = elelst.get_next(count);
			XASSERT(prevelement && nextelement);
			if (!prevelement || !nextelement)
				break;

			zuschnittliste_Gewinde_gesamt.SetAnzahlRohrstuecke(grohr->get_anzahl());
			
			int counter = 0;
			POSITION upos;
			// Element schon in Gesamtliste, dann Anzahl addieren
			for (upos = zuschnittliste_Gewinde_gesamt.GetHeadPosition(); upos;)
			{
				Element *ele = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetNext(upos));
				XASSERT(ele);
				if (!ele)
					break;

				if (ele->isEleRohr())
				{
					POSITION pp = zuschnittliste_Gewinde_gesamt.FindIndex(counter+1);
					Element* prevele = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(pp));
					POSITION np = zuschnittliste_Gewinde_gesamt.FindIndex(counter+2);
					Element* nextele = dynamic_cast<Element*>(zuschnittliste_Gewinde_gesamt.GetAt(np));
					XASSERT(prevele && nextele);
					if (!prevele || !nextele)
						break;

					if (ele->get_artikelnr() == element->get_artikelnr()
					//if (ele->get_id() == element->get_id() 
					// && ele->get_dn() == element->get_dn()
					 && ele->get_laenge() == element->get_laenge()
					 && (prevele->get_id() == prevelement->get_id() 
						|| prevele->get_id() == GEW    && prevelement->get_id() == GRFITTING 
						|| prevele->get_id() == GRFITTING && prevelement->get_id() == GEW) 
					 && (nextele->get_id() == nextelement->get_id() 
						|| nextele->get_id() == GEW    && nextelement->get_id() == GRFITTING 
						|| nextele->get_id() == GRFITTING && nextelement->get_id() == GEW)) 
					{
						ele->set_anzahl(ele->get_anzahl() + element->get_anzahl()*grohr->get_anzahl());
						upos--;
						break;
					}
				}
				counter++;
			}
			// in Gesamtliste packen
			if (upos == NULL) 
			{
				Element *e = new Element;
				e->copy(*element);
				e->set_anzahl(e->get_anzahl()*grohr->get_anzahl()); 
				zuschnittliste_Gewinde_gesamt.AddTail(e);
				
				Element *pe = new Element;
				pe->copy(*prevelement);
				zuschnittliste_Gewinde_gesamt.AddTail(pe);

				Element *ne = new Element;
				ne->copy(*nextelement);
				zuschnittliste_Gewinde_gesamt.AddTail(ne);
			}
		}
	}
}


