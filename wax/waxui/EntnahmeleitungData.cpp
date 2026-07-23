#include "stdafx.h"


#include <afxcoll.h>

#include "resource.h"
#include "EntnahmeleitungEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(EntnahmeleitungData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(EntnahmeleitungData, WaxPart, 1)

EntnahmeleitungData::EntnahmeleitungData()
{
	set_werkauftrag(new EntnahmeleitungDWB);
	set_icon(IDI_ENTNAHMELEITUNGDWB);
}

EntnahmeleitungData::~EntnahmeleitungData()
{
}

//Bearbeitungsklassen zurückliefern
void EntnahmeleitungData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(EntnahmeleitungEingabe));
#ifndef __WAX_FUER_DRITTE__
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
#endif
	addAVTabs(RuntimeModules);
}

//Serialisierung
void EntnahmeleitungData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}



