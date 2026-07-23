#include "stdafx.h"


#include "resource.h"
#include <afxcoll.h>

#include "PumpenprobierleitungEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PumpenprobierleitungData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(PumpenprobierleitungData, WaxPart, 1)


PumpenprobierleitungData::PumpenprobierleitungData()
{
	set_werkauftrag(new Pumpenprobierleitung);
	set_icon(IDI_PUMPENPROBIERLEITUNG);
}


PumpenprobierleitungData::~PumpenprobierleitungData()
{
}

//Bearbeitungsklassen zurückliefern
void PumpenprobierleitungData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PumpenprobierleitungEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}

//Serialisierung
void PumpenprobierleitungData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}



