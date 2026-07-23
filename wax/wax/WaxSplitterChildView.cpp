// MultiSplitterView.cpp: implementation of the CWaxSplitterChildView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaxView.h"
#include "WaxSplitterWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CWaxSplitterChildView::CWaxSplitterChildView(WaxSplitterWnd* pMultiSpliittView)
{
	m_pMultiSpliittView = pMultiSpliittView;
};


CView* CWaxSplitterChildView::SwitchChildView(UINT id,  int paneRow, int paneCol)
{

   CView* pOldView = (CView*) GetPane(paneRow, paneCol); 

   if (pOldView == NULL) 
	{
	   return NULL;
	}

   
	CView* pNewView = (CView*) m_pMultiSpliittView->GetDlgItem(id);
	pNewView->SetParent(this);
	if(pNewView == NULL )
	{
      return NULL;
	}

	CFrameWnd * mainWnd = (CFrameWnd *)AfxGetMainWnd();

	if (mainWnd == NULL) 
	{
		   return false;
	}
		
	pOldView->ShowWindow(SW_HIDE);
	
	pNewView->ShowWindow(SW_SHOW);
	pNewView->SetDlgCtrlID(IdFromRowCol(paneRow, paneCol));

	CWnd * pWnd =(CWnd *)pOldView; 
	if (!m_pMultiSpliittView->FindViewOrWindow(pWnd))
	{
		return NULL;
	}

	UINT oldId = m_pMultiSpliittView->FindIdOfViewOrWindow(pWnd);
	pOldView->SetParent(m_pMultiSpliittView);
	pOldView->SetDlgCtrlID(oldId);

	RecalcLayout();

   pOldView->Invalidate();
   pNewView->Invalidate();

   return pNewView;
}




CView* CWaxSplitterChildView::SwitchStaticChildViews(UINT uiUpperView, UINT uiLowerView)
{

   CView* pOldView = (CView*) GetPane(0, 0); 

   if (pOldView == NULL)
	{
	   return NULL;
	}

	pOldView = (CView*) GetPane(1, 0); 

	if (pOldView == NULL)
	{
	   return NULL;
	}
	SwitchChildView(uiUpperView,0, 0);
		m_pMultiSpliittView->GetViewPtr(uiUpperView)->SetParent(this);
	
	SwitchChildView( uiLowerView, 1, 0);
		m_pMultiSpliittView->GetViewPtr(uiLowerView)->SetParent(this);
  

   CFrameWnd * mainWnd = (CFrameWnd *)AfxGetMainWnd();

   if (mainWnd == NULL)
   {
	  
	   return false;
   }
	
 //  if(mainWnd->GetActiveView() == pOldView)
 // 		mainWnd->SetActiveView(pNewView);
	
   ShowWindow(SW_SHOW);
   pOldView->ShowWindow(SW_HIDE);;

   pOldView->Invalidate();
   Invalidate();

   return 0;
}

