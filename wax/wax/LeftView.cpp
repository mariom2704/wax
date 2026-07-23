#include "stdafx.h"
#include "resource.h"


#include "..\waxprj\WpPreisanfrage.h"
#include "..\waxprj\sammelauftrag.h"

#include "waxDoc.h"
#include "MainFrm.h"
#include "JScriptViewTemplate.h"
#include "WaxHtmlView.h"
#include "TabCtrlView.h"
#include "ChildFrm.h"
#include "..\WAXPRJ\GpVk3Preis.h"
#include "SammelAuftragViaList.h"
		#include "LeftView.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INDENT_SMALL_ICONS 10


IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_INITMENUPOPUP()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(WAXID_COPY, OnCopy)
	ON_UPDATE_COMMAND_UI(WAXID_COPY, OnUpdateCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCopy)
	ON_COMMAND(WAXID_PASTE, OnPaste)
	ON_UPDATE_COMMAND_UI(WAXID_PASTE, OnUpdatePaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdatePaste)
	ON_COMMAND(WAXID_COPY_TOFILE, OnCopyToFile)
	ON_UPDATE_COMMAND_UI(WAXID_COPY_TOFILE, OnUpdateCopyToFile)
	ON_COMMAND(WAXID_PASTE_FROMFILE, OnPasteFromFile)
	ON_UPDATE_COMMAND_UI(WAXID_PASTE_FROMFILE, OnUpdatePasteFromFile)
	ON_COMMAND(WAXID_CUT, OnCut)
	ON_UPDATE_COMMAND_UI(WAXID_CUT, OnUpdateCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateCut)
	ON_COMMAND(WAXID_DELETE, OnDelete)
	ON_UPDATE_COMMAND_UI(WAXID_DELETE, OnUpdateDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateDelete)
	//ON_UPDATE_COMMAND_UI(WAXID_NEU_SAMMEL_AUFTRAEGE_VIA_LIST, onComandSammelTaskViaList)
	ON_COMMAND_EX(ID_NEXT_PANE, OnNextPaneCmd)
	ON_COMMAND_EX(ID_PREV_PANE, OnNextPaneCmd)
	ON_COMMAND(WAXID_NEUEUNTERGLIEDERUNG_ROOT,	onNeueUntergliederungRoot)  
	ON_COMMAND(WAXID_NEUEUNTERGLIEDERUNG,		onNeueUntergliederung)
	ON_COMMAND(WAXID_NEUER_SAMMELAUFTRAG,		onNeuerSammelauftrag)
	ON_COMMAND(WAXID_NEUE_PREISANFRAGE,		    onNeuePreisanfrage) 
	ON_COMMAND(WAXID_NEU_STRANGROHR,			onNeuesStrangrohr) 
	ON_COMMAND(WAXID_NEU_GEWINDEROHR,			onGewinderohr)
	ON_COMMAND(WAXID_NEU_VERTEILER,				onNeuerVerteiler) 
	ON_COMMAND(WAXID_NEU_STATIONSVERTEILER,		onStationsVerteiler)
	ON_COMMAND(WAXID_NEU_CO2VERTEILER,			onCOIIVerteiler)
	ON_COMMAND(WAXID_NEU_MX1230MX200VERTEILER,	onMX1230MX200Verteiler)
	ON_COMMAND(WAXID_NEU_SAMMEL_AUFTRAEGE_VIA_LIST, onSammelTaskViaList)
	ON_COMMAND(WAXID_NEU_FORMSTUECKEUNDFITTINGE,onNeuFormstueckeundfittinge)
	ON_COMMAND(WAXID_NEUE_ENTNAHMELEITUNG,		onNeueEntnahmeleitung)
	ON_COMMAND(WAXID_NEUE_PUMPENPROBIERLEITUNG, onNeuePumpenprobierleitung)
	ON_COMMAND(WAXID_NEUER_PUMPENVERTEILER,		onNeuerPumpenverteiler)
	ON_COMMAND(WAXID_NEU_HANDELSLAENGE,			onNeuHandelslaenge)
	ON_COMMAND(WAXID_NEU_APAX,					onNeuApax)
	ON_COMMAND(WAXID_UMBENENNEN, onEditlabel)   
	ON_COMMAND(WAXID_UPDATESTATUS, onUpdatestatus)
	ON_COMMAND(ID_VOLOVIEW, onVoloview)
	ON_COMMAND(ID_EDIT_COPY, onEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, onEditPaste)
	ON_COMMAND(ID_EDIT_CUT, onEditCut)
	ON_COMMAND(ID_EDIT_DELETE, onEditDelete)
	ON_COMMAND(ID_NEWELEMENT, onNewelement)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	// Standard-Befehle zum Drucken
	ON_COMMAND(WAXID_DOTREEUPDATE, OnUpdateTree)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	//ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CLeftView::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CLeftView::OnTvnItemexpanded)
END_MESSAGE_MAP()


//Geschützter Konstruktor
CLeftView::CLeftView() : labelEdit(false), nextSelect(NULL)
{
	isDragging = false;
}

//Geschützter Destruktor
CLeftView::~CLeftView()
{
}

void CLeftView::OnInitialUpdate()
{
	/**
	Fenster initialisieren
	Diese Methode wird von der MFC einmalig nach der Erstellung des Fensters aufgerufen.
	Hier sind alle One-Time Initialisierungen zu finden. 

	  1. Erstellen der Imageliste für grosse und kleine Icons

	  2. Setzen der Hintergrundfarbe des Baumes und der Imagelist

	  3. Setzen des Iconhandles der Objekte

	 */
#ifdef _DEBUG
	COLORREF background = AfxGetApp()->GetProfileInt("Appearance", "Treecolor", RGB(249, 249, 255));

#else	
	COLORREF background = AfxGetApp()->GetProfileInt("Appearance", "Treecolor", RGB(68, 120, 175));
#endif

	CTreeView::OnInitialUpdate();
	GetTreeCtrl().ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT);
	
	imageList.Create( 32, 32, ILC_COLOR32, 0, 1); 
	imageList.SetBkColor(background);
	
	GetTreeCtrl().SetImageList(&imageList, TVSIL_NORMAL);
	GetTreeCtrl().SetIndent(INDENT_SMALL_ICONS);
	GetTreeCtrl().SetBkColor(background);
	GetTreeCtrl().SetInsertMarkColor(background);

	// Jetzt wird der Baum initialisiert und alle Root-Objekte in der Reihenfolge von m_Objects angezeigt.
	for (int i = 0; i < ((CWaxDoc*)m_pDocument)->get_objects().GetSize(); i++)
	{
		WaxPart *part = (WaxPart*)(((CWaxDoc*)m_pDocument)->get_objects().GetAt(i));
		if (part->get_deleted())
			continue;
		part->Show(GetTreeCtrl(),TVI_ROOT, TVI_LAST, iconman);
	}

	m_pMainSplitterFrame->SetCentralDoc((CWaxDoc*)m_pDocument);
}


//Rechte Maustaste wurde gedrückt
void CLeftView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	/***
	Hier wird das Kontextmenü angezeigt
	*/
	UNREFERENCED_PARAMETER(nFlags);
	UINT unusedflag;
	HTREEITEM sel = GetTreeCtrl().HitTest(point, &unusedflag);
	if (!sel)
		return;
	GetTreeCtrl().SelectItem(sel);
	OnContextMenu(this, point);
}



//Kopieren von Positionen
void CLeftView::OnCopy()
{
	/**
	 Neues Element erstellen
	 Bearbeitungsmethode der Message WAXID_COPY
	*/
	CopyToClipboard(GetSelection());
}

//Menüpunkt "Kopieren" an/abblenden
void CLeftView::OnUpdateCopy(CCmdUI* pCmdUI) 
{
	/***
	 Menüpunkt "Kopieren" ab/anschalten. Die Projektleitkarte kann nicht kopiert werden.
	 Der Pointer dat wird auf irgendeinen Wert gesetzt. Falls kein Item selektiert ist, 
	 wird auch der Copy Befehl disabled
	*/
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();

	PrjLeitkarteData *dat = (PrjLeitkarteData *)0xfffff;
	if (sel)
		dat = dynamic_cast<PrjLeitkarteData*>((CObject*)GetTreeCtrl().GetItemData(sel));
	pCmdUI->Enable(dat ? false : true);
}


//Kopieren von Positionen
void CLeftView::OnCopyToFile()
{
	/**
	 Position in Datei einfügen
	 Bearbeitungsmethode der Message WAXID_COPYTOFILE
	*/
	WaxPart* pt = GetSelection();
	XASSERT(pt);

	if (pt)
		pt->saveToFile();
}

//Menüpunkt "Kopieren" an/abblenden
void CLeftView::OnUpdateCopyToFile(CCmdUI* pCmdUI) 
{
	/**
	 Menüpunkt "In Datei kopieren" ab/anschalten
	 Dieser Menüpukt wird nie abgeschaltet werden, ausser im Fehlerfall (irregeleitetes 
	Objekt im Baum)
	*/
	pCmdUI->Enable(FALSE);//Erst mal auf False setzen, in GetSelection könnte eine Exception
	// auftreten, dann würde Enable() nicht mehr aufgerufen
	pCmdUI->Enable(GetSelection() ? TRUE : FALSE);
}

//Einfügen aus Datei
void CLeftView::OnPasteFromFile()
{
	/**
	 Position einfügen aus Datei
	*/
	WaxPart *pt = WaxPart::loadFromFile();
	if (!pt)
		return;
	HTREEITEM newit = doCopy(NULL, GetTreeCtrl().GetSelectedItem(), FALSE, pt);
	if (newit)
		GetTreeCtrl().SelectItem(newit);
	m_pDocument->SetModifiedFlag();
}

//Menüpunkt "OnPasteFromFile" an/abblenden
void CLeftView::OnUpdatePasteFromFile(CCmdUI* pCmdUI) 
{
	UNREFERENCED_PARAMETER(pCmdUI);
	/**
	 Menüpunkt "Position aus Datei einfügen" ab/anschalten muss noch implementiert werden
	*/
}

//Einfügen aus Zwischenablage
void CLeftView::OnPaste()
{
	/**
	Position einfügen
	*/
	WaxPart *pt = NULL;
	TRY
	{
		UINT format = ::RegisterClipboardFormat("WAXPART");
		XASSERT(format);
		if (!format)
			return;
		
		::OpenClipboard(this->m_hWnd);
		HANDLE hdata = ::GetClipboardData(format);
		if (hdata)
		{
			pt = WaxPart::loadFromFile(hdata);
			HTREEITEM newit = NULL;
			if (pt)
				newit = doCopy(NULL, GetTreeCtrl().GetSelectedItem(), FALSE, pt);
			if (newit)
				GetTreeCtrl().SelectItem(newit);
		}
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL
	::CloseClipboard();
	m_pDocument->SetModifiedFlag();
}

//Menüpunkt "OnPaste" an/abblenden
void CLeftView::OnUpdatePaste(CCmdUI* pCmdUI) 
{
	/**
	 Menüpunkt "Position einfügen" ab/anschalten
	*/
	UINT format = ::RegisterClipboardFormat("WAXPART");
	pCmdUI->Enable(::IsClipboardFormatAvailable(format)) ;
}


//Position ausschneiden
void CLeftView::OnCut()
{
	/**
	 Position ausschneiden
	*/
	OnCopy();
	OnDelete();
}

//Menüpunkt "Ausschneiden" an/abblenden
void CLeftView::OnUpdateCut(CCmdUI* pCmdUI) 
{
	/**
	 Menüpunkt "Position ausschneiden" ab/anschalten
	 Cut ist "Copy & Delete", deshalb wird hier OnUpdateDelete> aufgerufen.
	*/
	OnUpdateDelete(pCmdUI);
}


/*
void CLeftView::onComandSammelTaskViaList(CCmdUI* pCmdUI)
{

	if (WaxSys::isAV())
	{
		HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
		WaxPart *pt = (WaxPart*)GetTreeCtrl().GetItemData(sel);
			
		if (pt->IsKindOf(RUNTIME_CLASS(PrjGliederung)))
		{		
			WaxPart * p = dynamic_cast<WaxPart*>(pt->getFirstSubobject());
			p = dynamic_cast<WaxPart*>(p->getFirstSubobject());
			Verteiler* pW = dynamic_cast<Verteiler*>(p->get_werkauftrag());
			if(pW > NULL)
			{
				pCmdUI->Enable(TRUE);		
			}
			else
			{	
				pCmdUI->Enable(FALSE);		
			}
		}
	}

}
*/



//Position löschen
void CLeftView::OnDelete()
{
	/**
	 Position löschen
	*/
	if (MsgBox::show(getStammdaten().getText("IDS_FRAGE1a"), MsgBox::Question, MsgBox::YesNo) != MsgBox::RYes)
		return;

	HTREEITEM next = NULL, sel= NULL;

	sel = GetTreeCtrl().GetSelectedItem();
	XASSERT(sel);
	if (!sel)
		return;
	
	WaxPart *del = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	XASSERT(del);
	if (!del)
		return;

	next = GetTreeCtrl().GetNextVisibleItem(sel);
	if (del->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))	
	{
		next = GetTreeCtrl().GetParentItem(sel);
	}
	else
	{
		next = GetTreeCtrl().GetNextVisibleItem(sel);
		if(!next)
		{
			next = GetTreeCtrl().GetPrevVisibleItem(sel);
		}
		if(!next)
		{
			next = GetTreeCtrl().GetParentItem(sel);
		}
	}
	
	if (del->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))	
	{
		for (POSITION pos = del->get_subobjects().GetHeadPosition(); pos;)
		{
			WaxPart *wp = NULL;
			wp = dynamic_cast<WaxPart*> (del->get_subobjects().GetNext(pos));
			if (wp)
			{
				if (!wp->enable_delete())
					return;
				wp->setDeleted();
			}
		}
	}
	
	if (!del->IsKindOf(RUNTIME_CLASS(SammelAuftrag)) && !del->IsKindOf(RUNTIME_CLASS(PrjGliederung)) && !del->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)) )	
	{
		if (!del->enable_delete())
			return;
		del->setDeleted();
	}
	else
		del->set_deleted(true);


	GetTreeCtrl().Select(next, TVGN_CARET);
	GetTreeCtrl().DeleteItem(sel);
	m_pDocument->SetModifiedFlag();
}

//Menüpunkt "Löschen" an/abblenden
void CLeftView::OnUpdateDelete(CCmdUI* pCmdUI) 
{
	/**
	Menüpunkt "Position löschen" ab/anschalten
	Bedingung fürs Löschen ist, dass eine gültige Position ausgewählt wurde. 
	Die Projektleitkarte und die erste Gliederungsposition können nicht gelöscht werden.
	*/
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
	HTREEITEM par = GetTreeCtrl().GetParentItem(sel);
	
	BOOL delete_enabled = FALSE;

	bool gliederung = false;
	if (sel && !par && !labelEdit)
	{
		WaxPart *pt = (WaxPart*)GetTreeCtrl().GetItemData(sel);
		if (pt->IsKindOf(RUNTIME_CLASS(PrjGliederung)))
		{
			// Bei DB-Abfrage gibs keine Projektleitkarte.
			HTREEITEM first = GetTreeCtrl().GetFirstVisibleItem();
			if (first != sel)
			{
				HTREEITEM item = GetTreeCtrl().GetPrevSiblingItem(sel);
				if (item)
				{
					CObject *tmpobj = (CObject*)GetTreeCtrl().GetItemData(item);
					if (tmpobj)
					{
						WaxPart *p = dynamic_cast<WaxPart*>(tmpobj);
						if (p)
						{
							if (!p->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData)))	
								gliederung = true;
						}
					}
				}
			}
		}
	}

	if ((sel && par && !labelEdit) || gliederung)
	{
		delete_enabled = TRUE;
		WaxPart *pt = (WaxPart*)GetTreeCtrl().GetItemData(sel);

		// CTreeCtrl
		// Wenn es ne Gliederung ist, kann diese nur gelöscht werden, wenn sie keine Unterobjekte enthält, 
		// oder nur Unterobjekte, bei denen die WAid nicht gesetzt ist 
		
		// Freigabe wird über WAid geprüft, da bei Sammel 0 muss Unterobjekt geprüft werden!
		if (pt->IsKindOf(RUNTIME_CLASS(PrjGliederung)))
		{
			for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos; )
			{
				WaxPart *pa = NULL;
				pa = dynamic_cast<WaxPart*> (pt->get_subobjects().GetNext(pos));
				if (pa && !pa->get_deleted())
				{					
					if (pa->IsKindOf(RUNTIME_CLASS(PrjGliederung)) && !pa->get_subobjects().IsEmpty())
					{
						delete_enabled = FALSE; 
						break;
					}
					else if (pa->IsKindOf(RUNTIME_CLASS(SammelAuftrag)) && !pa->get_subobjects().IsEmpty())
					{
						WaxPart *p = NULL;
						p = dynamic_cast<WaxPart*>(pa->getFirstSubobject());
						if (p->get_werkauftrag()->get_waid()) // Freigegebener Werkauftrag
						{
							delete_enabled = FALSE; 
							break;
						}
					}
					else
					{
						if (pa->get_werkauftrag()->get_waid()) // Freigegebener Werkauftrag
						{
							delete_enabled = FALSE; 
							break;
						}
					}
				}
			}
		}
		// Freigabe wird über WAnr geprüft, da bei Sammel 0 muss Unterobjekt geprüft werden!
		else if (pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)) && !pt->get_subobjects().IsEmpty())
		{
			WaxPart *p = NULL;
			p = dynamic_cast<WaxPart*>(pt->getFirstSubobject());
			if (p)
			{
				if (!WaxSys::isAV() && (p->get_werkauftrag()->get_status().get_wanr() ||// Freigegebener Werkauftrag in Bearbeitung
									    p->get_werkauftrag()->get_status().get_status() == Status::Angebot ||
										p->get_werkauftrag()->get_status().get_status() == Status::Preisanfrage ||
										(p->get_werkauftrag()->get_status().get_status() == Status::Freigegeben && p->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer() > 0)))

					delete_enabled = FALSE; 
			}
			else
				delete_enabled = TRUE; 
		}
		else if (pt->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData)))
			delete_enabled = FALSE;
		else
			delete_enabled = pt->initDelete();

	}
//	TRACE("DELETE_ENABLED=%s\n", delete_enabled ? "TRUE" : "FALSE");
	pCmdUI->Enable(delete_enabled);		
}		




void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	/***
	 Selektiertes Item im Baum hat sich geändert 
	 Durch einen Aufruf von CWaxDoc::SetActiveObject wird die Anzeige umgeschaltet. Schlägt der
	Aufruf fehl, wird das alte Item wieder gewählt
	*/
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();

	static bool doupdate = true;
	
	if (!doupdate)
		return;	
	
	WaxPart *pt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	XASSERT(pt);
	if (!pt)
		return;
	if (!((CWaxDoc*)m_pDocument)->set_active_object(pt))
	{ // UpdateData ist fehlgeschlagen, altes Item wird selektiert
		doupdate = false;
		if (pNMTreeView && pNMTreeView->itemOld.hItem)
			GetTreeCtrl().Select(pNMTreeView->itemOld.hItem, TVGN_CARET);
		doupdate = true;
		return;
	}

	if (pNMTreeView && pNMTreeView->itemOld.hItem)
	{
		WaxPart *par = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(pNMTreeView->itemOld.hItem));	
		XASSERT(par);
		if (par)
		{
			GetTreeCtrl().SetItemText(pNMTreeView->itemOld.hItem, par->getBezeichnung());
			GetTreeCtrl().SetItemImage(pNMTreeView->itemOld.hItem, par->get_iconindex(iconman), par->get_iconindex(iconman));
		}
	}
	//Bei Sammelauftrag und Einzelauftrag wird Statusaktualisierung abgefragt, 
	//wenn der Auftrag freigegeben ist aber noch keine WAnr hat und nicht heute freigegeben wurde 
	if (pt->sollUpdateStatus())
		onUpdatestatus();
	
	GetTreeCtrl().SetItemText(sel, pt->getBezeichnung());
	GetTreeCtrl().SetItemImage(sel, pt->get_iconindex(iconman), pt->get_iconindex(iconman));
	GetTreeCtrl().SetFocus();
}

//Kontextmenü
void CLeftView::OnContextMenu(CWnd* pWnd, CPoint point)
{

	/**
	 Anzeige des Kontextmenüs
	 Diese Methode zeigt das Kontextmenü an. Ist point.x/y = -1, wird davon ausgegangen,
	dass die Kontextmenütaste gedrückt wurde
	*/
	UNREFERENCED_PARAMETER(pWnd);
	CMenu *pop;

	POINT pt = point;
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();

	// Bei der Projektleitkarte wird kein Menü angezeigt, auch nicht kopieren/ausschneiden usw.
	PrjLeitkarteData *dat = dynamic_cast<PrjLeitkarteData*>((CObject*)GetTreeCtrl().GetItemData(sel));
	if (dat)
		return;

	// Wenn irgendwohin geklickt wurde, ????
	if (point.x == -1 && point.y == -1)
	{
		VERIFY(sel);
		if (!sel)
			return;
		RECT r;
		GetTreeCtrl().GetItemRect(sel, &r, TRUE);
		pt.x = r.right;
		pt.y = r.bottom;
	}
		
	ClientToScreen(&pt);
	CMenu* m = ((CWaxDoc*)m_pDocument)->GetMenu(); // Aus WaxPart, muss hier gelöscht werden
	if (!m)
		return;
	
	pop = m->GetSubMenu(0);

	if (!pop)
		pop = m;

	if (!pop)
		return;

	OnInitMenuPopup(pop, 1, false) ;
	modifyPopupMenu(pop);
    CWnd *dest = getCurrentRightTabbedView();
	if (dest)
	{
		dest->UpdateData(TRUE);
		VERIFY(pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x+2, pt.y, dest));
	}
	modifyPopupMenu(pop, false);
	delete m;
}

//Baumansicht updaten
void CLeftView::OnUpdateTree()
{
	/**
	 Baumansicht updaten
	 Hier wird der Baum ab dem selektierten Item komplett neu aufgebaut, um alle Änderungen an
	Unterobjekten zu reflektieren. 
	*/
	CEdit *ed = GetTreeCtrl().GetEditControl();
	if (ed)// Wenn das Label editiert wird, soll es nicht abgebrochen werden durch ein Baumupdate
		return;
	
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
	VERIFY(sel);
	if (!sel) return;
	doTreeUpdate(sel);
	LPARAM dummy;
	OnSelchanged(NULL, &dummy);
}

//Baut den Baum ab einem bestimmten Item neu auf
void CLeftView::doTreeUpdate(HTREEITEM item)
{
	HTREEITEM sel = item;
	HTREEITEM nextsel = NULL;
	

	GetTreeCtrl().SetRedraw(FALSE);
	if (GetTreeCtrl().ItemHasChildren(sel))
	{
		HTREEITEM del = GetTreeCtrl().GetNextItem(sel, TVGN_CHILD);
		while (del)
		{
			GetTreeCtrl().DeleteItem(del);
			del = GetTreeCtrl().GetNextItem(sel, TVGN_CHILD);
		}
	}
	WaxPart *par = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	XASSERT(par);
	GetTreeCtrl().SetItemText(sel, par->get_werkauftrag()->get_bezeichnung());
	if (par) //Baumzweig neu aufbauen
	{
		for (POSITION pos = par->get_subobjects().GetHeadPosition(); pos != NULL;)
		{
			WaxPart *child = dynamic_cast<WaxPart*>(par->get_subobjects().GetNext(pos));
			XASSERT(child);
			if (child->get_deleted())
				continue;
			nextsel = child->Show(GetTreeCtrl(), sel, TVI_LAST, iconman);
			if (nextSelect != child)
				nextsel = NULL;
		}
	}
	nextSelect = NULL;
	if (nextsel)
		GetTreeCtrl().Select(nextsel, TVGN_CARET);
	GetTreeCtrl().SetRedraw(TRUE);
}



void CLeftView::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	UNREFERENCED_PARAMETER(nIndex);
	CMenu *pMenu = pPopupMenu;
	if (bSysMenu)
		return;     // don't support system menu

	XASSERT(pMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	XASSERT(state.m_pOther == NULL);
	XASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pMenu->m_hMenu)
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		XASSERT(state.m_pOther == NULL);
		XASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CLeftView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pResult);
	/*
	 Drag and Drop beginnen
	 Drag and Drop kann durch  WaxPart::m_bCanBeDragged ein/ausgeschaltet werden
	*/
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	itemDrag = pNMTreeView->itemNew.hItem;
	itemDrop = NULL;


	// Objekt muss existieren und es muss draggable sein
	WaxPart*obj = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(itemDrag));
	XASSERT(obj);



	if (!obj->get_dragable())
		return;

	dragImage = GetTreeCtrl().CreateDragImage(itemDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
	if( !dragImage )
		 return;

	isDragging = true;
	dragImage->BeginDrag(0, CPoint(-15,-15));
	POINT pt = pNMTreeView->ptDrag;
	GetTreeCtrl().ClientToScreen( &pt );
	dragImage->DragEnter(NULL, pt);
	GetTreeCtrl().SetCapture();
}

void CLeftView::OnMouseMove(UINT nFlags, CPoint point) 
{
	/**
	 Maus wird im Fenster bewegt
	 Wenn Drag and Drop aktiv ist, wird durch CImageList::DragMove das Bildchen mitgezogen
	*/
	CTreeView::OnMouseMove(nFlags, point);

	HTREEITEM       hitem;
	UINT            flags;

	if (isDragging)
	{
		POINT pt = point;
		GetTreeCtrl().ClientToScreen( &pt );
		CImageList::DragMove(pt);
		if ((hitem = GetTreeCtrl().HitTest(point, &flags)) != NULL)
		{
			CImageList::DragShowNolock(FALSE);
			GetTreeCtrl().SelectDropTarget(hitem);
			itemDrop = hitem;
			CImageList::DragShowNolock(TRUE);
		}
	}
}



void CLeftView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	/**
	 Maustaste wurde losgelassen
	 Wenn Drag and Drop aktiv ist, wird je nach Tastaturbetätigung (Strg kopiert!) das Item
	verschoben oder kopiert.
	*/
	CTreeView::OnLButtonUp(nFlags, point);

	if (isDragging == true)
	{
		bool bCopy = nFlags & MK_CONTROL ? true : false;
		isDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		delete dragImage;

		// Remove drop target highlighting
		GetTreeCtrl().SelectDropTarget(NULL);

		if( itemDrag == itemDrop )
			 return;

		// If Drag item is an ancestor of Drop item then return
		HTREEITEM htiParent = itemDrop;
		while( (htiParent = GetTreeCtrl().GetParentItem( htiParent )) != NULL )
		{
			 if( htiParent == itemDrag ) return;
		}

		GetTreeCtrl().Expand( itemDrop, TVE_EXPAND ) ;

		// Parentitem muss existieren
		HTREEITEM par = GetTreeCtrl().GetParentItem(itemDrag);
		if (!par)
			return;
		WaxPart *pt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(par));
		XASSERT(pt);
		if (!pt)
			return;
		
		// Objekt muss existieren
		CObject *obj = (CObject*)GetTreeCtrl().GetItemData(itemDrag);
		XASSERT(obj);
		if (!obj)
			return;

		// Parentitem muss Objekt in Liste haben
		POSITION pos = pt->get_subobjects().Find(obj);
		XASSERT(pos);
		if (!pos)
			return;

		// Jetzt kann Objekt kopiert werden
		HTREEITEM htiNew = doCopy( itemDrag, itemDrop, bCopy, obj);
		if (htiNew)
		{
			if (!bCopy)
			{
				// Objekt kann jetzt aus Parentitem gelöscht werden
				pt->get_subobjects().RemoveAt(pos);
				GetTreeCtrl().DeleteItem(itemDrag);
			}
			GetTreeCtrl().SelectItem( htiNew );
		}
	}

}


//Kopierfunktion für Drag and Drop (drag) und Cut and Paste (ins)
HTREEITEM CLeftView::doCopy(HTREEITEM drag, HTREEITEM drop, bool bCopy, CObject *ins)
{
	/***
	 Item aus Baum verschieben
	 Ein Item aus dem Baum wird mit allen Eigenschaften kopiert. Das eigentliche Item
	wird in OnLButtonUp gelöscht. Ist ins ein Pointer auf ein WaxPart, wird drag ignoriert.
	*/	
	// Ziel muss immer da sein
	XASSERT(drop);
	// darf aber nicht die Quelle sein
	XASSERT(drop != drag);
	// Entweder drag oder ins muss angegeben werden
	XASSERT(drag || ins);
	
	// Wenn o.g. Prüfungen greifen: nix tun
	if ( !drop || (drop == drag) || !(drag || ins))
		return NULL;

	HTREEITEM parent = drop;
	HTREEITEM insafter = TVI_FIRST;

	WaxPart *basept = dynamic_cast<WaxPart*>(ins ? ins : (CObject*)GetTreeCtrl().GetItemData(drag));
	WaxPart *pt = basept;

	if (!check_Sammelauftrag(const_cast<WaxPart*>(pt->get_parent()), pt))
		return NULL;

	if (bCopy)
	{
		pt = (WaxPart*)basept->GetRuntimeClass()->CreateObject();
		pt->copy(*basept);
	}


	WaxPart *parpt = NULL;

	// Solange im Baum zurückgehen, bis Gliederungsobjekt gefunden wurde
	while (1)
	{
		parpt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(parent));
		if (!parpt->IsKindOf(RUNTIME_CLASS(PrjGliederung)) && !parpt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)) && !parpt->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)) )
		{
			parent = GetTreeCtrl().GetParentItem(drop);
			if (!parent || parent == TVGN_ROOT)
				return NULL;
			insafter = drop;
			drop = parent;
			continue;
		}
		else
			break;
	}
	
	
	XASSERT(parpt);
	
	
	if (!drop || !parpt || !check_Sammelauftrag(parpt, dynamic_cast<WaxPart*>(ins)))
		return NULL;

	// Bearbeitung der m_Content Liste
	// Item nach einem anderen einfügen
	if (insafter != TVI_FIRST)
	{
		CObject *ins = NULL;
		ins = (CObject*)GetTreeCtrl().GetItemData(insafter);
		WaxPart *inspt = dynamic_cast<WaxPart*>(ins);
	
		XASSERT(inspt);
		if (!inspt)
			return NULL;
		POSITION pos = parpt->get_subobjects().Find(inspt);
		XASSERT(pos);
		if (!pos)
			return NULL;
		pt->get_werkauftrag()->initPosnr(inspt->get_werkauftrag()->get_posNr());		
		// ...und die KFarbe und die dnvl vorbelegen, wenn noch keine Elemente eingegeben wurden
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
		if (rohr)
		{
			Rohrleitung* prev = dynamic_cast<Rohrleitung*>(inspt->get_werkauftrag());
			if (prev && !rohr->get_elementArray().GetSize())
			{
				rohr->set_rohrKFarbe(prev->get_rohrKFarbe());		
				rohr->set_dnvl(prev->get_dnvl());		
			}
		}
		parpt->get_subobjects().InsertAfter(pos, pt);
	}
	else // Item als erstes unter eine Gliederung einfügen
	{
		parpt->get_subobjects().AddHead(pt);
		
	}
	pt->set_parent(parpt);


	//Sammelauftragswerte an Positionen vererben
	SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(parpt);
	if (sammel)
		sammel->vererbe_leitkarte();


	//
	return pt->Show(GetTreeCtrl(), parent, insafter, iconman);
}


WaxPart* CLeftView::GetSelection()
{
	/**
	 Selektiertes Objekt aus Baum ermitteln
	*/
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
	WaxPart *pt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	XASSERT(pt);
	if (!pt)
		AfxThrowUserException();
	return pt;
}


BOOL CLeftView::CopyToClipboard(CObject *obj, bool resetStatus) const
{
	/**
	 Kopieren einer Position ins Clipboard
	*/
	UNREFERENCED_PARAMETER(resetStatus);
	if (!obj)
		return false;
	
	BOOL clpopen = ::OpenClipboard(this->m_hWnd);
	XASSERT(clpopen);
	if (!clpopen)
		return false;

	BOOL clpempty = ::EmptyClipboard();
	XASSERT(clpempty);
	if (!clpempty)
		return false;
	
	
	UINT format = ::RegisterClipboardFormat("WAXPART");
	XASSERT(format);
	if (!format)
		return false;

	WaxPart *pt = dynamic_cast<WaxPart*>(obj);
	XASSERT(pt);
	if (!pt)
		return false;

	CSharedFile *f = new CSharedFile(GMEM_DDESHARE | GMEM_MOVEABLE);
	TRY
	{
		pt->saveToFile(f);
		XASSERT(::SetClipboardData(format, f->Detach()));
	}
	CATCH_ALL(e);
	{
		e->ReportError();
	}
	END_CATCH_ALL
	XASSERT(::CloseClipboard());
	delete f;
	return TRUE;
}

//Nächstes Fenster
BOOL CLeftView::OnNextPaneCmd(UINT nID)
{
	UNREFERENCED_PARAMETER(nID);
	/*
		 Focus auf rechtes Fenster mit Tabdialogen setzen
		 Wird aufgerufen, wenn F6 gedrückt wurde. nID kann ID_NEXT_PANE oder ID_PREV_PANE sein, ist
		 aber egal, da das nächste Fenster immer die rechte Seite ist
	*/

	m_pMainSplitterFrame->GetSplitterWnd()->GetViewPtr(WaxSplitterWnd::eWAXTABVIEW)->SetFocus();

	return TRUE;
}

//Neues Teil einfügen
void CLeftView::insertObj(WaxPart *obj)
{
	/***
	 Fügt ein Objekt vom Typ WaxPart in die Baumhierarchie ein
	 Das Einfügen erfolgt durch einen Aufruf von WaxPart::AddChild. 
	 Der aktive Tab wird auf Null gesetzt (Leitkarte), die Projektleitkartendaten
	 werden vererbt.
	*/	

	HTREEITEM parent = GetTreeCtrl().GetSelectedItem();
	WaxPart *parpt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(parent));
	if (!parpt || !parpt->subobj_allowed())
	{
		delete obj;
		return;
	}

	if (!check_Sammelauftrag(parpt, obj))
	{
		delete obj;
		return;
	}

	CTabCtrlView::resetActiveTab();
	WaxPart *pWaxPart = ((CWaxDoc*)m_pDocument)->get_active_object();
	XASSERT(pWaxPart);
	if (!pWaxPart)
	{
		delete obj;
		return;
	}
	nextSelect = pWaxPart->addChild(obj);
	
//	obj->get_werkauftrag()->set_changed(true);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, WAXID_PRJLEITKARTE);
}

//Neues Verteilerrohr einfügen
void CLeftView::onNeuerVerteiler()
{
	/**
	Neues Verteilerrohr einfügen
	Erzeugt ein neues Objekt vom Typ <c VerteilerData> und fügt es durch einen Aufruf von
	insertObj> in den Baum ein
	*/
	insertObj(new VerteilerData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

//Einfügen einer Formstücke und Fittinge Bestellung
void CLeftView::onNeuFormstueckeundfittinge()
{
	/**
	Neue FormstueckeUndFittinge Liste einfügen
	Erzeugt ein neues Objekt vom Typ <c FormstueckeUndFittingeDaten> und fügt es durch einen Aufruf von
	insertObj> in den Baum ein
	*/
	insertObj(new FormstueckeUndFittingeDaten);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

//Neues Strangrohr einfügen
void CLeftView::onNeuesStrangrohr()
{	
	/**
	Neues Strangrohr einfügen
	Erzeugt ein neues Objekt vom Typ <c StrangrohrData> und fügt es durch einen Aufruf von
	insertObj> in den Baum ein
	*/
	insertObj(new StrangrohrData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

//Neues Gewinderohr einfügen
void CLeftView::onGewinderohr()
{	
	/**
	Neues Gewinderohr einfügen
	Erzeugt ein neues Objekt vom Typ <c GewinderohrData> und fügt es durch einen Aufruf von
	insertObj> in den Baum ein
	*/
	insertObj(new GewinderohrData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

//Neuen Stationsverteiler einfügen
void CLeftView::onStationsVerteiler()
{	/**
	 Neuen Stationsverteiler einfügen
	 Erzeugt ein neues Objekt vom Typ <c StationsverteilerData> und fügt es durch einen Aufruf von
	insertObj in den Baum ein
	*/
	insertObj(new StationsverteilerData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}



void CLeftView::onCOIIVerteiler()
{
	insertObj(new COIIVerteilerData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}



void CLeftView::onMX1230MX200Verteiler()
{
	insertObj(new CMx2001230VerteilerData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}


void CLeftView::onSammelTaskViaList()
{ 
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();

	MxCObList listOfWerkauftrage;

	if (sel && !labelEdit)
	{
		
		WaxPart *pt = (WaxPart*)GetTreeCtrl().GetItemData(sel);

		
		for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos; )
		{	
			WaxPart *psa = NULL;
			psa = dynamic_cast<WaxPart*> (pt->get_subobjects().GetNext(pos));
			int countSubs =0;
			for (POSITION pos = psa->get_subobjects().GetHeadPosition(); pos; )
			{
				WaxPart *pa = NULL;
				pa = dynamic_cast<WaxPart*> (psa->get_subobjects().GetNext(pos));
				countSubs++;
			}

			if(countSubs == 1)
			{
				for (POSITION pos = psa->get_subobjects().GetHeadPosition(); pos; )
				{
					WaxPart *pa = NULL;
					pa = dynamic_cast<WaxPart*> (psa->get_subobjects().GetNext(pos));
					if (pa && !pa->get_deleted())
					{	
						Werkauftrag* werkauftrag = pa->get_werkauftrag();
						listOfWerkauftrage.AddTail((CObject*)werkauftrag);
					}
				}
			}
		}
	}
	SammelAuftragViaList* sammelAuftragViaList = new SammelAuftragViaList(&listOfWerkauftrage);
	sammelAuftragViaList->DoModal();

	delete sammelAuftragViaList;

}


void CLeftView::onNeueEntnahmeleitung()
{
	insertObj(new EntnahmeleitungData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

void CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UNREFERENCED_PARAMETER(pHint);
	UNREFERENCED_PARAMETER(pSender);
	UNREFERENCED_PARAMETER(lHint);
	CString *msg = (CString*)pHint;
	if (!msg)
		return;

	HTREEITEM sel = NULL;
	GetTreeCtrl().DeleteAllItems();
	// Jetzt wird der Baum initialisiert und alle Root-Objekte in der Reihenfolge von m_Objects angezeigt.
	for (int i = 0; i < ((CWaxDoc*)m_pDocument)->get_objects().GetSize(); i++)
	{
		WaxPart *part = (WaxPart*)(((CWaxDoc*)m_pDocument)->get_objects().GetAt(i));
		if (part->get_deleted())
			continue;
		
		sel = part->Show(GetTreeCtrl(), TVI_ROOT, TVI_LAST, iconman);
	}
	if (sel)
		GetTreeCtrl().SelectItem(sel);
}

//Bearbeitet das Popupmenu vor der Anzeige
void CLeftView::modifyPopupMenu(CMenu* menu, bool modify)
{	/**
	 Nimmt Änderungen am Popupmenü vor
	 mit modify false können die Änderungen wieder entfernt werden. Diese Methode kann 
	durcheinander kommen, wenn als Titel einer Untergliederung ein bereits vorhandener String
	benutzt wird. 
	*/
	CString str;
	// Benamsung der Untergliederung
	CString title = AfxGetApp()->GetProfileString("GLIEDERUNGSOBJEKT", "titel", "");
	if (!title.GetLength())
		title = getStammdaten().getText("IDS_GLIEDERUNG").c_str();
	int imax = menu->GetMenuItemCount();
	if (modify)
	{
		for (int i = 0; i < imax; i++)
		{
			menu->GetMenuString(i, str, MF_BYPOSITION | MF_STRING);
			UINT id = menu->GetMenuItemID(i);
			int index = -1;
			CString txt; txt = getStammdaten().getText("IDS_UNTERGLIEDERUNG").c_str();
			if ((index = str.Find(txt, 0)) != -1)
			{
				str = str.Left(index) + title + str.Right(str.GetLength() - index - strlen(txt));
				menu->ModifyMenu(i, MF_BYPOSITION, id, str);
			}
		}
	}
	else
	{
		for (int i = 0; i < imax; i++)
		{
			menu->GetMenuString(i, str, MF_BYPOSITION | MF_STRING);
			UINT id = menu->GetMenuItemID(i);
			CString txt; txt = getStammdaten().getText("IDS_UNTERGLIEDERUNG").c_str();
			int index = -1;
			if ((index = str.Find(title, 0)) != -1)
			{
				str = str.Left(index) + txt + str.Right(str.GetLength() - index - strlen(title));
				menu->ModifyMenu(i, MF_BYPOSITION, id, str);
			}
		}

	}
}

//Einfügen einer neuen Untergliederung in den Hauptzweig des 
//Projektes
void CLeftView::onNeueUntergliederungRoot()
{
	CWaxDoc *doc = (CWaxDoc*)m_pDocument;
	doc->get_objects().Add(new PrjGliederung);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, WAXID_PRJLEITKARTE);
	CString tmp;
	doc->UpdateAllViews(NULL, 0, (CObject*)&tmp);
}

//Einfügen einer neuen Untergliederung
void CLeftView::onNeueUntergliederung()
{
	insertObj(new PrjGliederung);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

//Einfügen eines neuen Sammelauftrages
void CLeftView::onNeuerSammelauftrag()
{
	insertObj(new SammelAuftrag); 
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

void CLeftView::onNeuePreisanfrage()
{
	insertObj(new WpPreisanfrage); 
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}


//Bezeichnung des Werkauftrages ändern
void CLeftView::onEditlabel()
{
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
	if (!sel)
		return;
	WaxPart *pt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	XASSERT(pt);
	if (!pt || !pt->get_werkauftrag())
		return;
	labelEdit = true;
	GetTreeCtrl().ModifyStyle(0, TVS_EDITLABELS);
	GetTreeCtrl().EditLabel(sel);
}

//Indicates the user finished editing a item label
void CLeftView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	GetTreeCtrl().ModifyStyle(TVS_EDITLABELS, 0);

	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
	if (!sel)
		return;
	WaxPart *pt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	XASSERT(pt);
	if (!pt || !pt->get_werkauftrag())
		return;
	CEdit *ed = GetTreeCtrl().GetEditControl();
	CString x;
	ed->GetWindowText(x);
	CWnd *edit = getCurrentRightTabbedView();
	if (edit)
		edit->UpdateData();
	if (x != pt->get_werkauftrag()->get_bezeichnung())
		pt->get_werkauftrag()->set_bezeichnung(x);
	if (edit)
		edit->UpdateData(false);// Ist eigentlich nur für den Bauabschnitt nötig
	*pResult = 1;
	labelEdit = false;
}

//Ruft die Statusaktualisierung in WaxDoc auf
void CLeftView::onUpdatestatus()
{
	((CWaxDoc*)m_pDocument)->OnStatusAktualisieren();
	GetTreeCtrl().SetFocus();
}

//Startet Voloview mit der aktuellen Grafik
void CLeftView::onVoloview()
{
	HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
	if (!sel)
		return;
	WaxPart *pt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(sel));
	if (!pt)
		return;
	string file = pt->get_grafic_file();
	if (file.length())
	{
		if ((DWORD)ShellExecute(NULL, "open", file.c_str(), NULL, NULL, SW_SHOWNORMAL) < 31)
			MsgBox::show(getStammdaten().getText("IDS_ERROR1b"), MsgBox::Error);
	}
}

void CLeftView::onNeuePumpenprobierleitung()
{
	insertObj(new PumpenprobierleitungData); 
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

void CLeftView::onNeuerPumpenverteiler()
{
	insertObj(new PumpenverteilerData); 
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

void CLeftView::onNeuHandelslaenge()
{
	/**
	Neue Handelslaenge einfügen
	Erzeugt ein neues Objekt vom Typ <c VerteilerData> und fügt es durch einen Aufruf von
	insertObj> in den Baum ein
	*/
	insertObj(new HandelslaengeData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

void CLeftView::onNeuApax()
{
	insertObj(new ApaxData);
	PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
}

void CLeftView::onEditCopy()
{
	if (!testCommand(ID_EDIT_COPY))
		return;
	OnCopy();
}

void CLeftView::onEditPaste()
{
	if (!testCommand(ID_EDIT_PASTE))
		return;
	OnPaste();
}

void CLeftView::onEditCut()
{
	if (!testCommand(ID_EDIT_CUT))
		return;
	OnCut();
}

void CLeftView::onEditDelete()
{
	if (!testCommand(ID_EDIT_DELETE))
		return;

	OnDelete();
}

//Ruft die OnUpdatexxx Methoden zu den übergebenen Menüids auf
bool CLeftView::testCommand(UINT id)
{
	CMenu dummy;
	dummy.CreateMenu();
	dummy.AppendMenu(MF_STRING, id, "dummy");
	CCmdUI cmdui;
	cmdui.m_pMenu = &dummy;
	cmdui.m_nIndex = 0;
	cmdui.m_nIndexMax = 1;
	switch (id)
	{
	case ID_EDIT_DELETE:
	case ID_EDIT_CUT:
		OnUpdateDelete(&cmdui);
		break;
	case ID_EDIT_COPY:
		OnUpdateCopy(&cmdui);
		break;
	case ID_EDIT_PASTE:
		OnUpdatePaste(&cmdui);
		break;

	default:
		XASSERT(false);
		return false;
	}
	return dummy.GetMenuState(id, MF_BYCOMMAND ) == MF_ENABLED;
}



void CLeftView::onNewelement()
{
	WaxPart *pt = GetSelection();
	if (!pt)
		return;
	CObject *newobj = pt->GetRuntimeClass()->CreateObject();
	WaxPart *newpt = dynamic_cast<WaxPart*>(newobj);
	XASSERT(newpt);
	if (!newpt)
		return;

	// Automatisch neu einfügen darf nicht mit Gliederungen oder Sammelaufträgen gehen,
	// Sonst sind Sammelaufträge in Sammelaufträgen möglich
	if (newpt->subobj_allowed())
	{
		delete newpt;
		return;
	}

	newpt->get_werkauftrag()->get_leitkarte().copy(pt->get_werkauftrag()->get_leitkarte());
	newpt->get_werkauftrag()->get_beschichtung().copy(pt->get_werkauftrag()->get_beschichtung());
	
	CopyToClipboard(newobj);
	OnPaste();
	delete newpt;

	//Automatisch in die Eingabemaske wechseln
	OnNextPaneCmd(0); 
}




//Liefert das aktuelle Eingabefenster der linken Seite.
CWnd* CLeftView::getCurrentRightTabbedView()
{
	return m_pMainSplitterFrame->GetSplitterWnd()->GetViewPtr(WaxSplitterWnd::eWAXTABVIEW);
}



//Called when the user double-clicks the left mouse button.
void CLeftView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/**
	Schaltet zwischen Druckvorschau und Eingabe um
	*/
	CTreeView::OnLButtonDblClk(nFlags, point);
	//if (getChildFrame())
	m_pMainSplitterFrame->PostMessage(WM_COMMAND, ID_ENDDRUCKVORSCHAU);	
}



//Called when the user double-clicks the right mouse button.
void CLeftView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	/**
	Schaltet zwischen Grafikanzeige und Eingabe um
	*/

	CTreeView::OnRButtonDblClk(nFlags, point);
	if (getChildFrame())
		getChildFrame()->PostMessage(WM_COMMAND, ID_VIEW_GRAFIK);		
}



//Liefert das ChildFrame Fenster
CWnd* CLeftView::getChildFrame()
{
	CWnd *par = GetParent();
	XASSERT(par);
	if (!par)
		return NULL;
	XASSERT(par->GetParent());
	return par->GetParent();
}


// Neues Teil anhängen
WaxPart* CLeftView::addObj(WaxPart *obj)
{
	/**
	 Fügt ein Objekt vom Typ <c WaxPart> in die Baumhierarchie ein
	 Das Einfügen erfolgt durch einen Aufruf von <mf WaxPart::AddChild>. 
	 Der aktive Tab wird auf Null gesetzt (Leitkarte)
	*/	
	HTREEITEM item = GetTreeCtrl().GetSelectedItem();
	WaxPart *parpt = NULL;
	parpt = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(item));
	
	HTREEITEM parentitem = GetTreeCtrl().GetParentItem(item);
	WaxPart *parent = NULL;
	if (parentitem)
		parent = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData(parentitem));

	if (!parent || !parent->subobj_allowed())
	{
		delete obj;
		return NULL;
	}

	CTabCtrlView::resetActiveTab();
	XASSERT(parent);
	if (!parent)
	{
		delete obj;
		return NULL;
	}

	POSITION pos = parent->get_subobjects().Find(parpt);
	XASSERT(pos);
	parent->get_subobjects().InsertAfter(pos, obj);
	
//	obj->get_werkauftrag()->set_changed(true);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, WAXID_PRJLEITKARTE);

	obj->set_parent(parent);
	obj->Show(GetTreeCtrl(), parentitem, TVI_LAST, iconman);
	return obj;
}

bool CLeftView::check_Sammelauftrag(WaxPart *pt, WaxPart *ins)
{
	// Prüfung,ob Sammelauftrag richtig benutzt wird: Hier können nur Objekte eines einzelnen Typs gespeichert
	// werden. Bereits freigegebene Werkaufträge können nicht nachträglich einem Sammelauftrag zugeordnet werden.
	if (!pt || !pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		return true;

	// In Sammelaufträge dürfen keine anderen Sammelaufträge oder Gliederungen eingefügt werden
	if (pt && pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)) && ins->subobj_allowed())
		return false;

	if (!ins)
		return false;

	WaxPart *firstsub = pt->getFirstSubobject();
	if (firstsub)
	{
		if (firstsub->GetRuntimeClass()->m_lpszClassName != ins->GetRuntimeClass()->m_lpszClassName)
		{
			MsgBox::show(getStammdaten().getText("IDS_ERROR2"), MsgBox::Error);
			return false;
		}

		if (!WaxSys::isAV() && firstsub->get_werkauftrag()->get_status().get_wanr())
		{
			MsgBox::show(getStammdaten().getText("IDS_ERROR3c"), MsgBox::Error);
			return false;
		}

	}
	// Diese Beschränkung könnte entfallen, wenn der Sammelauftrag leer ist. Dann müsste die Leitkarte geklont werden.
	// Problem: Anzeigemaske muss neu angezeigt werden.
	if (ins->get_werkauftrag()->get_waid())
	{
		MsgBox::show(getStammdaten().getText("IDS_ERROR4"), MsgBox::Error);
		return false;
	}
	
	if (pt->get_subobject_count() >= WaxSys::para_int(MAXPOSPERSA))
	{
		CString msg;
		msg.Format(getStammdaten().getText("IDS_ERROR_MAXAUFTRAEGE").c_str(), WaxSys::para_int(MAXPOSPERSA));
		MsgBox::show((LPCSTR)msg, MsgBox::Information);
		return false;
	}

	return true;
}

void CLeftView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTVCUSTOMDRAW pCDRW = (LPNMTVCUSTOMDRAW) pNMHDR;
	switch( pCDRW->nmcd.dwDrawStage )
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW ;	// ask for item notifications
			break ;
		
		case CDDS_ITEMPREPAINT:
		{


			HTREEITEM sel = GetTreeCtrl().GetSelectedItem();
			XASSERT(sel);
			if (!sel ||  sel ==(HTREEITEM) pCDRW->nmcd.dwItemSpec)
				return;


			WaxPart *pPart = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData((HTREEITEM) pCDRW->nmcd.dwItemSpec));

			SammelAuftrag* pSammel =  dynamic_cast<SammelAuftrag*>(pPart);
			
			if(pSammel != NULL)
			{
				for (POSITION posG = pSammel->get_subobjects().GetHeadPosition(); posG != NULL; )
				{
					WaxPart* waxpt = dynamic_cast<WaxPart*>(pSammel->get_subobjects().GetNext(posG));
					Werkauftrag* pwerkauftrag = waxpt->get_werkauftrag();
					if(!pwerkauftrag->get_GUID().IsEmpty())
					{
						pCDRW->clrTextBk = RGB(165, 255, 74);
						return;
					}
				}
			}
	
		}
	}
}



void CLeftView::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	WaxPart *pPart = 0;
	if(pNMTreeView->action == TVE_EXPAND)
	{
		// make sure there are no selected items in collapsed trees
		// otherwise the user might be confused by the behavior of
		// the tree control.
		HTREEITEM hItem = pNMTreeView->itemNew.hItem;
		pPart = dynamic_cast<WaxPart*>((CObject*)GetTreeCtrl().GetItemData((HTREEITEM) hItem));
		GpVk3Preis vk3Preis;
		vk3Preis.getVk3Preis(pPart);
	}
	*pResult = 0;
}
