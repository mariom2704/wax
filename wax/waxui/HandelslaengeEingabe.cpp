#include "stdafx.h"


#include "resource.h"


#include <mxcontrols/msgbox.h>
using namespace MxGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(HandelslaengeEingabe, WaxPartForm)
	ON_EN_CHANGE(IDC_VERTEILER_ROHR_POSNR,			onUpdate)	
	ON_EN_KILLFOCUS(IDC_VERTEILER_ROHR_ANZAHL,		onUpdate)
	ON_CBN_SELCHANGE(IDC_VERTEILERHL_ROHR_LAENGE,	onUpdate)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_DIN,		onUpdate)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_KFARBE,		onUpdate)
	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_ANFANGENDE, onUpdate)

	ON_CBN_SELCHANGE(IDC_VERTEILER_ROHR_DN,	do_update_rohr_dn)
	ON_CBN_KILLFOCUS(IDC_VERTEILER_ROHR_DN,	onChangeDn)
	ON_COMMAND(WM_DNUPDATE,					onChangeDn)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(HandelslaengeEingabe, WaxPartForm)

HandelslaengeEingabe::HandelslaengeEingabe( ) : WaxPartForm(IDD_HANDELSLAENGE, getStammdaten().getText("IDD_HANDELSLAENGE").c_str())
{
	rohrLaenge		= 6000;
	rohrDN			= 100;
	rohrQuali		= Rohrleitung::RQ2458GSCH; 
	rohrKFarbe		= 0; 
	rohrAnfangEnde	= NUT;	
}

HandelslaengeEingabe::~HandelslaengeEingabe()
{
}

BOOL HandelslaengeEingabe::OnInitDialog()
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
	// Init div. Comboboxen
	cbinitdata3 initdata[] = 
	{
		getStammdaten().getText("IDS_SONSTIGES").c_str(),				Rohrleitung::LEER,				IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2458GSCH + offset),			Rohrleitung::RQ2458GSCH,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440GSCH + offset),			Rohrleitung::RQ2440GSCH,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440NLOS + offset),			Rohrleitung::RQ2440NLOS,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2448NLOS + offset),			Rohrleitung::RQ2448NLOS,		IDC_VERTEILER_ROHR_DIN,
		bg.getBezeichnung(Rohrleitung::RQ2440_2458GSCH + offset),		Rohrleitung::RQ2440_2458GSCH,	IDC_VERTEILER_ROHR_DIN,        
		
		getStammdaten().getText("IDS_OHNE").c_str(),			Rohrleitung::KEINE,			IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_ROT").c_str(),				Rohrleitung::ROT,			IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_BLAU").c_str(),			Rohrleitung::BLAU,			IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_GELB").c_str(),			Rohrleitung::GELB,			IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_GRUEN").c_str(),			Rohrleitung::GRUEN,			IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_WEISS").c_str(),			Rohrleitung::WEISS,			IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_SCHWARZ").c_str(),			Rohrleitung::SCHWARZ,		IDC_VERTEILER_ROHR_KFARBE,
		getStammdaten().getText("IDS_LEER").c_str(),			Rohrleitung::LEER,			IDC_VERTEILER_ROHR_ANFANGENDE,
		getStammdaten().getText("IDS_GLATT").c_str(),			GLATT,			IDC_VERTEILER_ROHR_ANFANGENDE,
		getStammdaten().getText("IDS_NUT").c_str(),				NUT,			IDC_VERTEILER_ROHR_ANFANGENDE,
		getStammdaten().getText("IDS_GEW").c_str(),				GEW,			IDC_VERTEILER_ROHR_ANFANGENDE,
		"", 0, 0
	};
	INIT_CBDATA3(initdata);

	if (WaxSys::para_int(APSAD_ROHRE))
	{
		cbinitdata3 initdata_[] = 
		{
			bg.getBezeichnung(Rohrleitung::RQEN10217 + offset),		Rohrleitung::RQEN10217,		IDC_VERTEILER_ROHR_DIN,
			"", 0, 0
		};
		INIT_CBDATA3(initdata_);
	}

	// In der AV kann das Rohr bei "verzinkt" von scharzes in verzinktes geaendert werden
    if (WaxSys::isAV() && get_data()->get_werkauftrag()->get_beschichtung().isVerzinkt())
	{
		// Combobox Rohrquali
		cbinitdata3 initRohrquali[] = 
		{
			bg.getBezeichnung(Rohrleitung::RQ2440VZIN + offset),	Rohrleitung::RQ2440VZIN,		IDC_VERTEILER_ROHR_DIN,
			"",					0,		0
		};
		INIT_CBDATA3(initRohrquali);
	}
	
	rohr = dynamic_cast<HandelslaengeData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return false;
	Handelslaenge* handel = getHandelslaenge();

	// Sychronisieren
	if (rohr->doSync())
	{
		GetDlgItem(IDC_VERTEILER_ROHR_DIN)->EnableWindow(FALSE);
	}

	// Dialogfelder: Rohrdaten setzen
	rohrLaenge		= handel->get_laenge();
	rohrDN			= handel->get_dnvl();
	rohrQuali		= handel->get_rohrQuali();
	rohrAnfangEnde	= handel->get_rohrAnfang();
	rohrKFarbe		= handel->get_rohrKFarbe();
	
	// Update
	UpdateData(FALSE);

	// Setzen des korrekten Rohrqualitext bei Standardquali	
	onChangeDn();

	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_DN, "IDS_DN");
	setWindowsTextWithColon(IDC_STATIC_LAENGE_CA, "IDS_LAENGE_CA");
	setWindowsTextWithColon(IDC_STATIC_DIN, "IDS_DIN");
	setWindowsTextWithColon(IDC_STATIC_AE, "IDS_ANFANG_ENDE");
	setWindowsTextWithColon(IDC_STATIC_KF, "IDS_KAPPENFARBE");


	return true;
}

void HandelslaengeEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	WaxPartForm::DoDataExchange(pDX);

	CString dn;
	if (!pDX->m_bSaveAndValidate)
		dn.Format("%i", rohrDN);
	
	CString laenge;
	if (!pDX->m_bSaveAndValidate)
		laenge.Format("%i", rohrLaenge);

	//{{AFX_DATA_MAP(VerteilerEingabe)
	DDX_CBString(pDX,IDC_VERTEILER_ROHR_DN,			dn);		
	DDX_CBString(pDX,IDC_VERTEILERHL_ROHR_LAENGE,	laenge);		
	//}}AFX_DATA_MAP

	ddx_anzahl(pDX, IDC_VERTEILER_ROHR_ANZAHL);
	ddx_posnr(pDX,  IDC_VERTEILER_ROHR_POSNR);

	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_DIN,    rohrQuali);
	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_KFARBE, rohrKFarbe);
	ddx_cbdata(pDX, IDC_VERTEILER_ROHR_ANFANGENDE, rohrAnfangEnde);
	
	if (pDX->m_bSaveAndValidate) 
	{
		rohrDN = atoi(dn);
		rohrLaenge = atoi(laenge);
		try
		{
			rohr = dynamic_cast<HandelslaengeData*>(get_data());
			if (!rohr)
				return;
			Handelslaenge* handel = getHandelslaenge();
		
			handel->initDaten(handel->get_anzahl(), rohrDN, rohrLaenge, 
							 handel->get_posNr(), rohrQuali, SNAHT);
			handel->set_rohrKFarbe(rohrKFarbe);
			handel->set_rohrAnfang(rohrAnfangEnde);
			handel->set_rohrEnde(rohrAnfangEnde);
		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	} 
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool HandelslaengeEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 

	CString err = getHandelslaenge()->checkPlausis();
	if (err.GetLength() > 0)
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		return false;
	}

	// OK
	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}

//Indicates the display is updated after text changes
void HandelslaengeEingabe::onUpdate()
{
	// check Plausis
	doUpdate();

	// im Sammelauftrag alle Positionen synchronisieren
	WaxPart *parent = const_cast<WaxPart*>(get_data()->get_parent());
	if (parent->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		parent->doSync();
}

//Indicates a new combo box list item is selected
void HandelslaengeEingabe::onChangeDn()
{
	doUpdate();
	rohrDN = selchange_rohr_dn(IDC_VERTEILER_ROHR_DN, IDC_VERTEILER_ROHR_DIN);
	doUpdate();
}



/**
Zugriff auf das Entity Objekt Handelslaenge aus WaxPart
*/
Handelslaenge* HandelslaengeEingabe::getHandelslaenge()
{
	//WaxPart Objekt holen
	WaxPart* rohrData = dynamic_cast<WaxPart*>(get_data());
	XASSERT(rohrData);
	if (!rohrData)
		return NULL;


	//Aus dem WaxPart Verteiler holen
	Handelslaenge* rohr = dynamic_cast<Handelslaenge*>(rohrData->get_werkauftrag());
	XASSERT(rohr);

	return rohr;
}