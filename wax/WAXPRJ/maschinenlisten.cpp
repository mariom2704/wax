#include "stdafx.h"


#include "../rohrkonst/elementlist.h"
#include "maschinenlisten.h"

using namespace WaxDruck;


Maschinenlisten::Maschinenlisten(WaxPart* w) : rohr(NULL)
{
	stutzenAnlage2440 = -1;
	if (!w)
		return;
			
	GewinderohrData* WaxPartGewindeRohr = NULL;
	if(dynamic_cast<const SammelAuftrag*>(w))
	{
		POSITION pos = w->get_subobjects().GetHeadPosition();
		if(pos != 0)
		{
			WaxPart* next = dynamic_cast<WaxPart*>(w->get_subobjects().GetNext(pos));
			XASSERT(next);
			WaxPartGewindeRohr = dynamic_cast< GewinderohrData*>(next);
		}
	}
	else
	{
		 WaxPartGewindeRohr = dynamic_cast< GewinderohrData*>(w);
	}

	if(WaxPartGewindeRohr)
	{
		rohr = dynamic_cast<const Rohrleitung*>(WaxPartGewindeRohr->get_werkauftrag());
		zuschnittliste_Gewinde_gesamt.ResetAnzahlRohrstuecke();
		zuschnittliste_Gewinde_gesamt.ResetAnzahlFittinge();
		zuschnittliste_Gewinde_gesamt.ResetGesamtlaenge();
		for (POSITION pos = w->get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(w->get_subobjects().GetNext(pos));
			XASSERT(next);
			if (!next || next->get_deleted())
				continue;
			createSaegeMaschinenGewinderohrlaengen(next->get_werkauftrag());
		}
		POSITION pos = w->get_subobjects().GetHeadPosition();
		if( pos == NULL)
		{
			createSaegeMaschinenGewinderohrlaengen(WaxPartGewindeRohr->get_werkauftrag());
		}
		
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
	else
	{

		createMaschinenlisten(w->get_werkauftrag());
		rohr = dynamic_cast<const Rohrleitung*>(w->get_werkauftrag());
		for (POSITION pos = w->get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			WaxPart* next = dynamic_cast<WaxPart*>(w->get_subobjects().GetNext(pos));
			XASSERT(next);
			if (!next || next->get_deleted())
				continue;
			if (!rohr)
				rohr = dynamic_cast<const Rohrleitung*>(next->get_werkauftrag()); // QA PR 04.07.07: Ist das hier nötig? Bei der nächsten Überarbeitung prüfen.
			createMaschinenlisten(next->get_werkauftrag(), false);
		}
	}
	

}



Maschinenlisten::~Maschinenlisten()
{
	deleteMaschinenlisten();
}


//Erzeugt die Rohrnutenlisten und die Zuschnittslisten
void Maschinenlisten::createMaschinenlisten(Werkauftrag *werkauftrag, bool loeschen)
{
	/*
	Wird NULL übergeben, benutzt die Methode den ersten Eintrag in der walist
	*/
	
	if (loeschen)
		deleteMaschinenlisten();

	
//	if (!werkauftrag)
//		werkauftrag = dynamic_cast<Werkauftrag*>(walist.GetHead());
	XASSERT(werkauftrag);
	if (!werkauftrag)
		return;
	
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	ApaxWerkauftrag* apax = dynamic_cast<ApaxWerkauftrag*>(werkauftrag);

	if (!rohr && !apax)
		return;
	
	if (rohr)
	{
		createMaschinenRohrleitung(werkauftrag);
	}
	createMaschinenApax(werkauftrag);
}

//Löscht die Zuschnittslisten und die Rohrnutenlisten
void Maschinenlisten::deleteMaschinenlisten()
{
	CObList *lst[] = 
	{ 
		&zuschnittliste_38030_handsw, 
		&zuschnittliste_38030_anlage,
		&zuschnittliste_32060, 
		&zuschnittliste_32020,
		&zuschnittliste_32070, 
		&zuschnittliste_EK,
		&zuschnittliste_Gewinde_ab10Stueck_32070,
		&zuschnittliste_Gewinde_bis10Stueck_31010,
		&zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030,
		NULL 
	};

	for (int i = 0; lst[i]; i++)
	{
		for (POSITION p = lst[i]->GetHeadPosition(); p != NULL;)
		{
/*
#ifdef _DEBUG
			AfxMessageBox("Hello Maschinenlisten");
#endif
*/
			delete lst[i]->GetNext(p);;
		}
		lst[i]->RemoveAll();
	}
}




//Erfast die Daten fuer die Gewinderohrliste
void Maschinenlisten::createSaegeMaschinenGewinderohrlaengen(Werkauftrag* werkauftrag)
{
	
	Gewinderohr* grohr = dynamic_cast<Gewinderohr*>(werkauftrag);
	XASSERT(grohr);
	if (!grohr)
		return;


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
/**
Zuschnittlisten mit den Elementen füllen
*//*
Änderungen:
23.06.06, Jacobs: Rohrnutenlisten gelöscht. Supportnr. 13497
06.06.06, Jacobs: Zuschnittliste 38030 geteilt
*/
void Maschinenlisten::createMaschinenRohrleitung(Werkauftrag *werkauftrag)
{

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(werkauftrag);
	COIIVerteiler* pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(rohr);

	if(pCOIIVerteiler)
	{
		createMaschinenRohrleitungCOIIVerteiler(rohr);
		return;
	}


#define ADDELE(zliste) zliste.AddTail(new Element(*element));\
Element *ele = dynamic_cast<Element*>(elelst.get_prev(count));\
zliste.AddTail(new Element(*ele));\
Element * ele1 = dynamic_cast<Element*>(zliste.GetTail());\
if (ele1->get_id() != NUT && ele1->get_id() != GEW && ele1->get_id() != GLATT) ele1->set_id(Element::LEER);\
Element *ele2 = dynamic_cast<Element*>(elelst.get_next(count));\
zliste.AddTail(new Element(*ele2));\
Element * ele3 = dynamic_cast<Element*>(zliste.GetTail());\
	if (ele3->get_id() != NUT && ele3->get_id() != GEW && ele3->get_id() != GLATT) ele3->set_id(Element::LEER);

#define ADDUELE(zliste)\
Element *ele = new Element(*uelem);\
ele->set_laenge(eleSLaenge);\
zliste.AddTail(new Element(*ele));\
Element *ele1 = new Element;\
	ele1->set_id(Element::LEER);\
zliste.AddTail(new Element(*ele1));\
Element *ele2 = new Element;\
if (nut == 1) ele2->set_id(NUT);\
else if (gewinde == 1) ele2->set_id(GEW);\
else ele2->set_id(Element::LEER);\
zliste.AddTail(new Element(*ele2));\
delete ele;\
delete ele1;\
delete ele2;

	int nut = 0, gewinde = 0;
	int flanschLaenge = 0, eleSLaenge = 0;
	// Elementdaten
	int eleDn     = 0;
	int eleLaenge = 0;
	bool isAnlage = false;
	bool zusatzele = false;

	// Elementliste abarbeiten und Rohrstuecke mit Anfa/Ende 
	// in die entsprechende Anlagenliste packen 
	ElementList elelst(rohr->get_elementList());
	// Abarbeiten der Elementliste
	bool rohrAdded32020 = false;
	bool rohrAdded38030 = false;//zuschnittliste_38030
	bool rohrAdded32060 = false;
	bool rohrAdded32070 = false;
	

	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *element = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(element);
		if (!element)
			break;

		// Alles hinter diesen Elementen muss auch bei Strang bearbeitet werden
		if (element->isEleRedBogEinz())
			zusatzele = true;

		// Pos wird hier missbraucht, um die Anzahl der Rohrleitungen zu merken 
		element-> set_WaNr(rohr->get_status().get_wanr());
		element->set_rohrPosNr(rohr->get_posNr());
		element->set_pos(rohr->get_anzahl());
	
		if (element->isEleRohr() && element->get_laenge() > 0) 
		{
			// vorheriges und naechstes Element fuer diverse Abfragen
			Element* prev = elelst.get_prev(count);
			Element* next = elelst.get_next(count);
			XASSERT(prev && next);
			if (!prev || !next)
				break;

			// Unterelementliste des Rohrs abarbeiten Rohrstutzen mit/ohne Nut 
			// in die entsprechende Anlagenliste packen 
			CObList &uelemList = element->get_uelementList();
			for (POSITION pos = uelemList.GetHeadPosition(); pos != NULL;)	
			{
				Element *uelem = dynamic_cast<Element*>(uelemList.GetNext(pos));
				XASSERT(uelem);
				if (!uelem)
					break;
							
				uelem->set_rohrPosNr(rohr->get_posNr());
				uelem->set_pos(rohr->get_anzahl());
				uelem->set_WaNr(rohr->get_status().get_wanr());

				if (uelem->get_id() == ROHR)
				{
					eleDn     = uelem->get_dn();
					eleLaenge = uelem->get_laenge();
					isAnlage  = uelem->get_makeAnlage();



					nut = 0;
					gewinde = 0;
					// Anbauteil oder Nut des Stutzen bestimmen
					CObList &uuelemList = uelem->get_uelementList();
					for (POSITION pos = uuelemList.GetHeadPosition(); pos != NULL;)	
					{
						Element *uuelem = dynamic_cast<Element*>(uuelemList.GetNext(pos));
						XASSERT(uuelem);
						if (!uuelem)
							break;
						
						uuelem->set_WaNr(rohr->get_status().get_wanr());
						flanschLaenge = uuelem->get_laenge();

						if (uuelem->get_id() == NUT)
							nut = 1;			
						else if (uuelem->get_id() == GEW)
							gewinde = 1;
					}


					eleSLaenge = rohr->berechnungRohrstutzen(uelem->get_dn(), uelem->get_anschlussDN(), uelem->get_laenge(), flanschLaenge,uelem->get_makeAnlage());

					// Zuschnittslisten
					if ((eleDn > 20 && eleDn < 65 && eleSLaenge < 501) && !isAnlage) 
					{ 
						ADDUELE(zuschnittliste_38030_handsw);
						if(rohrAdded38030 == false)
						{
							rohrAdded38030 = true;
							Werkauftraege38030.AddTail(rohr);
						}
					} 
					else if ((eleDn > 20 && eleDn <= 125 && eleSLaenge < 501) && isAnlage) 
					{ 
						//Rohrqualli umstellen
						if (eleDn >= 32 && eleDn <= 50)
						{
							if (stutzenAnlage2440 < 0)
								stutzenAnlage2440 = 1;
							else if (stutzenAnlage2440 == 0)
								stutzenAnlage2440 = 2;			//Qulität variabel!
						}
						else
						{
							if (stutzenAnlage2440 < 0)
								stutzenAnlage2440 = 0;
							else if (stutzenAnlage2440 == 1)
								stutzenAnlage2440 = 2;			//Qulität variabel!
						}

						ADDUELE(zuschnittliste_38030_anlage);
					}
					else if (eleDn > 20 && eleDn < 65 && eleSLaenge > 500) 
					{ 						
						ADDUELE(zuschnittliste_32070);   
						if( rohrAdded32070 == false)
						{
							rohrAdded32070 = true;
							Werkauftraege32070.AddTail(rohr);
						}
					}
					else if ((eleDn >  50 && eleDn < 350 && eleSLaenge < 1500) 
					      || (eleDn > 200 && eleDn < 350 && eleSLaenge > 1499)) 
					{ 
						ADDUELE(zuschnittliste_32060);
						if(rohrAdded32060 == false)
						{
							rohrAdded32060 = true;
							Werkauftraege32060.AddTail(rohr);
						}
					} 
					else if (eleDn > 50 && eleDn < 250 && eleSLaenge > 1499) 
					{ 
						ADDUELE(zuschnittliste_32020);
						if(rohrAdded32020 == false)
						{
							rohrAdded32020 = true;
							Werkauftraege32020.AddTail(rohr);
						}
					}else if(eleDn >  300 && eleDn <= 400) 
					{ 
						ADDELE(zuschnittliste_EK);
					} 
				}
				else if (uelem->get_id() == STDSTUTZEN)
				{
					if (uelem->get_dn() >= 80 && uelem->get_dn() <= 125)
					{
						// Zuschnitt
						DNzuAussendurchmesser dntoad;
						int aussendurchmesser = dntoad.getAussendurchmesserToInt(uelem->get_anschlussDN());
						switch (uelem->get_dn())
						{
						case 80:
							if (uelem->get_laenge() - aussendurchmesser/2 <= 100)
								eleSLaenge = 100;
							else
								eleSLaenge = 250;
							break;
						case 100:
							if (uelem->get_laenge() - aussendurchmesser/2 <= 100)
								eleSLaenge = 115;
							else
								eleSLaenge = 250;
							break;
						case 125:
							if (uelem->get_laenge() - aussendurchmesser/2 <= 100)
								eleSLaenge = 125;
							else
								eleSLaenge = 250;
							break;
						}
						ADDUELE(zuschnittliste_38030_anlage);
					}
				}
			}
			if (!werkauftrag->isStrangrohr() || zusatzele)
			{
				eleDn     = element->get_dn();
				eleLaenge = element->get_laenge();

				// Zuschnittslisten
				if (eleDn > 20 && eleDn < 65 && eleLaenge < 501) 
				{ 
					ADDELE(zuschnittliste_38030_handsw);
					if(rohrAdded38030 == false)
					{
						rohrAdded38030 = true;
						Werkauftraege38030.AddTail(rohr);
					}
				} 
				else if (eleDn > 20 && eleDn < 65 && eleLaenge > 500) 
				{ 
					ADDELE(zuschnittliste_32070); //30.03.06 neue Zuschnittsliste
					if( rohrAdded32070 == false)
					{
						rohrAdded32070 = true;
						Werkauftraege32070.AddTail(rohr);
					}
				}
				else if ((eleDn >  50 && eleDn < 350 && eleLaenge < 1500) 
					  || (eleDn > 200 && eleDn < 350 && eleLaenge > 1499)) 
				{ 
					ADDELE(zuschnittliste_32060);
						if(rohrAdded32060 == false)
						{
							rohrAdded32060 = true;
							Werkauftraege32060.AddTail(rohr);
						}
					
				} 
				else if ((eleDn > 50) && (eleDn < 250) && (eleLaenge > 1499)) 
				{ 
					ADDELE(zuschnittliste_32020);
					if(rohrAdded32020 == false)
					{
						rohrAdded32020 = TRUE;
						Werkauftraege32020.AddTail(rohr);
					}
				}else if(eleDn >  300 && eleDn <= 400) 
				{ 
					ADDELE(zuschnittliste_EK);
				} 
			}
		}
	} 
}



void Maschinenlisten::createMaschinenRohrleitungCOIIVerteiler(Rohrleitung* rohr)
{


#define ADDUELECO(zliste)\
Element *ele = new Element(*uelem);\
ele->set_laenge(eleSLaenge);\
zliste.AddTail(new Element(*ele));\
Element *ele1 = new Element;\
if (fase1 == 1)\
{\
	ele1->set_bezeichnung("Glatt/Fase");\
	ele1->set_id(FASE_COII);\
}\
else ele1->set_id(Element::LEER);\
zliste.AddTail(new Element(*ele1));\
Element *ele2 = new Element;\
if (nut == 1) ele2->set_id(NUT);\
else if (gewinde == 1) ele2->set_id(GEW);\
else if (fase2 == 1)\
{\
	ele2->set_bezeichnung("Glatt/Fase");\
	ele2->set_id(FASE_COII);\
}\
else ele2->set_id(Element::LEER);\
zliste.AddTail(new Element(*ele2));\
delete ele;\
delete ele1;\
delete ele2;\

	int nut = 0, gewinde = 0, fase1 = 0, fase2 = 0;
	int flanschLaenge = 0, eleSLaenge = 0;
	// Elementdaten
	int eleDn     = 0;
	int eleLaenge = 0;
	bool isAnlage = false;

	
	// Elementliste abarbeiten und Rohrstuecke mit Anfa/Ende 
	// in die entsprechende Anlagenliste packen 
	ElementList elelst(rohr->get_elementList());
	// Abarbeiten der Elementliste

	for (int count=0; count<elelst.get_count(); count++)	
	{
		// Element aus der Liste 
		Element *element = dynamic_cast<Element*>(elelst.get_at(count));
		XASSERT(element);
		if (!element)
			break;


		// Pos wird hier missbraucht, um die Anzahl der Rohrleitungen zu merken 
		element->set_rohrPosNr(rohr->get_posNr());
		element->set_pos(rohr->get_anzahl());
	
		if (element->isEleRohr() && element->get_laenge() > 0) 
		{
			// vorheriges und naechstes Element fuer diverse Abfragen
			Element* prev = elelst.get_prev(count);
			Element* next = elelst.get_next(count);
			
		
			if( prev->get_id() == GEW  || prev->get_id() == GEWINDE_KAPPE_COII || prev->get_id() == GEWINDE_KAPPE_TYP_D )
			{
				//prev = new Element(GEW);
				prev->set_id(GEW);
				prev->set_bezeichnung("Gewinde");
			}
			if( next->get_id() == GEW || next->get_id() == GEWINDE_KAPPE_COII || next->get_id() == GEWINDE_KAPPE_TYP_D )
			{
				//next = new Element(GEW);
				next->set_id(GEW);
				next->set_bezeichnung("Gewinde");
			}

			if(prev->get_id() == BLINDFLANSCH_COII || prev->get_id() == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR || prev->get_id() == FLANSCH_COII || prev->get_id() == BODEN_COII)
			{
				prev = new Element(FASE_COII);
				prev->set_id(FASE_COII);
				prev->set_bezeichnung("Glatt/Fase");
			}

			if(next->get_id() == BLINDFLANSCH_COII || next->get_id() == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR || next->get_id() == FLANSCH_COII || next->get_id() == BODEN_COII)
			{
				next = new Element(FASE_COII);
				next->set_id(FASE_COII);
				next->set_bezeichnung("Glatt/Fase");
			}
			
			
			XASSERT(prev && next);
			if (!prev || !next)
				break;

			// Unterelementliste des Rohrs abarbeiten Rohrstutzen mit/ohne Nut 
			// in die entsprechende Anlagenliste packen 
			CObList &uelemList = element->get_uelementList();
			for (POSITION pos = uelemList.GetHeadPosition(); pos != NULL;)	
			{
				Element *uelem = dynamic_cast<Element*>(uelemList.GetNext(pos));
				XASSERT(uelem);
				if (!uelem)
					break;
							
				uelem->set_rohrPosNr(rohr->get_posNr());
				uelem->set_pos(rohr->get_anzahl());
			
				if (uelem->get_id() == ROHR)
				{
					eleDn     = uelem->get_dn();
					eleLaenge = uelem->get_laenge();
					isAnlage  = uelem->get_makeAnlage();



					nut = 0;
					gewinde = 0;
					fase1 = 0;
					fase2 = 0;
					// Anbauteil oder Nut des Stutzen bestimmen
					COIIVerteiler* pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(rohr);
					CObList &uuelemList = uelem->get_uelementList();
					bool bGegenFlansch = false;
					for (POSITION pos = uuelemList.GetHeadPosition(); pos != NULL;)	
					{
						Element *uuelem = dynamic_cast<Element*>(uuelemList.GetNext(pos));
						XASSERT(uuelem);
						if (!uuelem)
							break;						
						
						// Elemente werden nicht deletet, da Logik zu komplziert, erzeugt am Ende einen stack dump
						if(pos > 0) // Unterelement ist Gegenflansch
						{	

							Element *uuelemTemp = dynamic_cast<Element*>(uuelemList.GetNext(pos));
							if(GEGENFLANSCH_GEWINDE_COII == uuelemTemp->get_id() || GEGENFLANSCH_GEWINDE_MUFFE_COII == uuelemTemp->get_id())
							{
								fase2 = 1;
								flanschLaenge = uuelem->get_laenge();
								uelem = new Element(*uelem);
								pCOIIVerteiler->setQualiRohrstutzen(uelem);
								eleSLaenge = rohr->berechnungRohrstutzen(uelem->get_dn(), uelem->get_anschlussDN(), uelem->get_laenge(), flanschLaenge,uelem->get_makeAnlage());
								ADDUELECO(zuschnittliste_32070);
								fase2 = 0;
								fase1 = 1;
								uelem = new Element(*uelem);
								uelem->set_laenge(uuelemTemp->get_laenge());
								uuelem = new Element(*uuelem);
								uuelem->set_id(GEW);
								bGegenFlansch = true;
							}
						}
						
						
						flanschLaenge = uuelem->get_laenge();

						if (uuelem->get_id() == NUT)
							nut = 1;			
						else if (uuelem->get_id() == GEW)
							gewinde = 1;
						else if (uuelem->get_id() == FLANSCH_COII)
							fase2 = 1;

					}
					
					uelem = new Element(*uelem);
					pCOIIVerteiler->setQualiRohrstutzen(uelem);
					
					//Wenn es ein Gegenflansch ist berechnungRohrstutzen unterschiedliche DN und anschlussDN nötig,
					// damit kein Zuschlag gerechnet wird => Dummywerte werden übergeben
					int anschlussDN = 78;
					int iDn = 66;
					if(!bGegenFlansch)
					{
						iDn = uelem->get_dn();
						anschlussDN = uelem->get_anschlussDN();
					}

					eleSLaenge = rohr->berechnungRohrstutzen(iDn, anschlussDN, uelem->get_laenge(), flanschLaenge,uelem->get_makeAnlage());

					ADDUELECO(zuschnittliste_32070);
					
				}
			}
		
			eleDn     = element->get_dn();
			eleLaenge = element->get_laenge();

			zuschnittliste_32070.AddTail(new Element(*element));
		//	Element *ele = dynamic_cast<Element*>(elelst.get_prev(count));
			zuschnittliste_32070.AddTail(new Element(*prev));
			Element * ele1 = dynamic_cast<Element*>(zuschnittliste_32070.GetTail());
			if (ele1->get_id() != NUT && ele1->get_id() != GEW && ele1->get_id() != GLATT && ele1->get_id() != FASE_COII) 
				ele1->set_id(Element::LEER);
			
			//Element *ele2 = dynamic_cast<Element*>(next);
			zuschnittliste_32070.AddTail(new Element(*next));
			Element * ele3 = dynamic_cast<Element*>(zuschnittliste_32070.GetTail());
			if (ele3->get_id() != NUT && ele3->get_id() != GEW && ele3->get_id() != GLATT && ele3->get_id() != FASE_COII)
			{	
				ele3->set_id(Element::LEER);
			}
			//ADDELECO(zuschnittliste_32060);
		}
	} 

	COIIVerteiler* pCOIIVerteiler = dynamic_cast<COIIVerteiler*>(rohr);
	MxCObList&  materials = pCOIIVerteiler->get_material().get_matlist();

	for (POSITION pos = materials.GetHeadPosition(); pos != NULL;)	
	{
		const Material *material = dynamic_cast<const Material*>(materials.GetNext(pos));
		XASSERT(material);
		if (material)
		{
			if( material->get_id() == PROFIL)
			{
				fase1	= 0;
				fase2	= 0;
				gewinde = 0;
				Element* uelem = new Element();
				uelem->set_pos(1);
				uelem->set_anzahl(material->get_anzahl());
				uelem->set_artikelnr(material->get_artikelnr());
				uelem->set_bezeichnung(material->getBezeichnung());
				uelem->set_laengeZuschnitt(material->get_mass1());
				uelem->set_rohrPosNr(rohr->get_posNr());
				ADDUELECO(zuschnittliste_32070);
				delete uelem;
			}
		}
	}
}




void Maschinenlisten::createMaschinenApax(Werkauftrag *werkauftrag)
{
// Nur Zuschnittslisten
//Material* material = new Material;\
//material->copy(*mat);\
//zliste.AddTail(material);

#define ADDMAT(zliste)\
zliste.AddTail(new Element(ele));\
Element *ele1 = new Element;\
ele1->set_id(Element::LEER);\
zliste.AddTail(ele1);\
Element *ele2 = new Element;\
ele2->set_id(Element::LEER);\
zliste.AddTail(ele2);

	int eleSLaenge = 0; 
	int eleDn     = 0;
	int eleLaenge = 0;

	Rohrleitung rohr;

	// Materialiste abarbeiten und Rohrstuecke mit Anfa/Ende 
	// in die entsprechende Anlagenliste packen 
	const CObList &matlist = werkauftrag->get_material().get_matlist();
	for (POSITION pos = matlist.GetHeadPosition(); pos != NULL;)	
	{
		const Material *mat = dynamic_cast<const Material*>(matlist.GetNext(pos));
		XASSERT(mat);
		if (!mat)
			break;

		// ID nicht gesetzt bei neuem Material
		if (mat->get_id() == 0)
			continue;

		// falsche Kombination gesetzt
		bool ok = false;
		const CObList& grp = getStammdaten().get_bauelemente();
		for (POSITION gpos = grp.GetHeadPosition(); gpos;)
		{
			Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
			if (g->get_artikelnr() == mat->get_artikelnr() && g->get_elemid() == mat->get_id()) 
			{
				ok = true;
				break;
			}
		}
		if (!ok)
			continue;

		Element ele;
		// Todo: initStammdaten auf Standardmaterial anpassen
		ele.initStammdaten(mat->get_artikelnr(), mat->get_id());
		ele.set_anzahl(mat->get_anzahl());
		ele.set_laenge(mat->get_mass1());
		eleDn     = ele.get_dn();
		eleLaenge = ele.get_laenge();

		// Pos wird hier missbraucht, um die Anzahl der Rohrleitungen zu merken,
		// die bei der Ausgabe mit der Anzahl des Element multipliziert wird
		ele.set_rohrPosNr(werkauftrag->get_posNr());
		ele.set_pos(1);

		if ((ele.isEleRohr() || ele.get_id() == ROHR || ele.get_id() == STDSTUTZEN) && eleLaenge > 0) 
		{
			if (ele.get_id() == ROHR)
			{
				eleSLaenge = rohr.berechnungRohrstutzen(eleDn, ele.get_anschlussDN(), eleLaenge, 0,ele.get_makeAnlage());

				if (eleDn > 20 && eleDn < 65 && eleSLaenge < 501) 
				{ 
					ADDMAT(zuschnittliste_38030_handsw);
				} 
				else if (eleDn > 20 && eleDn < 65 && eleSLaenge > 500) 
				{ 
					ADDMAT(zuschnittliste_32070); 
				}
				else if ((eleDn >  50 && eleDn < 350 && eleSLaenge < 1500) 
					  || (eleDn > 200 && eleDn < 350 && eleSLaenge > 1499)) 
				{ 
					ADDMAT(zuschnittliste_32060);
				} 
				else if (eleDn > 50 && eleDn < 250 && eleSLaenge > 1499) 
				{ 
					ADDMAT(zuschnittliste_32020);
				}else if(eleDn >  300 && eleDn <= 400) 
				{ 
					ADDMAT(zuschnittliste_EK);
				} 
			}
			else if (ele.get_id() == STDSTUTZEN)
			{
				if (eleDn >= 80 && eleDn <= 125)
				{
					DNzuAussendurchmesser dntoad;
					int aussendurchmesser = dntoad.getAussendurchmesserToInt(ele.get_anschlussDN());
					switch (eleDn)
					{
					case 80:
						if (eleLaenge - aussendurchmesser/2 <= 100)
							eleSLaenge = 100;
						else
							eleSLaenge = 250;
						break;
					case 100:
						if (eleLaenge - aussendurchmesser/2 <= 100)
							eleSLaenge = 115;
						else
							eleSLaenge = 250;
						break;
					case 125:
						if (eleLaenge - aussendurchmesser/2 <= 100)
							eleSLaenge = 125;
						else
							eleSLaenge = 250;
						break;
					}
					ADDMAT(zuschnittliste_38030_anlage);
				}
			}
			else
			{
				if (eleDn > 20 && eleDn < 65 && eleLaenge < 501) 
				{ 
					ADDMAT(zuschnittliste_38030_handsw);
				} 
				else if (eleDn > 20 && eleDn < 65 && eleLaenge > 500) 
				{ 
					ADDMAT(zuschnittliste_32070); //30.03.06 neue Zuschnittsliste
				}
				else if ((eleDn >  50 && eleDn < 350 && eleLaenge < 1500) 
					  || (eleDn > 200 && eleDn < 350 && eleLaenge > 1499)) 
				{ 
					ADDMAT(zuschnittliste_32060);
				} 
				else if ((eleDn > 50) && (eleDn < 250) && (eleLaenge > 1499)) 
				{ 
					ADDMAT(zuschnittliste_32020);
				}else if(eleDn >  300 && eleDn <= 400) 
				{ 
					ADDMAT(zuschnittliste_EK);
				} 
			}
		}
	} 
}

