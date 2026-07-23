#include "stdafx.h"

#include "resource.h"
#include "PropPageGraphik.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PropPageGraphik, WaxPropPage)
	ON_BN_CLICKED(IDC_PARA_BEMASSUNGSLINIE,		on_update)
	ON_BN_CLICKED(IDC_PARA_TRENNLINIE,			on_update)
	ON_BN_CLICKED(IDC_PARA_RAHMEN,				on_update)
	ON_BN_CLICKED(IDC_PARA_BESCHRIFTUNGLAENGE,	on_update)
	ON_BN_CLICKED(IDC_PARA_SHOWFREIGABEPOS,		on_update)
	ON_EN_CHANGE(IDC_PARA_FONTGROESSE,			on_update)

	ON_CBN_SELCHANGE(IDC_PARA_VIEWERVORSCHAU, on_update)
	ON_CBN_SELCHANGE(IDC_PARA_VIEWERDRUCK, on_update)
	
END_MESSAGE_MAP()

/// Konstructor
PropPageGraphik::PropPageGraphik(MxParameter *para) : WaxPropPage(IDD_PARA_GRAPHIK, para)
{
	paraBemassungslinie		= parameter->val_int(BEMASSUNGSLINIE);
	paraTrennlinie			= parameter->val_int(TRENNLINIE);
	paraRahmen				= parameter->val_int(RAHMEN);
	paraBeschriftunglaenge	= parameter->val_int(BESCHRIFTUNGLAENGE);

	ADD_PARA_STR(GRAFIKFORMAT);
	ADD_PARA_STR(VIEWERDRUCK);
	ADD_PARA_STR(FONTGROESSE);
	
	ADD_PARA_CHK(BEMASSUNGSLINIE);
	ADD_PARA_CHK(TRENNLINIE);
	ADD_PARA_CHK(RAHMEN)
	ADD_PARA_CHK(BESCHRIFTUNGLAENGE);
}

/// Destructor
PropPageGraphik::~PropPageGraphik()
{
}

/// Default-Werte setzen
void PropPageGraphik::set_default()
{
	WaxPropPage::set_default();
	paraBemassungslinie		= parameter->def_val_int(BEMASSUNGSLINIE);
	paraTrennlinie			= parameter->def_val_int(TRENNLINIE);
	paraRahmen				= parameter->def_val_int(RAHMEN);
	paraBeschriftunglaenge	= parameter->def_val_int(BESCHRIFTUNGLAENGE);
}

/// Dialog initialisieren
BOOL PropPageGraphik::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();


	setWindowsText(IDC_STATIC_DRUCKLISTE, "IDC_STATIC_DRUCKLISTE");
	setWindowsTextWithColon(IDC_STATIC_FONTGROESSE, "IDC_STATIC_FONTGROESSE");
	setWindowsTextWithColon(IDC_STATIC_DWF, "IDC_STATIC_DWF");
	setWindowsText(IDC_PARA_RAHMEN, "IDC_PARA_RAHMEN");
	setWindowsText(IDC_PARA_TRENNLINIE, "IDC_PARA_TRENNLINIE");
	setWindowsText(IDC_STATIC_VORSCHAU, "IDC_STATIC_VORSCHAU");
	setWindowsTextWithColon(IDC_STATIC_GRAFIKFORMAT, "IDC_STATIC_GRAFIKFORMAT");
	setWindowsText(IDC_PARA_BEMASSUNGSLINIE, "IDC_PARA_BEMASSUNGSLINIE");
	setWindowsText(IDC_PARA_BESCHRIFTUNGLAENGE, "IDC_PARA_BESCHRIFTUNGLAENGE");



	return TRUE;  
}

/// Datenaustausch
void PropPageGraphik::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);
	
	if (!pDX->m_bSaveAndValidate)
	{
		((CComboBox*)GetDlgItem(IDC_PARA_GRAFIKFORMAT))->SelectString(-1, parameter->val_str(GRAFIKFORMAT));
		((CComboBox*)GetDlgItem(IDC_PARA_VIEWERDRUCK))->SelectString(-1, parameter->val_str(VIEWERDRUCK));
	}

	DDX_Check(pDX, IDC_PARA_BEMASSUNGSLINIE,	paraBemassungslinie);
	DDX_Check(pDX, IDC_PARA_TRENNLINIE,			paraTrennlinie);
	DDX_Check(pDX, IDC_PARA_RAHMEN,				paraRahmen);
	DDX_Check(pDX, IDC_PARA_BESCHRIFTUNGLAENGE, paraBeschriftunglaenge);
}

/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageGraphik::OnApply() 
{
	if (WaxPropPage::OnApply())
		return true;

	parameter->save_para(BEMASSUNGSLINIE,	paraBemassungslinie);
	parameter->save_para(TRENNLINIE, paraTrennlinie);
	parameter->save_para(RAHMEN, paraRahmen);
	parameter->save_para(BESCHRIFTUNGLAENGE, paraBeschriftunglaenge);

	return true;
}
