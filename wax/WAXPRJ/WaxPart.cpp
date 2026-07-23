#include "stdafx.h"



#include <mxcontrols/fileloader.h>

#include "DbFreigabe.h"
#include "WaxPartForm.h"
#include "IconManager.h"
#include "resource.h"
#include "..\waxgl\WaxGrafik.h"

#include "..\wax\docver.h"
#include "..\wax\waxdoc.h"
#include "WpPreisanfrage.h"
#include "Fortschrittanzeige.h"
#include "GpFreigabe.h"

#include <afxhtml.h>

#include <mxcontrols/msgbox.h>
#include <mxcontrols/OpenFileDlg.h>
#include <mxodbc\mxodbc.h>


using namespace MxGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(WaxPart, CWnd, VERSIONABLE_SCHEMA|1)

BEGIN_MESSAGE_MAP(WaxPart, CWnd)
	ON_COMMAND(ID_FREIGABE, on_freigeben)  
END_MESSAGE_MAP()

//MenuManager WaxPart::menu_manager;

///Konstruktor
WaxPart::WaxPart(WaxPart *pParent) : deleted(false), dragable(true), version(0), icon(0), werkauftrag(NULL), needUpdate(false), popUpMenu(NULL)
{
	set_parent(pParent);
	set_version(GetRuntimeClass()->m_wSchema);
	av = WaxSys::isAV();
	status_update_test = true;
	enable_update = false;

}

///Destruktor löscht alle Unterobjekte aus subobjects
WaxPart::~WaxPart()
{
	delete werkauftrag;
	subobjects.deleteAllElemente();
	delete popUpMenu;
}

///Serialisierung
void WaxPart::serialize(BinaryArchive &ar)
{
	/**
	Teil speichern 
	Es wird die Objektliste in subobjects serialisiert. Beim Laden wird version initalisiert.
	version kann in den Serialisierungsfunktionen abgefragt werden und bietet dadurch die Möglichkeit
	einer Versionierung des Dateiformates auf Unterfunktionsebene.
	*/ 
	if (ar.is_storing())
	{
		for (POSITION pos = get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			WaxPart *obj = (WaxPart*)get_subobjects().GetNext(pos);
			if (obj->get_deleted())
				continue;
			ar << (BYTE)1;
			
			ar.write_class(obj->GetRuntimeClass());
			obj->serialize(ar);
		}
		ar << (BYTE)0;
	}
	else
	{
		BYTE goon;
		ar >> goon;
		while (goon)
		{ //CRuntimeClass
			UINT schema = 0;
			CObject *load = ar.read_class(NULL, &schema)->CreateObject();
			if (load->IsKindOf(RUNTIME_CLASS(WaxPart)))
			{
				load->GetRuntimeClass()->m_wSchema = schema;
				((WaxPart*)load)->set_version(schema);
				((WaxPart*)load)->set_parent(this);
			}
			else
			{
				XASSERT(FALSE);
			}
			((WaxPart*)load)->serialize(ar);
			get_subobjects().AddTail(load);
			ar >> goon;
		}
	}
}

///Zuweisungsoperator
void WaxPart::copy(WaxPart &other)
{
	set_leitkarte(other.get_leitkarte());;
	for (POSITION pos = other.get_subobjects().GetHeadPosition(); pos != NULL;)
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(other.get_subobjects().GetNext(pos));
		XASSERT(pt);
		if (!pt)
			continue;
		CRuntimeClass *cl = pt->GetRuntimeClass();
		WaxPart *newpt = (WaxPart*)cl->CreateObject();
		newpt->copy(*pt);
		get_subobjects().AddTail(newpt);
	}
}

///Objekt im Baum anzeigen
HTREEITEM WaxPart::Show(CTreeCtrl &tree, HTREEITEM parent, HTREEITEM insafter, IconManager &iconman)
{
	if (this->get_deleted())
		return NULL;
	
	initIcon(&tree, iconman);
	int i_index = get_iconindex(iconman);

	HTREEITEM item = tree.InsertItem(getBezeichnung(), i_index, i_index, parent, insafter);

	if (item)
	{
		tree.SetItemData(item, (DWORD)this);
		for (POSITION pos = get_subobjects().GetHeadPosition(); pos != NULL;)
		{
			CObject *obj = get_subobjects().GetNext(pos);
			WaxPart *p = dynamic_cast<WaxPart*>(obj);
			XASSERT(p);
			if (p && !p->get_deleted())
				p->Show(tree, item, TVI_LAST, iconman);
		}
		if (initVisible())
			tree.EnsureVisible(item);
	}
	
	return item;
}

///Unterobjekt erzeugen
CObject* WaxPart::addChild(WaxPart *newchild, bool setleitkarte)
{
	/**
	Wenn newchild == NULL ist, wird über das RuntimeClass-Objekt ein neues Objekt erzeugt 
	und in die Liste subobjects eingehängt.
	Ist newchild != NULL wird das übergebene Objekt am Ende der Liste angefügt.
	Gibt den Pointer auf das neue Objekt zurück
	*/
	WaxPart *ret;
	if (!newchild)
		ret = dynamic_cast<WaxPart*>(GetRuntimeClass()->CreateObject());
	else
		ret = newchild;
	XASSERT(ret);
	if (!ret)
		return NULL;
	if (get_subobjects().Find(ret))
		return NULL;
	if (!get_subobjects().IsEmpty() && setleitkarte)
	{ // Initialisieren der Positionsnummer...
		for (POSITION pos = get_subobjects().GetTailPosition(); pos;)
		{
			WaxPart *pre = dynamic_cast<WaxPart*>(get_subobjects().GetPrev(pos));
			XASSERT(pre);
			if (pre && !pre->get_deleted())
			{
				ret->get_werkauftrag()->initPosnr(pre->get_werkauftrag()->get_posNr());		
				
				// ...und die Beschichtung vorbelegen...
				ret->get_werkauftrag()->set_beschichtung(pre->get_werkauftrag()->get_beschichtung());
				
				if (ret->GetRuntimeClass() == pre->GetRuntimeClass())
				{
					// ...und die KFarbe und dnvl vorbelegen
					Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(ret->get_werkauftrag());
					if (rohr)
					{
						Rohrleitung* prev = dynamic_cast<Rohrleitung*>(pre->get_werkauftrag());
						if (prev)
						{
							rohr->set_rohrKFarbe(prev->get_rohrKFarbe());
							if (!dynamic_cast<Gewinderohr*>(pre->get_werkauftrag()))
								rohr->set_dnvl(prev->get_dnvl());		
						}
					}
				}
				break;
			}
		}
	}

	get_subobjects().AddTail(ret);
	ret->set_parent(this);
	if (setleitkarte)
	{
		/// vk3 Flag wird nie neu gesetzt, muss aber beim Kopieren mitgenommen werden
		bool vk3 = ret->get_leitkarte().get_vk3();
		ret->set_leitkarte(get_leitkarte());
		ret->get_leitkarte().init_wunschliefertermin();	
		ret->get_leitkarte().set_vk3(vk3);
	}

	return ret;
}

///Menü initialisieren
void WaxPart::initPopUpMenu() 
{
	//menu = menu_manager.getObject(menuid);
	//menu_manager.incUsage(menuid);


	CMenu* menu = getPopUpMenu();
	appendStandardMenuItems(menu);

}

///Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
CRuntimeClass* WaxPart::getGraphicView()
{
	/**
	Diese Methode muss in abgeleiteten Klassen überschrieben werden, wenn eine Grafikansicht
	existiert. Die Grafikansicht kann eine beliebige, von CView abgeleitete Klasse sein
	*/

	if (WaxSys::para_str(GRAFIKFORMAT) == "SVG")
		return RUNTIME_CLASS(CHtmlView);
	return NULL;
} 

///Bearbeitungsklassen zurückliefern
void WaxPart::getForms( CObList &RuntimeModules) //@parm Liste mit RuntimeKlassen der Dialogklassen
{
	/**
	Rückgabe der Dialog-Klassen zur Bearbeitung des WaxParts
	Diese Methode muss in abgeleiteten Klassen überschrieben werden. 
	Dazu muss einfach RuntimeModules geleert werden und die Dialogklassen der Reihenfolge nach
	eingefügt werden. Diese Klassen werden als Tabs angezeigt.

	Wird als erster Aufruf in einer abgeleiteten Klasse WaxPart::getForms() aufgerufen, wird 
	standardmässig als erstes die Leitkarte eingefügt, aber nur, wenn parent kein Sammelauftrag ist.
	*/
	RuntimeModules.RemoveAll();
	XASSERT(get_parent());
	
	// Unterobjekte von Sammelaufträgen erhalten keine Leitkarte. Das ist leider nicht
	// gerade elegant...
	if (get_parent() && get_parent()->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		return;
	
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(SammelAuftragForm));
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(BeschichtungEingabeForm));
}

///Titel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
CString WaxPart::getBezeichnung() const
{
	/**
	Titel des Objektes zurückliefern
	Diese Methode kann bei anderen von WaxPart abgeleiteten Klassen überschrieben werden, um z.B.
	eine Positionsbezeichnung im Baum anzuzeigen. 
	Rückgabe: Referenz auf einen Titelstring.
	*/
	CString bez;
	if (get_parent() && get_parent()->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{	
		bez = get_werkauftrag_const()->getPosBezeichnung(false);	
	}
	else
	{
		bez = get_werkauftrag_const()->getPosBezeichnung(true);
	}

	if (get_werkauftrag_const()->get_status_const().get_storniert())
	{
		CString tmp;
		tmp = getStammdaten().getText("IDS_STORNIERT").c_str();
		CString ret;
		ret.Format("%s: %s", tmp, bez);
		return ret;
	}

	if(!get_werkauftrag_const()->get_anhang()->get_filename().IsEmpty())
	{
		
		if (WaxSys::isAV()) 
		{
			string value  = get_werkauftrag_const()->get_anhang()->get_filename();
			size_t found;
			found = value.find(".svg");
			if (found != string::npos)
			{
				Rohrleitung* tmprohr = dynamic_cast<Rohrleitung*>(werkauftrag);
				if(tmprohr)
				{
					if(!tmprohr->get_IsApaxBauteil())
					{
						bez += " (CAD)";
					}
					else
					{
						bez += " (mit Anhang)";
					}
				}
			}
			else
			{
				bez += " (mit Anhang)";
			}
		}
	}

	return bez;
}

///Grafikdatei erzeugen, normalerweise ein DWF File

void WaxPart::createGraphic()
{
	// Virtuelle Methode, muss in abgeleiteten Klassen implementiert werden
	Rohrleitung *tmprohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
	if (!tmprohr && !dynamic_cast<ApaxWerkauftrag*>(get_werkauftrag()))
		return;


	if (get_werkauftrag()->get_anhang() && 
		!get_werkauftrag()->get_anhang()->get_filename().IsEmpty() && 
		get_werkauftrag()->get_anhang()->get_showAnhang())
	{

		MxBase::Env env;
		
		CString strfile;
		strfile = CString(get_CounterGlobal().c_str()) + (LPCSTR)get_werkauftrag()->get_anhang()->get_filename();
		string fn = env.get_tempdir("wax") + (string) strfile;
		WaxSys::add_tmpfile(fn.c_str());

		fstream tmpfile;
		tmpfile.open(fn.c_str(), ios_base::out | ios_base::trunc | ios_base::binary);
		if (tmpfile.is_open())
			tmpfile.write((const char*)get_werkauftrag()->get_anhang()->get_data(), get_werkauftrag()->get_anhang()->get_len());

		set_grafic_file(fn);
		m_IsAnhangGraficfile = true;
	}
	else
	{

		m_IsAnhangGraficfile = false;
		if(dynamic_cast<ApaxWerkauftrag*>(get_werkauftrag()))// Apax Auftrag ohne anhang
			return;
		try
		{
			tmprohr->createModell();
		}
		catch (std::exception const& e)
		{
			MsgBox::show_error(e.what());
		}
		
		if (!this->get_grafic_file().length())
		set_grafic_file(WaxSys::GetTempFilename(WaxGrafik::Creator::get_fileextension(WaxGrafik::SVG)));
		
		WaxGrafik::Creator::create(tmprohr, get_grafic_file());
		
	}


}

///WM_COMMAND Nachrichten verarbeiten
BOOL WaxPart::PreTranslateMessage(MSG *pMsg)
{
	/**
	Abarbeiten einer eingehenden Message
	Diese Methode wird von CTabCtrlView::PreTranslateMessage aufgerufen. Damit wird die 
	Nachrichtenverwaltung umgestellt: Messages werden nicht mehr in den Dialogklassen bearbeitet, sondern
	direkt vom Datenobjekt. 
	Nach der Bearbeitung einer Message wird vom CTabCtrlView für alle Tabdialoge UpdateData(FALSE) aufgerufen,
	um evtl. Änderungen zu reflektieren. Der Baumzweig wird ebenfalls neu aufgebaut.
	*/
	const AFX_MSGMAP* msgmap = GetMessageMap();

	AFX_MSGMAP_ENTRY* entry[3];
	// Klasse und Basisklasse (evtl. zu erweitern)
	entry[0] = const_cast<AFX_MSGMAP_ENTRY*> (msgmap->lpEntries);
	entry[1] = const_cast<AFX_MSGMAP_ENTRY*> (msgmap->pfnGetBaseMap()->lpEntries);
	entry[2] = NULL;
	for (int i = 0; entry[i]; i++)
		{
		while (entry[i]->nMessage)
		{
			
	//		TRACE("Entry %i : nMessage = %i, nCode = %i, : nId = %i, nLastId = %i, nSig = %i\n", 
	//			entry->nMessage, entry->nCode, entry->nID, entry->nLastID, entry->nSig);
			if (entry[i]->nMessage == pMsg->message)
				if (entry[i]->nID == (UINT)pMsg->wParam)
				{
					union smg 
					{
						AFX_PMSG x;
						void (AFX_MSG_CALL CCmdTarget::*pfn_COMMAND)();
					};
					union smg exec;
					AFX_PMSG fnk;
					fnk = entry[i]->pfn;
					exec.x = fnk;
					(this->*exec.pfn_COMMAND)();
					return TRUE;
				}
			entry[i]++;
		}
	}
	
	
	return FALSE;
}

const bool WaxPart::get_dragable() const
{
	return dragable;
}

const bool WaxPart::get_deleted() const
{
	return deleted;
}

void WaxPart::set_deleted(bool value)
{
	deleted = value;
}

///DWF Filename abfragen
string WaxPart::get_grafic_file() 
{
	/**
	 Sollte das WaxPart eine Grafikview <c DWFView> benötigen, MUSS diese
	Methode überschrieben werden, damit das DWFView richtig initialisiert werden kann. 
	Diese Umständlichkeit rührt von der geschützen Erstellung der Viewklassen, 
	die eine Parameterübergabe verhindern.Das Framework erzeugt die Viewklassen automatisch und 
	ruft leider auch nicht OnInitialUpdate auf, wo normalerweise ein Aufruf von GetDocument() 
	erfolgen könnte. 
	*/
	if(!svgFile.empty())
	{
		graficfile = svgFile;
	}

	if(m_IsAnhangGraficfile && !DwfAndOtherGraficFiles.empty())
	{
		graficfile = DwfAndOtherGraficFiles;
	}
	return graficfile;
}

void WaxPart::set_grafic_file(const string& value)
{
	size_t found;
	found = value.find(".svg");
	if (found != string::npos)
	{
		svgFile = value;
	}
	else
	{
		DwfAndOtherGraficFiles = value;
	}
}

const UINT& WaxPart::get_version() const
{
	return version;
}

void WaxPart::set_version(UINT value)
{
	version = value & ~VERSIONABLE_SCHEMA;
}

void WaxPart::set_icon(UINT  value)
{
	icon = value;
}

void WaxPart::set_dragable(bool value)
{
	dragable = value;
}

WaxPartListe& WaxPart::get_subobjects() 
{
	return subobjects;
}

const WaxPartListe& WaxPart::get_subobjects() const
{
	return subobjects;
}

Werkauftrag* WaxPart::get_werkauftrag() 
{
	XASSERT(werkauftrag);
	return werkauftrag;
}

void WaxPart::set_werkauftrag(Werkauftrag* value)
{
	XASSERT(value);
	werkauftrag = value;
}


///Pointer Werkauftrag und intern auf NULL setzen
Werkauftrag* WaxPart::detach_werkauftrag()
{
	Werkauftrag *tmpWa = get_werkauftrag();

	werkauftrag = NULL;

	return tmpWa;
}



///Liefert eine Referenz auf das Kontextmenü zurück
CMenu* WaxPart::get_menu() 
{
	/**
	Diese Methode liefert eine Referenz des Kontextmenüs der Klasse zurück 
	Über das Kontextmenü kann ein Objekt weitergehend bearbeitet werden. Im Hauptmenü sind bis jetzt 
	nur allgemein gültige Menüpunkte (Kopieren, löschen usw. ) vorgesehen. Alle Messages die durch das Kontext
	menü erzeugt werden, werden an das Formobjekt weitergeleitet.

	Im Fehlerfall wird ein (am Anfang) leeres Menü zurückgegeben.
	*/
	initPopUpMenu();
	CMenu* ret = getPopUpMenu();
	popUpMenu = NULL;
	return ret;

	/*
	
	static CMenu on_error_fallback;
	if (!menu)
	{// Keine Initialisierung bisher, Standardmenu wird erzeugt
		menu = menu_manager.getObject(IDR_WAXPART);
	}
	CMenu *sub = NULL;
	// Wenn ein Menüpunkt in den Resourcen eingetragen ist, muss das Submenu genommen werden, 
	// ansonsten wird in WaxPart::initMenu ein Hauptmenü erzeugt.
	XASSERT(menu != NULL);

	if (menu)
		sub = menu->GetSubMenu(0);
	else
		sub = &on_error_fallback;

	return sub ? *sub : *menu;
	*/
}

void WaxPart::set_leitkarte( Leitkarte& value)
{
	XASSERT(get_werkauftrag());
	if (get_werkauftrag())
		get_werkauftrag()->get_leitkarte().copy(value);
}


///rekursive Vererbung der 'statischen' Leitkartendaten.
void WaxPart::setPrjLeitkarte(const Leitkarte& leitk)
{
	get_leitkarte().setPrjdata(leitk);
	for (POSITION pos = this->subobjects.GetHeadPosition();pos;)
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(subobjects.GetNext(pos));
		XASSERT(pt);
		if (pt)
			pt->setPrjLeitkarte(leitk);
	}
}

Leitkarte& WaxPart::get_leitkarte()
{
	static Leitkarte leit; // als leerer Dummy
	XASSERT(get_werkauftrag());
	if (get_werkauftrag())
		return get_werkauftrag()->get_leitkarte();
	return leit;
}

bool WaxPart::isPreisanfrage()const
{
	if (!get_parent())
		return false;

	return get_parent()->isPreisanfrage();
}



///Werkauftrag freigeben
void WaxPart::on_freigeben()
{

	GpFreigabe freigabe;
	int count = freigabe.countWerkauftraege(this);		
	Fortschrittanzeige fortschrittanzeige(0, count, getStammdaten().getText("msg_freigabe").c_str());
	

	//Daten sichern
	CMemFile f;
	if (!freigabe.saveWaxPart(this, &f))
		return;

	MXTable activeDB(WaxSys::getServerdb(), "Status");
	DbBase transation(activeDB.get_Db());
	transation.beginTransaction();

	if (freigeben(true, &fortschrittanzeige, activeDB.get_Db()))
		transation.commitTransaction();
	else
	{
		transation.rollbackTransaction();

		freigabe.restoreWaxPart(&f, this);
	}

}


void WaxPart::nextStepFortschritt(Fortschrittanzeige *fortschritt, Werkauftrag *wa)
{
		if (fortschritt == NULL)
			return;

		//Dialog evt. starten
		fortschritt->start();

		//Auftragsbez
		CString str;
		str.Format("%s: %s", getStammdaten().getText("msg_auftrag").c_str(), wa->get_leitkarte().get_prj_auftragsbez());
		fortschritt->setAnzeige1(str);

		if( wa->get_leitkarte().get_IsChoosenIppeNumbers())
		{
			str.Format("%s: %s - %s     %s: %s", getStammdaten().getText("msg_sdnr").c_str(), wa->get_leitkarte().get_ippeMatNr(), wa->get_leitkarte().get_ippeKnotenNr(), 
				                                 getStammdaten().getText("IDS_BAUABSCHNITT").c_str(), wa->get_leitkarte().get_prj_bauAbschnitt());

		}
		else
		{
			str.Format("%s: %I64d - %i     %s: %s", getStammdaten().getText("msg_sdnr").c_str(), wa->get_leitkarte().get_sdAuftragsnr(), wa->get_leitkarte().get_sdPositionsnr(), 
				                                 getStammdaten().getText("IDS_BAUABSCHNITT").c_str(), wa->get_leitkarte().get_prj_bauAbschnitt());
		}
		fortschritt->setAnzeige2(str);

		str.Format("%s: %s  ", getStammdaten().getText("msg_position").c_str(), wa->get_posNr());
		fortschritt->setAnzeige3(str);

		fortschritt->nextStep();
		fortschritt->updateCounterAnzeige();	
}



bool WaxPart::freigeben(bool sicherheitsabfrage, Fortschrittanzeige *fortschritt, DWORD dbhandle)
{
	if (sicherheitsabfrage)
	{
		CString str; str = getStammdaten().getText("IDS_AUFTRAG_FREI").c_str();
		if (AfxMessageBox(str, MB_ICONQUESTION | MB_YESNO) == IDNO)
			return true;
	}


	if (isPreisanfrage())
	{
		AfxMessageBox(getStammdaten().getText("ERR_NOT_REIGABE_PA").c_str(), MB_ICONERROR | MB_OK);
		return true;
	}

	if (!checkFreigabe())
		return true;

	WaxPart *wp = this;
	WaxPart *parent = const_cast<WaxPart*>(get_parent());


	bool result = true;
	TRY
	{
				
		// Prüfung auf Sammelauftrag
		if (get_parent() && parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		{
			CString str; str = getStammdaten().getText("IDS_FREIGABE_FUER_SAMMEL").c_str();
			if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION) == IDYES)
				return parent->freigeben(false, NULL, dbhandle);
			return true;
		}


		// CheckPlausis von Werkauftrag
		CString err = get_werkauftrag()->checkPlausis();
		if (err.GetLength() > 0)
		{
			AfxMessageBox(err, MB_ICONERROR | MB_OK);
			return false;
		}
		// Pruefung Rohrstutzen auf Abschlusselement
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_werkauftrag());
		if (rohr)
		{
			CString err = rohr->check_rohrstutzen();			
			if (err.GetLength() > 0)
			{
				AfxMessageBox(err, MB_ICONERROR | MB_OK);
				return false;
			}
		}

		if (!WaxSys::isAV()) 
		{
			wp->get_werkauftrag()->get_preis().updatePreis();
		}

		Server serv(Server::load, NULL, dbhandle);
		serv.set_werkauftrag(wp->get_werkauftrag());

		// Vor jeder Freigabe wird die WA Nummer upgedated.
		if (wp->get_werkauftrag()->get_waid())
		{
			serv.get_wanr();
		}


		//Fortschritt hochsetzen
		nextStepFortschritt(fortschritt, wp->get_werkauftrag());

		DbFreigabe wafrei(wp, dbhandle);

		// Prüfung, ob alle Eingabewerte ok sind
		// Plausis brauchen nur für einen Auftrag geprüft werden, da die Leitkartendaten
		// für alle Unterpositionen gleich sind
		if (wafrei.check_freigabe_plausis())
		{
			if (!wafrei.freigeben())
			{
				CString str; str = getStammdaten().getText("IDS_FREIGABE_NICHT2").c_str();
				AfxMessageBox(str);
				result = false;
			}
			if (result && wp->get_werkauftrag()->get_waid())
			{
				// Bei jeder erfolgreichen Freigabe werden die Statusvariablen upgedated
				serv.updateStatus();
				needUpdate = true;

				//Zum Testen, weil die WAID in der NL nicht immer in der Datei gespeichert wird
				if (!WaxSys::isAV())
				{
					MXDate cur = MXDate::GetCurrentTime();
					MxString date = cur.Format("%d.%m.%Y %H:%M:%S");

					MxString st = MxString("Freigabe WaxPart: WAID= {0} Datum:{1}").arg(wp->get_werkauftrag()->get_waid()).arg(date);
					LOG(st.getString());
				}
			}
			else
			{
				MxString st = "Freigabefehler: Keine WAID";
				LOG(st.getString());
			}
		}
		else
			result = false;
	}
	CATCH_ALL(e)
	{
		// Dummy Variable/Abfrage, damit beim xassert eine sinnvolle Fehlermeldung angezeigt wird 
		result = false;
		bool onfreigeben = false;
		XASSERT(onfreigeben == true);

		MxString st = "Freigabefehler: " + e->ReportError();
		LOG(st.getString());
	}
	END_CATCH_ALL


	AfxGetMainWnd()->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);

	return result;
}

///Erzeugt aus dem Standardicon des Objektes ein modifiziertes 
///(Ausrufezeichen). Dieses Icon wird angezeigt, wenn der Werkauftrag nicht 
///freigeben wurde.
HICON WaxPart::modify_icon(HICON src, UINT overlay_icon)
{
	/**
	Diese Methode erzeugt aus dem Standardicon und einem überlagerten Icon ein neues Icon, dass
	einen nicht freigegebenen Werkauftrag symbolisiert. 
	Funktioniert nur bei 32x32 Bit Icons 
	*/
	static bool iconwarn = false;
	BITMAP bmpAND, bmpXOR;

	// Icongrösse ist auf 32x32 festgelegt: In der ANDMask wird für jedes Pixel ein 
	// Bit gespeichert, dass festlegt, ob der Pixel angezeigt wird oder nicht. false
	// heisst: Pixel anzeigen
	const int bitcount = 32*32/8; // =128
	BYTE ANDmaskIcon[bitcount], ANDmaskIcon2[bitcount];
	BYTE* XORmaskIcon, *XORmaskIcon2;

	// Aus Icon die Bitmap herausziehen
	ICONINFO piconinfo;
	memset((void*)&piconinfo, 0, sizeof(ICONINFO));
	GetIconInfo(src,  &piconinfo);

	// AND Maske ist 128 Byte gross
	CBitmap *tmp = CBitmap::FromHandle(piconinfo.hbmMask);
	tmp->GetBitmap(&bmpAND);
	DWORD sz = tmp->GetBitmapBits(bitcount, ANDmaskIcon);
	XASSERT(sz == bitcount);

	// XOR Maske ist 4096 Byte gross bei 32 Bit (True Color)
	// 3072 Byte bei 24 Bit (High Color)
	// 2048 Byte bei 16 Bit (High Color)
	// 1024 Byte bei 8 Bit (256 Farben)
	tmp = CBitmap::FromHandle(piconinfo.hbmColor);
	tmp->GetBitmap(&bmpXOR);
	DWORD bmpsize = bmpXOR.bmBitsPixel*bmpXOR.bmHeight* (bmpXOR.bmWidth/8);
	XORmaskIcon = new BYTE[bmpsize];
	sz = tmp->GetBitmapBits(bmpsize, XORmaskIcon);
	XASSERT(sz == bmpsize);
	
	// Testen, ob die Farbauflösung unterstützt wird
	if (sz != 4096 && sz != 2048 && sz != 3072 && sz != 1024)
	{		
		delete XORmaskIcon;
		return src;
	}
	
	// Überlagerungsicon laden
	HICON nfrei = AfxGetApp()->LoadIcon(overlay_icon);
	GetIconInfo(nfrei,  &piconinfo);
	tmp = CBitmap::FromHandle(piconinfo.hbmColor);
	tmp->GetBitmap(&bmpXOR);
	XORmaskIcon2 = new BYTE[bmpsize];
	sz = tmp->GetBitmapBits(bmpsize, XORmaskIcon2);
	XASSERT(sz == bmpsize);

	tmp = CBitmap::FromHandle(piconinfo.hbmMask);
	tmp->GetBitmap(&bmpAND);
	sz = tmp->GetBitmapBits(bitcount, ANDmaskIcon2);
	XASSERT(sz == bitcount);
	
	// Auflösung 
	int aufloesung = bmpsize / bitcount;

	// cmax ist die Anzahl der Farbbytes in der XORmask
	int cmax = aufloesung / 8;

	for (int i = 0; i < bitcount; i++)
	{
		unsigned char mask = 0x80;
		for (int x = 0; x < 8 ; x++)
		{	
			if (!(ANDmaskIcon2[i] & mask))
			{
				// Pixel anschalten (auf 0 setzen)
				ANDmaskIcon[i] = ANDmaskIcon[i] & ~mask;
				// Pixel wird angezeigt, Farbe aus XORmaskIcon2 nach XORmaskIcon übertragen
				for (int c = 0; c < cmax ; c++) 
				{
					// Formel für den Farboffset für Pixel i * x
					int offset = c + x*cmax + i*aufloesung; 
					if ((DWORD)(offset) <= bmpsize)
						XORmaskIcon[offset] = XORmaskIcon2[offset];
					else
						XASSERT(FALSE);
				}
			}
			mask = mask >> 1;
		}
	}

	HICON ret = CreateIcon(AfxGetInstanceHandle(),    // application instance  
             32,              // icon width 
             32,              // icon height 
             1, // number of XOR planes 
             (BYTE)bmpXOR.bmBitsPixel, // number of bits per pixel 
			 ANDmaskIcon,     // AND bitmask  
			  XORmaskIcon);    // XOR bitmask  
	delete XORmaskIcon;
	delete XORmaskIcon2;
	return ret;
}

///Prüft, ob das Objekt gezeigt werden muss. Das ist der Fall, wenn nicht freigegeben wurde.
bool WaxPart::initVisible() 
{
	if (get_deleted())
		return false;
	bool ret = false;
	Werkauftrag *werk = get_werkauftrag();
	if (werk)
	{
		int show = WaxSys::para_int(SHOWFREIGABEPOS);
		if (!show)
			return false;
		
		// Indikator ist: Freigabedatum auf 0 und 'Keine Projektleitkarte und keine Gliederung'
		// Drag and Drop wirkt z.Zt. nur bei Verteilern u.ä.
		if (werk->get_status().get_freigabedatum().m_dt == 0.0 
			&& !this->IsKindOf(RUNTIME_CLASS(PrjGliederung)) 
			&& !this->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData)))
			return true;
		else
		{
			for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
			{
				WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
				if (pt)
				{
					if (pt->initVisible())
						ret = true;
				}
			}
		}
	}
	return ret;
}

///Ist der Menuepunkt Delete zu disablen
bool WaxPart::initDelete()
{
	/**
	true  := nicht freigegeben -> Menuepunkt enablen
	false := freigegeben       -> Menuepunkt disablen
	*/

	if (get_deleted())
		return true;
	
	Werkauftrag *werk = get_werkauftrag();
	if (werk)
	{
		// Indikator ist: WAnr und !AV  und 'Keine Projektleitkarte und keine Gliederung'
		// Drag and Drop wirkt z.Zt. nur bei Verteilern u.ä.
		if (!this->IsKindOf(RUNTIME_CLASS(PrjGliederung)) 
	   	 && !this->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData))
		 && !this->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)))
		{
			if (!av &&	(werk->get_status().get_wanr() != 0 ||
						 werk->get_status().get_status() == Status::Angebot ||
						 werk->get_status().get_status() == Status::Preisanfrage ||
			             (werk->get_status().get_status() == Status::Freigegeben && werk->get_leitkarte().get_preisanfrage().get_nummer() > 0)))
				return false;
		}
		else
		{
			for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
			{
				WaxPart *pt = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
				if (pt)
				{
					return pt->initDelete();
				}
			}
		}
	}
	return true;
}

///Ist das Icon für nicht freigegeben zu modifizieren
bool WaxPart::is_freigegeben() const
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

	if (get_deleted())
		return true;
	
	const Werkauftrag *werk = get_werkauftrag_const();
	if (!werk || !werk->get_waid())
		return false;
	return true;
}



///Ist das Icon für nicht freigegeben zu modifizieren
Status::STATUS WaxPart::get_status() const
{
	if (!get_werkauftrag_const())
		return Status::NichtGesetzt;

	return get_werkauftrag_const()->get_status_const().get_status();
}


///Prüfung, ob die Leitkarte neu angezeigt werden soll
bool WaxPart::needDlgUpdate()
{
	if (needUpdate)
	{
		needUpdate = false;
		return true;
	}
	return false;
}

///Status des Werkauftrages vom Waxserver erfragen. Wird aufgerufen, wenn der WA freigegeben wurde
bool WaxPart::updateStatus()
{
	/*
	Der Zugriff auf den Server (Klasse Server) ist nur aus WaxPrj.dll möglich. 
	Deshalb gibt es diese Wrappermethode. 
	*/
	/*
	pr: Dauert zu lange, muss vorher einmalig geprüft werden
	if (!Server::checkServerDB().GetLength())
		return false;
	*/
	Server serv(Server::load, NULL, NULL);
	serv.set_werkauftrag(get_werkauftrag());
	if (!(serv.updateStatus() && serv.updateLeitkarte(this->GetRuntimeClass()->m_lpszClassName)))
		return false;


	if (get_werkauftrag()->get_status().get_status() == Status::Angebot)
	{
		if (!serv.updatePreisanfrage())
			return false;
	}


	return true;
}

void WaxPart::set_parent(WaxPart* value)
{
	parent = value;
}

const WaxPart* WaxPart::get_parent() const
{
	return parent;
}


void WaxPart::updateGrafic(CWnd *window)
{
	if (!dynamic_cast<Rohrleitung*>(get_werkauftrag()) && !dynamic_cast<ApaxWerkauftrag*>(get_werkauftrag()))
		return;

	// Graphik erzeugen
	createGraphic();

	if (!window) // keine Grafikausgabe, nur Daten aktualisieren
		return;

	// Workaround: Voloview setzt den Focus auf sich selbst, deshalb wird hier der aktuelle Focus zwischengespeichert
	// und nach dem Update wieder richtig gesetzt
	CWnd *tmp = window->GetFocus();
	if (!tmp)
		return;
	CView *sender = dynamic_cast<CView*>(window->GetParent());
	XASSERT(sender);	

	// window muss ein WaxPartForm sein, sonst ist was schiefgelaufen
	WaxPartForm *frm = dynamic_cast<WaxPartForm*>(window);
	XASSERT(frm);
	if (!frm)
		return;
	// Wichtig! Dieser Aufruf sorgt für ein Update des unteren Grafikfensters 
	((CWaxDoc*)frm->get_document())->UpdateAllViews(sender, (LPARAM)&get_grafic_file());
	tmp->SetFocus();	
}

bool WaxPart::synchronize(WaxPart* with)
{
	/**
	Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
	einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
	erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
	Objekte in subobjects auf.
	*/
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		if (next && !next->get_deleted())
		{
			XASSERT(next->get_parent() == this);
			next->synchronize(with);
		}
	}
	return true;
}


void WaxPart::get_changelist(vector<const WaxPart*>& lst, const MXDate& refdt) const
{
	/**
	Erstellt eine Liste mit geänderten Werkaufträgen.
	Dazu wird das Änderungsdatum des Werkauftrags und aller Unteraufträge mit dem 
	Referenzdatum refdt verglichen. Alle Aufträge die ein späteres Datum haben, werden 
	in lst zusammengefasst
	*/
	if (get_werkauftrag_const()->get_aenderungsdatum().is_newer(refdt))
		lst.push_back(this);

	for (POSITION pos = subobjects.GetHeadPosition(); pos;)
	{
		const WaxPart* sub = dynamic_cast<const WaxPart*>(subobjects.GetNext(pos));
		XASSERT(sub);
		if (!sub)
			return;
		sub->get_changelist(lst, refdt);
	}
}

///Ruft Werkauftrag::reset für alle Unterobjekte auf. Dies ist nötig, wenn z.B 
///eine Gliederung aus einer Datei eingefügt wird. Dann müssen die 
///Statusinformationen aller Unterobjekte zurückgesetzt werden.
void WaxPart::resetWa()
{
	/**
	Erst wird der eigene Werkauftrag resettet, dann alle Unterobjekte.
	Wrapper für rekursiven Aufruf von Werkauftrag::reset.
	*/
	get_werkauftrag()->reset();
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(next);
		if (next)
			next->resetWa();
	}
}

///Fügt die AV Reiter für die Material, Arbeit und Sonderkosteneingabe hinzu. 
///Diese Methode muss in abgeleiteten Klassen in getForms() aufgerufen werden.
void WaxPart::addAVTabs(CObList& runtimeModules) const
{
#ifndef __WAX_FUER_DRITTE__
	if (av)
	{
		runtimeModules.AddTail((CObject*)RUNTIME_CLASS(ArbeitEingabeForm));
		runtimeModules.AddTail((CObject*)RUNTIME_CLASS(MaterialEingabeForm));
		runtimeModules.AddTail((CObject*)RUNTIME_CLASS(SonderkostenEingabeForm));
	}
#endif
}

///Liefert den ersten nicht gelöschten Unterauftrag. Falls keiner vorhanden ist, wird NULL zurückgeliefert.
WaxPart* WaxPart::getFirstSubobject()
{
	WaxPart *ret = NULL;

	if (get_subobjects().GetCount())
	{
		for (POSITION pos = get_subobjects().GetHeadPosition(); pos;)
		{
			WaxPart *sub = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
			XASSERT(sub);
			if (sub->get_deleted())
				continue;
			return sub;
		}
	}
	return ret;
}

///Liefert den ersten nicht gelöschten Unterauftrag. Falls keiner vorhanden ist, wird NULL zurückgeliefert.
const WaxPart* const WaxPart::getFirstSubobject() const
{
	WaxPart* const ret = NULL;

	if (get_subobjects_const().GetCount())
	{
		for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos;)
		{
			const WaxPart* const sub = dynamic_cast<const WaxPart* const>(get_subobjects_const().GetNext(pos));
			XASSERT(sub);
			if (sub->get_deleted())
				continue;
			return sub;
		}
	}
	return ret;
}

///Liefert den letzten nicht gelöschten Unterauftrag. Falls keiner vorhanden ist, wird NULL zurückgeliefert.
WaxPart* WaxPart::getLastSubobject()
{
	WaxPart *ret = NULL;

	if (get_subobjects().GetCount())
	{
		for (POSITION pos = get_subobjects().GetTailPosition(); pos;)
		{
			WaxPart *sub = dynamic_cast<WaxPart*>(get_subobjects().GetPrev(pos));
			XASSERT(sub);
			if (sub->get_deleted())
				continue;
			return sub;
		}
	}
	return ret;
}

///Prüft, ob dieser Auftrag der erste Auftrag eines Sammelauftrages (parent) ist
bool WaxPart::isFirstSubOfSammelauftrag()
{
	if (!parent && !parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		return false;
	return parent->getFirstSubobject() == this;
}

///Führt die Synchronisation für alle Unterobjekte durch
bool WaxPart::doSync(bool checkFreigabe)
{
	if (checkFreigabe && is_freigegeben())
		return false;

	XASSERT(get_parent());
	WaxPart *parent = const_cast<WaxPart*>(get_parent());
	if (!parent || !get_parent()->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		return false;

	CObList& sub = parent->get_subobjects();
	POSITION pos = sub.GetHeadPosition();
	WaxPart *p = dynamic_cast<WaxPart*>(sub.GetAt(pos));
	while (p->get_deleted())
	{
		if (pos == NULL)
			break;
		p = dynamic_cast<WaxPart*>(sub.GetNext(pos));
	}
	return synchronize(p);
}

void WaxPart::saveToFile(CFile* f)
{
	/** 
	Speichern eines WaxPart in einer Datei, ohne dass der FileOpen Dialog aufgerufen wird.
	Datei kann auch ein (namenloses) Memfile sein. FileLoader erwartet zum Speichern allerdings
	einen Dateinamen, so dass erstmal ein temporärer Name benutzt wird.
	*/
	string filename;
	if (!f)
	{	
		Filter flt(MAGIC);
		flt.set_dir(WaxSys::para_cstr(KOMP));
		flt.set_extension("wtp");
		flt.set_mask("Wax+ Teilprojekte (*.wtp)|*.wtp||");
		OpenFileDlg ofn(flt);
		filename = ofn.get_file(true);
		if (!filename.length())
			return;

	}
	else
		filename = WaxSys::GetTempFilename("wtp");

	FileLoader ldr(MAGIC);
	
	int err = ldr.open_file(filename, FileLoader::STORE);

	if (err == FileLoader::ABORTED)
		return; // Userabbruch

	if (err) // Anderer Fehler
	{
		MsgBox::show_error(ldr.get_error_text(err));
		return;
	}
		
	BinaryArchive* ar = ldr.get_arch(FileLoader::STORE);
	*ar << FILETOC;
	ar->write_class(GetRuntimeClass());
	serialize(*ar);
	*ar << 0;
	ldr.store_memfile();
	if (f)
	{
		// Inhalt des Fileloader Memfiles in das übergebene File speichern
		unsigned int size = 0;
		unsigned char *mem = ldr.detach_mem(size);
		f->Write(mem, size);
		ldr.attach_mem(size, mem);
	}
	else
	{ // In ausgewählte Datei speichern
		ldr.store(false, false);
	}
}

///Lädt ein WaxPart aus einer Datei
WaxPart *WaxPart::loadFromFile(HANDLE mem)
{
	/**
	Laden aus Datei: Liefert ein WaxPart Objekt aus der ausgewählten Datei bzw. dem globalen Speicher.
	Im Fehlerfall wird NULL zurückgeliefert.
	*/
	Filter flt(MAGIC);
	flt.set_extension("wtp");
	flt.set_dir(WaxSys::para_cstr(KOMP));
	flt.set_mask("Wax+ Teilprojekte (*.wtp)|*.wtp||");
	
	

	FileLoader ldr(MAGIC);
	WaxPart* pt = NULL;
	if (!mem)
	{
		OpenFileDlg ofn(flt);

		string fn = ofn.get_file();
		if (!fn.length())
			return NULL;
		int err = ldr.open_file(fn, FileLoader::LOAD | FileLoader::TRY_READONLY);
		if (err == FileLoader::ABORTED)
			return NULL; // Userabbruch

		if (!err)
			err = ldr.load();

		if (err)
		{
			MsgBox::show_error(ldr.get_error_text(err));
			return NULL;
		}
	}
	else
	{
		/** 
		Laden aus Zwischenablage: Globaler Speicher wird kopiert. 
		Der allokierte Speicher wird von FileLoader
		automatisch wieder freigegeben.
		*/
		unsigned char *m = (unsigned char*)::GlobalLock(mem);
		unsigned int len = ::GlobalSize(mem);
		
		if (!len)
			return NULL;
		unsigned char *newmem = new unsigned char[len];
		memcpy(newmem, m, len);
		ldr.attach_mem(len, newmem);
	}

	bool hascrc;
	if (!ldr.check_format(hascrc))
		MsgBox::show_error(ldr.get_error_text(FileLoader::FORMAT));
			
	BinaryArchive *ar = ldr.get_arch(FileLoader::LOAD);

	TRY
	{
		int marker;
		*ar >> marker;
		// FILETOC zeigt eine Komponente an. Der Magicstring ist für Komponenten und 
		// Projekte gleich. Ein Laden eines Projektes würde hier wahrscheinlich zum 
		// Absturz führen.
#ifdef _DEBUG
		XASSERT(marker == FILETOC); // Brauchts in der Releaseversion nicht
#endif
		if (marker != FILETOC)
			AfxThrowNotSupportedException();

		int objschema = ar->get_version(); // muss zwischengespeichert werden, da der Wert durch ReadClass
		// zurückgesetzt wird
		CRuntimeClass *waxclass = ar->read_class();
		ar->set_version(objschema);

		XASSERT(waxclass);
		if (!waxclass)
			return NULL;
		pt = dynamic_cast<WaxPart*>(waxclass->CreateObject());
		XASSERT(pt);
		if (!pt)
			return NULL;
		pt->serialize(*ar);
		*ar >> marker;
		XASSERT(!marker);
		
		// Statusinfos der Werkaufträge zurücksetzen
		pt->resetWa();
	}
	CATCH(CNotSupportedException, x)
	{
		// Generischer Fehler: Filetoc nicht vorhanden oder Ausstieg an anderer Stelle
		// wird als Unbekanntes Dateiformat gemeldet, keine weitere Info nötig.
		MsgBox::show_error(ldr.get_error_text(FileLoader::FORMAT));
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL
	return pt;
}

bool WaxPart::get_plausiOk(CString *msg)
{
	if (!get_werkauftrag()->get_plausiOk())
	{
		if (msg)
		{
			CString str; str = getStammdaten().getText("IDS_POSNR_TEXT").c_str();
			msg->Format(str, get_werkauftrag()->get_posNr());
		}
		return false;
	}
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *sub = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(sub);
		if (!sub->get_deleted() && !sub->get_plausiOk(msg))
			return false;
	}
	return true;
}

///Prüft die Eingaben der Leitkarte auf Vollständigkeit
bool WaxPart::testLeitkarte(bool isPreisanfrage)
{
	/**
	In Auslandsgesellschaften gibt es u.U. kein SAP, deshalb muss dort auch nicht alles eingetragen 
	werden.
	*/
	bool checksap = WaxSys::para_int(CHECKSAP) ? true : false;

	CString fehlendedaten = get_werkauftrag()->get_leitkarte().checkVars(checksap, !isPreisanfrage);
	if (fehlendedaten.GetLength())
	{
		CString str; str = getStammdaten().getText("IDS_FEHLENDE_DATEN1b").c_str();
		fehlendedaten = str + fehlendedaten;
	}

	if (checksap && !fehlendedaten.GetLength() && !get_werkauftrag()->checkSDMatNr())
	{
		CString str; str = getStammdaten().getText("IDS_FEHLENDE_DATEN2b").c_str();
		fehlendedaten = str;
	}

	CString msg;
	
	if (!fehlendedaten.GetLength() && !get_plausiOk(&msg) && !av)
	{
		CString str; str = getStammdaten().getText("IDS_PLAUSIS_FEHLGESCHLAGEN").c_str();
		fehlendedaten.Format(str, msg);
	}

	if (fehlendedaten.GetLength())
	{
		CString msg, str; str = getStammdaten().getText("IDS_AUFTRAG_TEXT").c_str();
		msg.Format(str, get_werkauftrag()->get_bezeichnung(), fehlendedaten);
		AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
		// Die AV Version kann auch WAs mit falschen Angaben freigeben. z.B. wenn eine 
		// nicht passende SD Materialnummer aus SAP eingelesen wurde.
		return av ? true : false;
	}
	return true;
}

void WaxPart::initIcon(CTreeCtrl* tree, IconManager &iconman)
{
	/**
	Initialisiert das Icon im Iconmanager und fügt es der ImageList des Baumes hinzu. 
	Der Iconmanager speichert den Index des Icons. Jedes Icon wird nur einmal in die 
	Imagelist eingetragen. Ausserdem wird gleich nach dem Icon noch das modifiziert Icon erstellt
	und ebenfalls in die Imagelist eingetragen. Hier wird vorausgesetzt, das das modifizierte
	Icon einen Index+1 hat. 
	*/
	CImageList *imageList = tree->GetImageList(TVSIL_NORMAL);
	XASSERT(imageList != NULL);
	XASSERT(icon != 0);

	if (imageList && icon)
	{
		// Icon nur dann in Imageliste einfügen, wenn noch kein Icon geladen wurde
		if (iconman.get_index(icon) == -1)
		{
			HICON *ike = iconman.getObject(icon);
			
			int index1 = imageList->Add(*ike);
			int index2 = imageList->Add(modify_icon(*ike, IDI_ICON_NICHT_FREIGEGEBEN));

			HICON cad = modify_icon(*ike, IDI_ACAD);
			imageList->Add(cad);
			imageList->Add(modify_icon(cad, IDI_ICON_NICHT_FREIGEGEBEN));

			// Aufeinanderfolgende Indizes werden vorausgesetzt
			XASSERT(index2 - index1 == 1); 
			iconman.set_index(icon, index1);					
		}
	}
}

int WaxPart::get_iconindex(IconManager& iconman) const
{
	/**
	Index des Icons. Bei nicht freigegebenen Was ist dies der Index+1. An dieser Stelle
	befindet sich das modifizierte Icon.
	*/
	int index = iconman.get_index(icon);
	if (!get_werkauftrag_const() || index < 0)
		return 0;

	if (get_werkauftrag_const()->get_cadpos())
			index += 2;

	if (!is_freigegeben() || 
		get_status() == Status::Angebot || 
		get_werkauftrag_const()->geaendert_nach_freigabe())
		index++;
			
	return index;
}




bool WaxPart::enable_delete()
{
	/**
	Liefert true, wenn der Werkauftrag gelöscht werden kann. Bei freigegebenen Aufträgen wird der Status
	aktualisiert
	*/
	if (get_werkauftrag()->get_waid() == 0)
		return true; // Nicht freigegebene Aufträge sind Ok fürs Löschen
	
	// falls User den Status nicht aktualisiert, das jetzt tun, 
	// sonst kann Position geloescht werden, die schon in Bearbeitung ist 
	if (get_werkauftrag()->get_status().get_wanr() == 0)
	{
		Server serv(Server::load, NULL, NULL);
		serv.set_werkauftrag(get_werkauftrag());
		if (!serv.updateStatus())
		{
			AfxMessageBox(getStammdaten().getText("IDS_LOESCHEN_KEINE_DATENBANK").c_str(), MB_OK | MB_ICONINFORMATION);
			return false;
		}
		
		if (get_werkauftrag()->get_status().get_wanr() != 0)
		{
			AfxMessageBox(getStammdaten().getText("IDS_LOESCHEN_IN_BEARBEITUNG").c_str(), MB_OK | MB_ICONINFORMATION);
			return false;
		}
	}
	// Bereits freigegebene Aufträge müssen einzeln bestätigt werden. Wg. Hr. Korinek Supp.Nr. 20162...
	CString msg = getBezeichnung() + "\r\n";
	CString question;
	question = getStammdaten().getText("IDS_LOESCHEN_FREIGEGEBENER_AUFTRAEGE_BESTAETIGEN").c_str();
	if (AfxMessageBox(msg + question, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2 ) != IDOK)
		return false;

	return true;
}

void WaxPart::setDeleted()
{
	// in DB stornieren	
	if (get_werkauftrag()->get_waid() != 0)
	{
		Server serv(Server::save, get_werkauftrag());
		get_werkauftrag()->get_status().set_storniert(true);
		serv.updateStatus();
	}
	deleted = true;
}

bool WaxPart::subobj_allowed() const
{
	/**
	Default sind keine Unterobjekte 'erlaubt'. Diese Methode wird aufgerufen, wenn eine 
	Position in den Baum eingefügt wird. 
	*/
	return false;
}

bool WaxPart::checkFreigabe()
{
	/**
	Aktionen und Tests ausführen vor der Freigabe.
	Rückgabe von false führt zum Abbruch.
	*/
	WaxPart *parent = const_cast<WaxPart*>(get_parent());

	XASSERT(parent);
	if (!parent)
		return false;
	
	if (!testLeitkarte(false))
		return false;

	return true;
}

int WaxPart::get_subobject_count() const
{
	/**
	Liste mit Unterobjekten wird rekursiv aufgerufen. Unterobjekte, die gelöscht sind
	werden nicht mitgerechnet.
	*/
	const WaxPart *tmp = NULL;
	int count = 0;
#ifdef _DEBUG
	int delcount = 0;
#endif
	for (POSITION pos = this->subobjects.GetHeadPosition(); pos; )
	{
		const CObject *next = subobjects.GetNext(pos);
		tmp = dynamic_cast<const WaxPart*>(next);
		if (tmp)
		{
			if (!tmp->get_deleted())
			{
				count += tmp->get_subobject_count() + 1;
			}
#ifdef _DEBUG
			else
			{
				delcount++; // Nur für Debuggingzwecke
			}
#endif
		}
	}
	return count;
}

bool WaxPart::sollUpdateStatus()
{
	/** 
	Bei Sammelauftrag und Einzelauftrag wird Statusaktualisierung abgefragt, 
	wenn der Auftrag freigegeben ist aber noch keine WAnr hat und nicht heute freigegeben wurde.
	Und das ganze nur einmal (status_update_test = true und wenn nicht AV Version
	*/
	if (!status_update_test || av)
		return false;

	if (WaxSys::para_int(CHECKSTATUS) == 0)
		return false;

	status_update_test = false;
	if (is_freigegeben() && !get_werkauftrag()->get_status().get_wanr())
		return get_werkauftrag()->get_status().isFreigabedatumOld();
	return false;
}

void WaxPart::set_status_update_test(bool value)
{
	status_update_test = value;
}

void WaxPart::createAutoArbeit()
{

	get_werkauftrag()->createAutoArbeit();
	get_werkauftrag()->set_UseSammelauftrag(false);
}

void WaxPart::createAutoSoko(bool deleteFixedSoko)
{
	get_werkauftrag()->get_preis().calcAutoSoko(deleteFixedSoko);
}

void WaxPart::vererbe_leitkarte()
{
	/**
	Vererben aller Leitkartendaten (ausser waid) an alle Unteraufträge
	*/
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart* next = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(next);
		if (!next || next->get_deleted())
			continue;
		
		next->get_werkauftrag()->get_leitkarte().clone(get_werkauftrag()->get_leitkarte());
		next->get_werkauftrag()->get_status().copy(get_werkauftrag()->get_status());
		next->get_werkauftrag()->set_beschichtung(get_werkauftrag()->get_beschichtung());
		next->get_werkauftrag()->set_bemerkung(get_werkauftrag()->get_bemerkung());
		next->get_werkauftrag()->set_sichtbereich(get_werkauftrag()->get_sichtbereich());
		next->get_werkauftrag()->set_zeugnis(get_werkauftrag()->get_zeugnis());
		next->get_werkauftrag()->set_KollilisteExport(get_werkauftrag()->get_KollilisteExport());
	}
}

bool WaxPart::update_enabled() const
{
	return enable_update;
}

void WaxPart::set_enable_update(bool enable)
{
	enable_update = enable;
}


bool WaxPart::check2rohrquali()
{
	/**
	Prüfung auf verschiedene Rohrqualitäten in der Auftragsliste.
	Wenn ein einzelnes Rohr bereits zwei Rohrqualis hat, brauchen wird gar nicht weiter
	zu prüfen. Ansonsten wird für die DN überprüft, da über DN65 eine andere Rohrqualität gilt,
	die allerdings dieselbe ID hat.	
	*/
	bool dn50 = false, dn65 = false;
	int rohrquali = 0;
	vector<const Rohrleitung*> rlist;
	if (const Rohrleitung* r = dynamic_cast<const Rohrleitung*>(get_werkauftrag()))
		rlist.push_back(r);

	for (POSITION pos = get_subobjects().GetHeadPosition(); pos != NULL;) 
	{

		const Rohrleitung *r = dynamic_cast<const Rohrleitung*>(((WaxPart*)get_subobjects().GetNext(pos))->get_werkauftrag());
		if (r)
			rlist.push_back(r);
	}
	int i = 0;
	for (vector<const Rohrleitung*>::iterator r = rlist.begin(); r != rlist.end(); r++)
	{
		if ((*r)->hasRohr2Quali())
			return true;

		// Die Rohrqualitäten der Werkaufträge können sich unterscheiden, ohne dass die Werkaufträge 
		// selber verschiedene Rohrqualitäten haben
		rohrquali += (*r)->get_rohrQuali(); 
		if (rohrquali / ++i != (*r)->get_rohrQuali())
			return true;

		// Ausnahme ist Standardrohr
		// Bei Standardrohr wird bei DN65 automatisch eine andere Rohrqualität angezogen
		if ((*r)->get_rohrQuali() == Rohrleitung::RQ2440_2458GSCH)
		{
			if ((*r)->get_dnvl() <= 50)
				dn50 = true;
			else
				dn65 = true;
			
			if (dn50 && dn65)
				return true;
		}
	}
	return false;
}


CMenu *WaxPart::getPopUpMenu()
{
	if (popUpMenu == NULL)
	{
		popUpMenu = new CMenu;
		popUpMenu->CreatePopupMenu();
	}

	return popUpMenu;
}


void WaxPart::appendMenuItems(CMenu *menu, menu_items items[])
{
	for (int i = 0; items[i].id != -1; i++) 
	{
		if (items[i].id != ID_SEPARATOR) 
		{
			menu->AppendMenu(MF_STRING, items[i].id, getStammdaten().getText(items[i].name).c_str());
		} else 
		{
			menu->AppendMenu(MF_SEPARATOR);
		}
	} 
}


void WaxPart::appendStandardMenuItems(CMenu *menu)
{
#ifdef __WAX_FUER_DRITTE__ 	
	menu->RemoveMenu(WAXID_NEU_APAX, MF_BYCOMMAND);
#endif
	
	menu_items items[] =
	{
#ifndef __WAX_FUER_DRITTE__ 	
		WAXID_UPDATESTATUS,		"IDS_UPDATESTATUS",
#endif
		WAXID_UMBENENNEN,		"IDS_UMBENENNEN",
		ID_SEPARATOR,			"",
		WAXID_CUT,				"IDS_MENU_CUT",
		WAXID_COPY,				"IDS_MENU_COPY",
		WAXID_PASTE,			"IDS_MENU_PASTE",
		WAXID_DELETE,			"IDS_MENU_DELETE",
		ID_SEPARATOR,			"",
		WAXID_COPY_TOFILE,		"IDS_MENU_COPY_TOFILE",
		WAXID_PASTE_FROMFILE,	"IDS_MENU_PASTE_FROMFILE",
		-1, "",
	};

	appendMenuItems(menu, items);
}


CString WaxPart::getStatusText()const
{
	CString txt;

	switch (get_werkauftrag_const()->get_status_const().get_status())
	{
	case Status::Positioniert:
		txt = getStammdaten().getText("STATUS_Positioniert").c_str();
		break;
	case Status::Freigegeben:
		txt = getStammdaten().getText("STATUS_Freigegeben").c_str();
		break;
	case Status::Preisanfrage:
		txt = getStammdaten().getText("STATUS_Preisanfrage").c_str();
		break;
	case Status::Angebot:
		txt = getStammdaten().getText("STATUS_Angebot").c_str();
		break;
	case Status::InBearbeitungAv:
		txt = getStammdaten().getText("STATUS_InBearbeitungAv").c_str();
		break;
	case Status::FreigegebenAv:
		txt = getStammdaten().getText("STATUS_FreigegebenAv").c_str();
		break;
	case Status::Storniert:
		txt = getStammdaten().getText("STATUS_Storniert").c_str();
		break;
	case Status::PreisanfrageStorniert:
		txt = getStammdaten().getText("STATUS_PreisanfrageStorniert").c_str();
		break;
	case Status::AngebotAbgelehnt:
		txt = getStammdaten().getText("STATUS_AngebotAbgelehnt").c_str();
		break;
	case Status::NichtGesetzt:
		txt = "";
		break;
	default:
		txt = getStammdaten().getText("STATUS_Unbekannt").c_str();
		break;
	}

	return txt;
}