#include "stdafx.h"

#include "resource.h"

#include "CO II VerteilerEingabeFormBemassung.h"
#include "CO II VerteilerData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(COIIVerteilerData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(COIIVerteilerData, WaxPart, VERSIONABLE_SCHEMA|1)

//Konstruktor
COIIVerteilerData::COIIVerteilerData()
{
	set_werkauftrag(new COIIVerteiler());
	set_icon(IDI_STATIONSVERTEILER);
}

COIIVerteilerData::COIIVerteilerData(bool rohrCOIIDruckstufe)
{
	set_werkauftrag(new COIIVerteiler());
	set_icon(IDI_STATIONSVERTEILER);
}

//Destruktor
COIIVerteilerData::~COIIVerteilerData()
{
}

//Liefert den Stationsverteiler
COIIVerteiler*  COIIVerteilerData::get_CommonCOMXVerteiler() 
{
	COIIVerteiler *vert = dynamic_cast<COIIVerteiler*>(get_werkauftrag());
	XASSERT(vert);
	if (!vert)
		AfxThrowNotSupportedException();
	return vert;
}

//Speichern der Daten
void COIIVerteilerData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Klassen zur Bearbeitung liefern
void COIIVerteilerData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(COIIVerteilerBemassung));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}



bool COIIVerteilerData::synchronize(WaxPart *with)
{
	return true;
}


