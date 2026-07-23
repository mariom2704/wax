// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LEX.h"

#include "MainFrm.h"
#include "Einstellungen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WM_ICON_NOTIFY			WM_USER+10


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND_RANGE(WM_LAUFWERKE, WM_LAUFWERKE+'Z', OnLaufwerke)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_POPUP_OPTION1, OnPopupOption1)
	ON_COMMAND(ID_POPUP_OPTION2, OnPopupOption2)
	ON_COMMAND(ID_HILFE, OnHilfe)
	ON_COMMAND(ID_OPENHOME, OnOpenhome)
	ON_COMMAND(ID_OPENTEMP, OnOpentemp)
	ON_COMMAND(ID_OPENSTARTUP, OnAltstartup)
	ON_COMMAND(ID_OPENDESKTOP, OnOpendesktop)
	ON_COMMAND(ID_OPENFONTS, OnOpenfonts)
	ON_COMMAND(ID_OPENINTERNET, OnOpeninternet)
	ON_COMMAND(ID_OPENPROGRAMS, OnOpenprograms)
	ON_COMMAND(ID_OPENRECENT, OnOpenrecent)
	ON_COMMAND(ID_OPENSENDTO, OnOpensendto)
	ON_COMMAND(ID_OPENSTARTMENU, OnOpenstartmenu)
	ON_COMMAND(ID_OPENTEMPLATES, OnOpentemplates)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CString hint;
	hint.LoadString(IDS_HINT);
	// Create the tray icon
	if (!m_TrayIcon.Create(this, WM_ICON_NOTIFY, hint, NULL, IDR_POPUP_MENU))
		return -1;

	m_TrayIcon.SetStandardIcon("MainFrame");
	
	return 0;
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	// Delegate all the work back to the default implementation in
	// CTrayIcon.
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame/CTrayIcon menu message handlers

void CMainFrame::OnPopupOption1() 
{
	Einstellungen e(NULL);
	e.DoModal();
}

void CMainFrame::OnPopupOption2() 
{
	MessageBox(_T("You chose Option 2"));
}

void CMainFrame::OnLaufwerke(UINT id)
{
	char Laufwerk = id - WM_LAUFWERKE;
	m_TrayIcon.OnTrayNotification(WM_LAUFWERKE, Laufwerk);
}

void CMainFrame::OnHilfe() 
{
	CDialog d(IDD_DIALOG_HILFE, this);
	d.DoModal();	
}

void CMainFrame::OnOpenhome() 
{
	char home[2*MAX_PATH];
	memset(home, 0, 2*MAX_PATH);
	HRESULT res = SHGetSpecialFolderPath(this->m_hWnd, home, CSIDL_PERSONAL, FALSE);
	// Liefert 1 zurück, sollte aber 0 sein. Pfad ist aber ok?????!!!
	//if (res != NOERROR)
	//	return;
	openPath(home);	
}

void CMainFrame::openPath(const CString path)
{
	STARTUPINFO start;
	memset(&start, 0, sizeof(STARTUPINFO));
	start.dwFlags = STARTF_USESHOWWINDOW;
	start.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION inf;

	CString cmdline;
	cmdline.Format("explorer /e,%s", path);

	LPSTR proc = const_cast<LPSTR>((LPCSTR)cmdline);
	if (!CreateProcess(NULL, proc, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &start, &inf))
	{
		AfxMessageBox(IDS_NIX_EXPLORER_START, MB_ICONERROR | MB_OK);
		return;
	} // CreateProces CreateShellpro
	WaitForInputIdle( inf.hProcess, 5000); // Warten bis Winword gestartet ist

}

void CMainFrame::OnOpentemp() 
{
	char home[2*MAX_PATH];
	memset(home, 0, 2*MAX_PATH);
	GetTempPath(2*MAX_PATH, home);
	// Liefert 1 zurück, sollte aber 0 sein. Pfad ist aber ok?????!!!
	//if (res != NOERROR)
	//	return;
	openPath(home);		
}

void CMainFrame::OnAltstartup() 
{
	char home[2*MAX_PATH];
	memset(home, 0, 2*MAX_PATH);
	HRESULT res = SHGetSpecialFolderPath(this->m_hWnd, home, CSIDL_STARTUP  , FALSE);
	// Liefert 1 zurück, sollte aber 0 sein. Pfad ist aber ok?????!!!
	//if (res != NOERROR)
	//	return;
	openPath(home);	

	
}

void CMainFrame::OnOpendesktop() 
{
	openSpecialFolder(CSIDL_DESKTOP);	
}

void CMainFrame::OnOpenfonts() 
{
	openSpecialFolder(CSIDL_FONTS);
}

void CMainFrame::OnOpeninternet() 
{
	openSpecialFolder(CSIDL_INTERNET);
	
}


void CMainFrame::OnOpenprograms() 
{
	openSpecialFolder(CSIDL_PROGRAMS);
}

void CMainFrame::OnOpenrecent() 
{
	openSpecialFolder(CSIDL_RECENT);	
}

void CMainFrame::OnOpensendto() 
{
	openSpecialFolder(CSIDL_SENDTO);
}

void CMainFrame::OnOpenstartmenu() 
{
	openSpecialFolder(CSIDL_STARTMENU);
}

void CMainFrame::OnOpentemplates() 
{
	openSpecialFolder(CSIDL_TEMPLATES);	
}

void CMainFrame::openSpecialFolder(int folder) const
{
	char home[2*MAX_PATH];
	memset(home, 0, 2*MAX_PATH);
	HRESULT res = SHGetSpecialFolderPath(this->m_hWnd, home, folder, FALSE);
	// Liefert 1 zurück, sollte aber 0 sein. Pfad ist aber ok?????!!!
	//if (res != NOERROR)
	//	return;
	openPath(home);	
}
