#include "stdafx.h"

#include "resource.h"

#include "Mx200 1230 VerteilerEingabeFormBemassung.h"
#include "Mx200 1230 VerteilerData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMx2001230VerteilerData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(CMx2001230VerteilerData, WaxPart, VERSIONABLE_SCHEMA|1)

//Konstruktor
CMx2001230VerteilerData::CMx2001230VerteilerData()
{
	set_werkauftrag(new CMx2001230Verteiler());
	set_icon(IDI_STATIONSVERTEILER);
}


//Destruktor
CMx2001230VerteilerData::~CMx2001230VerteilerData()
{
}

//Liefert den Stationsverteiler
CMx2001230Verteiler*  CMx2001230VerteilerData::get_CommonCOMXVerteiler() 
{
	CMx2001230Verteiler *vert = dynamic_cast<CMx2001230Verteiler*>(get_werkauftrag());
	XASSERT(vert);
	if (!vert)
		AfxThrowNotSupportedException();
	return vert;
}

//Speichern der Daten
void CMx2001230VerteilerData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Klassen zur Bearbeitung liefern
void CMx2001230VerteilerData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(CMx2001230VerteilerEingabeBemassung));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}



bool CMx2001230VerteilerData::synchronize(WaxPart *with)
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


