#include "stdafx.h"


#include "resource.h"
#include "WaxServer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ServerDBMyWa, ServerDBOpen)
END_MESSAGE_MAP()

ServerDBMyWa::ServerDBMyWa()
{
	m_lpszTemplateName = MAKEINTRESOURCE(IDD_OPEN_DATABASE_MYWA);
	sachbearbeiter = WaxSys::para_str(BEARBEITER);
	von = bis = MXDate::GetCurrentTime();
}

ServerDBMyWa::~ServerDBMyWa()
{
	
}

//Dialogelemente initialisieren
BOOL ServerDBMyWa::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	WaxSys::user_list((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER));

	// Standard alle nicht abgearbeiteten
	((CButton*)GetDlgItem(IDC_SELECT_NOTSAP))->SetCheck(TRUE);
	
	// Unsichtbare Vorgabe, wird in Basisklasse für den Aufbau des Projektes benötigt.
	((CButton*)(GetDlgItem(IDC_SORT_WA)))->SetCheck(1);

	createProgressWnd();
	return true;
}

BOOL ServerDBMyWa::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

//Import starten
void ServerDBMyWa::OnOK()
{
	ServerDBOpen::OnOK();
}



const CString ServerDBMyWa::createSQLSelect() const
{
	
	CString sap;
	// Button 'nicht abgearbeitete Werkaufträge'
	if (((CButton*)GetDlgItem(IDC_SELECT_NOTSAP))->GetCheck())
	{
		sap = "sap = 0";
	}
	// Button 'abgearbeitete Werkaufträge'
	else if (((CButton*)GetDlgItem(IDC_SELECT_SAP))->GetCheck())
	{
		sap = "sap = 1";
	}

	CString zeitraum;
	zeitraum.Format("bearbeitungsdatum >= %i AND bearbeitungsdatum <= %i",  von.toNornamlizedInt(false), bis.toNornamlizedInt(true));
	CString select;
	if (sap.GetLength())
		select.Format("sachbesegment = '%s' AND %s AND %s", sachbearbeiter, zeitraum, sap);
	else
		select.Format("sachbesegment = '%s' AND %s", sachbearbeiter, zeitraum);
	return select;
}

const CString ServerDBMyWa::createSQLSort(CString tablename) const
{
	return "waid";
}

// Initialisierung von neu geladenen Objekten
void ServerDBMyWa::initObject(WaxPart* pt)
{
	ServerDBOpen::initObject(pt);
}


void ServerDBMyWa::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_FREIGABE_VON, von);
	DDX_DateTimeCtrl(pDX, IDC_FREIGABE_BIS, bis);
	DDX_Text(pDX, IDC_SACHBEARBEITER, sachbearbeiter);
}

