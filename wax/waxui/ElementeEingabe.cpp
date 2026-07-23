#include "stdafx.h"


#include "resource.h"
#include "elementeeingabe.h"

#include <mxutil/ddx_extension.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
	
#define WM_UPDATE_FIELDS WM_USER+1234

IMPLEMENT_DYNCREATE(ElementeEingabe, WaxPartForm)

/*
Anderungen:
22.06.06, Jacobs: Bei IDC_ELEMENTE_ID Ereignishandler von ON_EN_KILLFOCUS in ON_CBN_SELCHANGE geändert,
                  damit gleich nach der Auswahl die anderen Felder richtig gesetzt werden
*/

BEGIN_MESSAGE_MAP(ElementeEingabe, ElementeEingabeForm)
	ON_COMMAND(IDS_ELEM_NEW,				onElemNew)
	ON_COMMAND(IDC_ELEM_NEW,				onElemNew)
	ON_COMMAND(IDS_ELEM_DELETE,				onElemDelete)
	ON_COMMAND(IDS_ELEM_CHANGE,				onElemChange)
	ON_COMMAND(IDC_ELEM_DELETE,				onElemDelete)
	ON_COMMAND(IDC_ELEM_CHANGE,				onElemChange)
	ON_NOTIFY(NM_CLICK,		IDC_ELEMDATEN,	onClickElemdaten)
	ON_NOTIFY(NM_RCLICK,	IDC_ELEMDATEN,	onRclickElemdaten)
	ON_NOTIFY(LVN_KEYDOWN,	IDC_ELEMDATEN,	onKeydownElemdaten)
	ON_EN_KILLFOCUS(IDC_ELEMENTE_LAENGE,	doUpdate)
	ON_EN_KILLFOCUS(IDC_ELEMENTE_ABSTREL,	onKillfocusElement_abstrel)
	ON_CBN_KILLFOCUS(IDC_ELEMENTE_WINKEL,	onKillfocusElement_eledn)
	ON_CBN_SELCHANGE(IDC_ELEMENTE_ELEDN,	onKillfocusElement_eledn)
	ON_CBN_KILLFOCUS(IDC_ELEMENTE_ELEDN,	onKillfocusElement_eledn)
	ON_CBN_SELCHANGE(IDC_ELEMENTE_ID,		onSelChangeElement_id)
	ON_COMMAND(WM_UPDATE_FIELDS,			elem_setFields)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DREHWINKEL, on_grafik_drehen)
	ON_BN_CLICKED(IDC_ELEMENTEINGABE_INVERTIEREN, on_elementeingabe_invertieren)
	ON_BN_CLICKED(IDC_ELEMENTE_ANLAGE, on_anlage)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//Destruktor
ElementeEingabe::~ElementeEingabe()
{
// Zu Verkettungselemente: Endestücke, z.B. Flansch auf Rohr, sind immer mit
// relativen Laenge 0. Verkettung wird erst angeklickt, wenn man z.B. einen
// Stutzen auf einen Stutzen setzen will. Jetzt gibt die relative Laenge den 
// Abstand vom Fuss des Anschlussstutzens bis Mitte Aufsatzstutzen. 
// Bei dessen Abschluss, z.B. wieder Flansch, bleibt Verkettung ausgewählt,
// und die relative Laenge ist ebenfalls wieder 0.    
}

//Konstruktor
ElementeEingabe::ElementeEingabe() : ElementeEingabeForm(IDD_ELEMENTE, getStammdaten().getText("IDD_ELEMENTE").c_str())
{

}

void ElementeEingabe::OnSize(UINT nType, int cx, int cy)
{	

	m_iIDCNew = IDC_ELEM_NEW;
	ElementeEingabeForm::OnSize( nType,  cx,  cy);
}

/**
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
*/
BOOL ElementeEingabe::OnInitDialog()
{
	if (!ElementeEingabeForm::OnInitDialog())
		return false;

	anschlussDN = get_rohrleitung()->get_dnvl();
	
	CString rohrbez;
	BauelementGruppen bg;
	rohrbez = bg.getBezeichnung(ROHR)+ " + ";
	
	CString leerstr;
	leerstr = getStammdaten().getText("IDS_LEER").c_str();
	
	cbinitdata3 cbdat[] = 
	{
		leerstr, Element::LEER, IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(NUT), translateElementIdToComboBox(NUT), IDC_ELEMENTE_ID,
		bg.getBezeichnung(NUTST), NUTST, IDC_ELEMENTE_ID,
		bg.getBezeichnung(MUFFE), MUFFE, IDC_ELEMENTE_ID,
		bg.getBezeichnung(GWRST), GWRST, IDC_ELEMENTE_ID,
		bg.getBezeichnung(ELEMENT_BOGEN), ELEMENT_BOGEN, IDC_ELEMENTE_ID,
		bg.getBezeichnung(BOG2S), BOG2S, IDC_ELEMENTE_ID,
		bg.getBezeichnung(KRED), KRED, IDC_ELEMENTE_ID,
		bg.getBezeichnung(ERED), ERED, IDC_ELEMENTE_ID,
		bg.getBezeichnung(EINZ), EINZ, IDC_ELEMENTE_ID,
		bg.getBezeichnung(BOHR50), BOHR50, IDC_ELEMENTE_ID,
		bg.getBezeichnung(ROHRNIPPEL), ROHRNIPPEL, IDC_ELEMENTE_ID,
		bg.getBezeichnung(POTENIALAUSGLEICH), POTENIALAUSGLEICH, IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(FFL), translateElementIdToComboBox(FFL), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(SFL), translateElementIdToComboBox(SFL), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(FFL16),		translateElementIdToComboBox(FFL16),IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(FFL10),		translateElementIdToComboBox(FFL10),IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(VFL10), translateElementIdToComboBox(VFL10), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(VFL10NEU), translateElementIdToComboBox(VFL10NEU), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(VFL16NEU), translateElementIdToComboBox(VFL16NEU), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(VFL16), translateElementIdToComboBox(VFL16), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(AFL),	translateElementIdToComboBox(AFL), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(GEW), translateElementIdToComboBox(GEW), IDC_ELEMENTE_ID,
		rohrbez + bg.getBezeichnung(GLATT), translateElementIdToComboBox(GLATT), IDC_ELEMENTE_ID,
		bg.getBezeichnung(KNICK), KNICK, IDC_ELEMENTE_ID,
		"", 0, 0
	};
	INIT_CBDATA3(cbdat);

	if (av)
	{ // Standardstutzen und Muffe sieht man nur in der AV Version
		cbinitdata3 cbdat[] = 
		{
			bg.getBezeichnung(STDSTUTZEN), STDSTUTZEN,IDC_ELEMENTE_ID,
			bg.getBezeichnung(STDMUFFE), STDMUFFE, IDC_ELEMENTE_ID,
			"", 0, 0
		};
		INIT_CBDATA3(cbdat);
		
		GetDlgItem(IDC_ELEMENTE_ANLAGE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ELEMENTE_ANLAGE)->ShowWindow(SW_HIDE); 
	}

	if (get_data()->get_werkauftrag()->get_status().get_wanr() != 0 && !av)
		GetDlgItem(IDC_ELEMENTE_ANSDN)->EnableWindow(FALSE);

	//Bereich für Combobox mit Dn Werten einstellen
	dnComboBox.setRange(10,400,true);

	if (list.GetItemCount() > 0)
		elem_setFields();

	
	setWindowsText(IDC_STATIC_TYP, "IDS_TYP");
	setWindowsText(IDC_STATIC_AN_DN, "IDS_AN_DN");
	setWindowsText(IDC_STATIC_DN, "IDS_DN");
	setWindowsText(IDC_LAENGE_DURCHMESSER, "IDS_LAENGE");
	setWindowsText(IDC_STATIC_WINKEL, "IDS_WINKEL");
	setWindowsText(IDC_STATIC_ABS_ABST, "IDS_ABS_ABST");
	setWindowsText(IDC_STATIC_REL_ABST, "IDS_REL_ABST");

	setWindowsText(IDC_ELEMENTE_ANLAGE, "IDS_ANLAGE");
	setWindowsText(IDC_STATIC_ROTATE, "IDS_ROTATE");
	setWindowsText(IDC_ELEMENTEINGABE_INVERTIEREN, "IDS_ELEMENTEINGABE_INVERTIEREN");

	setWindowsText(IDC_ELEM_NEW, "IDS_ELEM_NEW");
	setWindowsText(IDC_ELEM_CHANGE, "IDS_ELEM_CHANGE");
	setWindowsText(IDC_ELEM_DELETE, "IDS_ELEM_DELETE");
	

	return true;
}

void ElementeEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	ElementeEingabeForm::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ELEMENTE_ABSTREL,	abstRel);
	DDX_Text(pDX,		IDC_ELEMENTE_ABSTABS,	abstAbs);
	DDX_Text(pDX,		IDC_ELEMENTE_ANSDN,		anschlussDN);
	DDX_Text(pDX,		IDC_ELEMENTE_WINKEL,	winkel);
	DDX_Text(pDX,		IDC_ELEMENTE_LAENGE,	laenge);
	DDX_Control(pDX,	IDC_ELEMDATEN,			list);
	DDX_Control(pDX,	IDC_ELEMENTE_DURCHMESSER,ed_durchmesser);
	DDX_Control(pDX,	IDC_ELEMENTE_ELEDN,		dnComboBox);
	DDX_Text(pDX,		IDC_ELEMENTE_ELEDN,		dn);


	//ddx_cbdata(pDX, IDC_ELEMENTE_ELEDN, dn);

	ed_durchmesser.doDataExchange(pDX);

	CButton *bt = (CButton*)GetDlgItem(IDC_ELEMENTE_ANLAGE);
	makeAnlage = (bt->GetCheck() == 0) ? false : true;

	if (!pDX->m_bSaveAndValidate) 
	{
		if (get_rohrleitung()->get_elementArray().GetSize() > 0) 
		{
			Element* ele = get_rohrleitung()->get_elementArray().get_element_at(0);
			XASSERT(ele);
			if (ele)
				SetDlgItemInt(IDC_DREHWINKEL, ele->get_korrektur_winkel());
		}
		else
			SetDlgItemInt(IDC_DREHWINKEL, 0);

		show_elements(-1);
	}
	
	ddx_cbdata(pDX, IDC_ELEMENTE_ID, id);
}

bool ElementeEingabe::check_plausis()
{
	/**
	Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
	wird.
	*/
	// Rohr und Elemente checken
	CString err = get_rohrleitung()->checkPlausis();

	if (!ElementeEingabeForm::check_plausis())
		return false;
	

	if (err.GetLength())
	{
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONERROR);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		
		UpdateData();
		return false;
	}

	get_data()->get_werkauftrag()->set_plausiOk(true);
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(IDS_EINGABEN_KORREKT);
	return (true);
}

//Einfügen einer neuen Position
void ElementeEingabe::elem_setFields()
{
	ElementeEingabeForm::elem_setFields();

	CButton* bt;
	bt = (CButton*)GetDlgItem(IDC_ELEMENTE_ANLAGE);
	bt->SetCheck((makeAnlage)?1:0);

	///Enablen/Disablen von Eingabefeldern je nach ElementID
	enableDisableFields();

	UpdateData();
}


void ElementeEingabe::on_grafik_drehen(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	NMUPDOWN* lpnmud = (LPNMUPDOWN)pNMHDR;
	
	int winkel = iterate_drehwinkel(lpnmud->iDelta);

	get_rohrleitung()->apply_korrektur_winkel(winkel);
	
	show_elements(-1);
	doUpdate();
}

int ElementeEingabe::iterate_drehwinkel(int delta) 
{
	const int winkel[] = { 0, 90, 180, 270, -1 };

	int curwinkel = GetDlgItemInt(IDC_DREHWINKEL);

	// Cursor auf aktuellen Winkel stellen
	int i = 0;
	for (; winkel[i] != -1; i++)
		if (winkel[i] == curwinkel)
			break;
	
	if (winkel[i] == -1)
		i = 0;

	i -= delta;

	if (i < 0)
		i = 3;
	else if (i == 4)
		i = 0; 

	SetDlgItemInt(IDC_DREHWINKEL, winkel[i]);
	return winkel[i];
}

void ElementeEingabe::on_elementeingabe_invertieren()
{
	/**
	Umdrehen der Elementreihenfolge
	*/
	WaxPart* rohr = dynamic_cast<WaxPart*>(get_data());
	if (!rohr) 
		return;
	
	Rohrleitung* r = dynamic_cast<Rohrleitung*>(rohr->get_werkauftrag());
	if (!r)
		return;
	
	r->elementeingabe_invertieren();

	show_elements(0);
	doUpdate();
}

void ElementeEingabe::on_anlage()
{
	/**
	Anlagenflag ändern, Muffe und Stutzen müssen in Standardmuffe und Standardstutzen geändert werden,
	und zurück
	*/
	if (!av)
		return;

	CButton *anl = (CButton*)GetDlgItem(IDC_ELEMENTE_ANLAGE);

	int newid = 0;

	/* Einträge immer setzen, auch wenn id == newid, da sonst nur einmal eine Umschaltung erfolgt 
	(Änderung muss ja erst mal übernommen werden, damit sich id ändert*/
	if (id == NUTST || id == STDSTUTZEN) 
		newid = anl->GetCheck() ? STDSTUTZEN : NUTST;
	else if (id == MUFFE || id == STDMUFFE)
		newid = anl->GetCheck() ? STDMUFFE : MUFFE;

	if (newid)
		ddx_select_cbitem(this, IDC_ELEMENTE_ID, newid);
}


void ElementeEingabe::enableDisableFields()
{
	/**
	Automatisches Setzen von Feldern je nach Elementid. Hier wird bei Standardmuffen/Stutzen 
	automatisch das Anlagenflag gesetzt (nur Av Version), bzw. bei Normalmuffen/Stutzen das 
	Anlagenfeld entfernt
	*/
	ElementeEingabeForm::enableDisableFields();

	if (!av)
		return;

	if (id == NUTST || id == MUFFE)
	{
		((CButton*)GetDlgItem(IDC_ELEMENTE_ANLAGE))->SetCheck(false);
	}
	else if(id == STDSTUTZEN || id == STDMUFFE)
	{
		((CButton*)GetDlgItem(IDC_ELEMENTE_ANLAGE))->SetCheck(true);
	}
}