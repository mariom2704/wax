#include "stdafx.h"
#include "resource.h"
#include ".\stationsverteilereingabe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(StationsverteilerEingabe, WaxPartForm)

BEGIN_MESSAGE_MAP(StationsverteilerEingabe, WaxPartForm)
	//{{AFX_MSG_MAP(StationsverteilerEingabe)
	ON_BN_CLICKED(IDC_RADIO_DRUCKSTUFE1, doUpdate)
	ON_BN_CLICKED(IDC_RADIO_DRUCKSTUFE2, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_POSNR, doUpdate)
	ON_EN_KILLFOCUS(IDC_EDIT_ANZAHL, doUpdate)
	_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_VERT_ABG, OnCbnSelchangeComboVertAbg)
END_MESSAGE_MAP()

//Konstruktor
StationsverteilerEingabe::StationsverteilerEingabe() : WaxPartForm(IDD_STATVERT_EINGABE)
{
	rohrDruckstufe   = 0;
	rohrKFarbe       = 0;
}

//Destruktor
StationsverteilerEingabe::~StationsverteilerEingabe() 
{
}

BOOL StationsverteilerEingabe::OnInitDialog()
{
	// Dialoginitialisierung 
	if (!WaxPartForm::OnInitDialog())
		return false;

	StationsverteilerData *rohr = dynamic_cast<StationsverteilerData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return false;
	Stationsverteiler& stat = *rohr->get_stationsverteiler();

	// Dialogfelder: Rohrdaten setzen
	rohrKFarbe		= stat.get_rohrKFarbe();
	rohrDruckstufe  = stat.get_rohrDruckstufe();
	
	// Update
	UpdateData(FALSE);

	return true;
}

//Dialog initialisieren
void StationsverteilerEingabe::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);
	ddx_posnr(pDX, IDC_EDIT_POSNR);
	ddx_anzahl(pDX, IDC_EDIT_ANZAHL);

	//{{AFX_DATA_MAP(StatVert_Obfl)
	DDX_Radio(pDX,		IDC_RADIO_DRUCKSTUFE1,		rohrDruckstufe);
	//}}AFX_DATA_MAP

	StationsverteilerData *rohr = dynamic_cast<StationsverteilerData*>(get_data());
	XASSERT(rohr);
	Stationsverteiler& statVert = *rohr->get_stationsverteiler();

	if (pDX->m_bSaveAndValidate) 
	{
		// Stationsverteilerdaten setzen
		statVert.set_rohrDruckstufe(rohrDruckstufe == 0 ? 10 : 16);
		statVert.set_rohrKFarbe(rohrKFarbe);
	} 
	else 
		rohrDruckstufe = statVert.get_rohrDruckstufe() == 10 ? 0  : 1;
}



//Plausibilitätscheck, wird aufgerufen, bevor 
//UpdateData(TRUE) aufgerufen wird.
bool StationsverteilerEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 

	Stationsverteiler *pVerteiler = getStationsverteiler();

	if (!pVerteiler)
		return false;

	//Plausibilitäten des Verteilers prüfen
	CString err = pVerteiler->checkPlausis();
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

	CString str; str.LoadString(IDS_EINGABEN_KORREKT);
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}


Stationsverteiler *StationsverteilerEingabe::getStationsverteiler()
{
	//WaxPart Objekt holen
	WaxPart* rohrData = dynamic_cast<WaxPart*>(get_data());
	XASSERT(rohrData);
	if (!rohrData)
		return NULL;


	//Aus dem WaxPart (BasisKlasse von StrangrohrData)  Strangrohr holen
	Stationsverteiler* pVerteiler = dynamic_cast<Stationsverteiler*>(rohrData->get_werkauftrag());
	XASSERT(pVerteiler);
	if (!pVerteiler)
		return NULL;

	return pVerteiler;
}

///Combofeld Verteilerabgang unten wurde geändert
void StationsverteilerEingabe::OnCbnSelchangeComboVertAbg()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
}
