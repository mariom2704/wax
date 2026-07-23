#pragma once

/**
Splitterwindow
Eine Ableitung ist nötig, da bei Tastaturbedienung ein Drücken von F6 nur 
zu einem SetFocus() von CSplitterWnd führt. Es wird der Focus aber nicht auf Pane 0,0
gesetzt. Damit ist das Programm dann nur noch mit der Maus bedienbar. 

Diese Ableitung überschreibt SetFocus() und setzt den Focus auf die Eingabemaske. Weiterhin wird
bei F6 der Focus nicht auf das nächste Pane (Grafikansicht) gesetzt, sondern zurück auf den 
Baum.
*/

#include "WaxHtmlView.h"
#include "LeftView.h"

		#include "WaxSplitterChildView.h"


class WaxSplitterWnd : public CSplitterWnd  
{
	DECLARE_MESSAGE_MAP()

	private:
		map<CWnd*, UINT> views;
		bool m_bJScripFileIsLoaded;
		CString m_strJScriptURL;
		CString m_strJScriptHeader;
		CString m_strHeaderRows;
		bool m_bIsChildSplitterActive;
		CWaxSplitterChildView* m_pChildSplitter;
		static bool m_bLoadingDWFViewerFirsttime;

		enum eINNERVIEWTYPE 
		{
			eSTATIC_VIEW,
			eUPPER_DUMMY_VIEW,
			eLOWER_DUMMY_VIEW,
		};


	public:

		WaxSplitterWnd();

		static void SetLoadingDWFViewerFirsttime(bool bLoadingDWFViewerFirsttime)
		{
			m_bLoadingDWFViewerFirsttime = bLoadingDWFViewerFirsttime;
		}

		static bool GetLoadingDWFViewerFirsttime()
		{
			return m_bLoadingDWFViewerFirsttime;
		}

		virtual void SetActivePane( int row, int col, CWnd* pWnd = NULL );
		virtual void OnSetFocus( CWnd* pOldWnd );
		BOOL OnNextPaneCmd(UINT nID);
		CWaxSplitterChildView*  GetWaxSplitterChildView();

		enum eOUTERVIEWTYPE 
		{
			eWAXGENERICVIEW = eLOWER_DUMMY_VIEW + 1,
			eWAXTABVIEW,
			eWAXHTMLVIEW,
			eWAXJSCRIPTVIEW,
		};
		
		
		virtual ~WaxSplitterWnd()
		{
			if(m_pChildSplitter)
			{
				delete m_pChildSplitter;
			}
		};

		CView* GetViewPtr(UINT id);
		CView* SwitchView(UINT id, int paneRow, int paneCol);
		CView* SwitchStaticChild(eOUTERVIEWTYPE uiUpperView, eOUTERVIEWTYPE uiLowerView);
		CView* AddSwitchableView(UINT id , CRuntimeClass*, CCreateContext* pContext, int uiRow = -1, int uiCol = -1);
		BOOL AddSwitchableStatic( CCreateContext* pContext, UINT uiRow = 0, UINT uiCol = 0);
		bool FindViewOrWindow(CWnd* pWnd);
		UINT FindIdOfViewOrWindow(CWnd* pWnd);
		void Navigate(CString strURL, eOUTERVIEWTYPE WAXHtmlView, BOOL bDrawingOnly, CString strHeader = "-1", CString strRows = "-1" );
		BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
		BOOL SetInitializeTab(bool bInitializeTab);
		void RecalcLayout();
		void LoadJScriptTemplate();
		CLeftView* GetLeftView();
		LRESULT OnNavigateJScript(WPARAM, LPARAM);
		bool GetIsChildSplitterActive()
		{
			return m_bIsChildSplitterActive;
		}
		
};
