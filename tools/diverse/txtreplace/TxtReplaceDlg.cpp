// TxtReplaceDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "TxtReplace.h"
#include "TxtReplaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//@class  CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"
//@base public | CDialog

class CAboutDlg : public CDialog
{
public:
	////@cmember Konstruktor
	CAboutDlg();

// Dialogfelddaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTxtReplaceDlg Dialogfeld

CTxtReplaceDlg::CTxtReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTxtReplaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTxtReplaceDlg)
	m_toFind = _T("");
	m_replaceWith = _T("");
	m_status = _T("");
	m_bDeleteLine = FALSE;
	m_path = _T("");
	m_bganzesWort = FALSE;
	m_createBackup = TRUE;
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	udpateRegistry(false);
}

void CTxtReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTxtReplaceDlg)
	DDX_Text(pDX, IDC_EDIT_FIND, m_toFind);
	DDX_Text(pDX, IDC_EDIT_REPLACE_WITH, m_replaceWith);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Check(pDX, IDC_CHECK_DELETELINE, m_bDeleteLine);
	DDX_Text(pDX, IDC_EDIT_PATH, m_path);
	DDX_Check(pDX, IDC_CHECK_GANZES_WORT, m_bganzesWort);
	DDX_Check(pDX, IDC_CHECK_BACKUP, m_createBackup);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		udpateRegistry(true);
	}
}

BEGIN_MESSAGE_MAP(CTxtReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CTxtReplaceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_CHECK_DELETELINE, OnCheckDeleteline)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTxtReplaceDlg Nachrichten-Handler

BOOL CTxtReplaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{	
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	GetDlgItem(IDC_EDIT_REPLACE_WITH)->EnableWindow(!m_bDeleteLine);
	GetDlgItem(IDC_CHECK_GANZES_WORT)->EnableWindow(!m_bDeleteLine);
	
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CTxtReplaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void CTxtReplaceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR CTxtReplaceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTxtReplaceDlg::OnOK() 
{
	CWaitCursor w;
	CFileFind fFind;
	UpdateData(TRUE);
	
	m_statistics.init();
	BOOL bContinue = fFind.FindFile(m_path + "\\*.cpp");
	if (!bContinue)
	{
		fFind.Close();// the directory is empty; just close up and return.
	}
	else
	{
		doReplacement(fFind);
	}
	
	bContinue = fFind.FindFile(m_path + "\\*.h");
	if (!bContinue)
	{
		fFind.Close();
	}
	else
	{
		doReplacement(fFind);
	}
	
	m_status.Empty();
	UpdateData(FALSE);
	m_statistics.display();
}


/*
@mfunc Ruft den Browser <c BrowserDlg> auf und setzt m_Suchpfad
@comm Als Callbackfunktion für SHBrowseForFolder dient <mf ProjektSucheDlg::BrowseCallbackProc>
*/
void CTxtReplaceDlg::OnBrowse() 
{
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	char ach[MAX_PATH];
	strcpy (ach, m_path);
	
	bi.hwndOwner      = this->m_hWnd;
	bi.pidlRoot       = NULL;//(LPCSTR)m_Suchpfad;
	bi.pszDisplayName = ach;
	bi.lpszTitle      = NULL;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS;
	bi.lpfn           = (BFFCALLBACK)BrowseCallbackProc;
	bi.lParam         = 0;
	bi.iImage         = 0;

	pidl = SHBrowseForFolder(&bi);

	if (pidl)
	{
		UpdateData(TRUE);
		SHGetPathFromIDList(pidl, ach);
		m_path = ach;
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

CString CTxtReplaceDlg::m_path;
/*
@mfunc Callbackfunktion
@comm Erzwingt die Selektion des Suchpfades in der Browsermaske
*/
int CALLBACK CTxtReplaceDlg::BrowseCallbackProc(    HWND hwnd,     UINT uMsg, 
    LPARAM lParam,     LPARAM lpData    )
{
	UNREFERENCED_PARAMETER(lpData);
	UNREFERENCED_PARAMETER(lParam);
	if (uMsg == BFFM_INITIALIZED)
	{
		::PostMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCSTR)m_path);
	}
	return 0;
}


void CTxtReplaceDlg::applyOnFile(CString fname)
{
	m_status = fname;
		
	TRY
	{
		CString fn(m_path + (CString)"\\" + fname);
		CString backupfile(m_path + (CString)"\\~" + fname + (CString)".bak");

		CStdioFile f(fn, CFile::modeReadWrite | CFile::shareDenyNone );
		CString str;
		CString outstr;

		f.SeekToBegin();
		bool bupdateFile = false;

		while (f.ReadString(str))
		{
			int index = str.Find(m_toFind, 0);
			if ( index == -1)
			{
				outstr += str + "\n";
			}
			else
			{
				bupdateFile = true;	
				if (m_bDeleteLine) 
				{
					m_statistics.incReplaceCounter();
				}
				else // ganze Zeile löschen heisst hier: alle Zeilen mit String werden ignoriert
				{
					CString tmp;
					
					tmp = applyOnLine(str);

					outstr += tmp + "\n";
					
				}
			}
			
		}

		if (bupdateFile)
		{
			if (m_createBackup)
			{
				::CopyFile(fn, backupfile, FALSE);
			}
			
			f.SeekToBegin();
			f.SetLength(0);
			f.WriteString(outstr);
			m_statistics.incFileCounter();
		}
		// Dateiname wird auch geändert

		if (fname.Find(m_toFind) != -1)
		{
			CString oldfilename(m_path + (CString)"\\" + fname);
			
			CString nfname(fname);
			nfname = applyOnLine(fname);

			CString newfilename(m_path + (CString)"\\" + nfname);
			f.Close();
			TRY
			{
				if (oldfilename != newfilename)
				{
					f.Rename(oldfilename, newfilename);
					m_statistics.incRenamedFileCounter();
				}
			}
			CATCH_ALL(e)
			{
				e->ReportError();
			}
			END_CATCH_ALL
		}
	}
	CATCH_ALL(e)
	{
	//	e->ReportError();
	}
	END_CATCH_ALL
	UpdateData(FALSE);
}

void CTxtReplaceDlg::OnCheckDeleteline() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_REPLACE_WITH)->EnableWindow(!m_bDeleteLine);
	GetDlgItem(IDC_CHECK_GANZES_WORT)->EnableWindow(!m_bDeleteLine);
}

void CTxtReplaceDlg::doReplacement(CFileFind &fFind)
{
	BOOL bContinue = TRUE;
	while (bContinue) 
	{
		bContinue = fFind.FindNextFile();
		if (!fFind.IsDirectory() && !fFind.IsDots())
		{
			applyOnFile(fFind.GetFileName()); 
		}
	} 
	fFind.Close();
	
}

void CTxtReplaceDlg::udpateRegistry(bool bSave)
{
	if (bSave)
	{
		AfxGetApp()->WriteProfileString("init", "toFind", m_toFind);
		AfxGetApp()->WriteProfileString("init", "replaceWith", m_replaceWith);
		AfxGetApp()->WriteProfileString("init", "path", m_path);
		AfxGetApp()->WriteProfileInt("init", "bDeleteLine", m_bDeleteLine);
		AfxGetApp()->WriteProfileInt("init", "bganzesWort", m_bganzesWort);
	}
	else
	{
		m_toFind = AfxGetApp()->GetProfileString("init", "toFind", m_toFind);
		m_replaceWith = AfxGetApp()->GetProfileString("init", "replaceWith", m_replaceWith);
		m_path = AfxGetApp()->GetProfileString("init", "path", m_path);
		m_bDeleteLine = AfxGetApp()->GetProfileInt("init", "bDeleteLine", 0);
		m_bganzesWort = AfxGetApp()->GetProfileInt("init", "bganzesWort", 0);
	}
}

CString CTxtReplaceDlg::applyOnLine(const CString str)
{
	CString fname(str);
	int index = 0;
	CString tmp;
	while ((index = fname.Find(m_toFind, index)) != -1)
	{
		if (m_bganzesWort)
		{
			bool anfangok = false;
			bool endeok = false;
			if (index == 0 || fname[index-1] < 'A' || fname[index-1] > 'z') // Wortanfang gefunden
			{
				if (index == 0 || fname[index-1] < '0' || fname[index-1] > '9')
					anfangok = true;
			}
			if ((index +m_toFind.GetLength()) >= fname.GetLength() || fname[index+m_toFind.GetLength()] < 'A' || fname[index+m_toFind.GetLength()] > 'z')
			{
				if ((fname[index]+m_toFind.GetLength()) < '0' || (fname[index]+m_toFind.GetLength()) > '9')
					endeok = true;	
			}
			if (!anfangok || !endeok)
			{
				// Gefundener String ist nur Teil eines Wortes, überspringen
				index += m_toFind.GetLength();
				continue;
			}
		}
		tmp = fname.Left(index);
		tmp += m_replaceWith;
		tmp += fname.Right(fname.GetLength() - index - m_toFind.GetLength());
		fname = tmp;
		index += m_replaceWith.GetLength();
		m_statistics.incReplaceCounter();
	}
	TRACE("%s -> %s\n", str.Left(40), fname.Left(40));
	return fname;
}

