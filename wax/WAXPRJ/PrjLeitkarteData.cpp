#include "stdafx.h"


BEGIN_MESSAGE_MAP(PrjLeitkarteData, WaxPart)
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(PrjLeitkarteData, WaxPart, VERSIONABLE_SCHEMA|1)

//Konstruktor
PrjLeitkarteData::PrjLeitkarteData(WaxPart *pParent) : WaxPart(pParent)
{
	set_werkauftrag(new Werkauftrag);
	set_icon(IDI_LEITKARTE);
	set_dragable(false);
	CString str; str = getStammdaten().getText("IDS_PROJEKTLEITKARTE").c_str();
	get_werkauftrag()->set_bezeichnung(str);
}

//Destruktor
PrjLeitkarteData::~PrjLeitkarteData()
{
}



//Speichern der Daten
void PrjLeitkarteData::serialize(BinaryArchive &ar)
{
	get_leitkarte().serializeStatics(ar);
	get_leitkarte().serialize(ar);
}

//Dialogklasse zurückliefern
void PrjLeitkarteData::getForms( CObList &RuntimeModules) 
{
	RuntimeModules.AddHead((CObject*)RUNTIME_CLASS(PrjLeitkarteForm));
}

CString PrjLeitkarteData::getBezeichnung()const
{
	return get_werkauftrag_const()->get_bezeichnung();
}

/// Ist das Icon für nicht freigegeben zu modifizieren
bool PrjLeitkarteData::is_freigegeben() const
{
	// Projektleitkarte immer als freigegeben markiert.
	return true;
}
