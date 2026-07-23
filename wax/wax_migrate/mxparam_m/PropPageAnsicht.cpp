/// PropPageAnsicht.cpp : Cpp-Datei
/// Eigenschaftenseite PropPageAnsicht

#include "stdafx.h"

#include "resource.h"
#include "PropPageAnsicht.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PropPageAnsicht, WaxPropPage)
	ON_COMMAND(IDC_PARA_BAUMFARBE, onParaBaumfarbe)
	ON_BN_CLICKED(IDC_PARA_SYMBOLLEISTE, on_update)
	ON_BN_CLICKED(IDC_PARA_STATUSLEISTE, on_update)
	ON_BN_CLICKED(IDC_PARA_GRAPHIKFENSTER, on_update)
	ON_BN_CLICKED(IDC_PARA_SHOWFREIGABEPOS, on_update)

	ON_BN_CLICKED(IDC_ICONKLEIN, on_update)
	ON_BN_CLICKED(IDC_ICONMITTEL, on_update)
	ON_BN_CLICKED(IDC_ICONGROSS, on_update)
	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

/// Konstructor
PropPageAnsicht::PropPageAnsicht(MxParameter *para) : WaxPropPage(IDD_PARA_ANSICHT, para)
{
	paraBaumfarbe = parameter->val_int(BAUMFARBE);
	paraGraphikfenster = (parameter->val_int(GRAPHIKFENSTER)  == 1) ? true : false;
	paraShowfreigabepos = (parameter->val_int(SHOWFREIGABEPOS) == 1) ? true : false;
	paraIcongroesse = parameter->val_int(ICONGROESSE);
	paraSymbolleiste = parameter->val_int(SYMBOLLEISTE) == 1 ? true : false;
	paraStatusleiste = parameter->val_int(STATUSZEILE) == 1 ? true : false;

	ADD_PARA_X(ICONGROESSE);
	ADD_PARA_X(BAUMFARBE);
	ADD_PARA_XX(IDC_TREECOLOR, BAUMFARBE);
	ADD_PARA_XX(IDC_ICONGROSS, ICONGROESSE);
	ADD_PARA_XX(IDC_ICONMITTEL, ICONGROESSE);
	ADD_PARA_XX(IDC_ICONKLEIN, ICONGROESSE);

	ADD_PARA_CHK(GRAPHIKFENSTER);
	ADD_PARA_CHK(SHOWFREIGABEPOS);
	ADD_PARA_CHK(STATUSZEILE);
	ADD_PARA_CHK(SYMBOLLEISTE);
}

/// Destructor
PropPageAnsicht::~PropPageAnsicht()
{
}



/// Default-Werte setzen
void PropPageAnsicht::set_default()
{
	paraBaumfarbe		=  parameter->def_val_int(BAUMFARBE);
	paraGraphikfenster	= (parameter->def_val_int(GRAPHIKFENSTER)  == 1) ? true : false;
	paraShowfreigabepos = (parameter->def_val_int(SHOWFREIGABEPOS) == 1) ? true : false;
	paraIcongroesse		=  parameter->def_val_int(ICONGROESSE);

	paraSymbolleiste = parameter->def_val_int(SYMBOLLEISTE) ? true : false;
	paraStatusleiste = parameter->def_val_int(STATUSZEILE) ? true : false;
	update_baumfarbe();
}



/// Dialog initialisieren
BOOL PropPageAnsicht::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();

	setWindowsText(IDC_STATIC_ICON, "IDC_STATIC_ICON");
	setWindowsText(IDC_ICONKLEIN, "IDS_KLEIN");
	setWindowsText(IDC_ICONMITTEL, "IDS_MITTEL");
	setWindowsText(IDC_ICONGROSS, "IDS_GROSS");
	setWindowsTextWithColon(IDC_STATIC_EINGESTELLT, "IDS_EINGESTELLT");
	setWindowsText(IDC_PARA_BAUMFARBE, "IDC_PARA_BAUMFARBE");
	setWindowsText(IDC_STATIC_ANZEIGE, "IDC_STATIC_ANZEIGE");
	setWindowsText(IDC_PARA_GRAPHIKFENSTER, "IDC_PARA_GRAPHIKFENSTER");
	setWindowsText(IDC_PARA_SYMBOLLEISTE, "IDC_PARA_SYMBOLLEISTE");
	setWindowsText(IDC_PARA_SHOWFREIGABEPOS, "IDC_PARA_SHOWFREIGABEPOS");
	setWindowsText(IDC_PARA_STATUSLEISTE, "IDC_PARA_STATUSLEISTE");
	setWindowsText(IDC_STATIC_HFARBE, "IDC_STATIC_HFARBE");

	

	// Beispielkästchen für die Baumfarbe ausblenden
	GetDlgItem(IDC_TREECOLOR)->ShowWindow(SW_HIDE);
	return TRUE;  
}

BOOL PropPageAnsicht::OnEraseBkgnd(CDC* pDC)
{
	/**
	Beispielkästchen mit der Baumfarbe anzeigen
	*/
	BOOL ret = WaxPropPage::OnEraseBkgnd(pDC);
	CDC* dc = pDC;
	if (dc)
	{
		RECT rect;
		GetDlgItem(IDC_TREECOLOR)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		dc->Rectangle(&rect);
		rect.bottom -= 2;
		rect.left += 2;
		rect.right -= 2;
		rect.top += 2;
		CBrush br;
		br.CreateSolidBrush(paraBaumfarbe);
		dc->FillRect(&rect, &br);
	}
	return ret;
}

/// Datenaustausch
void PropPageAnsicht::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);

	BOOL checkGraphikfenster  = paraGraphikfenster  ? 1 : 0;
	BOOL checkShowfreigabepos = paraShowfreigabepos ? 1 : 0;
	BOOL checkSymbolleiste    = paraSymbolleiste    ? 1 : 0;
	BOOL checkStatusleiste    = paraStatusleiste    ? 1 : 0;
	
	DDX_Check(pDX, IDC_PARA_GRAPHIKFENSTER,  checkGraphikfenster);
	DDX_Check(pDX, IDC_PARA_SHOWFREIGABEPOS, checkShowfreigabepos);
	DDX_Check(pDX, IDC_PARA_SYMBOLLEISTE,    checkSymbolleiste);
	DDX_Check(pDX, IDC_PARA_STATUSLEISTE,    checkStatusleiste);

	paraGraphikfenster  = (checkGraphikfenster == 1)  ? true : false; 
	paraShowfreigabepos = (checkShowfreigabepos == 1) ? true : false; 
	paraSymbolleiste    = (checkSymbolleiste == 1)    ? true : false; 
	paraStatusleiste    = (checkStatusleiste == 1)    ? true : false; 

	BOOL checkKlein  = (paraIcongroesse == KLEIN)  ? 1 : 0;
	BOOL checkMittel = (paraIcongroesse == MITTEL) ? 1 : 0;
	BOOL checkGross  = (paraIcongroesse == GROSS)  ? 1 : 0;

	DDX_Check(pDX, IDC_ICONKLEIN,  checkKlein);
	DDX_Check(pDX, IDC_ICONMITTEL, checkMittel);
	DDX_Check(pDX, IDC_ICONGROSS,  checkGross);

	paraIcongroesse = checkGross ? GROSS : checkMittel ? MITTEL : KLEIN; 
}

BOOL PropPageAnsicht::OnApply() 
{
	/**
	Allgemeine Einstellungen in Datenbank eintragen
	*/
	if (WaxPropPage::OnApply())
		return true;

	parameter->save_para(BAUMFARBE, (int)paraBaumfarbe);
	parameter->save_para(GRAPHIKFENSTER, paraGraphikfenster);
	parameter->save_para(SHOWFREIGABEPOS, paraShowfreigabepos);
	parameter->save_para(ICONGROESSE, paraIcongroesse);
	parameter->save_para(STATUSZEILE, paraStatusleiste);
	parameter->save_para(SYMBOLLEISTE, paraSymbolleiste);

	return true;
}

void PropPageAnsicht::onParaBaumfarbe()
{
	/**
	Farbe über Standarddialog auswählen
	*/
	CColorDialog col(paraBaumfarbe, 0, this);
	if (col.DoModal() == IDOK)
	{
		paraBaumfarbe = col.GetColor();
		on_update();
		update_baumfarbe();
	}
}

void PropPageAnsicht::update_baumfarbe()
{
	/** 
	Sorgt für ein Neuzeichnen des Farbfensters
	*/
	RECT rect;
	GetDlgItem(IDC_TREECOLOR)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	InvalidateRect(&rect);
}

