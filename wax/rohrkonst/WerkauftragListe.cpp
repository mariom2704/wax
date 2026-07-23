#include "stdafx.h"

#include ".\werkauftragliste.h"
#include "EnStueckliste.h"
#include "Werkauftrag.h"
#include "Rohrleitung.h"
#include "Strangrohr.h"
#include "Verteiler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

#define APPEND_INT(_functionName,_getWert,_className)\
bool WerkauftragListe::_functionName(CArray<int,int> &ar)\
{\
	ar.RemoveAll();\
	MxIterator iter(*this);\
	while (iter.next())\
	{\
		_className *pWa = dynamic_cast<_className*>(getAt(iter));\
		XASSERT(pWa);\
		if (!pWa)\
			continue;\
		appendInt(_getWert,ar);\
	}\
	return true;\
}

#define APPEND_ULONG(_functionName,_getWert,_className)\
bool WerkauftragListe::_functionName(CArray<ULONG,ULONG> &ar)\
{\
	ar.RemoveAll();\
	MxIterator iter(*this);\
	while (iter.next())\
	{\
		_className *pWa = dynamic_cast<_className*>(getAt(iter));\
		XASSERT(pWa);\
		if (!pWa)\
			continue;\
		appendULong(_getWert,ar);\
	}\
	return true;\
}

#define APPEND_STRING(_functionName,_getWert,_className)\
bool WerkauftragListe::_functionName(CStringArray &ar)\
{\
	ar.RemoveAll();\
	MxIterator iter(*this);\
	while (iter.next())\
	{\
		_className *pWa = dynamic_cast<_className*>(getAt(iter));\
		XASSERT(pWa);\
		if (!pWa)\
			continue;\
		appendString(_getWert,ar,false);\
	}\
	return true;\
}


#define APPEND_INT64U(_functionName,_getWert,_className)\
bool WerkauftragListe::_functionName(CArray<__int64, __int64> &ar)\
{\
	ar.RemoveAll();\
	MxIterator iter(*this);\
	while (iter.next())\
	{\
		_className *pWa = dynamic_cast<_className*>(getAt(iter));\
		XASSERT(pWa);\
		if (!pWa)\
			continue;\
		appendInt64u(_getWert,ar);\
	}\
	return true;\
}

WerkauftragListe::WerkauftragListe(void)
{
}

WerkauftragListe::~WerkauftragListe(void)
{
}

/**
vom aktuellen Element die Stückliste ausgeben
*/
EnStueckliste* WerkauftragListe::getStuecklisteFromElement(const MxIterator& iterator)
{
	
	Werkauftrag* pWa = getWerkauftragFromElement(iterator);
	if (!pWa)
		return NULL;

	//Modell und Listen erzeugen
	pWa->createModell();	


	//Stückliste von Werkauftrag zurückgeben
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pWa);
	if (rohr && rohr->isVerteiler() && rohr->checkAnlage())
		return &rohr->get_stuecklisteSap();
	else
		return &pWa->get_stueckliste();
}


Werkauftrag	*WerkauftragListe::getWerkauftragFromElement(const MxIterator& iterator)
{
	Werkauftrag* pWa = dynamic_cast<Werkauftrag*>(getAt(iterator));
	XASSERT(pWa);
	if (!pWa)
		return NULL;

	return pWa;
}


int WerkauftragListe::getWaidFromElement(const MxIterator& iterator)
{
	Werkauftrag* pWa = getWerkauftragFromElement(iterator);
	if (!pWa)
		return 0;

	return pWa->get_waid();
}

/**
str nur in Liste schreiben, wenn noch nicht vorhanden
*/
void WerkauftragListe::appendString(const CString &str,CStringArray &ar,bool bCaseSensitiv)
{

	for (int i=0; i<ar.GetSize(); i++)
	{
		if (bCaseSensitiv)
		{
			if (ar[i] == str)
				return;
		}
		else
		{
			if (!str.CompareNoCase(ar[i]))
				return;
		}
	}
	ar.Add(str);

	return;
}

/**
wert nur in Liste schreiben, wenn noch nicht vorhanden
*/
void WerkauftragListe::appendInt(int wert,CArray<int,int> &ar)
{

	for (int i=0; i<ar.GetSize(); i++)
	{
		if (ar[i] == wert)
			return;
	}

	ar.Add(wert);

	return;
}

/**
wert nur in Liste schreiben, wenn noch nicht vorhanden
*/
void WerkauftragListe::appendULong(ULONG wert,CArray<ULONG,ULONG> &ar)
{

	for (int i=0; i<ar.GetSize(); i++)
	{
		if (ar[i] == wert)
			return;
	}

	ar.Add(wert);

	return;
}


void WerkauftragListe::appendInt64u(__int64 wert,CArray<__int64 , __int64> &ar)
{

	for (int i=0; i<ar.GetSize(); i++)
	{
		if (ar[i] == wert)
			return;
	}

	ar.Add(wert);

	return;
}

/**
wert nur in Liste schreiben, wenn noch nicht vorhanden
*/
void WerkauftragListe::appendDouble(double wert,CArray<double,double> &ar)
{

	for (int i=0; i<ar.GetSize(); i++)
	{
		if (ar[i] == wert)
			return;
	}

	ar.Add(wert);

	return;
}



//Werte in eine Liste schreiben
//in die Liste wird jeder Wert nur einmal geschrieben

APPEND_INT(getAllWaNr,pWa->get_status().get_wanr(),Werkauftrag)

APPEND_STRING(getAllBauabschnitte,pWa->get_leitkarte().get_prj_bauAbschnitt(),Werkauftrag)

APPEND_STRING(getAllAuftragsBez,pWa->get_leitkarte().get_prj_auftragsbez(),Werkauftrag)

APPEND_STRING(getAllPlz,pWa->get_leitkarte().get_prj_bauPlz(),Werkauftrag)

APPEND_STRING(getAllOrte,pWa->get_leitkarte().get_prj_bauOrt(),Werkauftrag)

APPEND_INT(getAllNls,pWa->get_leitkarte().get_prj_nl(),Werkauftrag)

APPEND_STRING(getAllSachbearbeiterNl,pWa->get_leitkarte().get_sachbearbeiterNL(),Werkauftrag)

APPEND_STRING(getAllSachbearbeiterOd,pWa->get_status().get_sachbearbeiterSegment(),Werkauftrag)

APPEND_STRING(getAllErstelldatum,pWa->get_erstellungsdatum().Format("%d.%m.%y"),Werkauftrag)

APPEND_STRING(getAllBearbeitungsdatum,pWa->get_status().get_bearbeitungsdatum().Format("%d.%m.%y"),Werkauftrag)

APPEND_STRING(getAllWunschliefertermin,pWa->get_leitkarte().get_wunschliefertermin().Format("%d.%m.%y"),Werkauftrag)

APPEND_STRING(getAllFreigabedatum,pWa->get_status().get_freigabedatum().Format("%d.%m.%y"),Werkauftrag)

APPEND_STRING(getAllBemerkungen,pWa->get_bemerkung(),Werkauftrag)

APPEND_STRING(getAllPsp,pWa->get_leitkarte().get_prj_pspElement(),Werkauftrag)

APPEND_INT64U(getAllSdAuftragsNr,pWa->get_leitkarte().get_sdAuftragsnr(),Werkauftrag)

APPEND_INT(getAllSdMaterialNr,pWa->get_leitkarte().get_sdMaterialnummer(),Werkauftrag)

APPEND_INT(getAllSdPositionsNr,pWa->get_leitkarte().get_sdPositionsnr(),Werkauftrag)

APPEND_STRING(getAllOberflaechen,pWa->get_beschichtung().get_bezeichnung(),Werkauftrag)


//RohrWerte

//RohrDn
APPEND_INT(getAllRohrDn,pWa->get_dnvl(),Rohrleitung)
//RohrQuali
APPEND_INT(getAllRohrQuali,pWa->get_rohrQuali(),Rohrleitung)

//Strangrohrwerte

//MuffenDN
APPEND_INT(getAllMuffenDn,pWa->get_muffenDN(),Strangrohr)
//MuffenID
//APPEND_INT(getAllMuffenID,pWa->get_muffenID(),Strangrohr)
//Muffenlaenge
APPEND_INT(getAllMuffenLaenge,pWa->getMuffenLaenge(),Strangrohr)


/**
Muffenkehlung der Strangrohre in der Liste bestimmen.
\param wert = -1 : unterschiedlich
			  0  : alle nicht gekehlt
			  1  : alle gekehlt
*/
bool WerkauftragListe::getAllMuffenKehlung(int &wert)
{
	wert = -1;

	MxIterator iter(*this);
	int count = 0;
	while (iter.next())
	{
		Strangrohr *pWa = dynamic_cast<Strangrohr*>(getAt(iter));
		XASSERT(pWa);
		if (!pWa)
		{
			wert = -1;
			return false;
		}

		if (count == 0)
			wert = pWa->get_muffenGekehlt();
		else 
		{
			int temp;
			(pWa->get_muffenGekehlt() ? temp = 1 : temp = 0);

			if (temp != wert)
			{
				wert = -1;
				return true;
			}
		}

		count++;
	}
	return true;
}


/**
Anzahl Muffen für den übergebenen Auftrag ermitteln
*/
int WerkauftragListe::getAnzahlMuffenVonWaNr(int waNr)
{
	int anzahl = 0;

	MxIterator iter(*this);
	while (iter.next())
	{
		Strangrohr *pWa = dynamic_cast<Strangrohr*>(getAt(iter));
		if (!pWa)
			continue;

		if (waNr == pWa->get_status().get_wanr())
		{
			anzahl += pWa->getAnzahlMuffen() * pWa->get_anzahl();
		}
	}

	return anzahl;
}

bool WerkauftragListe::getAllDuchmesser(CArray<double,double> &ar)
{
	ar.RemoveAll();
	MxIterator iter(*this);
	while (iter.next())
	{
		Strangrohr *pWa = dynamic_cast<Strangrohr*>(getAt(iter));
		XASSERT(pWa);
		if (!pWa)
			continue;
		appendDouble(pWa->get_durchmesser(),ar);
	}
	return true;
}

bool WerkauftragListe::getAllMuffenID(CArray<int,int> &ar)
{
	ar.RemoveAll();
	MxIterator iter(*this);
	while (iter.next())
	{
		Strangrohr *pWa = dynamic_cast<Strangrohr*>(getAt(iter));
		XASSERT(pWa);
		if (!pWa)
			continue;
		int muffenId = pWa->get_muffenID();

		//Werkaufträge aus dem CAD umstellen auf Muffen ID Muffe, damit Sie kompatibel zur WAX Eingabe sind
		if (muffenId == FRMUFFE || muffenId == KRMUFFE || muffenId == NEW_ROBBI_MUFFE)
			muffenId = MUFFE;

		appendInt(muffenId,ar);
	}
	return true;
}