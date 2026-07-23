#include "stdafx.h"
#include "WaxView.h"
#include "WaxHtmlView.h"
#include "TabCtrlView.h"
#include "JScriptViewTemplate.h"
#include "ChildFrm.h"
#include "..\WAXPRJ\JScriptDruck.h"
	
		#include "WaxSplitterWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




BEGIN_MESSAGE_MAP(WaxSplitterWnd, CSplitterWnd)
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_NAVIGATE_JSCRIPT, OnNavigateJScript)
END_MESSAGE_MAP()


bool WaxSplitterWnd::m_bLoadingDWFViewerFirsttime = false;

WaxSplitterWnd::WaxSplitterWnd():m_pChildSplitter(NULL), m_bJScripFileIsLoaded(false), 
m_bIsChildSplitterActive(false)
{}


void WaxSplitterWnd::OnSetFocus( CWnd* pOldWnd )
{
	UNREFERENCED_PARAMETER(pOldWnd);
	GetPane(0,0)->SetFocus();
}


BOOL WaxSplitterWnd::OnNextPaneCmd(UINT nID)
{
	UNREFERENCED_PARAMETER(nID);
	AfxMessageBox("NextPane in WaxSplitterWnd");
	return 0;
}


void WaxSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd)
{
	// set the focus to the pane
	CWnd* pPane = pWnd == NULL ? GetPane(row, col) : pWnd;
	if (pPane->IsKindOf(RUNTIME_CLASS(CView)))
	{
		CFrameWnd* pFrameWnd = GetParentFrame();
		ASSERT_VALID(pFrameWnd);
		pFrameWnd->SetActiveView((CView*)pPane);
	}
	else
	{
		TRACE0("WaxSplitterWindow: Next pane is not a view - calling SetFocus.\n");
		pPane->SetFocus();
	}
}


BOOL  WaxSplitterWnd::AddSwitchableStatic(CCreateContext* pContext, UINT uiRow, UINT uiCol)
{
	
	m_pChildSplitter = new  CWaxSplitterChildView(this);
	
	UINT IdDynamic = 0;
	if(uiRow != 0 || uiCol != 0)
	{
		IdDynamic = IdFromRowCol(0, 1);
	}
	else
	{
		IdDynamic = eSTATIC_VIEW;
	}
	m_pChildSplitter->CreateStatic(this, 2, 1, WS_CHILD|WS_VISIBLE|WS_BORDER, IdDynamic);
	views[m_pChildSplitter] = eSTATIC_VIEW;

	m_pChildSplitter->CreateView(0, 0, RUNTIME_CLASS(CWaxView), CSize(1, 1), pContext);
	m_pChildSplitter->CreateView(1, 0, RUNTIME_CLASS(CWaxView), CSize(1, 1), pContext);
	
	CRect rect; GetParent()->GetWindowRect(rect);
	m_pChildSplitter->SetRowInfo(0, (rect.bottom - rect.top) * 8/10, 50);
	m_pChildSplitter->RecalcLayout();
	
	AddSwitchableView(eUPPER_DUMMY_VIEW, RUNTIME_CLASS(CWaxView), pContext);
	AddSwitchableView(eLOWER_DUMMY_VIEW, RUNTIME_CLASS(CWaxView), pContext);

   return TRUE;
}



CView* WaxSplitterWnd::AddSwitchableView(UINT id, CRuntimeClass * pView,
								 CCreateContext* pContext,
								  int uiRow, int uiCol)
{
	CWnd* pWin;
	DWORD style;

	pWin  = (CWnd*) pView->CreateObject();
	style = WS_CHILD;

	UINT IdDynamic = 0;
	if(uiRow == -1 && uiCol == -1)
	{
		IdDynamic = id;
	}
	else
	{
		style |=  WS_VISIBLE ;
		IdDynamic = IdFromRowCol(uiRow, uiCol);
	}

	pWin->Create(NULL, NULL, style, CRect(0,0,1, 1) , this, IdDynamic, pContext);
	pWin->SendMessage(WM_INITIALUPDATE);
   
	views[pWin] = id;
   
   return (CView*)pWin;
}




CView* WaxSplitterWnd::SwitchView(UINT id, int paneRow, int paneCol)
{

	if(	dynamic_cast<CWaxSplitterChildView*>(GetPane(paneRow, paneCol)))
	{
		m_pChildSplitter->SwitchChildView(eUPPER_DUMMY_VIEW, 0,0);
		m_pChildSplitter->SwitchChildView(eLOWER_DUMMY_VIEW, 1, 0);
	}

	CView* pOldView = (CView*) GetPane(paneRow, paneCol);

	if (pOldView == NULL) 
	{
	   return NULL;
	}

  
	CFrameWnd * mainWnd = (CFrameWnd *)AfxGetMainWnd();

	if (mainWnd == NULL) 
	{
	   return false;
	}
	

	bool bMultiSplitter = false;
	if(dynamic_cast<CWaxSplitterChildView*>( GetDlgItem(id)))
	{
	   bMultiSplitter = true;
	}
	CView* pNewView = (CView*) GetDlgItem(id); 

   if(pNewView == NULL )
   {
      return NULL;
   }

   if(mainWnd->GetActiveView() == pOldView)
   {
	   if(!bMultiSplitter)
	   {
  			mainWnd->SetActiveView(pNewView);
	   }
   }
   
   pOldView->ShowWindow(SW_HIDE);
   pNewView->ShowWindow(SW_SHOW);
   pNewView->SetDlgCtrlID(IdFromRowCol(paneRow, paneCol));

   CWnd* pWnd =(CWnd *)pOldView;

  if (views.find(pWnd) == views.end()) 
  {
	return NULL;
  }

   UINT uiAssociatedViewId = views[pWnd]; 

   pOldView->SetDlgCtrlID(uiAssociatedViewId); 

   RecalcLayout();

   pOldView->Invalidate();
   pNewView->Invalidate();
   m_bIsChildSplitterActive = false;

   return pNewView;
}



CView* WaxSplitterWnd::SwitchStaticChild(eOUTERVIEWTYPE uiUpperView, eOUTERVIEWTYPE uiLowerView)
{
	SwitchView(eSTATIC_VIEW, 0, 1);
   	m_pChildSplitter->SwitchStaticChildViews( uiUpperView, uiLowerView);
	m_pChildSplitter->SetDlgCtrlID(IdFromRowCol(0, 1));
	m_bIsChildSplitterActive = true;

   return 0;
}



bool WaxSplitterWnd::FindViewOrWindow(CWnd* pWnd)
{
	if(views.find(pWnd) == views.end())
	{
		return false;
	}
	return true;
}



CLeftView* WaxSplitterWnd::GetLeftView()
{
	return (CLeftView*)GetPane(0,0);
}



UINT WaxSplitterWnd::FindIdOfViewOrWindow(CWnd* pWnd)
{
	return views[pWnd];	
}



CView* WaxSplitterWnd::GetViewPtr(UINT id)
{
	map<CWnd*, UINT>::iterator It, Iend = views.end();

	for (It = views.begin(); It != Iend; It++)
	{
		if ((*It).second == id)
		{
			return (CView*) (*It).first;
		}
	}
	return NULL;
}



void WaxSplitterWnd::LoadJScriptTemplate()
{
	if(!m_bJScripFileIsLoaded)
	{
		
		CFileFind finder;
		std::string app = WAX_BACKSLASH_HTML;
		std::string temp = getenv("TEMP");
		if (!temp.length()) 
		{
			temp = getenv("TMP");
		}
	
		temp += "\\" + app ;
		CString strHTMLFile;
		CString strTempWaxHTML = CJScriptPrinting::get_tempdirWaxHTML().c_str();
		if(finder.FindFile(CString(temp.c_str()) + "\\" + WAX_VIEWER_FOR_HTML_DWF_AND_SVG_xHTML, 0) == TRUE)
		{
			strHTMLFile = strTempWaxHTML + _T("\\") + WAX_VIEWER_FOR_HTML_DWF_AND_SVG_xHTML;
		}
		else
		{
			strHTMLFile = strTempWaxHTML + _T("\\") + WAX_VIEWER_FOR_HTML_DWF_AND_SVG_HTML;
		}

		((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->Navigate(strHTMLFile);
		m_bJScripFileIsLoaded = true;
	}
	return;
}


CWaxSplitterChildView*  WaxSplitterWnd::GetWaxSplitterChildView()
{
	 return m_pChildSplitter;
}


LRESULT WaxSplitterWnd::OnNavigateJScript(WPARAM bDrawingOnly, LPARAM lParam)
{
	CStringArray strDrawingOnly;
	CString strTemp;
	strTemp.Format(_T("%d"), bDrawingOnly);
	((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->SetDrawingOnly(bDrawingOnly ? true : false);
	strDrawingOnly.Add(strTemp);

	((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->ExecuteJavaScriptFunction("DrawingOnly", strDrawingOnly);
	CStringArray aPath;
	aPath.Add(m_strJScriptHeader);
	((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->ExecuteJavaScriptFunction("SetHeader", aPath);

	if(m_strHeaderRows.Find("-1") == -1)
	{
		aPath.RemoveAll();
		aPath.Add(m_strHeaderRows);
		((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->ExecuteJavaScriptFunction("ShowHeaderRows", aPath);
	}
	
	Fortschrittanzeige*	progress = NULL;
	if( !m_bLoadingDWFViewerFirsttime && (m_strJScriptURL.Find(".dwf") > -1 || m_strJScriptURL.Find(".pdf") > -1))
	{
		progress = new Fortschrittanzeige;
		progress->set_minWert(0);
		progress->set_maxWert(8);
		progress->set_ueberschrift(getStammdaten().getText("IDS_CAPTION_LOAD_VIEWER").c_str());
		progress->setAnzeige1(getStammdaten().getText("IDS_LOAD_VIEWER").c_str());

		progress->start();
		progress->nextStep(1);
		m_bLoadingDWFViewerFirsttime = true;
	}
	
	aPath.RemoveAll();
	aPath.Add(m_strJScriptURL);
	((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->ExecuteJavaScriptFunction("ShowDwfSvgDrawing", aPath);
	
	if(progress)
	{
		progress->setAnzeige1(getStammdaten().getText("IDS_LOAD_VIEWER_FINISHED").c_str());
		progress->nextStep(6);
		progress->nextStep(8);
		Sleep(500);
		progress->ende();
		delete progress; 
	}
	((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->ShowWindow(SW_HIDE);
	((CJScriptViewTemplate*)GetViewPtr(WaxSplitterWnd::eWAXJSCRIPTVIEW))->ShowWindow(SW_SHOW);
	return 0;
}



void WaxSplitterWnd::Navigate(CString strURL, eOUTERVIEWTYPE eViewType, BOOL bDrawingOnly, CString strHeader, CString strRows)
{
	m_strJScriptURL	   = strURL;
	m_strJScriptHeader = strHeader;
	m_strHeaderRows	   = strRows;

	if(eViewType == eWAXJSCRIPTVIEW)
	{		
		PostMessage(WM_NAVIGATE_JSCRIPT, bDrawingOnly, 0);
	}
	else
	{
		((WaxHtmlView*)GetViewPtr(WaxSplitterWnd::eWAXHTMLVIEW))->Navigate(strURL);
	}
}



BOOL WaxSplitterWnd::UpdateData(BOOL bUpdate) 
{
	if(	dynamic_cast<CWaxSplitterChildView*>(GetPane(0, 1)))
	{	
		if(!m_pChildSplitter->GetPane(0, 0)->UpdateData(bUpdate))
		{		
			return FALSE;
		}
	}
	else
	{
		if(!GetPane(0, 1)->UpdateData(bUpdate))
		{		
			return FALSE;
		}
	}
	return TRUE;
}



BOOL WaxSplitterWnd::SetInitializeTab(bool bInitializeTab)
{
	if(	dynamic_cast<CWaxSplitterChildView*>(GetPane(0, 1)))
	{	
		((CTabCtrlView*)m_pChildSplitter->GetPane(0, 0))->SetInitializeTab(bInitializeTab);
	}
	else
	{
		if(dynamic_cast<WaxHtmlView*>(GetPane(0, 1)))
		{
			((WaxHtmlView*)GetPane(0, 1))->SetInitializeTab(bInitializeTab);
		}
		else
		{
			((CTabCtrlView*)GetPane(0, 1))->SetInitializeTab(bInitializeTab);
		}
	}
	return TRUE;
}



void WaxSplitterWnd::RecalcLayout()
{
	if(	dynamic_cast<CWaxSplitterChildView*>(GetPane(0, 1)))
	{	
		m_pChildSplitter->RecalcLayout();
	}
	CSplitterWnd::RecalcLayout();
	return;
}