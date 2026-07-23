/////////////////////////////////////////////////////////////////////////////
// TrayIcon.cpp : implementation file
//
// This is a conglomeration of ideas from the MSJ "Webster" application,
// sniffing round the online docs, and from other implementations such
// as PJ Naughter's "CTrayIconifyIcon" (http://indigo.ie/~pjn/ntray.html)
// especially the "CTrayIcon::OnTrayNotification" member function.
//
// This class is a light wrapper around the windows system tray stuff. It
// adds an icon to the system tray with the specified ToolTip text and 
// callback notification value, which is sent back to the Parent window.
//
// The tray icon can be instantiated using either the constructor or by
// declaring the object and creating (and displaying) it later on in the
// program. eg.
//
//		CTrayIcon m_TrayIcon;	// Member variable of some class
//		
//		... 
//		// in some member function maybe...
//		m_TrayIcon.Create(pParentWnd, WM_MY_NOTIFY, "Click here", 
//						  hIcon, nTrayIconID);
//
//  Written by Chris Maunder (Chris.Maunder@cbr.clw.csiro.au)
// 
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LEX.h"
#include "resource.h"
#include "TrayIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CTrayIcon, CObject)

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon construction/creation/destruction

CTrayIcon::CTrayIcon()
{
	memset(&m_tnd, 0, sizeof(m_tnd));
	m_bEnabled = FALSE;
	m_bHidden  = FALSE;
	m_bitmp.LoadBitmap("IDB_LAUFWERKE");
	m_bitmp_Fixed.LoadBitmap("IDB_LAUFWERKE_FEST");
	m_bitmp_Loose.LoadBitmap("IDB_LAUFWERKE_LOSE");
	m_bitmp_Floppy.LoadBitmap("IDB_LAUFWERKE_FLOPPY");
	m_bitmp_CD.LoadBitmap("IDB_LAUFWERKE_CD");
	m_bitmp_Zip.LoadBitmap("IDB_LAUFWERKE_ZIP");
}

CTrayIcon::CTrayIcon(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szToolTip, 
					 HICON icon, UINT uID)
{
	Create(pWnd, uCallbackMessage, szToolTip, icon, uID);
	m_bHidden = FALSE;
}

BOOL CTrayIcon::Create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szToolTip, 
					   HICON icon, UINT uID)
{
	// this is only for Windows 95 (or higher)
	VERIFY(m_bEnabled = ( GetVersion() & 0xff ) >= 4);
	if (!m_bEnabled) return FALSE;

	//Make sure Notification window is valid
	VERIFY(m_bEnabled = (pWnd && ::IsWindow(pWnd->GetSafeHwnd())));
	if (!m_bEnabled) return FALSE;
	
	//Make sure we avoid conflict with other messages
	ASSERT(uCallbackMessage >= WM_USER);

	//Tray only supports tooltip text up to 64 characters
	ASSERT(_tcslen(szToolTip) <= 64);

	// load up the NOTIFYICONDATA structure
	m_tnd.cbSize = sizeof(NOTIFYICONDATA);
	m_tnd.hWnd	 = pWnd->GetSafeHwnd();
	m_tnd.uID	 = uID;
	m_tnd.hIcon  = icon;
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_tnd.uCallbackMessage = uCallbackMessage;
	strcpy (m_tnd.szTip, szToolTip);

	// Set the tray icon
	VERIFY(m_bEnabled = Shell_NotifyIcon(NIM_ADD, &m_tnd));
	return m_bEnabled;
}

CTrayIcon::~CTrayIcon()
{
	RemoveIcon();
}


/////////////////////////////////////////////////////////////////////////////
// CTrayIcon icon manipulation

void CTrayIcon::MoveToRight()
{
	HideIcon();
	ShowIcon();
}

void CTrayIcon::RemoveIcon()
{
	if (!m_bEnabled) return;

	m_tnd.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_tnd);
    m_bEnabled = FALSE;
}

void CTrayIcon::HideIcon()
{
	if (m_bEnabled && !m_bHidden) {
		m_tnd.uFlags = NIF_ICON;
		Shell_NotifyIcon (NIM_DELETE, &m_tnd);
		m_bHidden = TRUE;
	}
}

void CTrayIcon::ShowIcon()
{
	if (m_bEnabled && m_bHidden) {
		m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &m_tnd);
		m_bHidden = FALSE;
	}
}

BOOL CTrayIcon::SetIcon(HICON hIcon)
{
	if (!m_bEnabled) return FALSE;

	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = AfxGetApp()->LoadIcon(lpszIconName);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetStandardIcon(LPCTSTR lpIconName)
{
	HICON hIcon = LoadIcon(AfxGetInstanceHandle( ), lpIconName);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetStandardIcon(UINT nIDResource)
{
	HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));

	return SetIcon(hIcon);
}
 
HICON CTrayIcon::GetIcon() const
{
	HICON hIcon = NULL;
	if (m_bEnabled)
		hIcon = m_tnd.hIcon;

	return hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon tooltip text manipulation

BOOL CTrayIcon::SetTooltipText(LPCTSTR pszTip)
{
	if (!m_bEnabled) return FALSE;

	m_tnd.uFlags = NIF_TIP;
	_tcscpy(m_tnd.szTip, pszTip);

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetTooltipText(UINT nID)
{
	CString strText;
	VERIFY(strText.LoadString(nID));

	return SetTooltipText(strText);
}

CString CTrayIcon::GetTooltipText() const
{
	CString strText;
	if (m_bEnabled)
		strText = m_tnd.szTip;

	return strText;
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon notification window stuff

BOOL CTrayIcon::SetNotificationWnd(CWnd* pWnd)
{
	if (!m_bEnabled) return FALSE;

	//Make sure Notification window is valid
	ASSERT(pWnd && ::IsWindow(pWnd->GetSafeHwnd()));

	m_tnd.hWnd = pWnd->GetSafeHwnd();
	m_tnd.uFlags = 0;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

CWnd* CTrayIcon::GetNotificationWnd() const
{
	return CWnd::FromHandle(m_tnd.hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CTrayIcon implentation of OnTrayNotification

LRESULT CTrayIcon::OnTrayNotification(UINT wParam, LONG lParam) 
{
	CString startverz = AfxGetApp()->GetProfileString( "Explorer", "Start", "C:\\");
	STARTUPINFO start;
	memset(&start, 0, sizeof(STARTUPINFO));
	start.dwFlags = STARTF_USESHOWWINDOW;
	start.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION inf;

	
	//Return quickly if its not for this tray icon
	if (wParam == WM_LAUFWERKE)
	{
		CString cmdline;
		cmdline.Format("explorer /e,%c:\\", (char)lParam);

		LPSTR proc = const_cast<LPSTR>((LPCSTR)cmdline);
		if (!CreateProcess(NULL, proc, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &start, &inf))
		{
			AfxMessageBox(IDS_NIX_EXPLORER_START, MB_ICONERROR | MB_OK);
			return 1;
		} // CreateProces CreateShellpro
		WaitForInputIdle( inf.hProcess, 5000); // Warten bis Winword gestartet ist
		
		return 1;
		
	}
	
	if (wParam != m_tnd.uID)
		return 0L;
	BCMenu menu;
	BCMenu::SetIconSize(20, 18);
	//BCMenu::SetMenuDrawMode(1);
	BCMenu *pSubMenu;

	// Clicking with right button brings up a context menu
	if (LOWORD(lParam) == WM_RBUTTONDOWN)
	{	
		CString startverz = AfxGetApp()->GetProfileString( "Explorer", "Start", "C:\\");
		CString cmdline;
		cmdline.Format("explorer /e,%s", startverz);

		LPSTR proc = const_cast<LPSTR>((LPCSTR)cmdline);
		if (!CreateProcess(NULL, proc, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &start, &inf))
		{
			AfxMessageBox(IDS_NIX_EXPLORER_START, MB_ICONERROR | MB_OK);
			return 1;
		} // CreateProces CreateShellpro
		WaitForInputIdle( inf.hProcess, 5000); // Warten bis der Explorer gestartet ist
		
		return 1;
	}
// Clicking with left button brings up a context menu
	else if (LOWORD(lParam) == WM_LBUTTONDOWN)
	{	
		UINT toolbars[] =	{
			IDR_TOOLBAR,
		};
		if (!menu.LoadMenu(m_tnd.uID)) return 0;
		menu.LoadToolbars(toolbars, 1);
		if (!(pSubMenu = dynamic_cast<BCMenu *>(menu.GetSubMenu(0)))) return 0;

		// Make first menu item the default (bold font)
		//::SetMenuDefaultItem(pSubMenu->m_hMenu, 1, TRUE);

		InitMenu(pSubMenu);
		
		//Display and track the popup menu
		CPoint pos;
		GetCursorPos(&pos);

		::SetForegroundWindow(m_tnd.hWnd);  
		::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, m_tnd.hWnd, NULL);

		// BUGFIX: See "PRB: Menus for Notification Icons Don't Work Correctly"
		::PostMessage(m_tnd.hWnd, WM_USER, 0, 0);

		menu.DestroyMenu();
	} 
	return 1;
}


CString GetVolumeName(const char *path)
{
    const DWORD BufferLength = 64;
    char  VolumeNameBuffer[_MAX_PATH];
    char  FileSystemNameBuffer[BufferLength];

    DWORD MaximumComponentLength;
    DWORD FileSystemFlags;

    ::SetErrorMode(SEM_FAILCRITICALERRORS);

    if (!::GetVolumeInformation(path,
                                  VolumeNameBuffer,
                                  _MAX_PATH,
                                  NULL,
                                  &MaximumComponentLength,
                                  &FileSystemFlags,	
                                  FileSystemNameBuffer,
                                  BufferLength))
								  return "";

	CString ret;
	ret.Format(" - %s", VolumeNameBuffer);
    return ret;
}


void CTrayIcon::InitMenu(BCMenu *menu)
{
	return;
	char *ppDrive;
	static char pDrives[4096];
	static char pDriveStr[128];
	ppDrive = pDrives;
	DWORD len = ::GetLogicalDriveStrings(4096, pDrives);
	int i = 1;
	BCMenu *insert = menu;
	CBitmap *icon = NULL;
	return;
	while (ppDrive[0] != 0)
	{
		strcpy(pDriveStr, ppDrive);
		UINT drivetype = GetDriveType(ppDrive);
		strcat(pDriveStr, "\t");
			
		switch (drivetype)
		{
			case DRIVE_FIXED:
				strcat(pDriveStr, GetVolumeName(ppDrive)); 
				icon = &m_bitmp_Fixed;
			break;
			case DRIVE_REMOTE:
			{	
				char inf[2048];
				char fat[200];
				GetVolumeInformation(ppDrive, inf, 2048,NULL,NULL,NULL,fat,200);
				strcat(pDriveStr, GetVolumeName(ppDrive)); 
				strcat(pDriveStr, " [");strcat(pDriveStr, fat);strcat(pDriveStr, "]");
				icon = &m_bitmp_Loose;	
			}
			break;
			case DRIVE_REMOVABLE:
				icon = &m_bitmp_Floppy;
			break;
			case DRIVE_CDROM:
				strcat(pDriveStr, GetVolumeName(ppDrive));
				icon = &m_bitmp_CD;
			break;
		}
		insert->AppendMenu(MF_STRING|MF_ENABLED, WM_LAUFWERKE+pDriveStr[0], pDriveStr);
		insert->SetMenuText(WM_LAUFWERKE+pDriveStr[0], pDriveStr);
		//insert->SetMenuItemBitmaps(WM_LAUFWERKE+pDriveStr[0], MF_BYCOMMAND, icon, icon);	
		insert->EnableMenuItem(MF_ENABLED|MF_BYCOMMAND, WM_LAUFWERKE+pDriveStr[0]);
		
		i++;
		while (*ppDrive++ != 0);
	}
}
