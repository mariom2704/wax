/// PropPageFreigabe.cpp : Cpp-Datei
/// Eigenschaftenseite PropPageFreigabe

#include "stdafx.h"

#include "resource.h"
#include "PropPageFreigabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BEGIN_MESSAGE_MAP(PropPageFreigabe, WaxPropPage)
	ON_BN_CLICKED(IDC_PARA_AUTOFREIGABE, on_update)
	ON_BN_CLICKED(IDC_PARA_SAFEFREIGABE, on_update)
	ON_BN_CLICKED(IDC_PARA_CHECKSTATUS,	on_update)
	ON_BN_CLICKED(IDC_PARA_CHECKSAP, on_update)
	ON_EN_CHANGE(IDC_PARA_EMPFAENGER, on_update)
	ON_EN_CHANGE(IDC_PARA_DISPLAY, on_update)
	ON_EN_CHANGE(IDC_PARA_BETREFF, on_update)
END_MESSAGE_MAP()

/// Konstructor
PropPageFreigabe::PropPageFreigabe(MxParameter *para) : WaxPropPage(IDD_PARA_FREIGABESTATUS, para)
{
	paraAutoFreigabe = parameter->val_int(AUTOFREIGABE);
	paraSaveFreigabe  = parameter->val_int(SAFEFREIGABE);
	paraCheckStatus   = parameter->val_int(CHECKSTATUS);
	paraCheckSap      = parameter->val_int(CHECKSAP);

	ADD_PARA_CHK(AUTOFREIGABE);
	ADD_PARA_CHK(SAFEFREIGABE);
	ADD_PARA_CHK(CHECKSTATUS);
	ADD_PARA_CHK(CHECKSAP);

	ADD_PARA_STR(EMPFAENGER);
	ADD_PARA_STR(DISPLAYNAME);
	ADD_PARA_STR(BETREFF);

	if (!WaxSys::isAV())
		paraAutoFreigabe = true;
}


/// Destructor
PropPageFreigabe::~PropPageFreigabe()
{
}


/// Default-Werte setzen
void PropPageFreigabe::set_default()
{
	paraAutoFreigabe	= parameter->def_val_int(AUTOFREIGABE);
	paraSaveFreigabe	= parameter->def_val_int(SAFEFREIGABE);
	paraCheckStatus		= parameter->def_val_int(CHECKSTATUS);
	paraCheckSap		= parameter->def_val_int(CHECKSAP);
}



/// Dialog initialisieren
BOOL PropPageFreigabe::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();


	setWindowsText(IDC_STATIC_FREIGABE, "IDC_STATIC_FREIGABE");
	setWindowsText(IDC_STATIC_STATUS, "IDC_STATIC_STATUS");
	setWindowsText(IDC_STATIC_EMAILFERT, "IDC_STATIC_EMAILFERT");
	setWindowsText(IDC_PARA_AUTOFREIGABE, "IDC_PARA_AUTOFREIGABE");
	setWindowsText(IDC_PARA_CHECKSAP, "IDC_PARA_CHECKSAP");
	setWindowsText(IDC_PARA_SAFEFREIGABE, "IDS_SAFEFREIGABE");
	setWindowsText(IDC_PARA_CHECKSTATUS, "IDC_PARA_CHECKSTATUS");
	setWindowsText(IDC_PARA_SAFESTATUS, "IDS_SAFEFREIGABE");
	setWindowsText(IDC_STATIC_EMAILADR, "IDC_STATIC_EMAILADR");
	setWindowsText(IDC_STATIC_EMPF_BEZ, "IDC_STATIC_EMPF_BEZ");
	setWindowsText(IDC_STATIC_BETREFF, "IDC_STATIC_BETREFF");


	enable_item(this, IDC_PARA_AUTOFREIGABE, AUTOFREIGABE);
	enable_item(this, IDC_PARA_SAFEFREIGABE, SAFEFREIGABE);
	enable_item(this, IDC_PARA_CHECKSTATUS,CHECKSTATUS);  
	enable_item(this, IDC_PARA_CHECKSAP, CHECKSAP);

	if (!WaxSys::isAV())
		GetDlgItem(IDC_PARA_AUTOFREIGABE)->EnableWindow(FALSE);

	return TRUE;  
}

/// Datenaustausch
void PropPageFreigabe::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PARA_AUTOFREIGABE,  paraAutoFreigabe);
	DDX_Check(pDX, IDC_PARA_SAFEFREIGABE,   paraSaveFreigabe);
	DDX_Check(pDX, IDC_PARA_CHECKSTATUS,    paraCheckStatus);
	DDX_Check(pDX, IDC_PARA_CHECKSAP,       paraCheckSap);
}

/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageFreigabe::OnApply() 
{
	if (WaxPropPage::OnApply())
		return true;

	parameter->save_para(AUTOFREIGABE,paraAutoFreigabe);
	parameter->save_para(SAFEFREIGABE,paraSaveFreigabe);
	parameter->save_para(CHECKSTATUS, paraCheckStatus);
	parameter->save_para(CHECKSAP, paraCheckSap);

	return true;
}

