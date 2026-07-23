#include "stdafx.h"

#include "resource.h"


#include "VerteilerDaten.h"
#include <mxcontrols/msgbox.h>
using namespace MxGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(VerteilerEingabe, WaxPartForm)

	ON_EN_KILLFOCUS(IDC_VERTEILER_ROHR_LAENGE,  OnChangeVerteiler_rohr_laenge)		
	//ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_VERB,	OnSelendokVerteiler_rohr_verb)
	ON_CBN_KILLFOCUS(IDC_VERTEILER_ROHR_DN,		OnSelchangeVerteiler_rohr_dn)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_ANFANG, onSelchangeVerteiler_rohr_anfang)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_ENDE,	onSelchangeVerteiler_rohr_anfang)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_DN,	do_update_rohr_dn)
	ON_COMMAND(WM_DNUPDATE, OnSelchangeVerteiler_rohr_dn)
	ON_EN_KILLFOCUS(IDC_VERTEILER_ROHR_FERTIGUNGSHINWEIS,	doUpdate)		
	ON_EN_KILLFOCUS(IDC_VERTEILER_ROHR_POSNR,				doUpdate)		
	//ON_EN_KILLFOCUS(IDC_VERTEILER_ROHR_RLAENGEN,			doUpdate)
	ON_EN_CHANGE(IDC_VERTEILER_ROHR_ANZAHL,					doUpdate)		
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_DIN,				doUpdate)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_KFARBE,				doUpdate)
	ON_BN_CLICKED(IDC_UEBERLAENGE, OnBnClickedCheckUeberlaenge)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(VerteilerEingabe, WaxPartForm)

//Konstruktor
VerteilerEingabe::VerteilerEingabe() : WaxPartForm(IDD_VERTEILER, getStammdaten().getText("IDD_VERTEILER").c_str())
{
	/*
	Konstruktor
	Setzt die Dialogid auf IDD_VERTEILER durch Übergabe 
	an den Konstruktor von WaxPartForm
	*/
	fertigungshinweis = "";
	rohrLaenge       = 6000;
	rohrDN           = 100;
	rohrQuali        = Rohrleitung::RQ2458GSCH; 
	rohrKFarbe       = 0; 
	rohrAnfang		 = NUT;	
	rohrEnde		 = NUT;	
	schnittlaenge    = 6000;
	m_Ueberlaenge	= false;
	rohr = NULL;
}

//Destruktor
VerteilerEingabe::~VerteilerEingabe()
{
}

//Dialog initialisieren
BOOL VerteilerEingabe::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;

	BauelementGruppen bg;

	int offset = 30;

	// Combobox Rohrquali
	cbinitdata3 initRohrquali[] = 
	{
		getStammdaten().getText("IDS_SONSTIGES").c_str(),			Rohrleitung::LEER,				IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2458GSCH + offset),		Rohrleitung::RQ2458GSCH,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2458GSCH_DICK + offset),	Rohrleitung::RQ2458GSCH_DICK,	IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440GSCH + offset),		Rohrleitung::RQ2440GSCH,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440NLOS + offset),		Rohrleitung::RQ2440NLOS,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2448NLOS + offset),		Rohrleitung::RQ2448NLOS,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440_2458GSCH + offset),	Rohrleitung::RQ2440_2458GSCH,	IDC_VERTEILER_ROHR_DIN,  
		bg.getBezeichnung(Rohrleitung::RQ2440VZIN + offset),		Rohrleitung::RQ2440VZIN,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQEN10217VERZ + offset),		Rohrleitung::RQEN10217VERZ,		IDC_VERTEILER_ROHR_DIN,
		"",					0,		0
	};

	INIT_CBDATA3(initRohrquali);

	if (WaxSys::para_int(APSAD_ROHRE))
	{
		cbinitdata3 initdata_[] = 
		{
			bg.getBezeichnung(Rohrleitung::RQEN10217 + offset),	Rohrleitung::RQEN10217,		IDC_VERTEILER_ROHR_DIN,
			"", 0, 0
		};
		INIT_CBDATA3(initdata_);
	}

	// Combobox Kappenfarbe
	cbinitdata3 initKFarbe[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),	Rohrleitung::KEINE,	IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_GELB").c_str(),	Rohrleitung::GELB,	IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_ROT").c_str(),		Rohrleitung::ROT,	IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_BLAU").c_str(),	Rohrleitung::BLAU,	IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_GRUEN").c_str(),	Rohrleitung::GRUEN,	IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_WEISS").c_str(),	Rohrleitung::WEISS,	IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_SCHWARZ").c_str(),	Rohrleitung::SCHWARZ,IDC_VERTEILER_ROHR_KFARBE,

		getStammdaten().getText("IDS_GOLD").c_str(),	Rohrleitung::GOLD,IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_WEISSALU").c_str(),	Rohrleitung::WEISSALU,IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_PINK").c_str(),	Rohrleitung::PINK,IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_BRAUN").c_str(),	Rohrleitung::BRAUN,IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_ORANGE").c_str(),	Rohrleitung::ORANGE,IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_LILA").c_str(),	Rohrleitung::LILA,IDC_VERTEILER_ROHR_KFARBE,


		"",			0,		0
	};
	INIT_CBDATA3(initKFarbe);
	
	// Combobox Rohranfang
	cbinitdata3 initRohranfang[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),Element::LEER,	IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(GLATT),	GLATT,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(NUT),		NUT,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(GEW),		GEW,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(FFL),		FFL,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(FFL16),	FFL16,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(FFL10),	FFL10,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(SFL),		SFL,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(VFL10),	VFL10,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(VFL10NEU),VFL10NEU,		IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(VFL16),	VFL16,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(VFL16NEU),VFL16NEU,		IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(AFL),		AFL,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(SCH),		SCH,			IDC_VERTEILER_ROHR_ANFANG,
		bg.getBezeichnung(BOD),		BOD,			IDC_VERTEILER_ROHR_ANFANG,
		"",			0,		0
	};
	INIT_CBDATA3(initRohranfang);

	for (int i = 0; initRohranfang[i].item; i++)
		initRohranfang[i].item = IDC_VERTEILER_ROHR_ENDE;
	INIT_CBDATA3(initRohranfang);
	
	rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	
	// Sychronisieren
	if (get_data()->doSync() || (rohr->get_status().get_wanr() != 0 && !WaxSys::isAV()))
	{
		GetDlgItem(IDC_VERTEILER_ROHR_DIN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_VERTEILER_ROHR_DIN)->EnableWindow(TRUE);
	}

	// Dialogfelder: Rohrdaten setzen
	rohrLaenge		= rohr->get_laenge();
	rohrDN			= rohr->get_dnvl();
	m_Ueberlaenge	= rohr->get_Ueberlaenge();
	
	if (rohrLaenge > 9000 && (rohr->get_dnvl() < 50 || rohr->getLastRohrDN() < 50)) 
	{
		GetDlgItem(IDC_UEBERLAENGE)->EnableWindow(TRUE);
		if(m_Ueberlaenge)
		{
			((CButton*) GetDlgItem(IDC_UEBERLAENGE))->SetCheck(TRUE);
		}
		else
		{
			((CButton*) GetDlgItem(IDC_UEBERLAENGE))->SetCheck(FALSE);
		}
	}
	else
	{
		((CButton*) GetDlgItem(IDC_UEBERLAENGE))->SetCheck(FALSE);
		GetDlgItem(IDC_UEBERLAENGE)->EnableWindow(FALSE);
	}


	
	CEdit* ed = (CEdit*)GetDlgItem(IDC_VERTEILER_ROHR_FERTIGUNGSHINWEIS);
	ed->SetLimitText(512);

	rohrAnfang = rohr->get_rohrAnfang();
	rohrEnde   = rohr->get_rohrEnde();
	rohrQuali  = rohr->get_rohrQuali();
	rohrVerb   = rohr->get_rohrVerb();
	rohrKFarbe = rohr->get_rohrKFarbe();
	fertigungshinweis = rohr->get_fertigungshinweis();

	schnittlaenge = rohr->get_schnittlaenge();
	if (!WaxSys::isAV())
	{
		GetDlgItem(IDC_EDIT_SLAENGE)->ShowWindow(SW_HIDE); //nur in der Av sichtbar
		GetDlgItem(IDC_STATIC_SLAENGE)->ShowWindow(SW_HIDE);
	}

	setWindowsText(IDC_POSNR, "IDC_POSNR");
	setWindowsText(IDC_ANZAHL, "IDC_ANZAHL");
	setWindowsText(IDC_DN, "IDC_DN");
	setWindowsText(IDC_LAENGE, "IDC_LAENGE");
	setWindowsText(IDC_DIN, "IDC_DIN");
	setWindowsText(IDC_ANFANG, "IDC_ANFANG");
	setWindowsText(IDC_ENDE, "IDC_ENDE");
	setWindowsText(IDC_STATIC_FERTHINWEIS, "IDC_STATIC_FERTHINWEIS");
	setWindowsText(IDC_SCHNITTLAENGE, "IDC_SCHNITTLAENGE");
	setWindowsText(IDC_KAPPENFARBE, "IDS_KAPPENFARBE");
	setWindowsText(IDC_UEBERLAENGE, "IDS_UEBERLAENGE");



	// Update
	UpdateData(FALSE);
	
	// Setzen des korrekten Rohrqualitext bei Standardquali	
	OnSelchangeVerteiler_rohr_dn();

	// Felder fuer Verbindungsstücke enablen
	commitUpdate();

	return true;
}

void VerteilerEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Verteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
 	WaxPartForm::DoDataExchange(pDX);

	CString dn;
	if (!pDX->m_bSaveAndValidate)
		dn.Format("%i", rohrDN);

	DDX_Text(pDX,    IDC_VERTEILER_ROHR_LAENGE,		rohrLaenge);
	DDX_Text(pDX,    IDC_VERTEILER_ROHR_FERTIGUNGSHINWEIS,	fertigungshinweis);
	DDX_CBString(pDX,IDC_VERTEILER_ROHR_DN,			dn);
	DDX_Text(pDX,     IDC_EDIT_SLAENGE, schnittlaenge);

	

	if (pDX->m_bSaveAndValidate)
		rohrDN = atoi(dn);

	rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	CButton *bt = (CButton*) GetDlgItem(IDC_UEBERLAENGE);
	if (rohrLaenge > 9000 && (rohrDN < 50 || rohr->getLastRohrDN() < 50)) 
	{
		bt->EnableWindow(TRUE);
	}
	else
	{
		bt->EnableWindow(FALSE);
		((CButton*) GetDlgItem(IDC_UEBERLAENGE))->SetCheck(FALSE);
	}
	int index = bt->GetCheck();
	m_Ueberlaenge  = (index == 1) ? true : false;


	ddx_anzahl(pDX, IDC_VERTEILER_ROHR_ANZAHL);
	ddx_posnr(pDX,  IDC_VERTEILER_ROHR_POSNR);

	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_DIN,	   rohrQuali);
	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_KFARBE, rohrKFarbe);
	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_ANFANG, rohrAnfang);
	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_ENDE,   rohrEnde);


	if (pDX->m_bSaveAndValidate) 
	{
		try
		{
			if (!rohr)
				return;
			
			rohr->initDaten(rohr->get_anzahl(), rohrDN, rohrLaenge, 
						   rohr->get_posNr(), rohrQuali, rohrVerb);
			
			rohr->set_rohrKFarbe(rohrKFarbe);
			rohr->set_rohrAnfang(rohrAnfang);
			rohr->set_rohrEnde(rohrEnde);
			rohr->set_fertigungshinweis(fertigungshinweis);
			rohr->set_schnittlaenge(schnittlaenge);
			rohr->set_Ueberlaenge(m_Ueberlaenge);
		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	} 
	else
	{
		rohrAnfang = rohr->get_rohrAnfang();
		ddx_cbdata(pDX, IDC_VERTEILER_ROHR_ANFANG, rohrAnfang);

		rohrEnde = rohr->get_rohrEnde();
		ddx_cbdata(pDX, IDC_VERTEILER_ROHR_ENDE,   rohrEnde);
	}
}

//Plausibilitätscheck, wird aufgerufen, bevor 
//UpdateData(TRUE) aufgerufen wird.
bool VerteilerEingabe::check_plausis()
{
	CString err = rohr->checkPlausis();

	schnittlaenge = rohr->get_schnittlaenge();


	if (!WaxPartForm::check_plausis())
		return false; 

	if (err.GetLength() > 0)
	{
		CWnd* wnd = GetFocus();
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		if (wnd)
			wnd->SetFocus();
		return false;
	}

	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);

	UpdateData(FALSE);

	return true;
}

//Indicates the display is updated after text changes
void VerteilerEingabe::OnChangeVerteiler_rohr_laenge()
{
	commitUpdate();
}

void VerteilerEingabe::OnSelchangeVerteiler_rohr_dn()
{
	rohrDN = selchange_rohr_dn(IDC_VERTEILER_ROHR_DN, IDC_VERTEILER_ROHR_DIN);
	
	CString err = rohr->get_elementArray().changeElementDN(rohrDN);

	if (err.GetLength()) 
	{
		AfxMessageBox(err, MB_OK | MB_ICONERROR);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
	} 
	doUpdate();
}

//Indicates the user's selection is valid
/*
void VerteilerEingabe::OnSelendokVerteiler_rohr_verb()
{
	commitUpdate();
	onSelchangeVerteiler_rohr_anfang();	
	
	if (WaxSys::isAV())
	{
		if (((rohrLaenge > 6000 && rohrLaenge < 6071) || (rohrLaenge > 7449 && rohrLaenge < 7501) )
		  && (rohrVerb == NUT || rohrVerb == SNAHT) && (rohrDefLaengen.GetLength() == 0)) 
		{
			if (rohrLaenge > 6000 && rohrLaenge < 6071) 
				rohrDefLaengen.Format("%4d 70", rohrLaenge - 70);

			else if (rohrLaenge > 7449 && rohrLaenge < 7501) 
				rohrDefLaengen.Format("%4d 1501", rohrLaenge - 1501);

			rohrVerb = SNAHT;
			CComboBox* bx = (CComboBox*)GetDlgItem(IDC_VERTEILER_ROHR_VERB);
			for (int i = 0; i < 5; i++) 
			{
				if (rohrVerb == (int)bx->GetItemData(i)) 
				{
					bx->SetCurSel(i);		
					break;
				}
			}

			SetDlgItemText(IDC_VERTEILER_ROHR_RLAENGEN, rohrDefLaengen);
			
			GetDlgItem(IDC_VERTEILER_ROHR_VERB)->EnableWindow(FALSE);
			GetDlgItem(IDC_VERTEILER_ROHR_RLAENGEN)->EnableWindow(FALSE);
		}
		else 
		{
			GetDlgItem(IDC_VERTEILER_ROHR_VERB)->EnableWindow(TRUE);
			GetDlgItem(IDC_VERTEILER_ROHR_RLAENGEN)->EnableWindow(TRUE);
		}
	}

	commitUpdate();
}
*/
void VerteilerEingabe::commitUpdate()
{
	WaxPartForm::commitUpdate();

	// im Sammelauftrag alle Positionen synchronisieren
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync();

	UpdateData(FALSE);
}

void VerteilerEingabe::onSelchangeVerteiler_rohr_anfang()
{
	commitUpdate();
}

void VerteilerEingabe::OnBnClickedCheckUeberlaenge()
{
	if(((CButton*)GetDlgItem(IDC_UEBERLAENGE))->GetCheck() == 1)
	{
		AfxMessageBox(getStammdaten().getText("IDS_UEBERLAENGEN_ZULASSEN").c_str(), MB_OK | MB_ICONINFORMATION);
	}
	doUpdate();
}

