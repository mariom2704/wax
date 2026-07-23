#pragma once

#include <mxcontrols/tabctrl.h>


class WaxSplitterWnd;
class  CTabCtrlView : public CCtrlView  
{
	DECLARE_MESSAGE_MAP()
	friend class CChildFrame;
private:

	CChildFrame* m_pMainSplitterFrame;

	/// Fügt für den übergebenen Tabtitel eine Eintrag in die acceltab hinzu
	void addToAccelTab(int index, const CString& str);

	/// Liste mit Schnellzugriffstasten für die einzelnen Tabs. Wird beim Erzeugen 
	/// der Tabs initialisiert.
	std::multimap<int, char> acceltab;

	/// Aktiver Tab, wenn z.B. der zweite Tab ausgewählt, wird beim Umschalten auf 
	/// ein anderes Objekt automatisch der zweite Tab angezeigt
	static int activeTab;
	
	/// Tatsächlich aktiver Tab
	int currentTab;
	
	/// Flag für die Initialisierung
	bool m_Init;

	/// Font für die Überschrift der Tabs
	//CFont font;
	
	void onUpdateTabs();

	void onTestTabs();

	CToolTipCtrl *tooltip;

protected:
	/// Dynamische Erstellung verwendet geschützten Konstruktor
	CTabCtrlView();           
	DECLARE_DYNCREATE(CTabCtrlView)
	virtual INT_PTR OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	
public:
	void SetMainSplitter(CChildFrame* pMainSplitterFrame)
	{
		 m_pMainSplitterFrame = pMainSplitterFrame;
	};

	void SetInitializeTab(bool bInit)
	{
		m_Init = bInit;
	};
	/// Setzt activeTab auf 0 zurück
	static void resetActiveTab();

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	
	/// Alle Tabs updaten
	bool do_data_exchange(bool saveandvalidate);


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool on_initial_update();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung
	virtual void PreSubclassWindow();

	TabCtrl& GetTabCtrl () const { return *(TabCtrl*)this; };

	/// Nächstes Fenster
	 BOOL OnNextPaneCmd(UINT nID);
	
protected:
	/// Destruktor
	virtual ~CTabCtrlView();

	/// Fensterstil setzen
	virtual BOOL PreCreateWindow (CREATESTRUCT &cs);

	/// Message an Treeview senden
	void HandleMsgToLeftView(UINT cmd); 


	CString error;

	 void OnLButtonDown(UINT nFlags, CPoint point);
	 void OnSize(UINT nType, int cx, int cy);
	 int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	 void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	 void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	 void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	/// Called to repaint a portion of the window.
	 void OnPaint();
	

	/// Liste der Tabmodule (Runtime Classes)
	CObList runtimeModules;

	/// Liste der Tabmodule (Objekte)
	CObList runtimeObjects;
	CArray< WaxPartForm*, WaxPartForm* > m_ArrayListCtrls;

protected:
	/// Vertikale Scrollbar
	CScrollBar scrollVert;

	/// Horizontale Scrollbar
	CScrollBar scrollHorz;

	void setNextTab(bool leftdir);
	
	void changeCurrentTab();

	
};
