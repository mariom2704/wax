#include "stdafx.h"

#include ".\enstueckliste.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void EnStueckliste::clear()
{
	for (POSITION pos = GetHeadPosition(); pos; )	
		delete dynamic_cast<Element*>(GetNext(pos));
	RemoveAll();
}

void EnStueckliste::copy(const EnStueckliste& other)
{
	POSITION pos;
	for (pos = other.GetHeadPosition(); pos != NULL;)	
	{
		const Element* const element = dynamic_cast<const Element* const>(other.GetNext(pos));
		if (!element)
			continue;
		AddTail(new Element(*element));
	}
}

void EnStueckliste::add(Element* e)
{
	AddTail(e);
}

void EnStueckliste::add_copy(const Element& e)
{
	AddTail(new Element(e));
}

int  EnStueckliste::getIdFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return 0;
	return pEl->get_id();
}


/**
- Bei Rohr: Länge in m zurückgeben
- Bei Bauteil mit Stücklisteneinheit st: Anzahl zurückgegeben
*/
double  EnStueckliste::getMengeLaengeFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return 0;

	double laengeAnzahl = 0;

	if (pEl->isEleRohr())
		laengeAnzahl =  (double)(pEl->get_laenge() / 1000.0);
	else
		laengeAnzahl = pEl->get_anzahl();

	return laengeAnzahl;
}

int  EnStueckliste::getDnFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return 0;

	return pEl->get_dn();
}

long EnStueckliste::getArtikelNrFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return 0;

	return pEl->get_artikelnr();
}


double EnStueckliste::getPreisFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return 0;

	return pEl->get_festPreis();
}

CString EnStueckliste::getBezeichnungFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return "";

	return pEl->get_kurztext();
}



int EnStueckliste::getDnRohrFromElement(const MxCObListIterator &iterator)
{
	Element *pEl = castElement(iterator);
	if (!pEl)
		return 0;

	return pEl->get_anschlussDN();
}


/**
dynamic cast auf aktuelles Listenelement
*/
Element* EnStueckliste::castElement(const MxCObListIterator &iterator)
{
	Element* el = dynamic_cast<Element*>(getAt(iterator));
	return el;
}


