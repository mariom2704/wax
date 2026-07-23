#include "stdafx.h"


#include "resource.h"

#include "EntnahmeleitungEingabe.h"

#define el ((EntnahmeleitungDWB*)get_data()->get_werkauftrag())

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(EntnahmeleitungEingabe, WaxPartForm)
	ON_BN_CLICKED(IDC_OHNE, onClickedOhne)
	ON_BN_CLICKED(IDC_DN150, doUpdate)
	ON_BN_CLICKED(IDC_DN100, doUpdate)

	ON_BN_CLICKED(IDC_ENTLEERUNG_RECHTS, onClickedEntleerung_rechts)
	ON_BN_CLICKED(IDC_ENTLEERUNG_LINKS, onClickedEntleerung_links)
	ON_BN_CLICKED(IDC_TYP_A, onClickedTypA)
	ON_BN_CLICKED(IDC_TYP_B, onClickedTypB)
	ON_BN_CLICKED(IDC_ROHRNIPPEL, onClickedRohrnippel)
	ON_BN_CLICKED(IDC_SCHWMUFFE, doUpdate)
	ON_EN_KILLFOCUS(IDC_LAENGE1, doUpdate)
	ON_EN_KILLFOCUS(IDC_LAENGE2, doUpdate)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(EntnahmeleitungEingabe, WaxPartForm)

EntnahmeleitungEingabe::EntnahmeleitungEingabe()
: WaxPartForm(IDD_ENTNAHMELEITUNGDWB, getStammdaten().getText("IDD_ENTNAHMELEITUNGDWB").c_str())
{
	
}

EntnahmeleitungEingabe::~EntnahmeleitungEingabe()
{
	
}

BOOL EntnahmeleitungEingabe::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return false;

	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");

	setWindowsText(IDC_STATIC_TYP_SKIZZE, "IDC_STATIC_TYP_SKIZZE");
	setWindowsText(IDC_TYP_A, "IDC_TYP_A");
	setWindowsText(IDC_TYP_B, "IDC_TYP_B");
	setWindowsText(IDC_STATIC_ENTLEERUNG, "IDS_ENTLEERUNG");
	setWindowsText(IDC_ROHRNIPPEL, "IDC_ROHRNIPPEL");
	setWindowsText(IDC_SCHWMUFFE, "IDC_SCHWMUFFE");
	setWindowsText(IDC_OHNE, "IDS_OHNE");
	setWindowsText(IDC_STATIC_NW_DIN, "IDC_STATIC_NW_DIN");
	setWindowsText(IDC_STATIC_ENTL_SEITE, "IDC_STATIC_ENTL_SEITE");
	setWindowsText(IDC_ENTLEERUNG_RECHTS, "IDS_RECHTS");
	setWindowsText(IDC_ENTLEERUNG_LINKS, "IDS_LINKS");
	setWindowsText(IDC_STATIC_ABM, "IDC_STATIC_ABM");
	setWindowsTextWithColon(IDC_STATIC_ABM_L1, "IDC_STATIC_ABM_L1");
	setWindowsTextWithColon(IDC_STATIC_ABM_L2, "IDC_STATIC_ABM_L2");

	return true;
}

void EntnahmeleitungEingabe::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);

	ddx_posnr(pDX, IDC_POSNR);
	ddx_anzahl(pDX, IDC_ANZAHL);

	// DN
	BOOL checkdn100 = el->get_dnvl() == 100;
	BOOL checkdn150 = el->get_dnvl() == 150;
	
	DDX_Check(pDX, IDC_DN100, checkdn100);
	DDX_Check(pDX, IDC_DN150, checkdn150);

	el->set_dnvl(checkdn100 ? 100 : 150);

	// Laengen
	int laenge = el->get_laenge1();
	int laenge2 =  el->get_laenge2();

	DDX_Text(pDX, IDC_LAENGE1, laenge);
	DDX_Text(pDX, IDC_LAENGE2,laenge2);

	el->set_laenge1(laenge);
	el->set_laenge2(laenge2);

	// Typ (0 = typA)
	BOOL typa = el->get_entnahmeleitung_typ() ? false : true;
	BOOL typb = el->get_entnahmeleitung_typ() ? true : false;

	DDX_Check(pDX, IDC_TYP_A, typa);
	DDX_Check(pDX, IDC_TYP_B, typb);

	el->set_entnahmeleitung_typ(typb);

	// Entleerung
	BOOL checkrohrnippel = el->get_entleerung() == ROHR;
	BOOL checkohne = el->get_entleerung() == Element::LEER;
	BOOL checkmuffe = el->get_entleerung() == MUFFE;
	
	DDX_Check(pDX, IDC_ROHRNIPPEL, checkrohrnippel);
	DDX_Check(pDX, IDC_SCHWMUFFE, checkmuffe);
	DDX_Check(pDX, IDC_OHNE, checkohne);
	
	if (checkrohrnippel)
		el->set_entleerung(ROHR);
	else if (checkohne)
		el->set_entleerung(Element::LEER);
	else if (checkmuffe)
		el->set_entleerung(MUFFE);

	enableEntleerungSeite(el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_B && el->get_entleerung() != Element::LEER);

	// Entleerung Rechts/Links
	BOOL checkrechts = el->get_entleerungRechts() ? true :false;
	BOOL checklinks = el->get_entleerungRechts() ? false : true;
	
	DDX_Check(pDX, IDC_ENTLEERUNG_RECHTS, checkrechts);
	DDX_Check(pDX, IDC_ENTLEERUNG_LINKS, checklinks);

	el->set_entleerungRechts((checkrechts || el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_A) ? true : false);

	if (get_data()->get_werkauftrag()->get_status().get_wanr() == 0 || WaxSys::isAV())	
		GetDlgItem(IDC_LAENGE2)->EnableWindow(el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_A ? false : true);
	GetDlgItem(IDC_ENTNAHMELEITUNG_A)->ShowWindow(el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_A ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_ENTNAHMELEITUNG_B)->ShowWindow(el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_B ? SW_SHOW : SW_HIDE);
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool EntnahmeleitungEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false;

	CString l;
	int checklen1 = 0, checklen2 = 0;
	int gesamtlen = 0;

	// Zu prüfende Minimallängen festlegen
	if (el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_A)
	{
		checklen1 = el->get_dnvl() == 100 ? EntnahmeleitungDWB::MINLAENGE_DN100_L1_TYPA : EntnahmeleitungDWB::MINLAENGE_DN150_L1_TYPA;
		gesamtlen = el->get_laenge1();
	}
	else if (el->get_entnahmeleitung_typ() == EntnahmeleitungDWB::TYP_B)
	{
		checklen1 = el->get_dnvl() == 100 ? EntnahmeleitungDWB::MINLAENGE_DN100_L1_TYPB : EntnahmeleitungDWB::MINLAENGE_DN150_L1_TYPB;
		checklen2 = el->get_dnvl() == 100 ? EntnahmeleitungDWB::MINLAENGE_DN100_L2_TYPB : EntnahmeleitungDWB::MINLAENGE_DN150_L2_TYPB;
		gesamtlen = el->get_laenge1() + el->get_laenge2();
	}
	else 
		XASSERT(FALSE);

	// Ggbf. Focus auf Feld mit falsche Laenge setzen
	UINT newfocus = 0;
	// Gesamtlänge prüfen
	if (gesamtlen > EntnahmeleitungDWB::MAXLAENGE)
	{
		newfocus = IDC_LAENGE1;
	}
	else
	{
		// Einzellängen prüfen
		if (el->get_laenge1() < checklen1)
			newfocus = IDC_LAENGE1;
		else if (checklen2 && el->get_laenge2() < checklen2)
			newfocus = IDC_LAENGE2;
	}
	if (newfocus)
		GetDlgItem(newfocus)->SetFocus();

	// Rohr und Elemente checken
	CString err = el->checkPlausis();
	if (err.GetLength() > 0)
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		return false;
	}
	return true;
}

//Enabled oder disabled die Radiobuttons für die Seitenangabe der Entleerung
void EntnahmeleitungEingabe::enableEntleerungSeite(bool enable)
{
	GetDlgItem(IDC_ENTLEERUNG_RECHTS)->EnableWindow(enable);	
	GetDlgItem(IDC_ENTLEERUNG_LINKS)->EnableWindow(enable);
}

//Indicates the user clicked a button
void EntnahmeleitungEingabe::onClickedRohrnippel()
{
	doUpdate();
}

//Ohne Entleerung angeklickt, Entleerung auf Seite muss abgeschaltet oder 
//angeschaltet werden.
void EntnahmeleitungEingabe::onClickedOhne()
{
	doUpdate();
}

//Indicates the user clicked a button
void EntnahmeleitungEingabe::onClickedEntleerung_rechts()
{
	doUpdate();
}

//Indicates the user clicked a button
void EntnahmeleitungEingabe::onClickedEntleerung_links()
{
	doUpdate();
}

//Indicates the user clicked a button
void EntnahmeleitungEingabe::onClickedTypA()
{
	doUpdate();
}

//Indicates the user clicked a button
void EntnahmeleitungEingabe::onClickedTypB()
{
	doUpdate();
}
