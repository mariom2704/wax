#include "stdafx.h"
#include "resource.h"
#include "passwd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Passwd, CDialog)
END_MESSAGE_MAP()

Passwd::Passwd(CWnd* parent, string passwd_from_db) : CDialog(IDD_PASSWD, parent)
{
	passwd = "????????";
	dbpasswd = passwd_from_db;
}

Passwd::~Passwd()
{
}


BOOL Passwd::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return (FALSE);

	CString msg;
	
	msg = TextDataList::getInstance()->getText("IDS_PASSWORD").c_str();
	//msg.LoadString(IDS_PASSWORD);
	SetWindowText(msg);

	msg = TextDataList::getInstance()->getText("IDS_PASSWORD2").c_str();
	//msg.LoadString(IDS_PASSWORD2);
	GetDlgItem(IDC_MSG)->SetWindowText(msg);

	msg.LoadString(IDS_CANCEL);
	GetDlgItem(IDCANCEL)->SetWindowText(msg);

	ed_passwd.SetPasswordChar('*');

	return true;
}

void Passwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASSWD, passwd);
	DDX_Control(pDX, IDC_PASSWD, ed_passwd);

}

void Passwd::OnOK()
{
	CDialog::OnOK();	
}

bool Passwd::check_passwd() const
{
	/**
	Vergleicht das Passwort mit dem in der Tabelle Password
	*/
	return dbpasswd.compare((LPCSTR)passwd) == 0;
}
