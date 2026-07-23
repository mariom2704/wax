#include "stdafx.h"

#if !defined _LEFTVIEW_
#define _LEFTVIEW_

#include "..\waxprj\iconmanager.h"

class CWaxDoc;

/**
Anzeige des Projektbaumes 
Diese Klasse zeigt den Explorerbaum im linken Fenster an.
*/

class CChildFrame;
class CLeftView : public CTreeView  
{
	DECLARE_DYNCREATE(CLeftView)
	DECLARE_MESSAGE_MAP()
private:
	IconManager iconman;
	

	/// Liefert das ChildFrame Fenster
	CWnd* getChildFrame();
	CChildFrame* m_pMainSplitterFrame;
	/// Liefert das aktuelle Eingabefenster der linken Seite.
	CWnd* getCurrentRightTabbedView();

	/// Wird gesetzt, wenn ein neues Objekt im Baum eingefügt wird. Beim erneuten 
	/// Baumaufbau wird dieses Objekt ausgewählt.
	CObject* nextSelect;

	/// Wird auf true gesetzt, wenn die Bezeichnung editiert wird. Dann ist z.B. 
	/// ein Löschen nicht möglich.
	bool labelEdit;

	/// Ruft die OnUpdatexxx Methoden zu den übergebenen Menüids auf
	bool testCommand(UINT id);

	/// Bearbeitet das Popupmenu vor der Anzeige
	void modifyPopupMenu(CMenu* menu, bool modify = true);

	/// Baut den Baum ab einem bestimmten Item neu auf
	void doTreeUpdate(HTREEITEM item);

	/// Kopierfunktion für Drag and Drop (drag) und Cut and Paste (ins)
	HTREEITEM doCopy(HTREEITEM drag, HTREEITEM drop, bool bCopy, CObject *ins = NULL);
	/// Baumansicht updaten
	void OnUpdateTree();
	HTREEITEM itemDrop;
	HTREEITEM itemDrag;
	CImageList * dragImage;
	bool isDragging;
	/// Imageliste des Baumes
	CImageList imageList;

protected: 

	/// Geschützter Konstruktor
	CLeftView();

	/// Geschützter Destruktor
	virtual ~CLeftView();	

	
	/// Nächstes Fenster
	BOOL OnNextPaneCmd(UINT nID);

	/// / Prüfungen bei Sammelauftrag vor dem Einfügen von Positionen
	bool check_Sammelauftrag(WaxPart *pt, WaxPart *ins);
	
public:
	void SetMainSplitter(CChildFrame* pMainSplitterFrame)
	{
		 m_pMainSplitterFrame = pMainSplitterFrame;
	}
	/// Dokumentklasse zurückliefern
	CWaxDoc* GetDocument();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	/// Indicates the user finished editing a item label
	void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

protected:
	virtual void OnInitialUpdate();

	/// Rechte Maustaste wurde gedrückt
	void OnRButtonDown(UINT nFlags, CPoint point);

	BOOL CopyToClipboard(CObject *obj, bool resetStatus = false) const;
	WaxPart* GetSelection();
	/// Neues Teil einfügen
	void insertObj(WaxPart *obj);

	///  Neues Teil anhängen
	WaxPart* addObj(WaxPart *obj);

	/// Kontextmenü
	void OnContextMenu(CWnd* pWnd, CPoint point) ;

	/// Kopieren von Positionen
	void OnCopy();

	/// Menüpunkt "Kopieren" an/abblenden
	void OnUpdateCopy(CCmdUI* pCmdUI);

	/// Einfügen aus Zwischenablage
	void OnPaste();

	/// Menüpunkt "OnPaste" an/abblenden
	void OnUpdatePaste(CCmdUI* pCmdUI);

	/// Kopieren von Positionen
	void OnCopyToFile();

	/// Menüpunkt "Kopieren" an/abblenden
	void OnUpdateCopyToFile(CCmdUI* pCmdUI);

	/// Einfügen aus Datei
	void OnPasteFromFile();

	/// Menüpunkt "OnPasteFromFile" an/abblenden
	void OnUpdatePasteFromFile(CCmdUI* pCmdUI);

	/// Position ausschneiden
	void OnCut();

	/// Menüpunkt "Ausschneiden" an/abblenden
	void OnUpdateCut(CCmdUI* pCmdUI);

	/// Position löschen
	void OnDelete();

	/// Menüpunkt "Löschen" an/abblenden
	void OnUpdateDelete(CCmdUI* pCmdUI);


	void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	/// Einfügen einer Formstücke und Fittinge Bestellung
	void onNeuFormstueckeundfittinge();
	/// Neues Strangrohr einfügen
	void onNeuesStrangrohr();
	/// Neues Gewinderohr einfügen
	void onGewinderohr();
	/// Neues Verteilerrohr einfügen
	void onNeuerVerteiler();
	/// Neuen Stationsverteiler einfügen
	void onStationsVerteiler();

	void onCOIIVerteiler();

	void onMX1230MX200Verteiler();

	void onSammelTaskViaList();

	//void onComandSammelTaskViaList(CCmdUI* pCmdUI);

	/// Einfügen einer neuen Untergliederung in den Hauptzweig des Projektes
	void onNeueUntergliederungRoot();

	/// Einfügen einer neuen Untergliederung
	void onNeueUntergliederung();
	
	/// Einfügen eines neuen Sammelauftrages
	void onNeuerSammelauftrag();

	void onNeuePreisanfrage();

	/// /Bezeichnung des Werkauftrages ändern
	void onEditlabel();
	
	/// Ruft die Statusaktualisierung in WaxDoc auf
	void onUpdatestatus();
	
	/// Startet Voloview mit der aktuellen Grafik
	void onVoloview();
	void onNeueEntnahmeleitung();
	void onNeuePumpenprobierleitung();
	void onNeuerPumpenverteiler();
	void onNeuHandelslaenge();
	void onNeuApax();
	void onEditCopy();
	void onEditPaste();
	void onEditCut();
	void onEditDelete();
	void onNewelement();
	/// Called when the user double-clicks the left mouse button.
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	/// Called when the user double-clicks the right mouse button.
	void OnRButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
};




#endif 
