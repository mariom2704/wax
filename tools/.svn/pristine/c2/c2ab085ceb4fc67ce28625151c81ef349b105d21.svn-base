// SplitterWindowTutorialView.h : interface of the CSplitterWindowTutorialView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLITTERWINDOWTUTORIALVIEW_H__A4706335_62EE_40D5_9F0A_3D6398BE7AD5__INCLUDED_)
#define AFX_SPLITTERWINDOWTUTORIALVIEW_H__A4706335_62EE_40D5_9F0A_3D6398BE7AD5__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#include "SplitterWindowTutorialDoc.h"
#endif // _MSC_VER > 1000

class CSplitterWindowTutorialView : public CView
{
protected: // create from serialization only
	CSplitterWindowTutorialView();
	DECLARE_DYNCREATE(CSplitterWindowTutorialView)

// Attributes
public:
	CSplitterWindowTutorialDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWindowTutorialView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplitterWindowTutorialView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterWindowTutorialView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SplitterWindowTutorialView.cpp
inline CSplitterWindowTutorialDoc* CSplitterWindowTutorialView::GetDocument()
   { return (CSplitterWindowTutorialDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERWINDOWTUTORIALVIEW_H__A4706335_62EE_40D5_9F0A_3D6398BE7AD5__INCLUDED_)
