#include "stdafx.h"


#include "DialogHtmlView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(DialogHtmlView, CDialog)

DialogHtmlView::DialogHtmlView(CWnd* pParent /*=NULL*/)
	: CDialog(DialogHtmlView::IDD, pParent)
{
	m_url = _T("about:blank");
}

DialogHtmlView::~DialogHtmlView()
{
}

void DialogHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DialogHtmlView, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// DialogHtmlView-Meldungshandler

BOOL DialogHtmlView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  hier zusätzliche Initialisierung hinzufügen.
	m_browser.CreateFromControl(this,IDC_BROWSER);

	m_browser.Navigate(m_url);

	return TRUE;  // return TRUE unless you set the focus to a control
	// AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}

bool DialogHtmlView::printHtmlView()
{
	m_browser.Print(SimpleBrowser::PRINT_NO_PROMPT);

	//3 sec. warten damit der Druck abgeschlossen wird
	Sleep(3000);
	return true;
}

void DialogHtmlView::OnBnClickedOk()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.

	printHtmlView();


	OnOK();
}

void DialogHtmlView::set_url(const CString &url)
{
	m_url = url;
}