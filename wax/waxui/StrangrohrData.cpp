#include "stdafx.h"

#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(StrangrohrData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(StrangrohrData, WaxPart, 1)

//Konstruktor
StrangrohrData::StrangrohrData()
{
	set_werkauftrag(new Strangrohr);
	set_icon(IDI_EINZELROHR);
}

//Destruktor
StrangrohrData::~StrangrohrData()
{
}



//Klassen zur Bearbeitung liefern
void StrangrohrData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(StrangrohrEingabe));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(MuffenEingabe));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(AVElementEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}


//Speichern der Daten
void StrangrohrData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
//einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
//erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
//Objekte in subobjects auf.
bool StrangrohrData::synchronize(WaxPart *with)
{
	if (with == this)
		return false;
	
	Strangrohr *dest = dynamic_cast<Strangrohr*>(get_werkauftrag());
	Strangrohr *from = dynamic_cast<Strangrohr*>(with->get_werkauftrag());
	XASSERT(dest && from);
	if (!dest || !from)
		return false;

	dest->set_trockenanlage(from->get_trockenanlage());
	dest->set_rohrQuali(from->get_rohrQuali());
	dest->set_dnvl(from->get_dnvl());
	dest->set_muffenID(from->get_muffenID());
	dest->set_muffenDN(from->get_muffenDN());
	dest->set_muffenWinkel(from->get_muffenWinkel());
	dest->set_muffenGekehlt(from->get_muffenGekehlt());
	dest->set_durchmesser(from->get_durchmesser());
	dest->set_sonderMuffenLaenge(from->get_sonderMuffenLaenge());
	dest->Set_Fertigungsstaette(from->Get_Fertigungsstaette());
	//dest->set_avnachbearbeitung(from->get_avnachbearbeitung());
	return true;
}

