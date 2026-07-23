#include "stdafx.h"



#include "GpFreigabe.h"
#include <mxodbc\mxodbc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(PrjGliederung, WaxPart, VERSIONABLE_SCHEMA|1)

BEGIN_MESSAGE_MAP(PrjGliederung, WaxPart)
	ON_COMMAND(ID_FREIGABE, on_freigeben) 
END_MESSAGE_MAP()

PrjGliederung::PrjGliederung()
{
	/**
	Konstruktor
	*/
	set_icon(IDI_GLIEDERUNG);
	CString titel;
	getsetTitel(false, titel);
	set_werkauftrag(new Werkauftrag);
	get_werkauftrag()->set_bezeichnung(titel);
}

PrjGliederung::~PrjGliederung()
{
	
}


void PrjGliederung::initPopUpMenu()
{	

	CMenu* menu = getPopUpMenu();
	
	
	if(WaxSys::isAV())
	{
		menu_items items[] =
		{
			ADD_MENU_ITEM(WAXID_NEU_SAMMEL_AUFTRAEGE_VIA_LIST)
			{ID_SEPARATOR, ""},
			ADD_MENU_ITEM(WAXID_NEUER_SAMMELAUFTRAG)
			ADD_MENU_ITEM(WAXID_NEU_GEWINDEROHR)
			ADD_MENU_ITEM(WAXID_NEU_HANDELSLAENGE)
			ADD_MENU_ITEM(WAXID_NEU_STATIONSVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_STRANGROHR)
			ADD_MENU_ITEM(WAXID_NEU_VERTEILER)
			{ID_SEPARATOR, ""},
			ADD_MENU_ITEM(WAXID_NEU_FORMSTUECKEUNDFITTINGE)
			ADD_MENU_ITEM(WAXID_NEUE_ENTNAHMELEITUNG)
			ADD_MENU_ITEM(WAXID_NEUE_PUMPENPROBIERLEITUNG)
			ADD_MENU_ITEM(WAXID_NEUER_PUMPENVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_APAX)
			{ID_SEPARATOR, ""},
			#pragma message( "-----------------if def Verteiler wieder herausnehmen!!-----")
			
			ADD_MENU_ITEM(WAXID_NEU_CO2VERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_MX1230MX200VERTEILER)
			
			
			
	#ifndef __WAX_FUER_DRITTE__
			ADD_MENU_ITEM(WAXID_NEUE_PREISANFRAGE)
			{ID_SEPARATOR, ""},
	#endif

			ADD_MENU_ITEM(WAXID_NEUEUNTERGLIEDERUNG)
			ADD_MENU_ITEM(WAXID_NEUEUNTERGLIEDERUNG_ROOT)
			{ID_SEPARATOR, ""},
			{ -1, "" },
		};
		appendMenuItems(menu, items);
	}
	else
	{
		menu_items items[] =
		{
			ADD_MENU_ITEM(WAXID_NEUER_SAMMELAUFTRAG)
			ADD_MENU_ITEM(WAXID_NEU_GEWINDEROHR)
		//	ADD_MENU_ITEM(WAXID_NEU_HANDELSLAENGE)
			ADD_MENU_ITEM(WAXID_NEU_STATIONSVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_STRANGROHR)
			ADD_MENU_ITEM(WAXID_NEU_VERTEILER)
			{ID_SEPARATOR, ""},
			ADD_MENU_ITEM(WAXID_NEU_FORMSTUECKEUNDFITTINGE)
			ADD_MENU_ITEM(WAXID_NEUE_ENTNAHMELEITUNG)
			ADD_MENU_ITEM(WAXID_NEUE_PUMPENPROBIERLEITUNG)
			ADD_MENU_ITEM(WAXID_NEUER_PUMPENVERTEILER)
			ADD_MENU_ITEM(WAXID_NEU_APAX)
			{ID_SEPARATOR, ""},
			
	#ifndef __WAX_FUER_DRITTE__
			ADD_MENU_ITEM(WAXID_NEUE_PREISANFRAGE)
			{ID_SEPARATOR, ""},
	#endif

			ADD_MENU_ITEM(WAXID_NEUEUNTERGLIEDERUNG)
			ADD_MENU_ITEM(WAXID_NEUEUNTERGLIEDERUNG_ROOT)
			{ID_SEPARATOR, ""},
			{ -1, "" },
		};
		appendMenuItems(menu, items);
	}

	
	appendStandardMenuItems(menu);
}

void PrjGliederung::serialize(BinaryArchive &ar)
{
	/**
	Objekt abspeichern mit allen Unterobjeken
	*/
	if (get_deleted())
		return;
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
	if (ar.is_storing())
	{
		ar << kommentar;
	}
	else
	{
		ar >> kommentar;
	}
}




void PrjGliederung::getForms(CObList &RuntimeModules)
{
	/**
	Liste mit Bearbeitungsklassen/Masken zurückliefern
	*/
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(BauAbschnittAVSammelauftragGliederungForm));
}

#if 0
void PrjGliederung::on_freigeben()
{
	/**
	Freigeben eines ganzen Bauabschnitts
	*/
	freigeben();
}
#endif

bool PrjGliederung::freigeben(bool sicherheitsabfrage, Fortschrittanzeige *fortschritt, DWORD dbhandle)
{
	if (get_subobjects().GetCount() == 0)
		return true;
	if (sicherheitsabfrage)
	{
		CString msg;
		msg.Format(getStammdaten().getText("IDS_ALLE_POS_FREI").c_str(), get_werkauftrag()->get_bezeichnung());
		int res = AfxMessageBox(msg, MB_YESNO | MB_ICONQUESTION);
		if (res != IDYES)
			return true;
	}
	
	bool result = true;
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(pt);
		if (pt && !pt->get_deleted())
		{
			if (!pt->freigeben(false, fortschritt, dbhandle))
			{
				result = false;
				break;
			}
		}
	}

	return result;
}


void PrjGliederung::getsetTitel(bool save, CString &title)
{
	/**
	default Titel aus Registry setzen oder lesen
	*/
	if (save)
	{
		AfxGetApp()->WriteProfileString("GLIEDERUNGSOBJEKT", "titel", title);
	}
	else
	{
		title = AfxGetApp()->GetProfileString("GLIEDERUNGSOBJEKT", "titel", getStammdaten().getText("IDS_BAUABSCHNITT").c_str());
	}
}

bool PrjGliederung::testLeitkarte(bool isPreisanfrage)
{
	/**
	Prüft die Eingaben der Leitkarten aller Unteraufträge auf Vollständigkeit
	*/
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *sub = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(sub);
		if (sub)
		{
			if (!sub->testLeitkarte(isPreisanfrage))
				return false;
		}
	}
	return true;
}

CString PrjGliederung::getBezeichnung() const
{
	/**
	Wrapper für die Bezeichnung
	*/
	return get_werkauftrag_const()->get_bezeichnung();
}

bool PrjGliederung::subobj_allowed() const
{
	/**
	Nur Gliederungen und Sammelaufträge können Unterobjekte enthalten
	*/
	return true;
}

///Ist das Icon für nicht freigegeben zu modifizieren
bool PrjGliederung::is_freigegeben() const
{
	/**
	Zeigt an, dass der Werkauftrag einen Status hat, auf den hingewiesen werden muss.
	Dies ist bis jetzt nur: Werkauftrag angelegt aber nicht freigegeben. 
	Überlagertes Icon ist IDI_ICON_NICHT_FREIGEGEBEN (Ausrufezeichen in linker oberer Ecke)
	Es werden auch alle Unterobjekte überprüft, dieses Flag vererbt sich in der Hierarchie nach 
	oben, d.h. Auch eine Gliederung erhält das nicht freigegeben Symbol, wenn auch nur ein Unterobjekt
	nicht freigegeben wurde.
	false := nicht freigegeben -> !-Zeichen setzen
	true := freigegeben       -> kein !-Zeichen
	*/	
	const Werkauftrag* werk = get_werkauftrag_const();
	if (!werk || get_deleted())
		return true;
	
	for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos; )
	{
		const WaxPart *pt = dynamic_cast<const WaxPart*>(get_subobjects_const().GetNext(pos));
		if (!pt)
			continue;
		if (!pt->is_freigegeben())
			return false;
	}
	return true;
}
