#include "stdafx.h"


#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(HandelslaengeData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(HandelslaengeData, WaxPart, 1)

HandelslaengeData::HandelslaengeData()
{
	set_werkauftrag(new Handelslaenge);
	set_icon(IDI_HLAENGE);
}

HandelslaengeData::~HandelslaengeData()
{
}


//Bearbeitungsklassen zurückliefern
void HandelslaengeData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(HandelslaengeEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}



//Serialisierung
void HandelslaengeData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
//einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
//erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
//Objekte in subobjects auf.
bool HandelslaengeData::synchronize(WaxPart* with)
{
	if (with == this)
		return false;
	
	Handelslaenge *dest = dynamic_cast<Handelslaenge*>(get_werkauftrag());
	Handelslaenge *from = dynamic_cast<Handelslaenge*>(with->get_werkauftrag());
	XASSERT(dest && from);
	if (!dest || !from)
		return false;

	dest->set_rohrQuali(from->get_rohrQuali());
	
	return true;
}

