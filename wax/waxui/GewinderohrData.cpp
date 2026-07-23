#include "stdafx.h"


#include "resource.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(GewinderohrData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(GewinderohrData, WaxPart, 1)

//Konstruktor
GewinderohrData::GewinderohrData()
{
	set_werkauftrag(new Gewinderohr);
	set_icon(IDI_GEWINDEROHR);
}

//Destruktor
GewinderohrData::~GewinderohrData()
{
}



//Serialisierung
void GewinderohrData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}



//Bearbeitungsklassen zurückliefern
void GewinderohrData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(GRohranlageEingabe));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(GewinderohrEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}


//Methode, die in abgeleiteten Klassen überschrieben 
//werden kann, um über 
//einen Aufruf von synchronize(this) einen Abgleich von 
//Memberattributen zu 
//erreichen. Defaultmässig ruft synchronize die 
//synchronize Methode aller 
//Objekte in subobjects auf.
bool GewinderohrData::synchronize(WaxPart* with)
{
	if (with == this)
		return false;
	
	Gewinderohr *dest = dynamic_cast<Gewinderohr*>(get_werkauftrag());
	Gewinderohr *from = dynamic_cast<Gewinderohr*>(with->get_werkauftrag());
	XASSERT(dest && from);
	if (!dest || !from)
		return false;

	dest->set_typ(from->get_typ());
	dest->set_dichtmittel(from->get_dichtmittel());
	dest->set_rohrQuali(from->get_rohrQuali());
	dest->Set_Fertigungsstaette(from->Get_Fertigungsstaette());
	return true;
}

