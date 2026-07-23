#include "stdafx.h"


#include "resource.h"
#include <math.h>
#include <afxadv.h> // fuer CSharedFile

#include <mxutil/ddx_extension.h>
#include <mxcontrols/msgbox.h>
using namespace MxGui;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_UPDATE_FIELDS WM_USER+1234

IMPLEMENT_DYNCREATE(AVElementEingabe, WaxPartForm)


/*
Anderungen:
22.06.06, Jacobs: Bei IDC_ELEMENTE_ID Ereignishandler von ON_EN_KILLFOCUS in ON_CBN_SELCHANGE geändert,
                  damit gleich nach der Auswahl die anderen Felder richtig gesetzt werden
*/

BEGIN_MESSAGE_MAP(AVElementEingabe, ElementeEingabeForm)
	ON_WM_SIZE()
	ON_COMMAND(IDS_ELEM_NEW,				onElemNew)
	ON_COMMAND(IDC_ELEM_NEW,				onElemNew)
	ON_COMMAND(IDS_ELEM_CHANGE,				onElemChange)
	ON_COMMAND(IDS_ELEM_DELETE,				onElemDelete)
	ON_COMMAND(IDC_ELEM_CHANGE,				onElemChange)
	ON_COMMAND(IDC_ELEM_DELETE,				onElemDelete)
	ON_NOTIFY(NM_CLICK,	   IDC_ELEMDATEN,	onClickElemdaten)
	ON_NOTIFY(NM_RCLICK,   IDC_ELEMDATEN,	onRclickElemdaten)
	ON_NOTIFY(LVN_KEYDOWN, IDC_ELEMDATEN,	onKeydownElemdaten)
	ON_EN_KILLFOCUS(IDC_ELEMENTE_LAENGE,	doUpdate)
	ON_EN_KILLFOCUS(IDC_ELEMENTE_ABSTREL,	onKillfocusElement_abstrel)
	ON_CBN_KILLFOCUS(IDC_ELEMENTE_WINKEL,	onKillfocusElement_eledn)
	ON_BN_CLICKED(IDC_ELEMENTE_NACHARBEIT,  on_clicked_nacharbeit)
	ON_CBN_SELCHANGE(IDC_ELEMENTE_ELEDN,	onKillfocusElement_eledn)
	ON_CBN_KILLFOCUS(IDC_ELEMENTE_ELEDN,	onKillfocusElement_eledn)
	ON_CBN_SELCHANGE(IDC_ELEMENTE_ID,		onSelChangeElement_id)
	ON_COMMAND(WM_UPDATE_FIELDS, elem_setFields)
END_MESSAGE_MAP()

AVElementEingabe::AVElementEingabe(): ElementeEingabeForm(IDD_AVELEMENT, getStammdaten().getText("IDD_AVELEMENT").c_str())
{
}

AVElementEingabe::~AVElementEingabe()
{
}


/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
BOOL AVElementEingabe::OnInitDialog()
{
	if (!ElementeEingabeForm::OnInitDialog())
		return false;

	m_pElementArray = &get_rohrleitung()->get_avelementArray();

	CString str, rohbez;

	BauelementGruppen bg;
	
	str = getStammdaten().getText("IDS_LEER").c_str();
	rohbez = bg.getBezeichnung(ROHR)+ " + ";

	cbinitdata3 cbdat[] = 
	{
		str,									Element::LEER,						IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(NUT),		translateElementIdToComboBox(NUT),	IDC_ELEMENTE_ID,
		bg.getBezeichnung(NUTST),				NUTST,								IDC_ELEMENTE_ID,
		bg.getBezeichnung(MUFFE),				MUFFE,								IDC_ELEMENTE_ID,
		bg.getBezeichnung(SONDERMUFFE),			SONDERMUFFE,						IDC_ELEMENTE_ID,
		bg.getBezeichnung(GWRST),				GWRST,								IDC_ELEMENTE_ID,
		bg.getBezeichnung(ELEMENT_BOGEN),		ELEMENT_BOGEN,						IDC_ELEMENTE_ID,
		bg.getBezeichnung(BOG2S),				BOG2S,								IDC_ELEMENTE_ID,
		bg.getBezeichnung(BOHR24),				BOHR24,								IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(FFL),		translateElementIdToComboBox(FFL),	IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(FFL16),		translateElementIdToComboBox(FFL16),IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(FFL10),		translateElementIdToComboBox(FFL10),IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(SFL),		translateElementIdToComboBox(SFL),	IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(VFL10),		translateElementIdToComboBox(VFL10),IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(VFL10NEU),	translateElementIdToComboBox(VFL10NEU),IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(VFL16NEU),	translateElementIdToComboBox(VFL16NEU),IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(VFL16),		translateElementIdToComboBox(VFL16),IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(AFL),		translateElementIdToComboBox(AFL),	IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(GEW),		translateElementIdToComboBox(GEW),	IDC_ELEMENTE_ID,
		rohbez + bg.getBezeichnung(GLATT),		translateElementIdToComboBox(GLATT),IDC_ELEMENTE_ID,
		bg.getBezeichnung(KNICK),				KNICK,								IDC_ELEMENTE_ID,
	
		"", 0, 0
	};
	INIT_CBDATA3(cbdat);

	/*
	if (av)
	{ 
		cbinitdata3 cbdat[] = 
		{
			bg.getBezeichnung(KNICK), KNICK, IDC_ELEMENTE_ID,
			"", 0, 0
		};
		INIT_CBDATA3(cbdat);		
	}
	*/

	//Combobox mit DN Werten einstellen
	Stationsverteiler *stVert = dynamic_cast<Stationsverteiler*>(get_data()->get_werkauftrag());

	if (stVert)
		dnComboBox.setRange(10,200,true);
	else
		dnComboBox.setRange(10,65,true);

	// PR 040908 Versuchsweise auskommentiert, Verhalten nicht nachvollziehbar
#if 0
	bool firstav = false;
	Rohrleitung* testrohr; 
	SammelAuftrag* sa = dynamic_cast<SammelAuftrag*>(const_cast<WaxPart*>(get_data()->get_parent()));
	if (sa)
	{
		testrohr = dynamic_cast<Rohrleitung*>(sa->getFirstSubobject()->get_werkauftrag());
		if (testrohr)
			firstav = testrohr->get_avnachbearbeitung();
	}

	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());

	bool enable = ((!dynamic_cast<SammelAuftrag*>(const_cast<WaxPart*>(get_data()->get_parent())) 
					|| get_data()->isFirstSubOfSammelauftrag()
					|| firstav)
					&& (rohr->get_status().get_wanr() == 0 || av));
	
	enable_or_disable_dlgitems(enable);	

	enable = (eleArray()->GetSize() == 0 && enable);

	GetDlgItem(IDC_ELEMENTE_NACHARBEIT)->EnableWindow(enable);
	
	GetDlgItem(IDC_ELEMENTE_ABSTABS)->EnableWindow(FALSE);

	if (get_data()->get_werkauftrag()->get_status().get_wanr() != 0 && !WaxSys::isAV())
		GetDlgItem(IDC_ELEMENTE_ANSDN)->EnableWindow(FALSE);
#else
	enable_or_disable_dlgitems(get_rohrleitung()->get_avnachbearbeitung_per_zeichnung() ? false : true);	
#endif
	
	if (list.GetItemCount() > 0)
		elem_setFields();

	
	setWindowsText(IDC_STATIC_TYP, "IDS_TYP");
	setWindowsText(IDC_STATIC_AN_DN, "IDS_AN_DN");
	setWindowsText(IDC_STATIC_DN, "IDS_DN");
	setWindowsText(IDC_LAENGE_DURCHMESSER, "IDS_LAENGE");
	setWindowsText(IDC_STATIC_WINKEL, "IDS_WINKEL");
	setWindowsText(IDC_STATIC_ABS_ABST, "IDS_ABS_ABST");
	setWindowsText(IDC_STATIC_REL_ABST, "IDS_REL_ABST");
	setWindowsText(IDC_STATIC_ZEICHNUNG, "IDS_ZEICHNUNG");
	
	setWindowsText(IDC_ELEM_NEW, "IDS_ELEM_NEW");
	setWindowsText(IDC_ELEM_CHANGE, "IDS_ELEM_CHANGE");
	setWindowsText(IDC_ELEM_DELETE, "IDS_ELEM_DELETE");
	setWindowsText(IDC_ELEMENTE_NACHARBEIT, "IDS_NACHARBEIT");

	

	return true; 
}

void AVElementEingabe::OnSize(UINT nType, int cx, int cy)
{	

	m_iIDCNew = IDC_ELEM_NEW;
	ElementeEingabeForm::OnSize( nType,  cx,  cy);
}

void AVElementEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben |
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/
	ElementeEingabeForm::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ELEMENTE_ABSTREL,	abstRel);

	DDX_Text(pDX,	  IDC_ELEMENTE_ABSTABS,	abstAbs);
	DDX_Text(pDX,	  IDC_ELEMENTE_ANSDN,	anschlussDN);
	DDX_Text(pDX,	  IDC_ELEMENTE_ELEDN,	dn);
	DDX_Text(pDX,	  IDC_ELEMENTE_LAENGE,	laenge);
	DDX_Text(pDX,	  IDC_ELEMENTE_WINKEL,	winkel);
	DDX_Control(pDX,  IDC_ELEMDATEN,		list);
	DDX_Control(pDX,  IDC_ELEMENTE_DURCHMESSER,	ed_durchmesser);
	DDX_Control(pDX,  IDC_ELEMENTE_ELEDN,	dnComboBox);
	DDX_Control(pDX, IDC_ELEMENTE_NACHARBEIT, zeichnung);
	DDX_Text(pDX,	  IDC_ELEMENTE_ELEDN,	dn);

	//ddx_cbdata(pDX, IDC_ELEMENTE_ELEDN, dn);

	ed_durchmesser.doDataExchange(pDX);

	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;
	
	if (pDX->m_bSaveAndValidate) 
	{
		rohr->set_avnachbearbeitung_per_zeichnung(zeichnung.GetCheck() ? true : false);
	}
	else 
	{
		try
		{
			// Eingabefelder mit ersten Datensatz füllen
			if (rohr->get_avelementArray().GetSize() > 0) 
			{
				Element *ele = rohr->get_avelementArray().get_element_at(0);
				setAttributes(ele);
				ddx_cbdata(pDX, IDC_ELEMENTE_ID, id);
				
				zeichnung.SetCheck(false);
			}
			else if (rohr->get_avnachbearbeitung())
			{
				zeichnung.SetCheck(true);
			}
			show_elements(-1);
		}
		catch(Element::Error e)
		{
			MsgBox::show_error(e.get_errorText());
		}
	}
	ddx_cbdata(pDX, IDC_ELEMENTE_ID, id);

	if(DisableAllControls())
	{
		CWnd* wnd = GetDlgItem(IDC_ELEMDATEN);
		if(wnd != NULL)
		{
			wnd->EnableWindow(FALSE);
		}
	}
	

}

//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool AVElementEingabe::check_plausis()
{
	if (!ElementeEingabeForm::check_plausis())
		return false;

	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return false;
	
	// Rohr und Elemente checken
	CString err = "";
	err = rohr->checkPlausis();
	if (err.GetLength())
	{
		AfxMessageBox(err, MB_OK | MB_ICONERROR);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		return false;
	}

	get_data()->get_werkauftrag()->set_plausiOk(true);
	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return (true);
}

void AVElementEingabe::show_elements(int select)
{
	ElementeEingabeForm::show_elements(select);

	bool firstav = false;
	Rohrleitung* testrohr; 
	SammelAuftrag* sa = dynamic_cast<SammelAuftrag*>(const_cast<WaxPart*>(get_data()->get_parent()));
	if (sa)
	{
		testrohr = dynamic_cast<Rohrleitung*>(sa->getFirstSubobject()->get_werkauftrag());
		if (testrohr)
			firstav = testrohr->get_avnachbearbeitung();
	}
	
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(rohr);
	if (!rohr)
		return;

	bool enable = ((!dynamic_cast<SammelAuftrag*>(const_cast<WaxPart*>(get_data()->get_parent())) 
					|| get_data()->isFirstSubOfSammelauftrag()
					|| firstav)
					&& (rohr->get_status().get_wanr() == 0 || WaxSys::isAV())
					&& rohr->get_avelementArray().GetSize() == 0);
	GetDlgItem(IDC_ELEMENTE_NACHARBEIT)->EnableWindow(enable);
}

void AVElementEingabe::elem_setFields()
{
	ElementeEingabeForm::elem_setFields();

	///Enablen/Disablen von Eingabefeldern je nach ElementID
	enableDisableFields();

	UpdateData();	
}

//Indicates the user clicked a button
void AVElementEingabe::on_clicked_nacharbeit()
{
	enable_or_disable_dlgitems(zeichnung.GetCheck() ? false : true);
}

void AVElementEingabe::enable_or_disable_dlgitems(bool enable)
{
	/**
	Wird Nacharbeit (also per Zeichnung) aktiviert, müssen alle anderen Dialogitems abgeschaltet
	werden. Wird Nacharbeit deaktiviert müssen alle wieder aktiviert werden.
	*/
	static const UINT dlgitem[] = 
	{ 
		IDC_ELEMENTE_LAENGE, 
		IDC_ELEMENTE_ABSTREL, 
		IDC_ELEMENTE_ABSTABS,
		IDC_ELEMENTE_ELEDN,
		IDC_ELEMENTE_WINKEL, 
		IDC_ELEMENTE_ID, 
		IDC_ELEM_NEW,	
		IDC_ELEM_DELETE, 
		IDC_ELEM_CHANGE, 
		IDC_ELEMDATEN,
		IDC_ELEMENTE_ANSDN,
		0 
	};
	for (int i = 0; dlgitem[i]; i++)
	{
		GetDlgItem(dlgitem[i])->EnableWindow(enable);
	}

	GetDlgItem(IDC_ELEMENTE_ABSTABS)->EnableWindow(false);
}

