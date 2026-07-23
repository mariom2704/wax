#include "stdafx.h"
#include "updateAssist.h"
#include "updateAssistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ListCtrl *listbx = NULL;

#define WM_DOUPDATE (WM_USER+111)


CUpdateAssistDlg::CUpdateAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateAssistDlg::IDD, pParent), list(WM_DOUPDATE), assi(&list)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdateAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, list);
	DDX_Control(pDX, IDC_PROGRESS1, progr);
}

BEGIN_MESSAGE_MAP(CUpdateAssistDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXECUTE, onExecute)
	ON_BN_CLICKED(WM_DOUPDATE, on_do_update)
END_MESSAGE_MAP()


BOOL CUpdateAssistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
	
	listbx = &list;
	listbx->InsertColumn(0, "Datei", LVCFMT_LEFT, 300);
	listbx->InsertColumn(1, "Version", LVCFMT_LEFT, 150);
	listbx->InsertColumn(2, "Erstellungs/Änderungsdatum", LVCFMT_LEFT, 150);
	listbx->InsertColumn(3, "gesperrt", LVCFMT_LEFT, 180);
	listbx->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );


	return TRUE;  
}

void CUpdateAssistDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

HCURSOR CUpdateAssistDlg::OnQueryDragIcon()
{
	/**
	Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
	das zum Symbol verkleinerte Fenster mit der Maus zieht.
	*/
	return (HCURSOR) m_hIcon;
}

void CUpdateAssistDlg::onExecute() 
{
	set_actions();
	
	CWaitCursor w;

	list.DeleteAllItems();
	vector<string> stdnl, dbs, exe;
	read_ini(stdnl, dbs, exe);

	assi.add_output_string("Standardniederlassungen");
	
	progr.SetRange32(0, stdnl.size());
	progr.SetPos(0);
	progr.SetStep(1);
	vector<string>::iterator it;
	for (it = stdnl.begin(); it != stdnl.end(); it++)
	{
		assi.process_stdnl(*it);
		progr.StepIt();
	}


	if (((CButton *)GetDlgItem(IDC_DBVER))->GetCheck())
	{
		progr.SetRange32(0, dbs.size());
		progr.SetPos(0);
		assi.add_output_string("Datenbankeinträge");
		for (it = dbs.begin(); it != dbs.end(); it++)
		{
			assi.checkLastUpdate(*it);
			progr.StepIt();
		}
	}
	
	if (((CButton *)GetDlgItem(IDC_EXEVER))->GetCheck())
	{
		progr.SetRange32(0, exe.size());
		progr.SetPos(0);
		assi.add_output_string("Programmeinträge");
		for (it = exe.begin(); it != exe.end(); it++)
		{
			assi.check_version_info(*it);
			progr.StepIt();
		}
	}
}


void CUpdateAssistDlg::read_ini(vector<string> &nl, vector<string> &dbs, vector<string> &exe)
{
	string entry[] = { "[standardnl]", "[dbver]", "[exever]", "" };
	vector<string> *lsts[] = { &nl, &dbs, &exe };

	for (int i = 0; entry[i].length(); i++)
	{
		if (!assi.read_ini(entry[i], *lsts[i]))
			return;
		TRACE("Size: %i\n", lsts[i]->size());
	}
}


void CUpdateAssistDlg::set_actions()
{
	assi.reset_actions();
	if (((CButton *)GetDlgItem(IDC_SPANXDB))->GetCheck())
		assi.add_action(assistant::SPANXDB);

	if (((CButton *)GetDlgItem(IDC_WAXDB))->GetCheck())
		assi.add_action(assistant::WAXDB);

	if (((CButton *)GetDlgItem(IDC_SPANX_EXE))->GetCheck())
		assi.add_action(assistant::SPANX_EXE);

	if (((CButton *)GetDlgItem(IDC_SPLEDIT_EXE))->GetCheck())
		assi.add_action(assistant::SPLEDIT_EXE);

	if (((CButton *)GetDlgItem(IDC_WAX_EXE))->GetCheck())
		assi.add_action(assistant::WAX_EXE);

	if (((CButton *)GetDlgItem(IDC_WAX_EXE_ONLY))->GetCheck())
		assi.add_action(assistant::WAX_EXE_ONLY);

	if (((CButton *)GetDlgItem(IDC_STAMMDB_WAX))->GetCheck())
		assi.add_action(assistant::STAMMDB_WAX);

	if (((CButton *)GetDlgItem(IDC_STAMMDB_SPANX))->GetCheck())
		assi.add_action(assistant::STAMMDB_SPANX);
	
}

void CUpdateAssistDlg::on_do_update()
{
	CString exe;
	int idx = listbx->getSelectedItem(0, &exe);

	if (assi.do_update((string)(LPCSTR)exe))
		assi.check_version_info((string)(LPCSTR)exe, idx);
}