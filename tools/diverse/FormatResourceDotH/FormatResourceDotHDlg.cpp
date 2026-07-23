// FormatResourceDotHDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "FormatResourceDotH.h"
#include "FormatResourceDotHDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
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
// CFormatResourceDotHDlg Dialogfeld

CFormatResourceDotHDlg::CFormatResourceDotHDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormatResourceDotHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormatResourceDotHDlg)
	m_iCommand = 0;
	m_iControls = 0;
	m_iStrings = 0;
	m_File = _T("");
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFormatResourceDotHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormatResourceDotHDlg)
	DDX_Text(pDX, IDC_EDIT_COMMANDS, m_iCommand);
	DDV_MinMaxInt(pDX, m_iCommand, 0, 65500);
	DDX_Text(pDX, IDC_EDIT_CONTROLS, m_iControls);
	DDV_MinMaxInt(pDX, m_iControls, 0, 65500);
	DDX_Text(pDX, IDC_EDIT_STRINGTABLE, m_iStrings);
	DDV_MinMaxInt(pDX, m_iStrings, 0, 65500);
	DDX_Text(pDX, IDC_EDIT1, m_File);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFormatResourceDotHDlg, CDialog)
	//{{AFX_MSG_MAP(CFormatResourceDotHDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE, OnFile)
	ON_BN_CLICKED(IDC_BUTTON_ENUMERATE, OnButtonEnumerate)
	ON_EN_CHANGE(IDC_EDIT_STRINGTABLE, OnChangeEditStringtable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatResourceDotHDlg Nachrichten-Handler

BOOL CFormatResourceDotHDlg::OnInitDialog()
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
	
	// ZU ERLEDIGEN: Hier zusätzliche Initialisierung einfügen
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CFormatResourceDotHDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFormatResourceDotHDlg::OnPaint() 
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
HCURSOR CFormatResourceDotHDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFormatResourceDotHDlg::OnFile() 
{
	CString file, initpath;

	initpath = "d:\\";

	CFileDialog fo(TRUE, NULL, "Resource.h", 
		OFN_EXPLORER|OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , 
		"Header-Dateien (*.h)|*.h|Alle Dateien (*.*)|*.*||", this);
	fo.m_ofn.lpstrInitialDir = initpath;

	if (fo.DoModal() == IDOK)
	{
		m_File = fo.GetPathName();
		UpdateData(FALSE);
	}
	
	
}

void CFormatResourceDotHDlg::OnButtonEnumerate() 
{
	CStringList *all[] = {&m_strLstFoot, &m_strLstHead, &m_strLst, &m_strLstID, &m_strLstIDR, &m_strLstIDC, &m_strLstIDI, &m_strLstIDS, &m_strLstIDD, NULL };
	
	int index = 0;
	while (all[index])
	{
		all[index]->RemoveAll();
		index++;
	}


	TRY
	{
		//CFile
		CString bakfile(m_File);
		bakfile += ".bak";

		CStdioFile f(m_File, CFile::modeRead | CFile::shareDenyNone);
		::CopyFile(m_File, bakfile, FALSE);
		
		m_strLst.RemoveAll();
		m_lineCounter = m_iStrings;
		m_iAPSOffset = 0;

		CString str;
		while (f.ReadString(str))
			processString(str);
		
		f.Close();
		writeFile();
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL
}

void CFormatResourceDotHDlg::OnChangeEditStringtable() 
{
	UpdateData(TRUE);
	m_iCommand = m_iStrings + 1000;
	m_iControls = m_iStrings + 2000;
	UpdateData(FALSE);
}

void CFormatResourceDotHDlg::processString(CString str)
{
	CString line(str);
	if (line.Find("#define") == -1)
	{
		CStringList *all[] = {&m_strLstFoot, &m_strLst, &m_strLstID, &m_strLstIDR, &m_strLstIDC, &m_strLstIDI, &m_strLstIDS, &m_strLstIDD, NULL };
		int index = 0;
		while (all[index])
		{
			if (all[index]->IsEmpty())
				index++;
			else
				return;
		}
		m_strLstHead.AddTail(line+"\n");
		return;
	}
	if (line.Find("_APS") == -1)
	{

		
		int index = line.ReverseFind('\t');
		if (index <= 0)
			index = line.ReverseFind(' ');
		if (index <= 0)
			return;
		
		line = line.Left(index);
		line.TrimLeft();
		line.TrimRight();

		if (line.Find("ID_") != -1)
			m_strLstID.AddTail(line);
		
		else if (line.Find("IDS_") != -1)
			m_strLstIDS.AddTail(line);
		
		else if (line.Find("IDI_") != -1)
			m_strLstIDI.AddTail(line);
		
		else if (line.Find("IDR_") != -1)
			m_strLstIDR.AddTail(line);
		
		else if (line.Find("IDC_") != -1)
			m_strLstIDC.AddTail(line);

		else if (line.Find("IDD_") != -1)
			m_strLstIDD.AddTail(line);
		else
			m_strLst.AddTail(line);
	}
}

void CFormatResourceDotHDlg::writeFile()
{
	TRY
	{
		CString fname = m_File;
		CStdioFile f(fname, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone);
		CStringList *all[] = {&m_strLst, &m_strLstID, &m_strLstIDR, &m_strLstIDC, &m_strLstIDI, &m_strLstIDS, &m_strLstIDD, NULL };
		CStringList *lst = all[0];

		for (POSITION pos = m_strLstHead.GetHeadPosition(); pos != NULL;)
		{
			f.WriteString(m_strLstHead.GetNext(pos));
		}
		
		int index = 1;
		while (lst)
		{
			for (POSITION pos = lst->GetHeadPosition(); pos != NULL;)
			{
				CString outline;
				if (outline.GetLength() < 12)
					outline.Format("%s\t\t\t\t\t%i\n", lst->GetNext(pos), m_lineCounter++); 
				else if (outline.GetLength() < 16)
					outline.Format("%s\t\t\t%i\n", lst->GetNext(pos), m_lineCounter++); 
				else if (outline.GetLength() < 25)
					outline.Format("%s\t\t%i\n", lst->GetNext(pos), m_lineCounter++); 
				else 
					outline.Format("%s\t%i\n", lst->GetNext(pos), m_lineCounter++); 
				f.WriteString(outline);
			}
			lst = all[index++];			
		}
		writeAPSEntries();		
		for (POSITION pos = m_strLstFoot.GetHeadPosition(); pos != NULL;)
		{
			f.WriteString(m_strLstFoot.GetNext(pos));
		}	
		
	}
	CATCH_ALL(e)
	{
		e->ReportError();
	}
	END_CATCH_ALL

}

void CFormatResourceDotHDlg::writeAPSEntries()
{
	//CStringList
	CString apsentry;
	
	m_strLstFoot.AddTail("// Next default values for new objects\n");
	m_strLstFoot.AddTail("// \n");
	m_strLstFoot.AddTail("#ifdef APSTUDIO_INVOKED\n");
	m_strLstFoot.AddTail("#ifndef APSTUDIO_READONLY_SYMBOLS\n");
	
	int offset = m_lineCounter - m_iStrings;

	apsentry.Format("#define _APS_NEXT_RESOURCE_VALUE\t\t%i\n", m_lineCounter);
	m_strLstFoot.AddTail(apsentry);

	apsentry.Format("#define _APS_NEXT_CONTROL_VALUE\t\t%i\n", m_iControls + offset);
	m_strLstFoot.AddTail(apsentry);

	apsentry.Format("#define _APS_NEXT_SYMED_VALUE\t\t%i\n", m_iControls + offset);
	m_strLstFoot.AddTail(apsentry);

	apsentry.Format("#define _APS_NEXT_COMMAND_VALUE\t\t%i\n", m_iCommand + offset);
	m_strLstFoot.AddTail(apsentry);
	m_strLstFoot.AddTail("#endif\n");
	m_strLstFoot.AddTail("#endif\n");
	
		
}
