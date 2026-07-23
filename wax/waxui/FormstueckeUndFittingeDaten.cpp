#include "stdafx.h"

#include <afxcoll.h>
#include "resource.h"

#include "FormstueckeUndFittingeEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(FormstueckeUndFittingeDaten, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(FormstueckeUndFittingeDaten, WaxPart, 1)

//Konstruktor
FormstueckeUndFittingeDaten::FormstueckeUndFittingeDaten()
{
	set_werkauftrag(new FormstueckeFittinge);
	set_icon(IDI_FORMSTUECKEUNDFITTINGE);
}



FormstueckeUndFittingeDaten::~FormstueckeUndFittingeDaten()
{
}


//Bearbeitungsklassen zurückliefern
void FormstueckeUndFittingeDaten::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(FormstueckeUndFittingeEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	if (WaxSys::isAV())
	{
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(ArbeitEingabeForm));
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(SonderkostenEingabeForm));
	}
}

//Serialisierung
void FormstueckeUndFittingeDaten::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}


///Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
CRuntimeClass* FormstueckeUndFittingeDaten::getGraphicView()
{
	return NULL;
} 

