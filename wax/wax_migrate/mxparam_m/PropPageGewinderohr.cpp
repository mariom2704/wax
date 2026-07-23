
#include "stdafx.h"

#include "resource.h"
#include "PropPageGewinderohr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PropPageGewinderohr, WaxPropPage)
	ON_BN_CLICKED(IDC_ZMASSLISTE, on_update)
	ON_BN_CLICKED(IDC_ZMASSGRAPHIK, on_update)
	ON_BN_CLICKED(IDC_DN, on_update)
	ON_BN_CLICKED(IDC_ZOLL, on_update)
	ON_BN_CLICKED(IDC_WINKEL, on_update)
	ON_BN_CLICKED(IDC_AUSRICHTUNG, on_update)
	ON_BN_CLICKED(IDC_TEXTKURZ, on_update)
	ON_BN_CLICKED(IDC_TEXTMITTEL, on_update)
	ON_BN_CLICKED(IDC_TEXTLANG, on_update)
	ON_EN_KILLFOCUS(IDC_PARA_TRENNZEICHEN, update_examples)
	ON_EN_CHANGE(IDC_PARA_TRENNZEICHEN, on_update)
END_MESSAGE_MAP()

/// Konstructor
PropPageGewinderohr::PropPageGewinderohr(MxParameter *para) : WaxPropPage(IDD_PARA_GEWINDEROHR, para)
{
	paraZMassListe			= (parameter->val_int(ZMASSLISTE)   == 1) ? true : false;
	paraZMassGraphik		= (parameter->val_int(ZMASSGRAPHIK) == 1) ? true : false;
	paraDN2Zoll				= (parameter->val_int(DN2ZOLL)      == 1) ? true : false;
	paraWinkelAusrichtung	= (parameter->val_int(WINKELAUSRICHTUNG) == 1) ? true : false;
	paraTextformat			=  parameter->val_int(TEXTFORMAT);

	ADD_PARA_STR(TRENNZEICHEN);
	ADD_PARA_CHK(ZMASSLISTE);
	ADD_PARA_CHK(DN2ZOLL); 
	ADD_PARA_CHK(ZMASSGRAPHIK);
	ADD_PARA_CHK(TEXTFORMAT);

	ADD_PARA_XX(IDC_ZMASSLISTE,	ZMASSLISTE);
	ADD_PARA_XX(IDC_ZMASSGRAPHIK, ZMASSLISTE);
	ADD_PARA_XX(IDC_DN,	DN2ZOLL);
	ADD_PARA_XX(IDC_ZOLL, DN2ZOLL);
	ADD_PARA_XX(IDC_WINKEL,	ZMASSGRAPHIK);
	ADD_PARA_XX(IDC_AUSRICHTUNG, ZMASSGRAPHIK);
	ADD_PARA_XX(IDC_TEXTKURZ, TEXTFORMAT);
	ADD_PARA_XX(IDC_TEXTMITTEL, TEXTFORMAT);
	ADD_PARA_XX(IDC_TEXTLANG, TEXTFORMAT);
}

/// Destructor
PropPageGewinderohr::~PropPageGewinderohr()
{
}


/// Default-Werte setzen
void PropPageGewinderohr::set_default()
{
	WaxPropPage::set_default();
	paraTextformat			=  parameter->def_val_int(TEXTFORMAT);
	paraDN2Zoll				= (parameter->def_val_int(DN2ZOLL)           == 1) ? true : false;
	paraWinkelAusrichtung	= (parameter->def_val_int(WINKELAUSRICHTUNG) == 1) ? true : false;

	if (WaxSys::isAV())
	{
		paraZMassListe		= (parameter->def_val_int(ZMASSLISTE)   == 1) ? true : false;
		paraZMassGraphik	= (parameter->def_val_int(ZMASSGRAPHIK) == 1) ? true : false;
	}
	else
	{
		paraZMassListe		= (parameter->def_val_int(ZMASSLISTE)   == 1) ? true : false;
		paraZMassGraphik	= (parameter->def_val_int(ZMASSGRAPHIK) == 1) ? true : false;
	}
	update_examples();
}

/// Dialog initialisieren
BOOL PropPageGewinderohr::OnInitDialog() 
{
	WaxPropPage::OnInitDialog();
	

	setWindowsText(IDC_STATIC_KURZTXT, "IDC_STATIC_KURZTXT");
	setWindowsText(IDC_TEXTKURZ, "IDC_TEXTKURZ");
	setWindowsText(IDC_TEXTMITTEL, "IDC_TEXTMITTEL");
	setWindowsText(IDC_TEXTLANG, "IDC_TEXTLANG");
	setWindowsText(IDC_EXAMPLE1, "IDC_EXAMPLE1");
	setWindowsText(IDC_EXAMPLE2, "IDC_EXAMPLE2");
	setWindowsText(IDC_EXAMPLE3, "IDC_EXAMPLE3");
	setWindowsTextWithColon(IDC_STATIC_TRENNZEICHEN, "IDC_STATIC_TRENNZEICHEN");
	setWindowsText(IDC_ZMASSLISTE, "IDC_ZMASSLISTE");
	setWindowsText(IDC_STATIC_ZMASSABZ, "IDC_STATIC_ZMASSABZ");
	setWindowsText(IDC_ZMASSGRAPHIK, "IDC_ZMASSGRAPHIK");
	setWindowsText(IDC_STATIC_WINKELAUS, "IDC_STATIC_WINKELAUS");
	setWindowsText(IDC_WINKEL, "IDS_WINKEL");
	setWindowsText(IDC_AUSRICHTUNG, "IDC_AUSRICHTUNG");
	setWindowsText(IDC_STATIC_NENNWEITE, "IDS_NENNWEITE");
	setWindowsText(IDC_DN, "IDS_DN");
	setWindowsText(IDC_ZOLL, "IDS_ZOLL");


	if (!WaxSys::isAV())
		GetDlgItem(IDC_ZMASSGRAPHIK)->ModifyStyle(WS_VISIBLE, NULL);
	update_examples();
	return TRUE;  
}

/// Datenaustausch
void PropPageGewinderohr::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);

	// ZMass
	BOOL checkZMassListe   = paraZMassListe   ? 1 : 0;
	BOOL checkZMassGraphik = paraZMassGraphik ? 1 : 0;
	
	DDX_Check(pDX, IDC_ZMASSLISTE,   checkZMassListe);
	DDX_Check(pDX, IDC_ZMASSGRAPHIK, checkZMassGraphik);

	paraZMassListe   = (checkZMassListe   == 1) ? true : false; 
	paraZMassGraphik = (checkZMassGraphik == 1) ? true : false; 

	// Nennweite
	BOOL checkDN   = (paraDN2Zoll == false) ? 1 : 0;
	BOOL checkZoll = (paraDN2Zoll == true)  ? 1 : 0;
	
	DDX_Check(pDX, IDC_DN, checkDN);
	DDX_Check(pDX, IDC_ZOLL, checkZoll);

	paraDN2Zoll = (checkZoll == 1) ? true : false; 

	// Winkel
	BOOL checkWinkel      = (paraWinkelAusrichtung == false) ? 1 : 0;
	BOOL checkAusrichtung = (paraWinkelAusrichtung == true)  ? 1 : 0;
	
	DDX_Check(pDX, IDC_WINKEL,      checkWinkel);
	DDX_Check(pDX, IDC_AUSRICHTUNG, checkAusrichtung);

	paraWinkelAusrichtung = (checkAusrichtung == 1) ? true : false; 

	BOOL checkKurz	 = (paraTextformat == KURZ)   ? 1 : 0;
	BOOL checkMittel = (paraTextformat == MITTEL) ? 1 : 0;
	BOOL checkLang   = (paraTextformat == LANG)   ? 1 : 0;

	DDX_Check(pDX, IDC_TEXTKURZ,   checkKurz);
	DDX_Check(pDX, IDC_TEXTMITTEL, checkMittel);
	DDX_Check(pDX, IDC_TEXTLANG,   checkLang);

	paraTextformat = checkLang ? LANG : checkMittel ? MITTEL : KURZ; 
}

/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageGewinderohr::OnApply() 
{
	if (WaxPropPage::OnApply())
		return true;

	parameter->save_para(ZMASSLISTE, paraZMassListe   ? 1 : 0);
	parameter->save_para(ZMASSGRAPHIK, paraZMassGraphik ? 1 : 0);
	parameter->save_para(DN2ZOLL, paraDN2Zoll      ? 1 : 0);
	parameter->save_para(WINKELAUSRICHTUNG, paraWinkelAusrichtung ? 1 : 0);
	parameter->save_para(TEXTFORMAT, paraTextformat);

	return true;
}


void PropPageGewinderohr::update_examples()
{
	/**
	Beispieltexte updaten bei Änderung des Trennzeichens
	*/
	CString newchar;
	GetDlgItem(IDC_PARA_TRENNZEICHEN)->GetWindowText(newchar);
	set_example((LPCSTR)newchar, IDC_EXAMPLE1, TextDataList::getInstance()->getText("IDC_EXAMPLE1"));
	set_example((LPCSTR)newchar, IDC_EXAMPLE2, TextDataList::getInstance()->getText("IDC_EXAMPLE2"));
	set_example((LPCSTR)newchar, IDC_EXAMPLE3, TextDataList::getInstance()->getText("IDC_EXAMPLE3"));
}


void PropPageGewinderohr::set_example(string newchar, UINT idc, string ids)
{
	/**
	Setzt die Beispieltexte
	*/
	CString tmp;
	tmp.Format(ids.c_str(), newchar.c_str(), newchar.c_str());
	GetDlgItem(idc)->SetWindowText(tmp);
}