#include "stdafx.h"


#include "PropPageProjekt.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PropPageProjekt, WaxPropPage)
	ON_EN_CHANGE(IDC_PARA_MBTERMIN, on_update)
	ON_EN_CHANGE(IDC_PARA_LIEFERFRIST, on_update)
	ON_EN_CHANGE(IDC_PARA_KENNUNG, on_update)
	ON_EN_CHANGE(IDC_PARA_STAMMDB, on_update)
	ON_EN_CHANGE(IDC_PARA_EXCEL, on_update)
	ON_EN_CHANGE(IDC_PARA_PROJEKTE, on_update)
	ON_EN_CHANGE(IDC_PARA_KOMP, on_update)
	ON_EN_CHANGE(IDC_PARA_LOGDB, on_update)
	
	ON_BN_CLICKED(IDC_EXCEL, on_excel)
	ON_BN_CLICKED(IDC_PROJEKT, on_projekte)
	ON_BN_CLICKED(IDC_KOMP, on_komp)
	ON_BN_CLICKED(IDC_STAMMDB, on_stammdb)
	ON_BN_CLICKED(IDC_LOGDB, on_logdb)
END_MESSAGE_MAP()

/// Konstructor
PropPageProjekt::PropPageProjekt(MxParameter *para) : WaxPropPage(IDD_PARA_PROJEKT, para)
{
	
	ADD_PARA_STR(LIEFERFRIST);
	ADD_PARA_STR(MBTERMIN);
	
	ADD_PARA_STR(EXCEL);
	ADD_PARA_STR(PROJEKTE);
	ADD_PARA_STR(KOMP);
	ADD_PARA_STR(STAMMDB);
	ADD_PARA_STR(SAPSOAPSERVER);
	
	ADD_PARA_STR(IB_NL_KENNUNG);
	ADD_PARA_XX(IDC_PARA_IB_NL_KENNUNG, IB_NL_KENNUNG);

	ADD_PARA_XX(IDC_EXCEL, EXCEL);
	ADD_PARA_XX(IDC_PROJEKT, PROJEKTE);
	ADD_PARA_XX(IDC_KOMP, KOMP);
	ADD_PARA_XX(IDC_STAMMDB, STAMMDB);
}

/// Destructor
PropPageProjekt::~PropPageProjekt()
{
}


/// Dialog initialisieren
BOOL PropPageProjekt::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();
	
	GetDlgItem(IDC_STAMMDB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PARA_STAMMDB)->SetWindowText(WaxSys::verifyLocationOfStammDB());
	GetDlgItem(IDC_PARA_STAMMDB)->EnableWindow(FALSE);

	setWindowsText(IDC_STATIC_PFADE, "IDS_PFADE");
	setWindowsTextWithColon(IDC_STATIC_PROJEKT, "IDS_PROJEKTE");
	setWindowsTextWithColon(IDC_STATIC_KO, "IDC_STATIC_KO");
	setWindowsTextWithColon(IDC_STATIC_AUFTRAGTAB, "IDC_STATIC_AUFTRAGTAB");
	setWindowsText(IDC_STATIC_DB, "IDS_DATENBANKEN");
	setWindowsTextWithColon(IDC_STATIC_STAMMDB, "IDC_STATIC_STAMMDB");
	setWindowsText(IDC_STATIC_LEITK, "IDS_LEITKARTE2");
	setWindowsTextWithColon(IDC_STATIC_BNR, "IDS_BUERONR");
	setWindowsTextWithColon(IDC_STATIC_LFRIST, "IDS_LIEFERFRIST");
	setWindowsTextWithColon(IDC_STATIC_MBTERMIN, "IDS_MBTERMIN");
	
	if (!WaxSys::isAV())
	{
		GetDlgItem(IDC_PARA_MBTERMIN)->ModifyStyle(WS_VISIBLE, NULL);
		GetDlgItem(IDC_STATIC_MBTERMIN)->ModifyStyle(WS_VISIBLE, NULL);
	}

	if(!passwd_ok)
	{
		GetDlgItem(IDC_PARA_SAPSOAPSERVER)->EnableWindow(FALSE);
	}

	return TRUE;  
}

BOOL PropPageProjekt::OnApply() 
{
	/**
	Muss immer true zurückliefern, damit bei OK der Dialog beendet wird
	*/
	WaxPropPage::OnApply();
	return true;
}

/// Auswahl des Projektpfades per Dateipfad
void PropPageProjekt::on_excel() 
{
	browse_path(this, IDC_PARA_EXCEL);
}

/// Auswahl des Projektpfades per Dateipfad
void PropPageProjekt::on_projekte() 
{
	browse_path(this, IDC_PARA_PROJEKTE);
}

/// Auswahl des Komponentenpfades per Dateipfad
void PropPageProjekt::on_komp() 
{
	browse_path(this, IDC_PARA_KOMP);
}


/// Auswahl des Datenbankpfades per Dateipfad
void PropPageProjekt::on_stammdb() 
{
	browse_file(this, IDC_PARA_STAMMDB);
}

/// Auswahl des Datenbankpfades per Dateipfad
void PropPageProjekt::on_logdb() 
{
	browse_file(this, IDC_PARA_LOGDB);
}
