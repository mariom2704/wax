#include "stdafx.h"
#include "resource.h"
#include "DBPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DBPrompt, CDialog)
	ON_BN_CLICKED(ID_SERVER, onClickedServer)
	ON_BN_CLICKED(ID_LOKAL, onClickedLokal)
END_MESSAGE_MAP()


DBPrompt::DBPrompt() : CDialog(IDD_DBPROMPT)
{
	dbname = AfxGetApp()->GetProfileString("Database", "Path", "WaxUser1");	
}


BOOL DBPrompt::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	CString name = AfxGetApp()->GetProfileString("Database", "Name", "Server");
	CString name2 = AfxGetApp()->GetProfileString("Database", "Name2", "Lokal");

	GetDlgItem(ID_SERVER)->SetWindowText(name);
	GetDlgItem(ID_LOKAL)->SetWindowText(name2);
	return true;	
}

const CString& DBPrompt::get_dbname() const
{
	return dbname;
}

//Server-Datenbank (Datenbank1) soll geladen werden
void DBPrompt::onClickedServer()
{
	dbname = AfxGetApp()->GetProfileString("Database", "Path", "WaxUser1");	
	CDialog::OnOK();
}

//Lokale Datenbank (Datenbank2) soll geladen werden
void DBPrompt::onClickedLokal()
{
	dbname = AfxGetApp()->GetProfileString("Database", "Path2", "WaxUser1");	
	CDialog::OnOK();
}

void DBPrompt::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	EnableToolTips();
}

INT_PTR DBPrompt::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int index = CWnd::OnToolHitTest(point, pTI);
	if (pTI  && index != -1)
	{
		if (index == ID_SERVER)
		{
			pTI->lpszText = (LPSTR)malloc(2048);
			strcpy(pTI->lpszText, AfxGetApp()->GetProfileString("Database", "Path", "WaxUser1"));
		}
		else if (index == ID_LOKAL)
		{
			
			pTI->lpszText = (LPSTR)malloc(2048);
			strcpy(pTI->lpszText, AfxGetApp()->GetProfileString("Database", "Path2", "WaxUser2"));
		}
		else
			return -1;
	}
	return index;
}



void DBPrompt::OnCancel()
{
	CDialog::OnCancel();
}

