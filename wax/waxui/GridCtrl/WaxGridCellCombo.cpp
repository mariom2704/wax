#include "stdafx.h"
#include "WaxGridCellCombo.h"


CWaxInPlaceList::CWaxInPlaceList(CWnd* pParent, CRect& rect, DWORD dwStyle,  UINT nID, int nRow, int nColumn, COLORREF crFore, 
				COLORREF crBack,   CStringArray& Items, CString sInitText,   UINT nFirstChar):
CInPlaceList(pParent,  rect,  dwStyle,  nID, nRow,  nColumn, crFore, 
				 crBack,    Items,  sInitText,   nFirstChar)
{
	m_pParent = (CGridCtrl*) pParent;
};



BEGIN_MESSAGE_MAP(CWaxInPlaceList, CInPlaceList)
	//{{AFX_MSG_MAP(CInPlaceList)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWaxInPlaceList::OnSelchange() 
{
	 
	int iIndex = GetCurSel();
	m_pParent->SendMessage(WM_LBUTTONDOWN_POSTED, iIndex);
}


HBRUSH CWaxInPlaceList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CInPlaceList::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_LISTBOX)
	{
		if(m_ListBox.GetSafeHwnd() == NULL)
		{
			m_ListBox.SubclassWindow(pWnd->GetSafeHwnd());
	
		}
	}
	return hbr;
}



IMPLEMENT_DYNCREATE(CWaxGridCellCombo, CGridCellCombo)

CWaxGridCellCombo::CWaxGridCellCombo(void)
{}


CWaxGridCellCombo::~CWaxGridCellCombo(void)
{}


BOOL CWaxGridCellCombo::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{

    m_bEditing = TRUE;
    
    // CInPlaceList auto-deletes itself
    m_pEditWnd = new CWaxInPlaceList(GetGrid(), rect, GetStyle(), nID, nRow, nCol, 
                                  GetTextClr(), GetBackClr(), m_Strings, GetText(), nChar);

	return TRUE;
};



		
	
