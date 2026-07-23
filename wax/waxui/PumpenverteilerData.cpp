#include "stdafx.h"

#include "resource.h"
#include <afxcoll.h>
#include "PumpenverteilerEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PumpenverteilerData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(PumpenverteilerData, WaxPart, 1)


PumpenverteilerData::PumpenverteilerData()
{
	set_werkauftrag(new Pumpenverteiler);
	set_icon(IDI_PUMPENVERTEILER);
}

PumpenverteilerData::~PumpenverteilerData()
{
}

//Bearbeitungsklassen zurückliefern
void PumpenverteilerData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PumpenverteilerEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}


//Serialisierung
void PumpenverteilerData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

