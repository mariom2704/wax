#include "stdafx.h"


#include "encoblistmodell.h"
#include "Element.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace MxErrorHandling;

EnCObListModell::EnCObListModell(void)
{
}

EnCObListModell::~EnCObListModell(void)
{

}


/**
Übergebene Liste kopieren. Von den Elementen wird nur die Referenz kopiert

\param deleteThisElemente    = true: die Elemente der this-Liste werden vor dem kopieren mit delete gelöscht

\param deepCopyOtherElemente = true: von den Elementen der übergebenen Liste wird eine Kopie mit deepCopy erzeugt 
								     deepCopy muß in der Elementklasse implementiert sein!!!
							 = false: von den Elementen der übergebenen Liste wird nur die Referenz kopiert
*/
bool EnCObListModell::copy(const EnCObListModell &other/*,bool deleteThisElemente*/)
{
	//Attribute kopieren
	//...

	//Listeelemente löschen
//	if (deleteThisElemente)
		deleteAllElemente();
//	else
//		resetListe();
	
	WaxBase *pBase = NULL;

	//Alle Listenelemente kopiern
	MxCObListIterator i(other);
	while (i.next())
	{
		pBase = dynamic_cast<Element*>(getAt(i));
		XASSERT(pBase);

		if (!pBase)
			continue;

	//	if (deepCopyOtherElemente)
			pBase = pBase->deepCopy();
		
		
		//Neues Objekt in die Liste schreiben
		append(pBase);
	}

	return true;
}

/**
Elemente vom Typ elTyp im Bauteil zählen
*/
int EnCObListModell::getAnzahlElemente(int elemntId)
{
	int count = 0;
	MxCObListIterator iter(*this);
	while (iter.next())
	{
		Element *element = dynamic_cast<Element*>(getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		if (element->get_id() == elemntId) 
			count++;
	}
	return count;
}



///Rohre im Bauteil zählen
int EnCObListModell::getAnzahlRohre()
{
	int count = 0;
	MxCObListIterator iter(*this);
	while (iter.next())
	{
		Element *element = dynamic_cast<Element*>(getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleRohr()) 
			count++;
	}
	return count;
}

bool EnCObListModell::hasAnlagenTeile()
{
	MxCObListIterator iter(*this);
	while (iter.next())
	{
		Element *element = dynamic_cast<Element*>(getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		//Sobald ein Anlagenteil gefunden wird Methode verlassen
		if (element->get_makeAnlage()) 
			return true;

		//Unterelemente prüfen
		EnCObListModell &uListe = element->get_uelementList();
		if (uListe.hasAnlagenTeile())
			return true;
	}
	
	return false;
}


///Abgänge im Bauteil zählen
int EnCObListModell::getAnzahlAbgaenge()				
{
	return countAbgaenge(false);
}
int EnCObListModell::getAnzahlAnlagenteile()
{
	return countAbgaenge(true);
}

int EnCObListModell::countAbgaenge(bool hasAnlage)
{
	int count = 0;
	MxCObListIterator iter(*this);
	while (iter.next())
	{
		Element *element = dynamic_cast<Element*>(getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		if (element->isEleAbgang()) 
		{
			if (hasAnlage)
			{
				if (element->get_makeAnlage())
					count++;
			}
			else		
				count++;
		}
	}
	return count;
}


/**
Dn des keinsten Elementes im Hauptstrang ermitteln
\param bNurRohre = true: Dn des kleinsten Rohres
*/
int EnCObListModell::getMinDn(bool bNurRohre)
{
	int dn = 0;

	MxCObListIterator iter(*this);
	while (iter.next())
	{
		Element *element = dynamic_cast<Element*>(getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		//Wenn bNurRohre=true, Min Dn von den Rohren ermitteln, sonst von allen Bauteilen
		if ((element->isEleRohr() && bNurRohre) || !bNurRohre) 
		{
			if (dn == 0)
				dn = element->get_dn();
			else
			{
				if (element->get_dn() < dn)
					dn = element->get_dn();
			}
		}
	}
	return dn;
}

/**
Dn des größten Elementes im Hauptstrang ermitteln
\param bNurRohre = true: Dn des größten Rohres
*/
int EnCObListModell::getMaxDn(bool bNurRohre)							
{
	int dn = 0;
		
	MxCObListIterator iter(*this);
	while (iter.next())
	{
		Element *element = dynamic_cast<Element*>(getAt(iter));
		XASSERT(element);
		if (!element)
			break;

		//Wenn bNurRohre=true, Max Dn von den Rohren ermitteln, sonst von allen Bauteilen
		if ((element->isEleRohr() && bNurRohre) || !bNurRohre) 
		{
			if (element->get_dn() > dn)
				dn = element->get_dn();
		}
	}
	return dn;
}


///neues Element an die Liste hängen
bool EnCObListModell::newElement(int dn, int andn, int id, int abs, int rel, int laenge, int winkel, int rlaenge, bool initStamm)
{
	Element ele;

	try 
	{ 
		ele.set_id(id);

		ele.set_dn(dn);
		ele.set_anschlussDN(andn);

		if (initStamm)
			ele.initStammdaten(dn, andn, ele.get_id());
		ele.initDaten(abs, rel, laenge, ele.get_id(), winkel);

		ele.set_rohrLaenge(rlaenge);
	}
	catch(Element::Error e ) 
	{
		errorhandler.appendError(MxError(e.get_errorText(), MxError::Information));
		return false;
	}

	//in Liste schreiben
	append(new Element(ele)); // Hier erst neues Heap-Element erzeugen, kein Speicherleck, wenn vorher Fehler auftrat

	return true;
}




#ifdef _DEBUG
/*
Ausgabe der Elemente in der Liste
*/
void EnCObListModell::Dump(CDumpContext &dc) const
{
	//Werte von elementList ausgeben
	dc << "\n\nRohrModell: ";

	MxCObList::Dump(dc);
}

#endif