#include "stdafx.h"

#include "resource.h"

#include "PumpenverteilerEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PumpenverteilerEingabe, WaxPartForm)

	ON_EN_KILLFOCUS(IDC_ANZAHL, doUpdate)
	ON_EN_KILLFOCUS(IDC_POSNR, doUpdate)
	
	ON_BN_CLICKED(IDC_GLATT_DN2, doUpdate)
	ON_BN_CLICKED(IDC_GLATT_DN3, doUpdate)
	ON_BN_CLICKED(IDC_FLACHFLANSCH_DN2, doUpdate)
	ON_BN_CLICKED(IDC_FLACHFLANSCH_DN3, doUpdate)
	ON_BN_CLICKED(IDC_NUTSTUTZEN_DN2, doUpdate)
	ON_BN_CLICKED(IDC_NUTSTUTZEN_DN3, doUpdate)
	
	CB_NOTIFIERS(IDC_DN1)
	CB_NOTIFIERS(IDC_DN2)
	CB_NOTIFIERS(IDC_DN3)
	CB_NOTIFIERS(IDC_WINKEL_DN1)
	CB_NOTIFIERS(IDC_WINKEL_DN4)

	ON_EN_KILLFOCUS(IDC_LAENGEDN2_L1, doUpdate)
	ON_EN_KILLFOCUS(IDC_LAENGEDN2_L2, doUpdate)
	ON_EN_KILLFOCUS(IDC_DN3, doUpdate)
	ON_EN_KILLFOCUS(IDC_GESAMTLAENGE, doUpdate)

	ON_BN_CLICKED(IDC_2ZOLLSCHWMUF, doUpdate)
	ON_EN_KILLFOCUS(IDC_LAENGE4, doUpdate)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(PumpenverteilerEingabe, WaxPartForm)

#define pv ((Pumpenverteiler*)get_data()->get_werkauftrag())

PumpenverteilerEingabe::PumpenverteilerEingabe()
: WaxPartForm(IDD_PUMPENVERTEILER, getStammdaten().getText("IDD_PUMPENVERTEILER").c_str())
{
	radiobuttons.set_dlg(this);
}

PumpenverteilerEingabe::~PumpenverteilerEingabe()
{
	
}

void PumpenverteilerEingabe::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);

	// Anzahl und Positionsnummer
	ddx_anzahl(pDX, IDC_ANZAHL);
	ddx_posnr(pDX, IDC_POSNR);

	// 1, Groupbox DN1 - Pumpendruckstutzen
	int nennweite = pv->get_dnPumpDruckSt();
	int winkel = pv->get_winkelRohnippel();
	DDX_Text(pDX, IDC_DN1, nennweite);
	DDX_Text(pDX, IDC_WINKEL_DN1, winkel);
	pv->set_dnPumpDruckSt(nennweite);
	pv->set_winkelRohnippel(winkel);

	// 2. Groupbox DN2 - Probieranschluss
	int dnProbAnschl = pv->get_dnProbAnschl();
	int laengeProbAnschl = pv->get_laengeProbAnschl();
	int abstandProbAnschl = pv->get_abstandProbAnschl();
	DDX_Text(pDX, IDC_DN2, dnProbAnschl);
	DDX_Text(pDX, IDC_LAENGEDN2_L1,laengeProbAnschl);
	DDX_Text(pDX, IDC_LAENGEDN2_L2,abstandProbAnschl);
	pv->set_dnProbAnschl(dnProbAnschl);
	pv->set_laengeProbAnschl(laengeProbAnschl);
	pv->set_abstandProbAnschl(abstandProbAnschl);	

	// 3. Groupbox DN3 - Sprinkleranlagenanschluss
	int dnSprAnlAnschl = pv->get_dnSprAnlAnschl();
	int gesamthoehe = pv->get_gesamthoehe();
	DDX_Text(pDX, IDC_DN3, dnSprAnlAnschl);
	DDX_Text(pDX, IDC_GESAMTLAENGE,gesamthoehe);
	pv->set_dnSprAnlAnschl(dnSprAnlAnschl);
	pv->set_gesamthoehe(gesamthoehe);

	// 4. Groupbox Anschluss für Auffüllbehälter
	BOOL muffe = pv->get_schwMuffe();
	int laenge4 =  pv->get_abstandSchwMuffe();
	int winkelSchwMuffe = pv->get_winkelSchwMuffe();
	DDX_Text(pDX, IDC_LAENGE4,laenge4);
	DDX_Check(pDX,IDC_2ZOLLSCHWMUF, muffe);
	DDX_Text(pDX, IDC_WINKEL_DN4, winkelSchwMuffe);
	pv->set_abstandSchwMuffe(laenge4);
	pv->set_schwMuffe(muffe ? true : false);
	pv->set_winkelSchwMuffe(winkelSchwMuffe);

	// Anschlusstypen DN2 und DN3
	if (pDX->m_bSaveAndValidate)
	{
		pv->set_probAnschl(radiobuttons.getRadioButton(true)); 
		pv->set_sprAnlAnschl(radiobuttons.getRadioButton(false));
	}
	else
	{
		radiobuttons.setRadioButtons(pv->get_probAnschl(), pv->get_sprAnlAnschl());		
	}

	// Umschaltung reduziert / nicht reduziert
	pv->set_reduziert(pv->get_dnSprAnlAnschl() != pv->get_dnPumpDruckSt());
	GetDlgItem(IDC_PUMPENVERTEILER_RED)->ShowWindow(pv->get_reduziert() ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_PUMPENVERTEILER_NRED)->ShowWindow(pv->get_reduziert() ? SW_HIDE : SW_SHOW);

	if (get_data()->get_werkauftrag()->get_status().get_wanr() == 0 || WaxSys::isAV())	
	{
		GetDlgItem(IDC_LAENGE4)->EnableWindow(pv->get_schwMuffe() ? true : false);
		GetDlgItem(IDC_WINKEL_DN4)->EnableWindow(pv->get_schwMuffe() ? true : false);
	}
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool PumpenverteilerEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return true; // Hier gibt es nur Warnungen

	// Rohr und Elemente checken
	CString err = pv->checkPlausis();
	if (err.GetLength() > 0)
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		return false;
	}

	return true;
}

BOOL PumpenverteilerEingabe::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;


	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsTextWithColon(IDC_STATIC_L3, "IDC_STATIC_L3");
	setWindowsText(IDC_STATIC_DN1, "IDC_STATIC_DN1");
	setWindowsTextWithColon(IDC_STATIC_NENNWEITE1, "IDS_NENNWEITE");
	setWindowsTextWithColon(IDC_STATIC_WINKEL_ROHRNIPPEL, "IDC_STATIC_WINKEL_ROHRNIPPEL");
	setWindowsText(IDC_STATIC_DN2, "IDC_STATIC_DN2");
	setWindowsTextWithColon(IDC_STATIC_NENNWEITE2, "IDS_NENNWEITE");
	setWindowsTextWithColon(IDC_STATIC_ANL1, "IDC_STATIC_ANL1");
	setWindowsTextWithColon(IDC_STATIC_ANL2, "IDC_STATIC_ANL2");
	setWindowsText(IDC_FLACHFLANSCH_DN2, "IDS_FLACHFLANSCH");
	setWindowsText(IDC_NUTSTUTZEN_DN2, "IDS_NUTSTUTZEN");
	setWindowsText(IDC_GLATT_DN2, "IDS_GLATT");
	setWindowsTextWithColon(IDC_STATIC_NENNWEITE3, "IDS_NENNWEITE");
	setWindowsText(IDC_FLACHFLANSCH_DN3, "IDS_FLACHFLANSCH");
	setWindowsText(IDC_NUTSTUTZEN_DN3, "IDS_NUTSTUTZEN");
	setWindowsText(IDC_GLATT_DN3, "IDS_GLATT");
	setWindowsText(IDC_STATIC_AN_AUF, "IDC_STATIC_AN_AUF");
	setWindowsText(IDC_2ZOLLSCHWMUF, "IDC_2ZOLLSCHWMUF");
	setWindowsTextWithColon(IDC_STATIC_ANH4, "IDC_STATIC_ANH4");
	setWindowsTextWithColon(IDC_STATIC_WINKELSM2, "IDC_STATIC_WINKELSM2");
	setWindowsTextWithColon(IDC_STATIC_DN3, "IDC_STATIC_DN3");

	return true;
}