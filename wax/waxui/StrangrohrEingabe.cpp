#include "stdafx.h"

#include "resource.h"


#include <mxcontrols/msgbox.h>
#include "StrangrohrEingabe.h"
using namespace MxGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define DOUPDATE_MSG (WM_USER+1234)

BEGIN_MESSAGE_MAP(StrangrohrEingabe, WaxPartForm)
	ON_EN_KILLFOCUS(IDC_STRANGROHR_FERTIGUNGSHINWEIS,  doUpdate)		
	ON_EN_KILLFOCUS(IDC_STRANGROHR_ANZAHL,  doUpdate)		
	ON_EN_KILLFOCUS(IDC_STRANGROHR_POSNR,   doUpdate)		
	ON_EN_KILLFOCUS(IDC_STRANGROHR_LAENGE,  doUpdate)		
	ON_CBN_SELCHANGE(IDC_STRANGROHR_ANFANG,	doUpdate)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_ENDE,	doUpdate)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_DIN,	doUpdate)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_KFARBE, doUpdate)
	ON_CBN_KILLFOCUS(IDC_STRANGROHR_DN,		OnSelchangeStrangrohr_dn)
	ON_CBN_SELCHANGE(IDC_STRANGROHR_DN,		do_update_rohr_dn)
	ON_COMMAND(WM_DNUPDATE,					OnSelchangeStrangrohr_dn)
	ON_COMMAND(DOUPDATE_MSG,				doUpdate) 
	ON_BN_CLICKED(IDC_UEBERLAENGE, OnBnClickedCheckUeberlaenge)
	ON_BN_CLICKED(IDC_RADIO_EIS, &StrangrohrEingabe::OnBnClickedOnEis)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(StrangrohrEingabe, WaxPartForm)

///Destruktor
StrangrohrEingabe::~StrangrohrEingabe()
{}

///Konstruktor
StrangrohrEingabe::StrangrohrEingabe() : WaxPartForm(IDD_STRANGROHR, getStammdaten().getText("IDD_STRANGROHR").c_str())
{
	rohrLaenge      = 6000;
	rohrDN          = 50;
	rohrQuali       = Rohrleitung::RQ2458GSCH; 
	rohrKFarbe      = 0; 
	trockenanlage	= false;
	m_Ueberlaenge	= false;
	m_iAuswaertsfertigung = FALSE;
	

	rohrAnfang		= NUT;	
	rohrEnde		= NUT;	
	fertigungshinweis = "";
}


/**
Zugriff auf das Entity Objekt Strangrohr aus WaxPart
*/
Strangrohr* StrangrohrEingabe::getStrangrohr()
{
	//WaxPart Objekt holen
	WaxPart* rohrData = dynamic_cast<WaxPart*>(get_data());
	
	Werkauftrag* wa = dynamic_cast<Werkauftrag*>(rohrData->get_werkauftrag());
	wa->set_zulassungsstelle(rohrData->get_leitkarte().get_zulassungsnummer());


	XASSERT(rohrData);
	if (!rohrData)
		return NULL;


	//Aus dem WaxPart (BasisKlasse von StrangrohrData)  Strangrohr holen
	Strangrohr* strang = dynamic_cast<Strangrohr*>(rohrData->get_werkauftrag());

	XASSERT(strang);
	if (!strang)
		return NULL;

	return strang;
}




///Dialog initialisieren
BOOL StrangrohrEingabe::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	
	if (!WaxPartForm::OnInitDialog())
		return false;
	
	BauelementGruppen bg;
	int offset = 30;

	// Combobox Rohrquali
	cbinitdata3 initRohrquali[] = 
	{
		getStammdaten().getText("IDS_SONSTIGES").c_str(),			Rohrleitung::LEER,				IDC_STRANGROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2458GSCH + offset),		Rohrleitung::RQ2458GSCH,		IDC_STRANGROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2458GSCH_DICK + offset),	Rohrleitung::RQ2458GSCH_DICK,	IDC_STRANGROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440GSCH + offset),		Rohrleitung::RQ2440GSCH,		IDC_STRANGROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440NLOS + offset),		Rohrleitung::RQ2440NLOS,		IDC_STRANGROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2448NLOS + offset),		Rohrleitung::RQ2448NLOS,		IDC_STRANGROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440VZIN + offset),		Rohrleitung::RQ2440VZIN,		IDC_STRANGROHR_DIN,
		"",					0,		0
	};
	INIT_CBDATA3(initRohrquali);

	if (WaxSys::para_int(APSAD_ROHRE))
	{
		cbinitdata3 initdata_[] = 
		{
			bg.getBezeichnung(Rohrleitung::RQEN10217 + offset),		Rohrleitung::RQEN10217,		IDC_STRANGROHR_DIN,
			"", 0, 0
		};
		INIT_CBDATA3(initdata_);
	}

	

	// Combobox Kappenfarbe
	cbinitdata3 initKFarbe[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),	Rohrleitung::KEINE,	IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_GELB").c_str(),	Rohrleitung::GELB,	IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_ROT").c_str(),	    Rohrleitung::ROT,	IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_BLAU").c_str(),	Rohrleitung::BLAU,	IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_GRUEN").c_str(),	Rohrleitung::GRUEN,	IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_WEISS").c_str(),	Rohrleitung::WEISS,	IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_SCHWARZ").c_str(), Rohrleitung::SCHWARZ,IDC_STRANGROHR_KFARBE,

		getStammdaten().getText("IDS_GOLD").c_str(),	Rohrleitung::GOLD,IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_WEISSALU").c_str(),	Rohrleitung::WEISSALU,IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_PINK").c_str(),	Rohrleitung::PINK,IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_BRAUN").c_str(),	Rohrleitung::BRAUN,IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_ORANGE").c_str(),	Rohrleitung::ORANGE,IDC_STRANGROHR_KFARBE,
		getStammdaten().getText("IDS_LILA").c_str(),	Rohrleitung::LILA,IDC_STRANGROHR_KFARBE,
		"",			0,		0
	};
	INIT_CBDATA3(initKFarbe);

	// Combobox Rohranfang
	cbinitdata3 initRohranfang[] = 
	{
		getStammdaten().getText("IDS_LEER").c_str(),	Element::LEER,	IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(GLATT),						GLATT,			IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(NUT),							NUT,			IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(GEW),							GEW,			IDC_STRANGROHR_ANFANG,
		bg.getBezeichnung(SCH),							SCH,			IDC_STRANGROHR_ANFANG,
		"",			0,		0
	};
	INIT_CBDATA3(initRohranfang);

	for (int i = 0; initRohranfang[i].item; i++)
		initRohranfang[i].item = IDC_STRANGROHR_ENDE;
	INIT_CBDATA3(initRohranfang);

	

	//StrangrohrData holen
	StrangrohrData* rohrData = dynamic_cast<StrangrohrData*>(get_data());
	XASSERT(rohrData);
	if (!rohrData)
		return false;


	Strangrohr* strang = getStrangrohr();
	if (!strang)
		return false;

	
	
	int iLaenge		= strang->get_laenge();
	m_Ueberlaenge	= strang->get_Ueberlaenge();
	
	
	CString strWindowText = _T("E.I.S Fertigung");
	GetDlgItem(IDC_RADIO_EIS)->SetWindowTextA(strWindowText);
	if(WaxSys::isAV())
	{
		GetDlgItem(IDC_RADIO_EIS)->GetWindowTextA(strWindowText);
	//	GetDlgItem(IDC_RADIO_EIS)->SetWindowTextA(strWindowText + getStammdaten().getText("IDD_BOHRSCHELLEN_MOEGLICH").c_str());
		m_iAuswaertsfertigung = strang->Get_Fertigungsstaette();
	}
	else
	{
		GetDlgItem(IDC_RADIO_EIS)->ShowWindow(SW_HIDE);
	}
	

	if (iLaenge > 9000 && (strang->get_dnvl() < 50 || strang->getLastRohrDN() < 50)) 
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

	// Sychronisieren
	if (rohrData->doSync(false) || (strang->get_status().get_wanr() != 0 && !WaxSys::isAV()))
	{
		rohrDN = strang->get_dnvl();
		trockenanlage= strang->get_trockenanlage();
		rohrQuali = strang->get_rohrQuali();

		GetDlgItem(IDC_STRANGROHR_DIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STRANGROHR_DN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STRANGROHR_TANL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_EIS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STRANGROHR_DIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STRANGROHR_DN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STRANGROHR_TANL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_EIS)->EnableWindow(TRUE);
	}
	CEdit* ed = (CEdit*)GetDlgItem(IDC_STRANGROHR_FERTIGUNGSHINWEIS);
	ed->SetLimitText(512);


	if(!strang->get_GUID().IsEmpty())
	{
		GetDlgItem(IDC_STATIC_GUID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STRANGROHR_GUID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STRANGROHR_GUID)->SetWindowText(strang->get_GUID());
	}
	else
	{
		GetDlgItem(IDC_STATIC_GUID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STRANGROHR_GUID)->ShowWindow(SW_HIDE);
	}
	
	// Dialogfelder: Rohrdaten setzen
	rohrLaenge = strang->get_laenge();
	rohrKFarbe = strang->get_rohrKFarbe();
	rohrAnfang = strang->get_rohrAnfang();
	rohrEnde   = strang->get_rohrEnde();
	fertigungshinweis = strang->get_fertigungshinweis();

	if (strang->get_dnvl() >= 32 && strang->get_dnvl() <= 80)
	{
		rohrQuali = strang->get_rohrQuali();
		rohrDN = strang->get_dnvl();
	}
	else 
	{
		rohrQuali = Rohrleitung::RQ2440_2458GSCH;
		rohrDN = 50;
	}

	trockenanlage = strang->get_trockenanlage();
	int index = (trockenanlage) ? 1 : 0;
	CButton *bt = (CButton*)GetDlgItem(IDC_STRANGROHR_TANL);
	bt->SetCheck(index);

	// Update
	UpdateData(FALSE);

	// Setzen des korrekten Rohrqualitext bei Standardquali	
	OnSelchangeStrangrohr_dn();

	// Felder fuer Verbindungsstücke enablen
	commitUpdate();


	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_DN, "IDS_DN");
	setWindowsTextWithColon(IDC_STATIC_LAENGE, "IDS_LAENGE");
	setWindowsTextWithColon(IDC_STATIC1_DIN, "IDS_DIN");
	setWindowsTextWithColon(IDC_STATIC_ANFANG, "IDS_ANFANG");
	setWindowsTextWithColon(IDC_STATIC_ENDE, "IDS_ENDE");
	setWindowsTextWithColon(IDC_STATIC_KAPPENFARBE, "IDS_KAPPENFARBE");
	setWindowsTextWithColon(IDC_STRANGROHR_TANL, "IDS_TROCKENANLAGE");
	setWindowsText(IDC_STATIC_FERT, "IDC_STATIC_FERT");
	setWindowsText(IDC_UEBERLAENGE, "IDS_UEBERLAENGE");
	
	DisableAllControls();

	return true;
}

///Datenaustausch
void StrangrohrEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	WaxPartForm::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(StrangrohrEingabe)
	DDX_Text(pDX,		IDC_STRANGROHR_LAENGE,	rohrLaenge);
	DDX_Text(pDX,		IDC_STRANGROHR_DN,		rohrDN);
	DDX_Text(pDX,		IDC_STRANGROHR_FERTIGUNGSHINWEIS, fertigungshinweis);
	DDX_Check(pDX, IDC_RADIO_EIS, m_iAuswaertsfertigung);
	//}}AFX_DATA_MAP
	
	ddx_anzahl(pDX, IDC_STRANGROHR_ANZAHL);
	ddx_posnr(pDX,  IDC_STRANGROHR_POSNR);

	ddx_cbdata(pDX, IDC_STRANGROHR_DIN,	   rohrQuali);
	ddx_cbdata(pDX, IDC_STRANGROHR_KFARBE, rohrKFarbe);
	ddx_cbdata(pDX, IDC_STRANGROHR_ANFANG, rohrAnfang);
	ddx_cbdata(pDX, IDC_STRANGROHR_ENDE,   rohrEnde);


	
	Strangrohr* strang = getStrangrohr();
	CButton *bt = (CButton*) GetDlgItem(IDC_UEBERLAENGE);
	if (rohrLaenge > 9000 && (rohrDN < 50 || strang->getLastRohrDN() < 50)) 
	{
		bt->EnableWindow(TRUE);
	}
	else
	{
		bt->EnableWindow(FALSE);
		((CButton*) GetDlgItem(IDC_UEBERLAENGE))->SetCheck(FALSE);
	}
	Werkauftrag::FERTIGUNGSSTAETTE Fertigungsstaette = (Werkauftrag::FERTIGUNGSSTAETTE)  strang->Get_Fertigungsstaette();
	((CButton*) GetDlgItem(IDC_RADIO_EIS))->SetCheck(FALSE);
	if(Fertigungsstaette == Werkauftrag::MAGDEBURG)
	{
		((CButton*) GetDlgItem(IDC_RADIO_EIS))->SetCheck(TRUE);
	}
	int index = bt->GetCheck();
	m_Ueberlaenge  = (index == 1) ? true : false;

	bt = (CButton*)GetDlgItem(IDC_STRANGROHR_TANL);
	index = bt->GetCheck();
	trockenanlage = (index == 1) ? true : false;

	if (pDX->m_bSaveAndValidate) 
	{
		try
		{
			Strangrohr* strang = getStrangrohr();
			if (!strang)
				return;

			strang->initDaten(strang->get_anzahl(), rohrDN, rohrLaenge, 
			strang->get_posNr(), rohrQuali, SNAHT);
			strang->set_trockenanlage(trockenanlage);
			strang->set_Ueberlaenge(m_Ueberlaenge);
			strang->set_rohrKFarbe(rohrKFarbe);
			strang->set_rohrAnfang(rohrAnfang);
			strang->set_rohrEnde(rohrEnde);
			strang->set_fertigungshinweis(fertigungshinweis);
			strang->Set_Fertigungsstaette(m_iAuswaertsfertigung);
		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	} 
}

///Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird
bool StrangrohrEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 
		
	// Rohr und Elemente checken 
	Strangrohr* strang = getStrangrohr();
	if (!strang)
		return false;

	CString	err = strang->checkPlausis();
	if (err.GetLength() > 0)
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		return false;
	}

	CString str; 
	str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}

///Indicates a new combo box list item is selected
void StrangrohrEingabe::OnSelchangeStrangrohr_dn()
{
	rohrDN = selchange_rohr_dn(IDC_STRANGROHR_DN, IDC_STRANGROHR_DIN);

	Strangrohr* strang = getStrangrohr();
	if (!strang)
		return;

	CString err = strang->get_avelementArray().changeElementDN(rohrDN);
	strang->changeMuffenToDependsOnPipe80();
	if (err.GetLength()) 
	{
		AfxMessageBox(err, MB_OK | MB_ICONERROR);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
	} 

	doUpdate();
}

///Indicates the edit control is losing the input focus
void StrangrohrEingabe::commitUpdate()
{
	WaxPartForm::commitUpdate();

	// im Sammelauftrag alle Positionen synchronisieren
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync(false);

	UpdateData(FALSE);
}



void StrangrohrEingabe::OnBnClickedCheckUeberlaenge()
{
	if(((CButton*)GetDlgItem(IDC_UEBERLAENGE))->GetCheck() == 1)
	{
		AfxMessageBox(getStammdaten().getText("IDS_UEBERLAENGEN_ZULASSEN").c_str(), MB_OK | MB_ICONINFORMATION);
	}
	doUpdate();
}





void StrangrohrEingabe::OnBnClickedOnEis()
{
	int iAuswaertsfertigung = ((CButton*)GetDlgItem(IDC_RADIO_EIS))->GetCheck();
	WaxPart* parent = const_cast<WaxPart*>(get_data()->get_parent());
	if(!parent)
	{
		return;
	}
	if(iAuswaertsfertigung)
	{
		if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		{
			if(!((SammelAuftrag*)parent)->SammelCheckPlausiFertigungMagdeburg())
			{
				((CButton*)GetDlgItem(IDC_RADIO_EIS))->SetCheck(FALSE);
				return;
			}
		}
	}
	m_iAuswaertsfertigung = iAuswaertsfertigung;
	Strangrohr* rohr = getStrangrohr();
	rohr->Set_Fertigungsstaette(m_iAuswaertsfertigung);
	if(parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync(false);

	doUpdate();
	
}
