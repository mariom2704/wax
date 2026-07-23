// mkStammdb.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "mkStammdb.h"
#include "mkStammdbDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMkStammdbApp

BEGIN_MESSAGE_MAP(CMkStammdbApp, CWinApp)
	//{{AFX_MSG_MAP(CMkStammdbApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMkStammdbApp Konstruktion

CMkStammdbApp::CMkStammdbApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CMkStammdbApp-Objekt

CMkStammdbApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMkStammdbApp Initialisierung

BOOL CMkStammdbApp::InitInstance()
{
	MkStammdbDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, so dass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
