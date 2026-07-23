#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
END_MESSAGE_MAP()

AboutDlg::AboutDlg() : CDialog(IDD_ABOUTBOX)
{
	version.LoadString(IDR_MAINFRAME);
}


void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	/**
	Datenaustausch der Dialogelemente. Hier wird die Versionsnummer angezeigt, wie in IDS_VERSION
	definiert
	*/
	CString copyright;
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VERSION, version);
	if (!pDX->m_bSaveAndValidate)
	{
		CString tmp;
		GetDlgItem(IDC_STATIC_COPYRIGHT)->GetWindowText(tmp);
		copyright.Format(tmp, COleDateTime::GetCurrentTime().GetYear());
	}
	DDX_Text(pDX, IDC_STATIC_COPYRIGHT, copyright);

}
