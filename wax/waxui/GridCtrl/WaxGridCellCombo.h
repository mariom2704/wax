#pragma once
#include "gridcellcombo.h"






class CWaxListBox : public CListBox
{};


class CWaxInPlaceList : public CInPlaceList
{

	public:
		CWaxInPlaceList(CWnd* pParent,  CRect& rect,  
					 DWORD dwStyle, UINT nID,           
					 int nRow, int nColumn,
					 COLORREF crFore, COLORREF crBack,
					 CStringArray& Items, CString sInitText,
					 UINT nFirstChar);

		CWaxListBox m_ListBox;
		CGridCtrl*  m_pParent;

	protected:

		//{{AFX_MSG(CWaxInPlaceList)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnSelchange();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()


};



class CWaxGridCellCombo : public CGridCellCombo
{
	DECLARE_DYNCREATE(CWaxGridCellCombo)
	public:
		CWaxGridCellCombo(void);
	    
		virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
		
		virtual ~CWaxGridCellCombo(void);
};
