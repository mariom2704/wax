#include "stdafx.h"
#include "resource.h"
#include "DBAuswahl.h"
#include <odbcinst.h>
#include <mxodbc\mxodbc.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DBAuswahl, CDialog)
	ON_BN_CLICKED(IDC_DBSEL, OnDbsel)
	ON_BN_CLICKED(IDC_DBSEL2, OnClickedDbsel2)
END_MESSAGE_MAP()


DBAuswahl::~DBAuswahl()
{
}

DBAuswahl::DBAuswahl() : CDialog(IDD_DATABASE_SELECT), pfad("WaxUser"), pfad2("WaxUser"), prompt(false)
{
	
	m_bFinished = false;
	TCHAR path[_MAX_PATH];
	GetModuleFileName(0, path, _MAX_PATH);
	CString strPath = path;
	int iWhere = strPath.ReverseFind('\\');
	CString pfadDefault = strPath.Mid(0, iWhere);
	pfadDefault += "\\waxuser.mdb";
	
	pfad   = AfxGetApp()->GetProfileString("Database", "Path", "");
	pfad2  = AfxGetApp()->GetProfileString("Database", "Path2", "");
	name   = AfxGetApp()->GetProfileString("Database", "Name", "Server");
	name2  = AfxGetApp()->GetProfileString("Database", "Name2", "Lokal");
	prompt = AfxGetApp()->GetProfileInt("Database", "prompt", 0) ? true : false;

	CFileStatus status;
	if(!CFile::GetStatus( pfad, status) && !prompt)
	{
	
		if(!CFile::GetStatus( pfadDefault, status ))
		{
			AfxMessageBox(_T("User database does not exist or\ncould not be found in the Default Paths !!\nWax will be finished now"));
			m_bFinished = true;
		}
		else
		{
			pfad = pfadDefault;
		}
	}

}



//Tooltips ausgeben
INT_PTR DBAuswahl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int index = CWnd::OnToolHitTest(point, pTI);
	if (pTI  && index != -1)
	{
		pTI->lpszText = NULL;
		CString msg;
		msg.LoadString(index);
		if (msg.GetLength())
		{
			pTI->lpszText = (LPSTR)malloc(2048);
			strcpy(pTI->lpszText, msg);
		}
		else
			return -1;
	}
	return index;
}

//Subclassing Methode der MFC, wird für einen 
//Aufruf von EnableToolTips() überschrieben.
void DBAuswahl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	EnableToolTips(TRUE);
}

//Dialogelemente initialisieren
BOOL DBAuswahl::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;
	
	setText(IDC_STATIC_DB1, "IDC_STATIC_DB1");
	setText(IDC_STATIC_BEZ1, "IDC_STATIC_BEZ1");
	setText(IDC_STATIC_DBINFO1, "IDC_STATIC_DBINFO1");

	setText(IDC_STATIC_DB2, "IDC_STATIC_DB2");
	setText(IDC_STATIC_BEZ2, "IDC_STATIC_BEZ2");
	setText(IDC_STATIC_DBINFO2, "IDC_STATIC_DBINFO2");

	setText(IDC_PROMPT, "IDC_PROMPT");

	CString tmp;
	tmp = getStammdaten().getText("IDS_DATABASE_SELECT", true).c_str();
	if (!tmp.IsEmpty())
		SetWindowText(tmp);

	return TRUE; 
}


void DBAuswahl::setText(int id, const string &dbid)
{
	CString tmp;
	tmp = getStammdaten().getText(dbid, true).c_str();

	if (!tmp.IsEmpty())
		SetDlgItemText(id, tmp);
}



//Datenaustausch mit Dialogelementen
void DBAuswahl::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBPATH, pfad);
	DDX_Text(pDX, IDC_EDIT_DBPATH2, pfad2);
	DDX_Text(pDX, IDC_NAME, name);
	DDX_Text(pDX, IDC_NAME2, name2);
	DDX_Check(pDX, IDC_PROMPT, prompt);
}

//Datenbankpfad abspeichern
void DBAuswahl::OnOK()
{
	DBConnection con, con1;
	UpdateData(TRUE);
	con.set_userdbentry(pfad);
	MXTable test(con.get_userdb(true), "Systemparameter");
	CString dberror;	
	if (test.getCount() == -1)
	{
		dberror = pfad + "\n";
	}
	
	if (pfad2.GetLength())
	{
		con1.set_userdbentry(pfad2);
		MXTable test1(con1.get_userdb(true), "Systemparameter");
		if (test1.getCount() == -1)
		{
			dberror += pfad2 + "\n";
		}
	}

	if (dberror.GetLength())
	{
		CString msg;
		msg = getStammdaten().getText("IDS_WRONGDATABASE").c_str();
		msg = dberror + msg;
		AfxMessageBox(msg, MB_OK);
		return;
	}
	CDialog::OnOK();
	AfxGetApp()->WriteProfileString("Database", "Path", pfad);
	AfxGetApp()->WriteProfileString("Database", "Path2", pfad2);
	AfxGetApp()->WriteProfileString("Database", "Name", name);
	AfxGetApp()->WriteProfileString("Database", "Name2", name2);
	AfxGetApp()->WriteProfileInt("Database", "prompt", prompt);

}

const CString& DBAuswahl::get_pfad2() const
{
	return pfad2;
}

const CString& DBAuswahl::get_pfad() const
{
	return pfad;
}

void DBAuswahl::set_pfad(const CString& value)
{
	pfad = value;
	return;
}

void DBAuswahl::OnDbsel() 
{
	/**
	Auswahl der Datenbank per Dateipfad
	*/
	UpdateData(true);
	selectDB(pfad);
}


//Indicates the user clicked a button
void DBAuswahl::OnClickedDbsel2()
{
	/**
	Auswahl der Datenbank per Dateipfad
	*/
	UpdateData(true);
	selectDB(pfad2);
}


//Wählt eine Access Datenbank nach Pfadangabe aus
bool DBAuswahl::selectDB(CString& pfad)
{
	/*
	Auswahl der Datenbank per Dateipfad
	*/
	CString file, initpath;

	initpath = pfad;

	CFileDialog fo(TRUE, NULL, NULL, 
		OFN_EXPLORER|OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR , 
		"Access-Datenbanken (*.mdb)|*.mdb||", this);
	
	fo.m_ofn.lpstrInitialDir = initpath;
	fo.m_ofn.lpfnHook=NULL;
	fo.m_ofn.Flags &= ~OFN_ENABLEHOOK;
	fo.m_ofn.hwndOwner = this->m_hWnd;

	if (GetOpenFileName(&fo.m_ofn))
	{
		pfad = fo.GetPathName();
		UpdateData(FALSE);
		Sleep(100);// Workaround für 'Öffnen schlägt fehl wg. Zugriffverletzung'
		return true;
	}
	return false;
}


//Erzeugt aus einem Datenbankpfad einen Connectstring
CString DBAuswahl::getConnect(CString connect) 
{
	CString odbcConnect;
		
	if (connect.FindOneOf("\\.") != -1)
		odbcConnect.Format("DRIVER=%s;DSN='';DBQ=%s", getDriver(), connect);
	else
		odbcConnect.Format("%s;", connect);
	return odbcConnect;
}


//Sucht aus den installierten Treibern den Treiberstring für Access heraus
CString DBAuswahl::getDriver() 
{
	/**
	Erstellen des Connectstrings
	*/
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
 
  
	// Get the names of the installed drivers ("odbcinst.h" has to be included )
	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
	{
		return "";
	}
	
	// Search for the driver...
	do {
		if( strstr( pszBuf, "Access" ) != 0 ) {// Found !
			TRACE("AccessTreiber: %s\n", pszBuf);
			return CString(pszBuf);
		}
		pszBuf = strchr(pszBuf, '\0') + 1;
	}
	while(pszBuf[1] != '\0');
	
	return "";
}





bool DBAuswahl::get_prompt() const
{
	return prompt ? true : false;
}

