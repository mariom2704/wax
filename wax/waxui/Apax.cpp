#include "stdafx.h"



#include "resource.h"
#include "ApaxEingabeForm.h"

#include <mxbase/env.h>
#include <fstream>
#include <afxhtml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ApaxData, WaxPart)
END_MESSAGE_MAP()


IMPLEMENT_SERIAL(ApaxData, WaxPart, 1)

ApaxData::ApaxData()
{
	set_werkauftrag(new ApaxWerkauftrag);
	set_icon(IDI_APAX);
}

ApaxData::~ApaxData()
{
}

//Bearbeitungsklassen zurückliefern
void ApaxData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	if (WaxSys::isAV())
	{
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(ApaxPosition));
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(ApaxEingabeForm));
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(MaterialEingabeForm));
		RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(SonderkostenEingabeForm));
	}
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PrjAnhangForm));
}

//Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich 
//ist
CRuntimeClass* ApaxData::getGraphicView()
{
	if (!get_werkauftrag()->get_anhang())
		return NULL;

	if (get_werkauftrag()->get_anhang()->is_graphic() )
		return RUNTIME_CLASS(CHtmlView);
	return NULL;
}


//Serialisierung
void ApaxData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}



#ifdef __OLDAPAX
// PR 31.3.2008 Erstmal deaktiviert

//Lädt einen Apax aus Altwax
void ApaxData::serializeOldApax(BinaryArchive & ar)
{
	CString apx;
	XASSERT(ar.IsLoading());
	if (ar.IsLoading())
	{
		ar >> apx;
		TRACE("%s\n", apx);
	}
}
#endif 


