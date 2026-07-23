#include "stdafx.h"



#include "DlgDate.h"


// DlgDate-Dialogfeld

IMPLEMENT_DYNAMIC(DlgDate, CDialog)

DlgDate::DlgDate(COleDateTime startdate, CWnd* pParent /*=NULL*/)
	: CDialog(DlgDate::IDD, pParent)
{
	date = startdate;
}

DlgDate::~DlgDate()
{
}

void DlgDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, calendarCtl);
}


BEGIN_MESSAGE_MAP(DlgDate, CDialog)
	ON_BN_CLICKED(IDOK, &DlgDate::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgDate-Meldungshandler

BOOL DlgDate::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	int i = date.GetDay();
	i = date.GetMonth();
	i = date.GetYear();

	calendarCtl.SetCurSel(date);

	return true;
}


void DlgDate::OnBnClickedOk()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.


	calendarCtl.GetCurSel(date);

	OnOK();
}


COleDateTime DlgDate::get_date()
{
	return date;
}