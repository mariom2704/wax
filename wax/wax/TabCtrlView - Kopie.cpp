#include "stdafx.h"

#include "resource.h"
#include "wax.h"
#include "Waxdoc.h"
#include "ChildFrm.h"


#include "..\waxui\CO II VerteilerEingabeFormBemassung.h"

		#include "TabCtrlView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Aktiver Tab, wenn z.B. der zweite Tab ausgewählt, wird beim Umschalten auf 
//ein anderes Objekt automatisch der zweite Tab angezeigt
int CTabCtrlView::activeTab = 0;

#define IDUPDATETAB WM_USER+1234
#define IDTESTTAB WM_USER+1235

BEGIN_MESSAGE_MAP(CTabCtrlView, CCtrlView)
	ON_COMMAND_RANGE(WAXID_FIRST, WAXID_LAST, HandleMsgToLeftView)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND_EX(ID_NEXT_PANE, OnNextPaneCmd)
	ON_COMMAND_EX(ID_PREV_PANE, OnNextPaneCmd)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_COMMAND(IDUPDATETAB, onUpdateTabs)
	ON_COMMAND(IDTESTTAB, onTestTabs)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CTabCtrlView, CCtrlView)

//Dynamische Erstellung verwendet geschützten Konstruktor
CTabCtrlView::CTabCtrlView() : CCtrlView ("SysTabControl32", AFX_WS_DEFAULT_VIEW | WS_TABSTOP )
{
	m_Init = false;
	currentTab = 0;
	tooltip = NULL;
	
//	m_nFlags |= WF_TOOLTIPS|WF_TRACKINGTOOLTIPS ;
}

//Destruktor
CTabCtrlView::~CTabCtrlView()
{
	for (POSITION pos = runtimeObjects.GetHeadPosition(); pos != NULL;)
		delete runtimeObjects.GetNext(pos);
	delete tooltip;
}

//Nächstes Fenster
BOOL CTabCtrlView::OnNextPaneCmd(UINT nID)
{
	UNREFERENCED_PARAMETER(nID);
	m_pMainSplitterFrame->GetSplitterWnd()->GetLeftView()->SetFocus();
	return TRUE;
}



//Message an Treeview senden
void CTabCtrlView::HandleMsgToLeftView(UINT cmd) 
{ 
	CWaxDoc *doc = (CWaxDoc*)GetDocument();
	if (!doc->get_active_object())
		return;
		
	POSITION pos = runtimeObjects.FindIndex(currentTab);
	XASSERT(pos);
	if (pos)
	{
		WaxPartForm *tmp = dynamic_cast<WaxPartForm*>(runtimeObjects.GetAt(pos));
		if (!tmp || !tmp->m_hWnd)
			return;
		if (cmd == WAXID_PRJLEITKARTE && dynamic_cast<PrjLeitkarteForm*>(tmp)) // Rekursion darf nicht vorkommen
			return;
		if (tmp->UpdateData(TRUE))
		{
			m_pMainSplitterFrame->GetSplitterWnd()->GetLeftView()->SendMessage(WM_COMMAND, cmd); 
		}
	}
} 

void CTabCtrlView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	/**
	Linke Maustaste wurde gedrückt
	Setzt den Focus auf den neu ausgewählten Tab
	*/
	CCtrlView::OnLButtonDown(nFlags, point);
	changeCurrentTab();
}

void CTabCtrlView::DoDataExchange(CDataExchange* pDX)
{
	/**
	Datenaustausch
	Diese Methode wird benutzt, um die View zu initialisieren, bzw. durch einen Aufruf von
	<mf .DoFinalUpdateData> UpdateData() für den aktuellen Tabdialog aufzurufen. Eigentlich 
	sollte hier >mf .OnInitialUpdate> aufgerufen werden
	*/
	UNREFERENCED_PARAMETER(pDX);
	if (!m_Init)
	{
		m_Init = true;
		if (!on_initial_update())
			AfxThrowUserException();
		return;
	}
	
	if (!do_data_exchange(pDX->m_bSaveAndValidate ? true : false))
		AfxThrowUserException();
}

//Fensterstil setzen
BOOL CTabCtrlView::PreCreateWindow (CREATESTRUCT& cs)
{
    // Initialize the common controls library
    ::InitCommonControls ();
    // Give the base class a chance to initialize
    if (!CCtrlView::PreCreateWindow (cs))
		return FALSE;
    // Apply default control styles
    cs.style |= TCS_SINGLELINE | TCS_TABS ;//| WS_HSCROLL| WS_VSCROLL;
    return TRUE;
}    

// Alle Tabdialogdaten einlesen
bool CTabCtrlView::do_data_exchange(bool saveandvalidate)
{
	WaxPartForm* form = NULL;
	for (POSITION pos = runtimeObjects.GetHeadPosition(); pos != NULL;)
	{
		form = (WaxPartForm*)runtimeObjects.GetNext(pos);
		if (!form)
			continue;
		if (!form->UpdateData(saveandvalidate))
			return false;
	}
	if (form)
		form->check_changes();
	return true;
}

BOOL CTabCtrlView::PreTranslateMessage(MSG* pMsg) 
{
	
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		(pMsg->wParam == VK_TAB || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT))
	{
		setNextTab(GetAsyncKeyState(VK_SHIFT) ? true : false);
		changeCurrentTab();
		GetTabCtrl().SetCurFocus(currentTab);
		return TRUE;
	}

	BOOL ret = CCtrlView::PreTranslateMessage(pMsg);
	WaxPart *pt = NULL;

// Message wird vom aktiven WaxPart bearbeitet
	if (pMsg->message == WM_COMMAND)
	{
		CWaxDoc *doc = (CWaxDoc*)GetDocument();
		XASSERT(doc->get_active_object());
		pt = doc->get_active_object();
		if (!pt)
			return FALSE;
		ret = pt->PreTranslateMessage(pMsg);
		if (ret)
		{
			// WaxPart hat die Message verarbeitet. Jetzt wird autosave aufgerufen.
			doc->doAutosave(true);
		}
		
	}
	tooltip->RelayEvent(pMsg);
	if (pt && pt->needDlgUpdate())
	{
		UpdateData(FALSE);
	}
	
	return 0;
}



void CTabCtrlView::OnSize(UINT nType, int cx, int cy) 
{
	/**
	Tabdialog in der Grösse ändern
	Alle Tabdialog müssen ebenfalls in der Grösse geändert werden
	*/
	CCtrlView::OnSize(nType, cx, cy);
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetTabCtrl().GetClientRect(&tabRect);
	GetTabCtrl().GetItemRect(0, &itemRect);
	itemRect.top += 5;
	itemRect.bottom += 5;
	
	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;
	int i = 0;

	// Fenstergrösse für Tabdialoge muss auch geändert werden
	for (POSITION pos = runtimeObjects.GetHeadPosition(); pos != NULL;)
	{
		WaxPartForm *tmp = dynamic_cast<WaxPartForm*>(runtimeObjects.GetNext(pos));
		XASSERT(tmp);
		if (tmp)
			tmp->SetWindowPos(&wndTop, nX, nY, nXc, nYc, i++ == currentTab ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	}
}



bool CTabCtrlView::on_initial_update() 
{
	/**
	View initialisieren
	Diese Methode wird bei Views innerhalb eines Splitterwindows nicht automatisch aufgerufen, 
	so dass ein Aufruf an anderer geeigneter Stelle vorgenommen wird. Hier ist das <mf .DoDataExchange>.

	Wenn das Programm an dieser Stelle mit unbehandelter Exception abstürzt, muss das
	RTTI im Projekt aktiviert werden:(Projekte/Einstellungen/C/C++/Programmiersprache C++)
	*/
	CCtrlView::OnInitialUpdate();

	CWaxDoc *doc = (CWaxDoc*)GetDocument();
	XASSERT(doc->get_active_object());
	if (!doc->get_active_object())
		return false;
	
	// Das WaxPart Objekt liefert seine Dialoge in runtimeModules zurück. Diese Dialoge werden der 
	// Reihenfolge zur Erstellung der Tabs benutzt.
	WaxPart *pt = (WaxPart *)doc->get_active_object();
	runtimeModules.RemoveAll();
	pt->getForms(runtimeModules);
	
	// Font für die Tabüberschriften setzen. Standard ist Systemfont, was nicht besonders gut aussieht.
	// Für Dialog ist MS Sans Serif üblich
	int i = 0;
	CFont font;
	font.CreateFont(12, 5, 0, 0, FW_SEMIBOLD/**FW_BOLD*/, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_TT_ALWAYS, PROOF_QUALITY, FF_SWISS   , "MS Sans Serif");
	GetTabCtrl().SetFont(&font, TRUE);

	CRect itemRect;	
	// Grösse der Tabüberschriften muss einberechnet werden
	GetTabCtrl().GetItemRect(0, &itemRect);
	
	
	
	RECT rect;
	int maxdx = 0;
	int maxdy = 0;

	// doUpdate disablen, da sonst bei jedem Reiter, der beim ersten Element eine Message gesetzt hat,
	// die Grafik neu erzeugt wird. Nach dem Aufbau der Reiter wird doUpdate wieder enabled.
	pt->set_enable_update(false);
	POSITION pos;

	for( int i = m_ArrayListCtrls.GetSize()-1 ; 0 <= i; i--)
	{	
		m_ArrayListCtrls[i]->DestroyWindow();
		delete m_ArrayListCtrls[i];
		m_ArrayListCtrls.RemoveAt(i);
		GetTabCtrl().DeleteItem(i);
	
	}
	//GetTabCtrl().DeleteAllItems();
	runtimeObjects.RemoveAll();

	for (pos = runtimeModules.GetHeadPosition(); pos != NULL;)
	{
		// Dialogobjekt erzeugen
		CObject *rtmod = runtimeModules.GetNext(pos);
		CObject *crtmod = ((CRuntimeClass*)rtmod)->CreateObject();

		WaxPartForm *tmp = dynamic_cast<WaxPartForm*>(crtmod);
		m_ArrayListCtrls.Add(tmp);
		XASSERT(tmp); // Klassen in Liste müssen von WaxPartForm abgeleitet sein !!
		if (!tmp)
			continue;
		
		// Der Name des Tabs ist in den Resourcen zu definieren als String mit derselben ID wie der Dialog
		CString tit = tmp->get_name();
		// Tab zu Acceleratortab hinzufügen
		addToAccelTab(i, tit);
		// Text bis zum ersten Newline als Titel verwenden. Text nach dem Newline wird zur Popup Hilfe benutzt
		GetTabCtrl().InsertItem( TCIF_TEXT | TCIF_PARAM, i++, tit, 0, 100+i);
				
		tmp->createForm(this, ((CWaxDoc*)GetDocument())->get_active_object(), GetDocument());
		tmp->ShowWindow(SW_HIDE);
		tmp->UpdateData(FALSE);
		tmp->GetWindowRect(&rect);

		if ((rect.bottom - rect.top) > maxdy)
			maxdy = rect.bottom - rect.top;
		if ((rect.right - rect.left) > maxdx)
			maxdx = rect.right - rect.left;
		
		runtimeObjects.AddTail(tmp);
	}
	
	CRect clientRect;	
	// Grösse der Tabüberschriften muss einberechnet werden
	GetTabCtrl().GetClientRect(&clientRect);

	// Alle Dialoge im Tabcontrol auf die maximale Breite und Höhe setzen, damit sie ordentlich
	// in das Fenster passen
	WaxPartForm *tmp = NULL;
	for (pos = runtimeObjects.GetHeadPosition(); pos != NULL;)
	{
		tmp = dynamic_cast<WaxPartForm*>(runtimeObjects.GetNext(pos));
		XASSERT(tmp);
		if (!tmp)
			return false;
		tmp->SetWindowPos(&wndTop, itemRect.left, itemRect.bottom+6, maxdx, maxdy + itemRect.bottom, SWP_HIDEWINDOW);
	}

	// Der letzte Dialog prüft auf Änderungen, deshalb muss auch nur der letzte Dialog den CRC erzeugen
	if (tmp)
		tmp->initCrc();

	i = GetTabCtrl().GetItemCount() - 1;
	if (i >= 0)
	{
		int seltab = activeTab > i ? i: activeTab;
		
		GetTabCtrl().SetCurSel(seltab);
		currentTab = seltab;
		WaxPartForm *tmp = dynamic_cast<WaxPartForm*>(runtimeObjects.GetAt(runtimeObjects.FindIndex(currentTab)));
		XASSERT(tmp);
		tmp->ShowWindow(SW_SHOW);
		GetTabCtrl().SetWindowPos(&wndTop, itemRect.left, itemRect.top, maxdx, maxdy+ itemRect.bottom, SWP_SHOWWINDOW);
	}		
	#define TTS_BALLOON 0x40
	if(!tooltip)
	{
		tooltip = new CToolTipCtrl;
		tooltip->Create(this, TTS_BALLOON);
	}
	pt->set_enable_update();
	return true;
}

void CTabCtrlView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	/**
	Tastatureingaben abfangen
	Eine vernünftige Tastaturbedienung ist nur dann möglich, wenn zwischen Baum und Eingabemasken
	gewechselt werden kann. Standardmässig ist dies nicht implementiert. Diese Methode fängt 
	die Tasten F6, TAB und LEFT, RIGHT, UP und DOWN ab. 

	F6 wechselt von der Eingabe zum Baum
	TAB wechselt in die Eingabemaske 
	LEFT und RIGHT verhalten sich wie TAB und SHIFTTAB
	UP und DOWN wechseln zu den Feldern in der Eingabemase
	*/
	CCtrlView::OnKeyDown(nChar, nRepCnt, nFlags);

	// Per Hotkey den richtigen Tab auswählen
	if (nChar >= 'a' && nChar <= 'z' || nChar >= 'A' && nChar <= 'Z')
	{
		/**
		bekannte Schwäche des folgenden Algorithmus: Bei mehreren gleichen Hotkeys, wird immer der
		erste gefundene ausgewählt. Wirkt wie unwillkürliches hin- und herspringen.
		*/
		multimap<int, char>::iterator it;
		for (it = acceltab.begin(); it != acceltab.end(); it++)
		{
			if (it->second == (char)nChar && it->first != currentTab)
			{
				GetTabCtrl().SetCurSel(it->first);
				changeCurrentTab();
				GetTabCtrl().SetCurFocus(currentTab);
				return;
			}
		}
		return;
	}
	switch (nChar)
	{
		case VK_F6:
			m_pMainSplitterFrame->GetSplitterWnd()->GetLeftView()->SetFocus();; 
			break;
		case VK_DOWN:
		case VK_TAB:
			{
				// Vor dem Umschalten wird die Eingabe geprüft
				POSITION pos = runtimeObjects.FindIndex(currentTab);
				XASSERT(pos);
				if (pos)
				{
					WaxPartForm *tmp = (WaxPartForm*)((CRuntimeClass*)runtimeObjects.GetAt(pos));
					tmp->SetFocus();
				}
			}
			break;
		case VK_LEFT:
			setNextTab(true);
			changeCurrentTab();
			GetTabCtrl().SetCurFocus(currentTab);
			break;
		case VK_RIGHT:
			setNextTab(false);
			changeCurrentTab();
			GetTabCtrl().SetCurFocus(currentTab);
			break;
		case VK_UP:
		case VK_NEXT:
			setNextTab(false);
			changeCurrentTab();
			break;
		case VK_PRIOR:
			setNextTab(false);
			changeCurrentTab();
			break;
	}
}


void CTabCtrlView::changeCurrentTab()
{
	/**
	Anzeige mit Tab Control abgleichen
	Diese Methode kann aufgerufen werden, um die aktuelle Auswahl des Tabs mit dem angezeigten Dialog abzugleichen
	*/
	if(currentTab == GetTabCtrl().GetCurFocus()) 
		return;
	// Vor dem Umschalten wird die Eingabe geprüft
	POSITION pos = runtimeObjects.FindIndex(currentTab);
	XASSERT(pos);
	bool changefocus = true;
	if (pos)
	{
		WaxPartForm *tmp = dynamic_cast<WaxPartForm*>((CObject*)runtimeObjects.GetAt(pos));

		
		
		XASSERT(tmp);
		if (!tmp)
			return;
		
		
		tmp->get_data()->get_werkauftrag()->set_plausiOk(true);
				
		if (!tmp->UpdateData(TRUE))
		{
			tmp->get_data()->get_werkauftrag()->set_plausiOk(false);
		}
		else
		{
			if (!tmp->check_plausis())
				tmp->get_data()->get_werkauftrag()->set_plausiOk(false);
		}
			
		
		
		CWnd *foc = GetFocus();
		if (foc == this)
				changefocus = false;
		tmp->ShowWindow(SW_HIDE);
	}
	// Dateneingabe OK, jetzt kann der neue Dialog angezeigt werden
	currentTab = activeTab = GetTabCtrl().GetCurFocus(); // neu angezeigten Tab in statischer Variable setzen
	pos = runtimeObjects.FindIndex(currentTab);
	XASSERT(pos);
	if (pos)
	{
		WaxPartForm *tmp = (WaxPartForm*)((CRuntimeClass*)runtimeObjects.GetAt(pos));
		tmp->UpdateData(FALSE);
		tmp->ShowWindow(SW_SHOW);
		if (changefocus)
		{
			tmp->SetFocus();
		}
	}
}

void CTabCtrlView::setNextTab(bool leftdir)
{
	/**
	Auf nächsten oder vorigen Tab setzen
	Einsatz findet diese Methode, wenn Tastaturbefehle kommen
	Wenn der Tab selbst den Focus geändert hat (z.B. Left Taste), wird nichts geändert.
	*/
	if(currentTab == GetTabCtrl().GetCurFocus()) 
	{
		int maxindex = GetTabCtrl().GetItemCount() - 1;
		GetTabCtrl().SetCurSel(leftdir ? (currentTab-1 >= 0 ? currentTab : maxindex) : (currentTab+1 > maxindex ? 0 : currentTab+1));
	}
	
}

INT_PTR CTabCtrlView::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	/**
	Tooltipfunktion
	Diese Methode zeigt zu jedem Tab den Tooltiptext an. Leider funktioniert das eingebaute
	Toolfenster nicht. Deshalb wurde das Attribut 
	*/
	static int lasttab = -1;
	int res = CView::OnToolHitTest( point, pTI );
	
	if (pTI/** && res == 1*/)
	{
		pTI->lpszText = (LPSTR)malloc(2048);
		
		bool found = false;
		int i;
		for (i = 0; i < GetTabCtrl().GetItemCount(); i++)
		{
			RECT r;
			GetTabCtrl().GetItemRect( i, &r); 
			if (point.x >= r.left && point.x <= r.right && point.y >= r.top && point.y <= r.bottom)
			{
				found = true;
				break;
			}
		}
		
		if (found)
		{
			if (i != lasttab)
			{
				tooltip->SendMessage(TTM_ACTIVATE, FALSE);
				lasttab = i;
			}
			POSITION pos = runtimeObjects.FindIndex(i);
			XASSERT(pos);
			if (pos)
			{
				WaxPartForm *tmp = (WaxPartForm*)((CRuntimeClass*)runtimeObjects.GetAt(pos));
				if (tmp)
				{
					tooltip->AddTool( &GetTabCtrl(), tmp->get_name(false));
					tooltip->SendMessage(TTM_ACTIVATE, TRUE);
				}
			}	
		}
		else
			lasttab = -1;
	}
	return res;
}

void CTabCtrlView::PreSubclassWindow() 
{
	/**
	Subclassing Methode der MFC
	Wird hier benutzt zum Aufruf von EnableToolTips(). Dadurch wird ein Aufruf
	von <mf .OnToolHitTest> erzwungen. 
	*/
	CCtrlView::PreSubclassWindow();
	EnableToolTips(TRUE);
}


void CTabCtrlView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(nSBCode == SB_THUMBPOSITION)
	{
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void CTabCtrlView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNREFERENCED_PARAMETER(pScrollBar);
	UNREFERENCED_PARAMETER(nPos);
	UNREFERENCED_PARAMETER(nSBCode);
}


//Setzt activeTab auf 0 zurück
void CTabCtrlView::resetActiveTab()
{
	CTabCtrlView::activeTab = 0;
}

//Called to repaint a portion of the window.
void CTabCtrlView::OnPaint()
{
	CCtrlView::OnPaint();
	PostMessage(WM_COMMAND, IDUPDATETAB);
}

void CTabCtrlView::onUpdateTabs()
{
	if (!GetTabCtrl().GetItemCount())
		return;
	const int ntabs = GetTabCtrl().GetItemCount()+1;
	int *items = new int[ntabs];
	memset(items, -1, ntabs*sizeof(int));

	int i = 0;
	for (POSITION pos = runtimeObjects.GetHeadPosition(); pos != NULL; i++)
	{
		WaxPartForm *form = dynamic_cast<WaxPartForm*>(runtimeObjects.GetNext(pos));
		XASSERT(form);
		if (form)
			items[i] = form->get_highlight();
	}
	GetTabCtrl().highlightItems(items);
	delete []items;
}


void CTabCtrlView::addToAccelTab(int index, const CString& str)
{
	int i = 0;
	if ((i = str.Find('&', 0)) != -1 && i < str.GetLength())
	{
		acceltab.insert(make_pair(index, toupper(str.GetAt(i+1))));
	}
	else
		acceltab.insert(make_pair(index, toupper(str.GetAt(0))));

}

///Wird aufgerufen, wenn die View aktiviert oder deaktiviert wird  
void CTabCtrlView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CView::OnActivateView( bActivate, pActivateView, pDeactiveView);
	// Nur, wenn die Tabview verlasen wird, die Pos. testen  
	if (!bActivate)
	{
#ifdef _DEBUG // Zum Deaktivieren dieses Kommandos, da dieses staendig gesandt wird beim debuggen
		if(CWaxApp::m_Posting_WM_COMMAND_IDTESTTAB_OnActivate)
#endif 
		{
			// gepostet, weil sonst beim Mauszeiger das Ikon und die Bezeichnung der Pos. gezeigt wird
			PostMessage(WM_COMMAND, IDTESTTAB);
		}
	}
}

///Position pruefen
void CTabCtrlView::onTestTabs()
{ 
	/*
	// Todo: Meldung kommt leider 2x, genau wie bei der Prüfung, 
	// wenn der Reiter gewechselt wird.
	POSITION pos = runtimeObjects.FindIndex(currentTab);
	XASSERT(pos);
	if (pos)
	{
		WaxPartForm *tmp = dynamic_cast<WaxPartForm*>((CObject*)runtimeObjects.GetAt(pos));
		XASSERT(tmp);
 		if (!tmp)
			return;
		
		if (!tmp->UpdateData(TRUE) || !tmp->check_plausis())
		{
			if (!tmp->check_plausis())
				tmp->get_data()->get_werkauftrag()->set_plausiOk(false);
			return; // Update hat nicht geklappt
		}
		tmp->get_data()->get_werkauftrag()->set_plausiOk(true);
	}
	*/
}
