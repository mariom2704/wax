// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E2B26DD7_886E_11D1_9BF5_00A0243D1382__INCLUDED_)
#define AFX_MAINFRM_H__E2B26DD7_886E_11D1_9BF5_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "TrayIcon.h"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CTrayIcon m_TrayIcon;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	static void openPath(const CString path);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPopupOption1();
	afx_msg void OnPopupOption2();
	afx_msg void OnHilfe();
	afx_msg void OnOpenhome();
	afx_msg void OnOpentemp();
	afx_msg void OnAltstartup();
	afx_msg void OnOpendesktop();
	afx_msg void OnOpenfonts();
	afx_msg void OnOpeninternet();
	afx_msg void OnOpenprograms();
	afx_msg void OnOpenrecent();
	afx_msg void OnOpensendto();
	afx_msg void OnOpenstartmenu();
	afx_msg void OnOpentemplates();
	//}}AFX_MSG
	afx_msg void OnLaufwerke(UINT id);
	afx_msg LONG OnTrayNotification(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
private:
	void openSpecialFolder(int folder) const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E2B26DD7_886E_11D1_9BF5_00A0243D1382__INCLUDED_)
