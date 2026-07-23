#pragma once


class CWaxView;
class WaxPart;


#include <mxcontrols/bcmenu.h>
#include "WaxSplitterWnd.h"




class CMehrfachDruckFrameWnd;
class CChildFrame:public CMDIChildWnd  
{

	private:

	UINT m_Cmd;
	/// Breite der LeftView, wird beim Beenden gespeichert
	int wndWidth;

	bool m_bDruckVorschau;

	/// true wenn das Grafikfenster unterhalb der Eingabe angezeigt werden soll
	bool m_OnlineGraficInStaticLowerSplitter;

	///Anstossen vom Autosave
	void OnTimer(UINT_PTR timerid);

	///Setzt den Timer auf den Wert, der in der Registry unter "Common"/"Autosave" eingestellt ist.
	void setTimer();

	///Sorgt für ein UpdateData(TRUE) für alle Tabs
	bool updateAllTabs();

	///Gibt das aktuell ausgewählte Objekt zurück
	WaxPart *getActiveWaxPart();
	BOOL ShowDrawingsWithHeaderForPrinting(WaxPart *pt);
	BOOL ShowDrawingOnly(WaxPart* pt);
	BOOL ShowDialogforWaxDataInput(WaxPart* pt);

	void onFilePrintDirect();
	void onFilePrint();
	void onUpdateFilePrint(CCmdUI* pCmdUI);

	CWaxDoc* m_pCurrentDoc;

	DECLARE_DYNCREATE(CChildFrame)
	DECLARE_MESSAGE_MAP()

 protected:
	/// Erstes Splitterwindow: Enthält CLeftView und m_wndSplitter2
	WaxSplitterWnd m_wndSplitter;

	/// In der Registry gespeicherte Fenstergrösse für Grafikfenster
	RECT *m_rViewSize;

	/// Hilfsobjekt zur Erstellung von Splitterwindows siehe <mf .OnUpdateView>.
	CCreateContext m_CreateContext;

	/// Grafik anzeigen (Entspricht dem Menüpunkt)
	bool m_bGrafikView;


	void OnViewGrafik();
	void OnUpdateViewGrafik(CCmdUI* pCmdUI);
	void OnDruckvorschau();
	void onExternPrintpreview();
	void OnUpdateDruckvorschau(CCmdUI* pCmdUI);
	/// Schaltet Online Grafik an und aus
	void OnNografik();
	void OnUpdateNografik(CCmdUI * pCmdUI);
	/// Wrapper Methode für die Freigabe. Die Funktion in der Toolbar wird nur 
	/// enabled, wenn auch ein Eintrag in der MessageMap vorhanden ist. Die 
	/// Freigabe wird allerdings vom WaxPart behandelt.
	void onFreigabe();
	void onPreisanfrageFreigabe();
	void onFreigabeAngebot();
	void onFreigabeAngebotsPreis();
	void onEmailBesteller();
	
	void init_crc();

	void onUpdateFreigabe(CCmdUI* pCmdUi);
	/// Menüpunkt Preise
	void OnPreise();
	/// Menüpunkt Excel
	void OnExcel();
	/// Menüpunkt SAP
	void OnSap();
	/// Blendet das Projektfenster ein oder aus indem die Breite auf 0 bzw. letzten Wert gesetzt wird
	void onShowexplore();
	void onUpdateShowexplore(CCmdUI* cmdui);
	void onNewelement();
	void onUpdateNewelement(CCmdUI* pCmdUI);
	void onEnddruckvorschau();
	void onProjektPreise();
	LRESULT OnNavigateAnhang(WPARAM, LPARAM);

	virtual BOOL DestroyWindow( );

 public:
	
	
	/// Konstruktor
	CChildFrame();
	WaxSplitterWnd* GetSplitterWnd()
	{
		return &m_wndSplitter;
	}

	void SetCentralDoc(CWaxDoc* pDocument)
	{
		m_pCurrentDoc = (CWaxDoc*) pDocument;
	}

	CWaxDoc* GetCentralDoc()
	{
		if(m_pCurrentDoc == NULL)
		{
			return NULL;
		}
		else
		{
			return m_pCurrentDoc;
		}
	}
	
	/// View updaten
	void OnUpdateView();

	bool onUpdateView();

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void ActivateFrame(int nCmdShow = -1);

	/// Virtueller Destruktor
	virtual ~CChildFrame();

	void set_DruckVorschau(bool value);

	const bool get_DruckVorschau() const;


	afx_msg void OnSize(UINT nType, int cx, int cy);
};

