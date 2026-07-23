// SplitterWindowTutorial.h : main header file for the SPLITTERWINDOWTUTORIAL application
//

#if !defined(AFX_SPLITTERWINDOWTUTORIAL_H__B1908DE6_C1E7_4534_BA59_E550AD3E58F8__INCLUDED_)
#define AFX_SPLITTERWINDOWTUTORIAL_H__B1908DE6_C1E7_4534_BA59_E550AD3E58F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSplitterWindowTutorialApp:
// See SplitterWindowTutorial.cpp for the implementation of this class
//

class CSplitterWindowTutorialApp : public CWinApp
{
public:
	CSplitterWindowTutorialApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWindowTutorialApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSplitterWindowTutorialApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERWINDOWTUTORIAL_H__B1908DE6_C1E7_4534_BA59_E550AD3E58F8__INCLUDED_)
