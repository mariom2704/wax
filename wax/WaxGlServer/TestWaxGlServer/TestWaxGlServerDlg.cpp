// TestWaxGlServerDlg.cpp : Implementierungsdatei
//
#include "stdafx.h"
#include "TestWaxGlServer.h"
#include "TestWaxGlServerDlg.h"
#include "../WaxGlServer/interface.h"
#include <boost/boost.h>
#include <boost/filesystem/operations.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// TestWaxGlServerDlg-Dialogfeld




TestWaxGlServerDlg::TestWaxGlServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TestWaxGlServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



TestWaxGlServerDlg::~TestWaxGlServerDlg()
{
	if (!convert_terminate())
	{
		TRACE("Enladen fehlgeschlagen");
	}
}

void TestWaxGlServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG, log);
	DDX_Control(pDX, IDC_EXPLORER1, viewer);
	DDX_Control(pDX, IDC_COMBO_FILEFORMAT, fileformat);
}

BEGIN_MESSAGE_MAP(TestWaxGlServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONVERT, &TestWaxGlServerDlg::OnBnClickedConvert)
	ON_BN_CLICKED(IDC_LOAD, &TestWaxGlServerDlg::OnBnClickedLoad)
END_MESSAGE_MAP()


// TestWaxGlServerDlg-Meldungshandler

BOOL TestWaxGlServerDlg::OnInitDialog()
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

	add_logstring("Lade Systemparameter...");
	if (!convert_init_waxsys())
	{
		add_logstring("Lade Systemparameter fehlgeschlagen");
	}

	add_logstring("Lade Stammdaten...");
	if (!convert_init_stammdb())
	{
		add_logstring("Lade Stammdaten fehlgeschlagen");
	}

	fileformat.SelectString(-1, "SVG");

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void TestWaxGlServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void TestWaxGlServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
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

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR TestWaxGlServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void TestWaxGlServerDlg::OnBnClickedConvert()
{
	std::string xml, content, fname;
	
	fname = get_selected_fileformat();
	std::string msg = boost::str( boost::format("Konvertiere zu %1%...") % fname );
	add_logstring(msg);
	
	xml = load_xml(xmlfile);
	
	if (get_selected_fileformat() == "DWF")
		content = convert2dwf(xml.c_str());
	else
		content = convert2svg(xml.c_str());


	fname = store_file(content);

	viewer.Navigate(fname.c_str(), NULL, NULL, NULL, NULL);
}

std::string TestWaxGlServerDlg::get_selected_fileformat() const
{
	return fileformat.GetCurSel() == 0 ? "DWF" : "SVG";
}


std::string TestWaxGlServerDlg::load_xml(const string& file) const
{
	string xmlcontent;
	ifstream myfile;
	myfile.open (xmlfile.c_str(), ios::binary);
	while (!myfile.eof())
	{
		string line;
		myfile >> line;
		xmlcontent += line + " ";
	}
	myfile.close();
	return xmlcontent;
}



std::string TestWaxGlServerDlg::store_file(const string& content) const
{

	const std::string fn = get_selected_fileformat() == "DWF" ? "tmp.dwf" : "tmp.svg";
	ofstream myfile;
	myfile.open (fn.c_str(), ios::trunc);
	myfile << content;
	myfile.close();

	boost::filesystem::path p = boost::filesystem::system_complete(".\\" + fn);

	return p.native_file_string();
}

void TestWaxGlServerDlg::add_logstring(const string& str)
{
	log.AddString(str.c_str());
	log.RedrawWindow();
}

void TestWaxGlServerDlg::OnBnClickedLoad()
{
	// szFilters is a text string that includes two file name filters:
   // "*.my" for "MyType Files" and "*.*' for "All Files."
   char szFilters[]=
      "XML Dateien (*.xml)|*.xml|Alle Dateien (*.*)|*.*||";

   // Create an Open dialog; the default file name extension is ".my".
   CFileDialog fileDlg (TRUE, "xml", "*.xml",
      OFN_NOCHANGEDIR| OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
   if( fileDlg.DoModal ()==IDOK )
   {
      CString pathName = fileDlg.GetPathName();
       
	  SetWindowText(pathName);
	  GetDlgItem(IDC_CONVERT)->EnableWindow();
	  xmlfile = (LPCSTR)pathName;
   }
}
