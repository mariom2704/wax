#include "stdafx.h"


#include "resource.h"


#include "CadWerkauftragData.h"
#include "../rohrkonst/cadwerkauftrag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CadWerkauftragData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(CadWerkauftragData, WaxPart, 1)

CadWerkauftragData::CadWerkauftragData()
{
	set_werkauftrag(new CadWerkauftrag);
	set_icon(IDI_CADWERKAUFTRAG);
}

CadWerkauftragData::~CadWerkauftragData()
{
}


//Bearbeitungsklassen zurückliefern
void CadWerkauftragData::getForms(CObList& RuntimeModules)
{
	WaxPart::getForms(RuntimeModules);
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(VerteilerEingabe));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(ElementeEingabe));
	addAVTabs(RuntimeModules);
}


//Serialisierung
void CadWerkauftragData::serialize(BinaryArchive& ar)
{
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
}

//Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
//einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
//erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
//Objekte in subobjects auf.
bool CadWerkauftragData::synchronize(WaxPart* with)
{
	if (with == this)
		return false;
	
	
	return true;
}

