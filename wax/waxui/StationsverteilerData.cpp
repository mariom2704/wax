#include "stdafx.h"

#include "resource.h"

#include "StationsVerteilerBemassung.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(StationsverteilerData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(StationsverteilerData, WaxPart, VERSIONABLE_SCHEMA|1)

//Konstruktor
StationsverteilerData::StationsverteilerData()
{
	set_werkauftrag(new Stationsverteiler);
	set_icon(IDI_STATIONSVERTEILER);
}

//Destruktor
StationsverteilerData::~StationsverteilerData()
{
}

//Liefert den Stationsverteiler
Stationsverteiler*  StationsverteilerData::get_stationsverteiler() 
{
	Stationsverteiler *vert = dynamic_cast<Stationsverteiler*>(get_werkauftrag());
	XASSERT(vert);
	if (!vert)
		AfxThrowNotSupportedException();
	return vert;
}

//Speichern der Daten
void StationsverteilerData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Klassen zur Bearbeitung liefern
void StationsverteilerData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
//	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(StationsverteilerEingabe));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(StationsverteilerBemassung));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(AVElementEingabe));


#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}



bool StationsverteilerData::synchronize(WaxPart *with)
{
	if (with == this)
		return false;
	
	Stationsverteiler *dest = dynamic_cast<Stationsverteiler*>(get_werkauftrag());
	Stationsverteiler *from = dynamic_cast<Stationsverteiler*>(with->get_werkauftrag());
	XASSERT(dest && from);
	if (!dest || !from)
		return false;
// PR 040908: Versuchsweise deaktiviert, da in Strangrohr schon ewig auskommentiert. 
//	dest->set_avnachbearbeitung(from->get_avnachbearbeitung()); // Macht das Sinn? Keine Ahnung...
	return true;
}


