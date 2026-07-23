#include "stdafx.h"

#include "resource.h"
#include <mxutil/ddx_extension.h>
#include "StationsVerteilerBemassung.h"

#include <mxcontrols/msgbox.h>
using namespace MxGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(StationsverteilerBemassung, WaxPartForm)

BEGIN_MESSAGE_MAP(StationsverteilerBemassung, WaxPartForm)

	ON_CBN_SELENDOK(IDC_COMBO_VERT_ANF, doUpdate)
	ON_CBN_SELENDOK(IDC_COMBO_VERT_END, doUpdate)
	ON_CBN_SELENDOK(IDC_COMBO_VERT_ABG, doUpdate)
	ON_CBN_SELENDOK(IDC_COMBO_VERT_ABGOBEN, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ABGOBEN_LANG,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ABST_ABGOBEN,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN_ABGOBEN,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ABG_LANG,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ABST_ABG,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN_ABG,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN_VL,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_LAENGE,	doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_VL_LAENGE, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN1,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN2,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN3,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN4,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN5,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_DN6,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L0,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L1,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L2,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L3,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L4,		doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_L5,		doUpdate)
	ON_BN_CLICKED(IDC_RADIO_DRUCKSTUFE1, doUpdate)
	ON_BN_CLICKED(IDC_RADIO_DRUCKSTUFE2, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_POSNR, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ANZAHL, doUpdate)

END_MESSAGE_MAP()

//Konstruktor
StationsverteilerBemassung::StationsverteilerBemassung() : WaxPartForm(IDD_STATVERT_BEMASSUNG, getStammdaten().getText("IDD_STATVERT_BEMASSUNG").c_str()), m_StretchBitmapControl(IDB_BITMAP_STATVERT_EDIT)
{
	int i = 0;

	elementID     = ROHR;
	elementLaenge = 0;
	for (i=0; i<6; i++) 
	{
		elementDN[i]      = 0;
		elementRelAbst[i] = 0;
	}
	elementlEnde    = FFL;
	elementrEnde    = FFL; 
	extraAbgAbsAbst = 0;
	extraAbgDN      = 0;
	extraAbgID      = 0;
	extraAbgLaenge  = 0;
	extraAbgObenAbsAbst = 0;
	extraAbgObenDN      = 0;
	extraAbgObenID      = 0;
	extraAbgObenLaenge  = 0;
	rohrDN          = 100;
	rohrLaenge      = 6000;

	rohrDruckstufe   = 0;
}

//Destruktor
StationsverteilerBemassung::~StationsverteilerBemassung()
{
}

//Dialog initialisieren
BOOL StationsverteilerBemassung::OnInitDialog()
{
	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;

	rohr = dynamic_cast<StationsverteilerData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return false;

	init();


	BauelementGruppen bg;
	CString str; str = getStammdaten().getText("IDS_LEER").c_str();
	cbinitdata3 cbinit[] =
	{
		str,						Element::LEER,	IDC_COMBO_VERT_ANF,
		bg.getBezeichnung(FFL),	FFL,	IDC_COMBO_VERT_ANF,
		bg.getBezeichnung(SCH),	SCH, IDC_COMBO_VERT_ANF,
		bg.getBezeichnung(NUT),		NUT,	IDC_COMBO_VERT_ANF,
		bg.getBezeichnung(GLATT),	GLATT,	IDC_COMBO_VERT_ANF,
		str,						Element::LEER,	IDC_COMBO_VERT_END,
		bg.getBezeichnung(FFL),	FFL,	IDC_COMBO_VERT_END,
		bg.getBezeichnung(SCH),	SCH, IDC_COMBO_VERT_END,
		bg.getBezeichnung(NUT),		NUT,	IDC_COMBO_VERT_END,
		bg.getBezeichnung(GLATT),	GLATT,	IDC_COMBO_VERT_END,
		str,						Element::LEER,	IDC_COMBO_VERT_ABG,
		bg.getBezeichnung(MUFFE),	MUFFE,	IDC_COMBO_VERT_ABG,
		bg.getBezeichnung(GWRST),	GWRST, 	IDC_COMBO_VERT_ABG,
		bg.getBezeichnung(NUTST),	NUTST,	IDC_COMBO_VERT_ABG,
		str,						Element::LEER,	IDC_COMBO_VERT_ABGOBEN,
		bg.getBezeichnung(MUFFE),	MUFFE,	IDC_COMBO_VERT_ABGOBEN,
		bg.getBezeichnung(GWRST),	GWRST, 	IDC_COMBO_VERT_ABGOBEN,
		bg.getBezeichnung(NUTST),	NUTST,	IDC_COMBO_VERT_ABGOBEN,
		"", 0, 0
	};
	INIT_CBDATA3(cbinit);



	setCtrlStatus();

	// Sychronisieren
	rohr->doSync();


	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsTextWithColon(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsText(IDC_STATIC_DRUCK, "IDS_DRUCKSTUFE");
	setWindowsText(IDC_STATIC_EXT_EL, "IDC_STATIC_EXT_EL");
	setWindowsText(IDC_STATIC_OBEN, "IDS_OBEN");
	setWindowsText(IDC_STATIC_UNTEN, "IDS_UNTEN");
	setWindowsText(IDC_STATIC_L1, "IDS_LAENGE");
	setWindowsText(IDC_STATIC_L2, "IDS_LAENGE");
	setWindowsText(IDC_STATIC_ROHRENDE, "IDS_ANFANG_ENDE");
	setWindowsText(IDC_STATIC_ANFANG, "IDS_ANFANGSELEMENT");
    setWindowsText(IDC_STATIC_ENDE, "IDS_ENDELEMENT");
	setWindowsText(IDC_STATIC_ABGANG, "IDC_STATIC_ABGANG");


	// Update
	UpdateData(FALSE);
	return true;
}

void StationsverteilerBemassung::init()
{
	Stationsverteiler& statVert = *rohr->get_stationsverteiler();

	// Rohrdaten	
	rohrLaenge = statVert.get_laenge();
	rohrDN     = statVert.get_dnvl();
	elementrEnde = statVert.get_elementrEnde();
	elementlEnde = statVert.get_elementlEnde();

	rohrDruckstufe  = statVert.get_rohrDruckstufe();

	
	if (elementrEnde == VFL16NEU  || elementrEnde == VFL10NEU ||  elementrEnde == VFL10 || elementrEnde == VFL16)
		elementrEnde = FFL;
	else if (elementrEnde == BOD)
		elementrEnde = SCH;
	if (elementlEnde == VFL16NEU ||  elementlEnde == VFL10NEU || elementlEnde == VFL10 || elementlEnde == VFL16)
		elementlEnde = FFL;
	else if (elementlEnde == BOD)
		elementlEnde = SCH;

	for (int i = 0; i < Stationsverteiler::MAXCOUNT;i++)
	{
		// Abstaende der oberen Abgaenge
		elementRelAbst[i] = statVert.get_elementRelAbst(i);
		// Nennweiten der Abgänge
		elementDN[i] = statVert.get_elementDN(i);
	}

	elementLaenge = statVert.get_elementLaenge();

	extraAbgAbsAbst = statVert.get_extraAbgAbsAbst();
	extraAbgDN      = statVert.get_extraAbgDN();
	extraAbgLaenge  = statVert.get_extraAbgLaenge();
	extraAbgID      = statVert.get_extraAbgID();
	
	extraAbgObenAbsAbst = statVert.get_extraAbgObenAbsAbst();
	extraAbgObenDN      = statVert.get_extraAbgObenDN();
	extraAbgObenLaenge  = statVert.get_extraAbgObenLaenge();
	extraAbgObenID      = statVert.get_extraAbgObenID();

	set_rohrRestlaenge();
}

void StationsverteilerBemassung::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Statioensverteiler übergeben
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	WaxPartForm::DoDataExchange(pDX);

	ddx_posnr(pDX, IDC_EDIT_POSNR);
	ddx_anzahl(pDX, IDC_EDIT_ANZAHL);


	//{{AFX_DATA_MAP(StationsverteilerBemassung)
	DDX_Text(pDX,IDC_EDIT_DN1,	elementDN[0]);
	DDX_Text(pDX,IDC_EDIT_DN2,	elementDN[1]);
	DDX_Text(pDX,IDC_EDIT_DN3,	elementDN[2]);
	DDX_Text(pDX,IDC_EDIT_DN4,	elementDN[3]);
	DDX_Text(pDX,IDC_EDIT_DN5,	elementDN[4]);
	DDX_Text(pDX,IDC_EDIT_DN6,	elementDN[5]);
	DDX_Text(pDX,IDC_EDIT_L0,	elementRelAbst[0]);
	DDX_Text(pDX,IDC_EDIT_L1,	elementRelAbst[1]);
	DDX_Text(pDX,IDC_EDIT_L2,	elementRelAbst[2]);
	DDX_Text(pDX,IDC_EDIT_L3,	elementRelAbst[3]);
	DDX_Text(pDX,IDC_EDIT_L4,	elementRelAbst[4]);
	DDX_Text(pDX,IDC_EDIT_L5,	elementRelAbst[5]);
	DDX_Text(pDX,IDC_EDIT_LAENGE,	elementLaenge);
	DDX_Text(pDX,IDC_EDIT_ABST_ABG,	extraAbgAbsAbst);
	DDX_Text(pDX,IDC_EDIT_DN_ABG,	extraAbgDN);
	DDX_Text(pDX,IDC_EDIT_ABG_LANG,	extraAbgLaenge);
	DDX_Text(pDX,IDC_EDIT_ABST_ABGOBEN,	extraAbgObenAbsAbst);
	DDX_Text(pDX,IDC_EDIT_DN_ABGOBEN,	extraAbgObenDN);
	DDX_Text(pDX,IDC_EDIT_ABGOBEN_LANG,	extraAbgObenLaenge);
	DDX_Text(pDX,IDC_EDIT_DN_VL,	rohrDN);
	DDX_Text(pDX,IDC_EDIT_VL_LAENGE,rohrLaenge);
	DDX_Radio(pDX,IDC_RADIO_DRUCKSTUFE1,rohrDruckstufe);
	DDX_Control(pDX,IDC_STATIC_STATIONS_VERTEILER, m_StretchBitmapControl);
	//}}AFX_DATA_MAP

	ddx_cbdata(pDX, IDC_COMBO_VERT_ANF, elementlEnde);
	ddx_cbdata(pDX, IDC_COMBO_VERT_END, elementrEnde);
	ddx_cbdata(pDX, IDC_COMBO_VERT_ABG, extraAbgID);
	ddx_cbdata(pDX, IDC_COMBO_VERT_ABGOBEN, extraAbgObenID);


	rohr = dynamic_cast<StationsverteilerData*>(get_data());
	XASSERT(rohr);
	Stationsverteiler& statVert = *rohr->get_stationsverteiler();

	if (pDX->m_bSaveAndValidate)
	{
		try
		{
			// Rohrdaten setzen
			statVert.set_dnvl(rohrDN);
			statVert.set_laenge(rohrLaenge);

			// Elementdaten setzen
			statVert.setElementdaten(elementDN, elementRelAbst, elementID, elementLaenge,	
				statVert.correctElemID(elementlEnde, rohrDN), 
				statVert.correctElemID(elementrEnde, rohrDN),	
				extraAbgAbsAbst, extraAbgDN, extraAbgID, extraAbgLaenge,
				extraAbgObenAbsAbst, extraAbgObenDN, extraAbgObenID, extraAbgObenLaenge);

			statVert.set_elementrEnde(statVert.correctElemID(elementrEnde, rohrDN));
			statVert.set_elementlEnde(statVert.correctElemID(elementlEnde, rohrDN));
			
			statVert.set_extraAbgID(extraAbgID);//?

			statVert.set_rohrDruckstufe(rohrDruckstufe == 0 ? 10 : 16);
		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	}
	else
		rohrDruckstufe = statVert.get_rohrDruckstufe() == 10 ? 0  : 1;
}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) 
//aufgerufen wird.
bool StationsverteilerBemassung::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false;

	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) 
	//aufgerufen wird.
	Stationsverteiler& statVert = *rohr->get_stationsverteiler();
	
	// Rohr und Elemente checken
	CString err = statVert.checkPlausis();
	if (err.GetLength() > 0)
	{
		CWnd* wnd = GetFocus();
		init();
		UpdateData(FALSE);
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		if (wnd)
			wnd->SetFocus();
		return false;
	}

	// OK
	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}

//Update Control
//set rohrRestlaenge
void StationsverteilerBemassung::set_rohrRestlaenge()
{
	int rohrRestlaenge = rohrLaenge - elementRelAbst[0] - elementRelAbst[1]
									- elementRelAbst[2] - elementRelAbst[3]
									- elementRelAbst[4] - elementRelAbst[5];
	SetDlgItemInt(IDC_EDIT_LX, rohrRestlaenge);
}

///Enabled und disabled die Dialogelemente in Abhängigkeit der Eingaben
void StationsverteilerBemassung::setCtrlStatus()
{
	/**
	Dialogkontrolle 
	Hier werden Steuerelementen aktiviert und deaktiviert  
	der einer ElementID entspricht. 
	*/
	if (extraAbgID == 0)
	{
		//wenn nicht gesetzt dann Eingabefelder deaktivieren
		GetDlgItem(IDC_EDIT_ABG_LANG)->EnableWindow(FALSE);
		extraAbgLaenge = 0;

		GetDlgItem(IDC_EDIT_ABST_ABG)->EnableWindow(FALSE);
		extraAbgAbsAbst = 0;

		GetDlgItem(IDC_EDIT_DN_ABG)->EnableWindow(FALSE);
		extraAbgDN = 0;
	}
	else if (extraAbgID == NUTST || extraAbgID == GWRST) 
	{
		GetDlgItem(IDC_EDIT_ABG_LANG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ABST_ABG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DN_ABG)->EnableWindow(TRUE);
	} 
	else if (extraAbgID == MUFFE) 
	{
		int laenge = 0;
		const CObList& grp = getStammdaten().get_bauelemente();
		for (POSITION gpos = grp.GetHeadPosition(); gpos;)
		{
			Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
			if ((g->get_elemid() == extraAbgID) 
			 && (g->get_dn() == extraAbgDN) 
			 && (g->get_dnvl() == rohrDN || g->get_dnvl() == 0)) 
			{
				laenge = g->get_mass1();
			}
		}
		extraAbgLaenge = laenge;
		GetDlgItem(IDC_EDIT_ABG_LANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABST_ABG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DN_ABG)->EnableWindow(TRUE);

	}

	if (extraAbgObenID == 0)
	{
		//wenn nicht gesetzt dann Eingabefelder deaktivieren
		GetDlgItem(IDC_EDIT_ABGOBEN_LANG)->EnableWindow(FALSE);
		extraAbgObenLaenge = 0;

		GetDlgItem(IDC_EDIT_ABST_ABGOBEN)->EnableWindow(FALSE);
		extraAbgObenAbsAbst = 0;

		GetDlgItem(IDC_EDIT_DN_ABGOBEN)->EnableWindow(FALSE);
		extraAbgObenDN = 0;
	}
	if (extraAbgObenID == NUTST || extraAbgObenID == GWRST) 
	{
		GetDlgItem(IDC_EDIT_ABGOBEN_LANG)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ABST_ABGOBEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DN_ABGOBEN)->EnableWindow(TRUE);
	} 
	else if (extraAbgObenID == MUFFE) 
	{
		int laenge = 0;
		const CObList& grp = getStammdaten().get_bauelemente();
		for (POSITION gpos = grp.GetHeadPosition(); gpos;)
		{
			Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
			if ((g->get_elemid() == extraAbgObenID) 
			 && (g->get_dn() == extraAbgObenDN) 
			 && (g->get_dnvl() == rohrDN || g->get_dnvl() == 0)) 
			{
				laenge = g->get_mass1();
			}
		}
		
		extraAbgObenLaenge = laenge;
		GetDlgItem(IDC_EDIT_ABGOBEN_LANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ABST_ABGOBEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DN_ABGOBEN)->EnableWindow(TRUE);
	}

	int dnids[] = { IDC_EDIT_DN1, IDC_EDIT_DN2, IDC_EDIT_DN3, IDC_EDIT_DN4, IDC_EDIT_DN5, IDC_EDIT_DN6 };
	int lids[] =  { IDC_EDIT_L0,  IDC_EDIT_L1,  IDC_EDIT_L2,  IDC_EDIT_L3,  IDC_EDIT_L4,  IDC_EDIT_L5 };


	if (!GetDlgItem(dnids[0])->IsWindowEnabled())
		GetDlgItem(lids[0])->EnableWindow(false);

	for (int i = 0; i < 5; i++)
	{
		if (elementDN[i] == 0)
			GetDlgItem(lids[i+1])->EnableWindow(false);
		else if (get_data()->get_werkauftrag()->get_status().get_wanr() == 0 || WaxSys::isAV())
		{
			GetDlgItem(lids[i])->EnableWindow(true);
			GetDlgItem(lids[i+1])->EnableWindow(true);
		}
	}
}

//Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
//Plausicheck true liefert.
void StationsverteilerBemassung::commitUpdate()
{
	//Updatedata hier notwendig da sonst die Druckstufe nicht richtig eingelesen wird!
    UpdateData();

	WaxPartForm::commitUpdate();
	set_rohrRestlaenge();
	setCtrlStatus();

	// Update
	UpdateData(FALSE);
}
