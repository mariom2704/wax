#include "stdafx.h"


#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(VerteilerData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(VerteilerData, WaxPart, 1)

//Konstruktor
VerteilerData::VerteilerData()
{
	set_werkauftrag(new Verteiler);
	set_icon(IDI_VERTEILER);
}

//Destruktor
VerteilerData::~VerteilerData()
{
}



//Bearbeitungsklassen zurückliefern
void VerteilerData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(VerteilerEingabe));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(ElementeEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}


//Serialisierung
void VerteilerData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
//einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
//erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
//Objekte in subobjects auf.
bool VerteilerData::synchronize(WaxPart* with)
{
	if (with == this)
		return false;
	
	Verteiler *dest = dynamic_cast<Verteiler*>(get_werkauftrag());
	Verteiler *from = dynamic_cast<Verteiler*>(with->get_werkauftrag());
	XASSERT(dest && from);
	if (dest && from)
	{
		dest->set_rohrQuali(from->get_rohrQuali());
		return true;
	}

	return false;
}

