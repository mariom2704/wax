#include "stdafx.h"
#include "resource.h"
#include "DBInfo.h"
#include <mxodbc\mxodbc.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DBInfo, CDialog)
END_MESSAGE_MAP()


DBInfo::DBInfo() : CDialog(IDD_DATABASE_INFO)
{
}

BOOL DBInfo::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	DBConnection dbcon;

	CString strConnect = dbcon.get_connection_info(WaxSys::get_userdb(), "", true).c_str();
	int iWo = -1;
	iWo = strConnect.Find("user") + 4;
	if( iWo > -1)
	{
		strConnect.Insert(iWo, ".mdb");
	}
	GetDlgItem(IDC_DBCONNECT)->SetWindowText(strConnect);
	
	dbcon.set_db((LPCSTR)WaxSys::verifyLocationOfStammDB());

	strConnect =  dbcon.get_connection_info(true).c_str();
	iWo = -1;
	iWo = strConnect.Find("stamm") + 5;
	int iWoII = strConnect.Find("dat");
	if( iWo > -1 && iWoII == -1)
	{
		strConnect.Insert(iWo, ".mdb");
	}
	GetDlgItem(IDC_DBCONNECT2)->SetWindowText(strConnect);

	dbcon.set_db((LPCSTR)WaxSys::para_str(SERVERDB));
	GetDlgItem(IDC_DBCONNECT3)->SetWindowText(dbcon.get_connection_info(true).c_str());

	setText(IDC_STATIC_USERDB,				"IDC_STATIC_USERDB");
	setText(IDC_STATIC_STAMMDB,				"IDC_STATIC_STAMMDB");
	setText(IDC_STATIC_FREIGABEDB,			"IDC_STATIC_FREIGABEDB");
	setText(IDC_STATIC_FREIGABESOAPSERVER,	"IDC_STATIC_FREIGABESOAPSERVER");


	CString tmp;
	tmp = getStammdaten().getText("IDS_DATABASE_INFO", true).c_str();
	if (!tmp.IsEmpty())
	{
		SetWindowText(tmp);
	}

	CString strSAPSoapServer =  WaxSys::getSAPSoapServer();
	int  iLength = strSAPSoapServer.GetLength();
	int iwhere = strSAPSoapServer.Find("/", iLength/2-5);
	strSAPSoapServer.Insert(iwhere, _T("\n"));

	GetDlgItem(IDC_CONNECT_SOAP_SERVER)->SetWindowText(strSAPSoapServer);

	return true;
}


void DBInfo::setText(int id, const string &dbid)
{
	CString tmp;
	tmp = getStammdaten().getText(dbid, true).c_str();

	if (!tmp.IsEmpty())
		SetDlgItemText(id, tmp);
}
