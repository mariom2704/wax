// SystemTray.h : main header file for the SYSTEMTRAY application
//

#if !defined(AFX_SYSTEMTRAY_H__E2B26DD3_886E_11D1_9BF5_00A0243D1382__INCLUDED_)
#define AFX_SYSTEMTRAY_H__E2B26DD3_886E_11D1_9BF5_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSystemTrayApp:
// See SystemTray.cpp for the implementation of this class
//
#define WM_LAUFWERKE (WM_USER + 123)
#define WM_EXEC_COMMAND (WM_USER + 200)

class CSystemTrayApp : public CWinApp
{
public:
	CSystemTrayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTrayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSystemTrayApp)
	afx_msg void OnAppAbout();
	afx_msg void OnLaufwerke();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRAY_H__E2B26DD3_886E_11D1_9BF5_00A0243D1382__INCLUDED_)
