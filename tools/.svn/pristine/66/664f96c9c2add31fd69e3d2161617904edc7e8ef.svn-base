// Einstellungen.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "lex.h"
#include "Einstellungen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld Einstellungen 


Einstellungen::Einstellungen(CWnd* pParent /*=NULL*/)
	: CDialog(Einstellungen::IDD, pParent)
{
	//{{AFX_DATA_INIT(Einstellungen)
	m_Verzeichnis = _T("");
	//}}AFX_DATA_INIT
	m_Verzeichnis = AfxGetApp()->GetProfileString( "Explorer", "Start", "C:\\");
}


void Einstellungen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Einstellungen)
	DDX_Text(pDX, IDC_EDIT_VERZEICHNIS, m_Verzeichnis);
	DDV_MaxChars(pDX, m_Verzeichnis, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Einstellungen, CDialog)
	//{{AFX_MSG_MAP(Einstellungen)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten Einstellungen 

void Einstellungen::OnOK() 
{
	CDialog::OnOK();
	AfxGetApp()->WriteProfileString( "Explorer", "Start", m_Verzeichnis);
}



static CString Suchpfad;
static int CALLBACK BrowseCallbackProc(    HWND hwnd,     UINT uMsg, 
    LPARAM lParam,     LPARAM lpData    )
{
	if (uMsg == BFFM_INITIALIZED)
	{
		::PostMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCSTR)Suchpfad);
	}
	return 0;
}

void Einstellungen::OnButton1() 
{
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	char ach[MAX_PATH];
	Suchpfad = m_Verzeichnis;

	bi.hwndOwner      = this->m_hWnd;
	bi.pidlRoot       = NULL;//(LPCSTR)m_Suchpfad;
	bi.pszDisplayName = ach;
	bi.lpszTitle      = NULL;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS;
	bi.lpfn           = (BFFCALLBACK)::BrowseCallbackProc;
	bi.lParam         = 0;
	bi.iImage         = 0;

	pidl = SHBrowseForFolder(&bi);

	if (pidl)
	{
		SHGetPathFromIDList(pidl, ach);
		m_Verzeichnis = ach;
		IMalloc *pm;
		SHGetMalloc(&pm);
		if (pm)
		{
				pm->Free(pidl);
				pm->Release();
		}
		UpdateData(FALSE);
	}
}
