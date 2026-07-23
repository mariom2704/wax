#include "stdafx.h"


#include "resource.h"

#include <math.h>
#include <afxadv.h> // fuer CSharedFile
#include "GewinderohrEingabe.h"
#include "..\waxstamm\formstueckeundfittinge.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_UPDATE_FIELDS WM_USER+1234
#define WM_ZUSAETZLICHER_FITTING (WM_UPDATE_FIELDS+1)


IMPLEMENT_DYNCREATE(GewinderohrEingabe, WaxPartForm)

BEGIN_MESSAGE_MAP(GewinderohrEingabe, WaxPartForm)
	ON_WM_SIZE()

	ON_COMMAND(IDS_ELEM_NEW,	OnNew)
	ON_COMMAND(IDS_ELEM_CHANGE,	OnChange)
	ON_COMMAND(IDS_ELEM_COPY,	OnCopy)
	ON_COMMAND(IDS_ELEM_DELETE,	OnDelete)
	ON_COMMAND(IDS_ELEM_CUT,	OnCut)
	ON_COMMAND(IDS_ELEM_PASTE,	OnPaste)
	
	ON_NOTIFY(NM_RCLICK,	IDC_GROHR_LIST, OnRclickGrohr_list)
	ON_NOTIFY(NM_CLICK,		IDC_GROHR_LIST, OnClickGrohr_list)
	ON_NOTIFY(LVN_KEYDOWN,	IDC_GROHR_LIST, OnKeydownGrohr_list)
	
	ON_NOTIFY(NM_RETURN, IDC_GROHR_TREE, onGrohr_tree)
	ON_NOTIFY(NM_DBLCLK, IDC_GROHR_TREE, onGrohr_tree)

	ON_BN_CLICKED(IDC_GROHR_ZM1, onClickedGrohr_zm)
	ON_BN_CLICKED(IDC_GROHR_ZM2, onClickedGrohr_zm)
	ON_BN_CLICKED(IDC_GROHR_ZM3, onClickedGrohr_zm)

	ON_CBN_SELCHANGE(IDC_GROHR_DN2,			onSelchangeGrohr_dn2)
	ON_CBN_SELCHANGE(IDC_GROHR_DN3,			onSelchangeGrohr_dn3)
	ON_EN_KILLFOCUS(IDC_GROHR_LAENGE,		OnKillfocusGrohr_laenge)
	ON_EN_KILLFOCUS(IDC_GROHR_ANZAHL_NEU,	doUpdate)
	ON_EN_KILLFOCUS(IDC_GROHR_POSNR_NEU,	doUpdate)
	ON_CBN_SELCHANGE(IDC_GROHR_LENDE,		doUpdate) 
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB,		onSelchangeTab)
	ON_CBN_SELCHANGE(IDC_GROHR_DN,			onSelchangeGrohr_dn)
	ON_CBN_KILLFOCUS(IDC_GROHR_DN,			onSelchangeGrohr_dn)
	ON_COMMAND(WM_ZUSAETZLICHER_FITTING,	onZusaetzlicherFitting)
	ON_EN_KILLFOCUS(IDC_GROHR_ZML_ENDE,		onChangeGrohr_zml_ende)
	ON_COMMAND(WM_UPDATE_FIELDS, elem_setFields)

END_MESSAGE_MAP()

#define ele_INIT_SAFE(index) \
Element *ele = data->get_elementArray().get_element_at(index);  \
if (!ele)	return;

#define ele_INIT_SAFE_RETURN_FALSE(index) \
Element *ele = data->get_elementArray().get_element_at(index));  \
if (!ele)	return false;


//Konstruktor
GewinderohrEingabe::GewinderohrEingabe() 
: WaxPartForm(IDD_GEWINDEROHR, getStammdaten().getText("IDD_GEWINDEROHR").c_str()), list(0,0,0), data(NULL)
{
	dn2zoll = (WaxSys::para_int(DN2ZOLL) == 0) ? false : true;
	m_bErzeugt		= FALSE;
}

//Destruktor
GewinderohrEingabe::~GewinderohrEingabe()
{

}

//Called after the size of CWnd has changed.
void GewinderohrEingabe::OnSize(UINT nType, int cx, int cy)
{
	WaxPartForm::OnSize(nType, cx, cy);
	
	if(m_bErzeugt)
	{
		CRect rectUpperLeft;
		GetDlgItem(IDC_STATIC_ZMASS)->GetWindowRect(&rectUpperLeft);
		ScreenToClient(&rectUpperLeft);
		
		CRect rectList;
		GetDlgItem(IDC_GROHR_LIST)->GetWindowRect(&rectList);
		ScreenToClient(&rectList);

		GetDlgItem(IDC_GROHR_LIST)->SetWindowPos(NULL, rectList.left, rectList.top, rectUpperLeft.right - 10, cy - rectList.top -10, SWP_SHOWWINDOW );

		//Move Tree 
		CRect rectCheckKopf;
		GetDlgItem(IDC_GROHR_KOPF)->GetWindowRect(&rectCheckKopf);
		ScreenToClient(&rectCheckKopf);
		
		GetDlgItem(IDC_TAB)->SetWindowPos(NULL,  rectUpperLeft.right + 10, rectCheckKopf.bottom + 5, cx - rectUpperLeft.right - 20,   cy - rectCheckKopf.bottom - 15, SWP_SHOWWINDOW );

		CRect rectTab;
		GetDlgItem(IDC_TAB)->GetWindowRect(&rectTab);
		ScreenToClient(&rectTab);

		GetDlgItem(IDC_GROHR_TREE)->SetWindowPos(NULL,  rectTab.left + 10, rectTab.top + 35, rectTab.Width()-20,   rectTab.Height()- 45, SWP_SHOWWINDOW );
		GetDlgItem(IDC_GROHR_TREE_ZUS)->SetWindowPos(NULL,  rectTab.left + 10, rectTab.top + 35, rectTab.Width()-20,   rectTab.Height()- 45, SWP_SHOWWINDOW );

	
		int index = tab.GetCurSel();
		if(index > -1)
		{
			selectTab(index, true);
		}
		else
		{
			selectTab(0, true);
		}
	}
}



void GewinderohrEingabe::createPopupMenu(CPoint point)
{
	CMenu menu; 
	CString txt;
	menu.CreatePopupMenu();

	menu_items items[] =
	{
		ADD_MENU_ITEM(IDS_ELEM_NEW)
		ADD_MENU_ITEM(IDS_ELEM_CHANGE)
		ADD_MENU_ITEM(IDS_ELEM_COPY)
		ADD_MENU_ITEM(IDS_ELEM_CUT)
		ADD_MENU_ITEM(IDS_ELEM_PASTE)
		ADD_MENU_ITEM(IDS_ELEM_DELETE)
		{ -1, "" },
	};

	WaxPartForm::createPopupMenu(point, items);
}

// Fuegt die gueltigen items ein 
void GewinderohrEingabe::insertItems()
{
	tree.SetRedraw(false);
	tree_zus.SetRedraw(false);
	tree.DeleteAllItems();
	tree_zus.DeleteAllItems();
	insertElemente();
	insertFittinge();
	tree.SetRedraw();
	tree_zus.SetRedraw();
}

// Fuegt die Elementliste in den Materialbaum ein
void GewinderohrEingabe::insertElemente()
{
	// 1) Elemente: Glatt, Gewinde, Nut 
	// 2) Nur gueltige Nennweiten anzeigen 
	TV_INSERTSTRUCT TreeCtrlItem;
	char titel[255];

	HTREEITEM parent = TVI_ROOT;
	int elemid = -1;
	int dn = getReferenceDN();
	for (POSITION pos = getStammdaten().get_bauelemente().GetHeadPosition(); pos;)
	{
		Bauelemente::data *f = (Bauelemente::data *)getStammdaten().get_bauelemente().GetNext(pos);

		if (f->get_elemid() == GLATT || f->get_elemid() == GEW || f->get_elemid() == NUT)
		{
	
			if (f->get_elemid() != elemid)
			{
				elemid = f->get_elemid(); 
				BauelementGruppen bg;
				strcpy(titel, bg.getBezeichnung(f->get_elemid()).Left(254));
			}

			if (f->get_dn() == dn)
			{
				CString kurztext;
				if (dn2zoll)
					kurztext.Format("%s (%s\")", titel, WaxStamm::dn2zoll(f->get_dn()));
				else
					kurztext.Format("%s (DN%i)", titel, f->get_dn());
				CString bez;
				char tit[255];
				strcpy (tit, kurztext.Left(254));
				TreeCtrlItem = makeInsertStruct(parent, 0, tit, FALSE, f);
				tree.InsertItem(&TreeCtrlItem);
			}
		}
	}
	
}

// Fuegt die F&F-Liste in den Materialbaum ein
void GewinderohrEingabe::insertFittinge(bool ensureVisible)
{
	// 1) Reihenfolge im Baum: T-Stueck, Winkel, Kreuzstueck, Muffe, Red-Muffe, 
	//                         Red-Nippel, Doni, Kappe, Stopfen, Verschraubung, Winkelverschraubung 
	// 2) Nur gueltige Nennweiten anzeigen 
	//
	// 3) Nur gueltige Gewindeanschluesse anzeigen
	//
	int gruppenID[] = 
	{ 
		FormstueckeUndFittinge::T_STUECK,
		FormstueckeUndFittinge::WINKEL,
		FormstueckeUndFittinge::WINKEL_A1_45,
		FormstueckeUndFittinge::WINKEL_A4,
		FormstueckeUndFittinge::WINKEL_30,
		FormstueckeUndFittinge::KREUZ_STUECK, 
		FormstueckeUndFittinge::F_MUFFE, 
		FormstueckeUndFittinge::RED_MUFFE,    
		FormstueckeUndFittinge::RED_NIPPEL,
		FormstueckeUndFittinge::DOPPELNIPPEL, 
		FormstueckeUndFittinge::KAPPE,
		FormstueckeUndFittinge::STOPFEN,
		FormstueckeUndFittinge::VERSCHRAUBUNG, 
		FormstueckeUndFittinge::WINKELVERSCHRAUBUNG, 
		0
	};
	int grplast = 0;

	ensureVisible = false;
	TV_INSERTSTRUCT TreeCtrlItem;
	HTREEITEM parent = TVI_ROOT;
	HTREEITEM rootitem = TVI_ROOT;

	bool neunzigbar = (data->get_typ() == Gewinderohr::GAS) ? true : false; 
	
	CTreeCtrl *t[] = {&tree, &tree_zus, NULL};

	int dn = getReferenceDN();
	bool newele = false;
	for (int i = 0; t[i]; i++)
	{
		// Das Selektiertes Element wird für das Einfügen der passenden Fittinge gebraucht. 
		// Bei Null werden alle Fittinge eingefügt
		Element* ele = NULL;
		if (i)
			ele = getSelectedElement();
		else
			ele = getSelectedElement(false);

		for (int index = 0; gruppenID[index]; index++)
		{
			grplast = gruppenID[index];

			for (POSITION pos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); pos;)
			{
				FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(pos);

				// Elementid (Fitting) und Gruppenid müssen übereinstimmen
				if (f->get_elemid() != GRFITTING || f->get_gruppenid() != gruppenID[index])
					continue;
			
				// Bezeichnung fuer Gruppe einfuegen (nur einmal)
				if (f->get_gruppenid() == grplast) 
				{
					TreeCtrlItem = makeInsertStruct(rootitem, 0, f->get_gruppe(), TRUE, NULL);
					parent = t[i]->InsertItem(&TreeCtrlItem);
					grplast = 99; 
				}
				
				// Nur gueltige Nennweiten einfuegen, Gewinde muss passen
				if (isEleDNok(f, dn))
				{
					if (!ele)
					{
						ele = new Element;
						newele = true;
					}
					int id = ele->get_gruppenid();
					ele->set_gruppenid(f->get_gruppenid());
					
					const int rel_abstand = GetDlgItemInt(IDC_GROHR_LAENGE);
					if (f->get_gruppenid() == ele->get_gruppenid() || passendesGewinde(rel_abstand, ele, f->get_gruppenid())) 
					{
						// F&F der Gruppe einfuegen			
						if (f->get_neunzigbar() && neunzigbar || f->get_wasser_sprinkler() && !neunzigbar)
						{
							CString bez;

							TreeCtrlItem = makeInsertStruct(parent, 0, f->get_kurztext(), FALSE, f);
							t[i]->InsertItem(&TreeCtrlItem);
						}
					}
					ele->set_gruppenid(id);

				}
			}
			// Leere Untergruppen wieder aus dem Baum löschen 
			if (!t[i]->GetChildItem(parent))
				t[i]->DeleteItem(parent);
		}
		if (newele)
		{
			newele = false;
			delete ele;
		}
	}
}

BOOL GewinderohrEingabe::OnInitDialog()
{
	GewinderohrData *rohr = dynamic_cast<GewinderohrData*>(get_data());
	XASSERT(rohr);
	if (!rohr)
		return false;

	data = dynamic_cast<Gewinderohr*>(rohr->get_werkauftrag()); // Liefert Referenz, muss immer ein Pointer sein

	/*
	Dialoginitialisierung 
	Hier werden den Comboboxtexten ein Integerwert zugewiesen, 
	der einer ElementID entspricht. 
	*/
	if (!WaxPartForm::OnInitDialog())
		return false;

	CString str; str = getStammdaten().getText("IDS_FITTING_RECHTS").c_str();
	tab.InsertItem(0, str);
	str = getStammdaten().getText("IDS_ZUSAETZLICH_FITTING").c_str();
	tab.InsertItem(1, str);


	// Combobox Rohranfang
	cbinitdata3 initRohranfang[] = 
	{
		getStammdaten().getText("IDS_GLATT").c_str(),	GLATT,	IDC_GROHR_LENDE,
		getStammdaten().getText("IDS_NUT").c_str(),		NUT,	IDC_GROHR_LENDE,
		getStammdaten().getText("IDS_GEWIND").c_str(),	GEW,	IDC_GROHR_LENDE,
		"",			0,		0
	};
	INIT_CBDATA3(initRohranfang);

	// Nennweite
	CStringArray cbinitstr;
	if (dn2zoll)
	{
		cbinitstr.Add(WaxStamm::dn2zoll(15));
		cbinitstr.Add(WaxStamm::dn2zoll(20));
		cbinitstr.Add(WaxStamm::dn2zoll(25));
		cbinitstr.Add(WaxStamm::dn2zoll(32));
		cbinitstr.Add(WaxStamm::dn2zoll(40));
		cbinitstr.Add(WaxStamm::dn2zoll(50));
	}
	else
	{
		cbinitstr.Add("15");
		cbinitstr.Add("20");
		cbinitstr.Add("25");
		cbinitstr.Add("32");
		cbinitstr.Add("40");
		cbinitstr.Add("50");
	}
	cbinitdata2 initNennweite[] = 
	{
		15, IDC_GROHR_DN,
		20, IDC_GROHR_DN,
		25, IDC_GROHR_DN,
		32, IDC_GROHR_DN,
		40, IDC_GROHR_DN,
		50, IDC_GROHR_DN,
		0,  0
	};
	INIT_CBDATA2(initNennweite, cbinitstr);
	
	// PR: Anordnung auf Wunsch von Sommer geändert
	CString bez;
	bez = getStammdaten().getText("IDS_ANZAHL").c_str();				list.InsertColumn(ANZAHL,		bez,	LVCFMT_LEFT,  40);
	bez = getStammdaten().getText("IDS_NENNWEITE").c_str();				list.InsertColumn(NENNWEITE,	bez,	LVCFMT_LEFT,  70);
	bez = getStammdaten().getText("IDS_ROHRLAENGE").c_str();			list.InsertColumn(LAENGE,		bez,	LVCFMT_LEFT,  50);
	bez = getStammdaten().getText("IDS_TEXT").c_str();					list.InsertColumn(TEXT,			bez,	LVCFMT_LEFT, 270); 
	bez = getStammdaten().getText("IDS_WINKEL").c_str();				list.InsertColumn(WINKEL_,		bez,	LVCFMT_LEFT,  50);
	bez = getStammdaten().getText("IDS_ZUSAETZLICH_FITTING2").c_str();	list.InsertColumn(VERKETTUNG,	bez,	LVCFMT_LEFT,  60);

	SetDlgItemInt(IDC_GROHR_LAENGE, 0);
	SetDlgItemInt(IDC_GROHR_WINKEL, 0);


	fillElementList(-1, false);
	// Update
	UpdateData(FALSE);

	// ZMass links
	SetDlgItemInt(IDC_GROHR_ZML_ENDE, data->get_zmassLinks());

	// Radiobuttons für ZMass Eingabe/Berechnung initialisieren
	CComboBox *initzm = NULL;
	CButton *initbutzm = NULL;
	switch (data->get_zmassTyp())
	{
		case Element::EINGABE:
			initbutzm = (CButton*)GetDlgItem(IDC_GROHR_ZM1);
			break;
		case Element::FITTING:
			initbutzm = (CButton*)GetDlgItem(IDC_GROHR_ZM2);
			initzm = (CComboBox*)GetDlgItem(IDC_GROHR_DN2);
			break;
		case Element::MUFFE_AUF_VERTEILER:
			initbutzm = (CButton*)GetDlgItem(IDC_GROHR_ZM3);
			initzm = (CComboBox*)GetDlgItem(IDC_GROHR_DN3);
			break;
	}
	XASSERT(initbutzm);
	if (initbutzm)
		initbutzm->SetCheck(1);

	if (initzm)
	{
		CString sel;
		sel.Format("%i", data->get_zmassRefDN());
		initzm->SelectString(-1, sel);
	}
	
	onClickedGrohr_zm();
	dnRohr = data->get_dnvl();

	insertItems();

	setWindowsText(IDC_STATIC_ROHR_FITTING, "IDC_STATIC_ROHR_FITTING");
	setWindowsText(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsText(IDC_STATIC_ANZAHL, "IDS_ANZAHL");
	setWindowsText(IDC_STATIC_NW, "IDS_NENNWEITE");
	setWindowsText(IDC_STATIC_LAENGE, "IDS_LAENGE");
	setWindowsText(IDC_STATIC_WINKEL, "IDS_WINKEL");
	setWindowsText(IDC_STATIC_ANFANG, "IDS_ANFANG");
	setWindowsText(IDC_STATIC_ZM_LINKS, "IDC_STATIC_ZM_LINKS");
	setWindowsText(IDC_STATIC_ZM_RECHTS, "IDC_STATIC_ZM_RECHTS");
	setWindowsText(IDC_STATIC_ZMASS, "IDC_STATIC_ZMASS");
	setWindowsText(IDC_GROHR_ZM1, "IDS_EINGABE");
	setWindowsText(IDC_GROHR_ZM2, "IDS_FITTING");
	setWindowsText(IDC_GROHR_ZM3, "IDS_MUFFE_VERT");
	setWindowsText(IDC_GROHR_KOPF, "IDC_GROHR_KOPF");


	setWindowsText(IDS_ELEM_NEW, "IDS_ELEM_NEW");
	setWindowsText(IDS_ELEM_CHANGE, "IDS_ELEM_CHANGE");
	setWindowsText(IDS_ELEM_COPY, "IDS_COPY");
	setWindowsText(IDS_ELEM_CUT, "IDS_ELEM_CUT");
	setWindowsText(IDS_ELEM_PASTE, "IDS_PASTE");
	setWindowsText(IDS_ELEM_DELETE, "IDS_ELEM_DELETE");

	
	return true;
}


void GewinderohrEingabe::DoDataExchange(CDataExchange* pDX)
{
	/*
	Dialogdatenaustausch 
	Hier werden die Dialogdaten an die Klasse Standardverteiler übergeben
	Erzeugung der Rohrkonstruktion und der Graphik  
	*/

	WaxPartForm::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB, tab);
	DDX_Control(pDX, IDC_GROHR_KOPF, kopfstueck);
	DDX_Control(pDX,	IDC_GROHR_TREE,		tree);
	DDX_Control(pDX,	IDC_GROHR_LIST,		list);
	DDX_Control(pDX,	IDC_GROHR_TREE_ZUS,	tree_zus);

	int rohrAnfang = data->get_rohrAnfang();
	ddx_cbdata(pDX, IDC_GROHR_LENDE, rohrAnfang);

	ddx_anzahl(pDX, IDC_GROHR_ANZAHL_NEU);
	ddx_posnr(pDX,  IDC_GROHR_POSNR_NEU);
	
	if (pDX->m_bSaveAndValidate) 
	{
		data->set_rohrAnfang(rohrAnfang);
	} 
	
	ddx_cbdata(pDX, IDC_GROHR_DN, dnRohr);
	m_bErzeugt = TRUE;
}

//Plausibilitätscheck, wird aufgerufen, bevor 
//UpdateData(TRUE) aufgerufen 
//wird.
bool GewinderohrEingabe::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 

	// Rohr und Elemente checken
	CString err = doPlausiCheck();
	if (err.GetLength())
	{
		AfxMessageBox(err, MB_ICONINFORMATION | MB_OK);
		disableCheck();
		return false;
	}
	get_data()->get_werkauftrag()->set_plausiOk(true);
	return true;
}

const CString GewinderohrEingabe::doPlausiCheck()
{
	/**
	Führt die Prüfung durch und setzt den Statustext. 
	*/
	CString err = data->checkPlausis();
	if (err.GetLength() > 0)
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
	else
	{
		CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	}
	return err;
}

void GewinderohrEingabe::elem_setFields()
{
	// Datensatz makiert?
	int index = list.getSelectedItem();
	if (index == -1) 
		return;

	updateTabOrder(index ? true : false);
	ele_INIT_SAFE(index);

	// Buttons setzen
	kopfstueck.SetCheck(ele->get_kopfstueck());

	// Werte in Felder setzen
	SetDlgItemInt(IDC_GROHR_LAENGE,	ele->get_abstRel());
	SetDlgItemInt(IDC_GROHR_WINKEL, ele->get_winkel());
	
	int dn = ele->get_dn();
	CDataExchange cb(this, false);
	ddx_cbdata(&cb, IDC_GROHR_DN, dn);

	setZMassLinksRechts(ele);
	// Damit bei Aenderung der dn die passenden Fitt. eingefuegt werden  
	// F&F-Baum neu fuellen
	insertItems(); 
	selectFitting(ele);
}
	

bool GewinderohrEingabe::doInsert(Element* ele, int index)
{
	EnContainerRohrElemente& elementArray = data->get_elementArray();

	if (index == -1)
		index = 0;
	elementArray.InsertAt(index, ele);
	doPlausiCheck();
	setZMassLinksRechts(ele);
	setAnlage(ele, index);
	return true;
}

void GewinderohrEingabe::fillElementList(int select, bool redraw_graphic)
{
	// Selektiertes Element suchen und merken
	int curselect;
	if (select == -1)
	{
		curselect = list.getSelectedItem();
		if (curselect == -1)
			curselect = 0;
	}
	else
		curselect = select;
	list.SetRedraw(FALSE); 
	// Löschen aller Strings
	list.DeleteAllItems();

	EnContainerRohrElemente& elementArray =  data->get_elementArray();
	// Neu füllen mit Elementdaten
	for (int i=0; i<elementArray.GetSize(); i++) 
	{
		CString txt;

		ele_INIT_SAFE(i);

		int index = list.GetItemCount();
		index = index > 0 ? index : 0;

		txt.Format("%i", data->get_anzahl());
		index = list.InsertItem(index, txt);

		txt.Format("%i", ele->get_abstRel());
		list.SetItemText(index, LAENGE, txt);

		txt = WaxStamm::dn2str(dn2zoll, ele->get_anschlussDN());
		list.SetItemText(index, NENNWEITE,	txt);
		
		if (ele->isEleRohrQ1())
		{
			Element e;
			e.set_id(data->get_rohrEnde());
			//txt.Format("%s %s", ele->get_kurztext(), e.get_bezeichnung());
			txt.Format("%s", e.get_bezeichnung());
		}
		else 
			txt.Format("%s", ele->get_kurztext());
		list.SetItemText(index, TEXT, txt);
		
		txt.Format("%i", ele->get_winkel());
		list.SetItemText(index, WINKEL_, txt);

		txt.Format("%s", (ele->get_verkettung()>0)?"x":"-");
		list.SetItemText(index, VERKETTUNG, txt);
	}

	// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
	// sonst den letzten in der Liste
	curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
	list.selectItem(curselect);
	list.SetRedraw(TRUE);

	if (redraw_graphic)
		get_data()->updateGrafic(this);
	PostMessage(WM_COMMAND, WM_UPDATE_FIELDS);
}

///Setzt die Laenge ggbf. Null 
void GewinderohrEingabe::setLaengeNull()
{
	// Bei zus. Fitt. oben immer, sonst bei Doppel- und Red-Nippel   
	if (tree.IsWindowVisible())
	{
		FormstueckeUndFittinge::data *f = NULL;
		CTreeCtrl *t = tree.IsWindowVisible() ? &tree : &tree_zus;
		HTREEITEM item = t->GetSelectedItem();
		if (item)
		{
			f = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)tree.GetItemData(item));
			if (f)
			{
				if (f->get_gruppenid() != FormstueckeUndFittinge::RED_NIPPEL && f->get_gruppenid() != FormstueckeUndFittinge::DOPPELNIPPEL)
					return;
			}
			else
				return;
		}
		
	}
	SetDlgItemInt(IDC_GROHR_LAENGE, 0);
}

void GewinderohrEingabe::OnNew()
{
	/**
	Klick auf Button "Hinzufügen" 
	Fügt einen Eintrag in die ComboBox entsprechend der eingegebenen Daten an.  
	*/
	// Setzt die Laenge ggbf. Null 
	setLaengeNull();

	FormstueckeUndFittinge::data *f = NULL;
	Bauelemente::data *b = NULL;
	HTREEITEM item = NULL;
	CString kurztext;
	int artnr = 0;
	CTreeCtrl *t = tree.IsWindowVisible() ? &tree : &tree_zus;
	item = t->GetSelectedItem();
	if (item)
	{
		f = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)tree.GetItemData(item));
		if (!f)
			b = dynamic_cast<Bauelemente::data *>((CObject*)tree.GetItemData(item));
	}
	
	if (f)
	{
		artnr = f->get_artikelnr();
		CString str;
		if (kopfstueck.GetCheck())
			str = getStammdaten().getText("IDS_KOPF").c_str();
		kurztext.Format("%s%s", str, f->get_kurztext());
		data->set_rohrEnde(GLATT);
	}
	else if (b)
		data->set_rohrEnde(b->get_elemid());
	else
		return;

	int dn  = getReferenceDN();
	int winkel = GetDlgItemInt(IDC_GROHR_WINKEL);
	int laenge = GetDlgItemInt(IDC_GROHR_LAENGE);
	
	Element *ele = new Element;
	
	ele->set_abstAbs(laenge);
	ele->set_abstRel(laenge);
	ele->set_anschlussDN(dn);
	ele->set_dn(dn);
	ele->set_kurztext(kurztext);

	if (f)
	{
		ele->set_id(GRFITTING);
		ele->set_winkel(winkel);
		ele->set_verkettung(tab.GetCurSel());
		ele->set_artikelnr(artnr);
		
		ele->set_dn1(f->get_dn1());
		ele->set_dn2(f->get_dn2());
		ele->set_dn3(f->get_dn3());
		ele->set_dn4(f->get_dn4());
	
		ele->set_zmass(f->get_zmass1());
		ele->set_zmass2(f->get_zmass2());
		ele->set_zmass3(f->get_zmass3());
		ele->set_laenge(f->get_laenge());
		
		ele->set_gruppenid(f->get_gruppenid());
		ele->set_gfzeichen(f->get_gfzeichen());
		ele->set_nurFitting(laenge == 0);
		ele->set_kopfstueck(kopfstueck.GetCheck());

		ele->setAnschlussNennweite();
	}
	else 
		ele->set_id(ROHR_Q1);

	int select = list.getSelectedItem();
	if (select == -1)
		data->set_dnvl(dn);
	if (select != -1)
		select++;
	if (doInsert(ele, select))
	{
		// Element in Liste
		fillElementList(select, true);	
		UpdateData(FALSE);
	}
	else 
	{
		delete ele;
	}
}

void GewinderohrEingabe::OnChange()
{
/*
Klick auf Button "Ändern" 
Ändert den Eintrag in der ComboBox entsprechend der eingegebenen Daten.  
*/
	// Setzt die Laenge ggbf. Null 
	setLaengeNull();

	FormstueckeUndFittinge::data *f = NULL;
	Bauelemente::data *b = NULL;
	CString kurztext;
	int artnr = 0;

	CTreeCtrl *t = tree.IsWindowVisible() ? &tree : &tree_zus;
	HTREEITEM item = t->GetSelectedItem();
	if (item)
	{
		f = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)t->GetItemData(item));
		if (!f)
			b = dynamic_cast<Bauelemente::data *>((CObject*)t->GetItemData(item));
	}
	
	if (f)
	{
		artnr = f->get_artikelnr();
		CString str;
		if (kopfstueck.GetCheck())
			str = getStammdaten().getText("IDS_KOPF").c_str();
		kurztext.Format("%s%s", str, f->get_kurztext());
	}
	else if (b)
		data->set_rohrEnde(b->get_elemid());
	else
		return;
	
	// Datensatz makiert?
	int index = list.getSelectedItem();
	if (index == -1) 
		return;

	int dn = dnRohr;
	// Datensatz holen und merken
	ele_INIT_SAFE(index);
	Element eleMerk;
	eleMerk.copy(*ele);

	const int laenge = GetDlgItemInt(IDC_GROHR_LAENGE);
	// Datensatz aendern
	ele->set_abstAbs(laenge);
	ele->set_abstRel(laenge);
	ele->set_anschlussDN(dn);
	ele->set_dn(dn);
	ele->set_winkel(GetDlgItemInt(IDC_GROHR_WINKEL));
	ele->set_verkettung(tab.GetCurSel());
	ele->set_artikelnr(artnr);
	ele->set_kurztext(kurztext);

	if (f)
	{
		ele->set_id(GRFITTING);
		ele->set_dn1(f->get_dn1());
		ele->set_dn2(f->get_dn2());
		ele->set_dn3(f->get_dn3());
		ele->set_dn4(f->get_dn4());
		ele->set_zmass(f->get_zmass1());
		ele->set_zmass2(f->get_zmass2());
		ele->set_zmass3(f->get_zmass3());
		ele->set_laenge(f->get_laenge());
		ele->set_gruppenid(f->get_gruppenid());
		ele->set_gfzeichen(f->get_gfzeichen());
		ele->set_nurFitting(laenge==0);
		ele->set_kopfstueck(kopfstueck.GetCheck());
		ele->setAnschlussNennweite();
		ele->set_changeDN(false);
	}
	else
	{
		ele->set_id(ROHR_Q1);
		ele->set_laenge(0);
		ele->set_zmass(0);
	}
	
	if (index == 0)
		data->set_dnvl(dn);
	setZMassLinksRechts(ele);
	setAnlage(ele, index);		
	doPlausiCheck();
	fillElementList(-1, true);
}

void GewinderohrEingabe::OnCopy()
{
/*
Klick auf Button "Kopieren" 
Hier werden die Elementdaten entsprechend dem ausgewählten Index 
in die Zwischenablage kopiert.  
*/
	// Datensatz makiert?	
	int index = list.getSelectedItem();
	if (index == -1) 
		return;

	ele_INIT_SAFE(index);

	// in die Zwischenablage
	UINT format = ::RegisterClipboardFormat("GWPART");
	XASSERT(format);

	CSharedFile *f = new CSharedFile(GMEM_DDESHARE | GMEM_MOVEABLE);
	BinaryArchive ar( f, BinaryArchive::store);
	ar.set_version(1000); // letzte Version
	ele->serialize(ar);
	ar.close();	

	::OpenClipboard(this->m_hWnd);
	::EmptyClipboard();
	::SetClipboardData(format, f->Detach());
	::CloseClipboard();
	delete f;
}

void GewinderohrEingabe::OnDelete()
{
	/**
	Klick auf Button "Löschen" 
	Löscht den markierten Datansatz aus der ComboBox und 
	löscht die Daten aus dem Array je nach Index.  
	*/
	// Datensatz makiert?
	int index = list.getSelectedItem();
	if (index == -1) 
		return;

	ele_INIT_SAFE(index);
	data->get_elementArray().RemoveAt(index);  
	delete ele;

	fillElementList((index-1) < 0 ? 0 : index-1, true);
}

void GewinderohrEingabe::OnCut()
{
	OnCopy();
	OnDelete();
}

void GewinderohrEingabe::OnPaste()
{
	/**
	Klick auf Button "Einfügen" 
	Fuegt einen Eintrag in die ComboBox entsprechend der Datenfelder hinter 
	dem markierten (Einfachklick)Datensatzes an. Der zu kopierende Datensatz ist mit Doppelklick  
	auszuwaehlen.  
	*/
	// Element aus der Zwischenablage holen
	UINT format = ::RegisterClipboardFormat("GWPART");
	XASSERT(format);
	if (!format)
		return;
	if (!::IsClipboardFormatAvailable(format))
		return;

	::OpenClipboard(this->m_hWnd);

	HANDLE hdata = ::GetClipboardData(format);
	::GlobalLock(hdata);

	CSharedFile f(GMEM_DDESHARE | GMEM_MOVEABLE, 0);
	f.SetHandle( hdata, FALSE );
	BinaryArchive ar( &f, BinaryArchive::load);
	ar.set_version(1000); // letzte Version
	Element *ele = new Element;
	ele->serialize(ar);

	ar.close();
	::GlobalUnlock(f.Detach());
	::CloseClipboard();

	int select = list.getSelectedItem();

	if (doInsert(ele, select+1))
	{
		fillElementList(select+1, true);
	}
	else
	{
		delete ele;
	}
}

//Indicates that the user has clicked the right mouse button in the control
void GewinderohrEingabe::OnRclickGrohr_list(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Pointer auf NMHDR ist generisch: Er muss auf die richtige Struktur
	// gecastet werden. 
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Der Punkt im Clientwindow ist in ptAction gespeichert
	CPoint point( pNMListView->ptAction.x, pNMListView->ptAction.y);

	// Wenn nicht auf ein Item geklickt wurde, ist iItem gleich -1
	// Menü soll nur angezeigt werden, wenn ein Item angeklickt wurde
	if (pNMListView->iItem == -1) 
		return;

	// relative Koordinaten auf absolute umrechnen
	list.ClientToScreen(&point);
	createPopupMenu(point);
	*pResult = 1;
}

//Indicates that the user has clicked the left mouse button in the control
void GewinderohrEingabe::OnClickGrohr_list(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UNREFERENCED_PARAMETER(pResult);
	PostMessage(WM_COMMAND, WM_UPDATE_FIELDS);
}

//Indicates that a key has been pressed
void GewinderohrEingabe::OnKeydownGrohr_list(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	*pResult = 0;


	// Nur bei neu ausgewähltem Eintrag werden die Felder aktualisiert (sonst Flackern bei Tastaturdruck)
	UINT keys [] = { VK_UP, VK_DOWN, VK_HOME, VK_END, VK_PRIOR, VK_NEXT, 0 };
	int i;
	for (i = 0; keys[i]; i++)
	{
		if (pTVKeyDown->wVKey == keys[i])
		{
			PostMessage(WM_COMMAND, WM_UPDATE_FIELDS);
			break;
		}
	}

	// Hotkeyfunktionen
	struct hotkey
	{
		UINT msg;
		UINT key;
	} hotkeys [] = 
	{
		{ IDS_ELEM_DELETE, VK_DELETE }, // Löschen über DEL
		{ IDS_ELEM_PASTE,	VK_INSERT }, // Einfügen eine Kopie über EINF
		{0, 0 }
	};
	
	for (i = 0; hotkeys[i].msg; i++)
	{
		if (hotkeys[i].key == pTVKeyDown->wVKey)
		{
			PostMessage(WM_COMMAND, hotkeys[i].msg);
			break;
		}
	}

}

//Indicates that the control has the input focus and the user has pressed the 
//enter key
void GewinderohrEingabe::onGrohr_tree(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;
	HTREEITEM item = tree.GetSelectedItem();
	if (tree.ItemHasChildren(item))
		return;
	
	OnNew();
}

//Indicates the user clicked a button
void GewinderohrEingabe::onClickedGrohr_zm()
{
	static const struct zm 
	{
		UINT id; // Radiobutton
		UINT edit; // dazugehöriges Editfeld
		int flag; // dazugehöriges Flag aus Element::ZMASSTYP
	} set_zm [] = 
	{
		{ IDC_GROHR_ZM1, IDC_GROHR_ZML_ENDE, Element::EINGABE },
		{ IDC_GROHR_ZM2, IDC_GROHR_DN2, Element::FITTING },
		{ IDC_GROHR_ZM3, IDC_GROHR_DN3, Element::MUFFE_AUF_VERTEILER },
		{0, 0 },
	};

	// Entsprechendes Editfeld freischalten und zmassTyp setzen
	if (get_data()->get_werkauftrag()->get_status().get_wanr() == 0 || WaxSys::isAV())
	{
		for (int i = 0; set_zm[i].id; i++)
		{
			GetDlgItem(set_zm[i].edit)->EnableWindow(false);
			if (((CButton*)GetDlgItem(set_zm[i].id))->GetCheck())
			{
				data->set_zmassTyp(set_zm[i].flag);
				GetDlgItem(set_zm[i].edit)->EnableWindow(true);
			}
		}
	}

	
	// Initialisierung der Eingabefelder
	if (data->get_zmassTyp() != Element::EINGABE)
	{
		CString dnfeld;
		GetDlgItemText(IDC_GROHR_DN, dnfeld); // für die evtl. Vorauswahl

		if (data->get_zmassTyp() == Element::FITTING)
			onSelchangeGrohr_dn2();
		else
			onSelchangeGrohr_dn3();
	}
}

//Indicates a new combo box list item is selected
void GewinderohrEingabe::onSelchangeGrohr_dn2()
{
	// Aktuellen Wert holen
	CString dnnew;
	int index = ((CComboBox*)GetDlgItem(IDC_GROHR_DN2))->GetCurSel();
	if (index == CB_ERR)
		return;
	int dn2 = GetDlgItemInt(IDC_GROHR_DN2);
	if (dn2)
		data->set_zmassRefDN(dn2);
	
	int zmass = 0;
	// Z-Mass zu DN holen, wird kein Wert gefunden, ist Z-Mass 0. 
	for (POSITION eg = getStammdaten().get_zmass().GetHeadPosition(); eg != NULL;)
	{
		const ZMass::data *zm = dynamic_cast<const ZMass::data *>(getStammdaten().get_zmass().GetNext(eg));
		XASSERT(zm);
		if (!zm)
			continue;
		if (zm->get_dn() == dn2 && zm->get_dnvl() == 0)
		{ 
			zmass = zm->get_zmass_fitting();
			break;
		}
	}
	if (!zmass)
	{
		CString str; str = getStammdaten().getText("IDS_ZMASS_NICHT_GESETZT").c_str();
		AfxMessageBox(str, MB_ICONERROR);
		return;
	}

	setZMassRohranfang(zmass);
}

//Indicates a new combo box list item is selected
void GewinderohrEingabe::onSelchangeGrohr_dn3()
{
	/* 
	Setzt das ZMass Linkes Ende für die Auswahl "Muffe auf Verteiler"
	*/
	int index = ((CComboBox*)GetDlgItem(IDC_GROHR_DN3))->GetCurSel();
	if (index == -1)
		return;
	const int dn = data->get_dnvl();
	int dn3 = GetDlgItemInt(IDC_GROHR_DN3);
	if (dn3)
	{
		data->set_zmassRefDN(dn3);
	}
	int zmass = 0;
	// Z-Mass zu DN holen
	for (POSITION eg = getStammdaten().get_zmass().GetHeadPosition(); eg != NULL;)
	{
		const ZMass::data *zm = dynamic_cast<const ZMass::data *>(getStammdaten().get_zmass().GetNext(eg));
		XASSERT(zm);
		if (!zm)
			continue;
		if (zm->get_dn() == dn && zm->get_dnvl() == dn3)
		{ 
			zmass = zm->get_zmass_verteiler();
			break;
		}
	}
	if (!zmass)
	{
		CString str; str = getStammdaten().getText("IDS_ZMASS_NICHT_GESETZT").c_str();
		AfxMessageBox(str, MB_ICONERROR);
		return;
	}

	setZMassRohranfang(zmass);
}

void GewinderohrEingabe::onChangeGrohr_zml_ende()
{
	setZMassRohranfang(GetDlgItemInt(IDC_GROHR_ZML_ENDE));	
}

void GewinderohrEingabe::setZMassRohranfang(int zmassLinks)
{
	data->set_zmassLinks(zmassLinks);
	// Neues zmass im Eingabefeld setzen
	SetDlgItemInt(IDC_GROHR_ZML_ENDE, data->get_zmassLinks());

	// Wenn das erste Element ausgewählt wurde, muss auch das Feld mit Z Mass links gesetzt werden
	if (list.getSelectedItem() == 0)
		SetDlgItemInt(IDC_GROHR_ZML, data->get_zmassLinks());
}



//Indicates the edit control is losing the input focus
void GewinderohrEingabe::OnKillfocusGrohr_laenge()
{
	selectTab(FITTINGS);
	insertItems();
	selectFitting();
}



Element* GewinderohrEingabe::getSelectedElement(bool verkettung) 
{
	int select = list.getSelectedItem();
	if (data->get_elementArray().GetSize() > 0)
	{	
		if (select == -1)
			//return dynamic_cast<Element*>(data->get_elementArray().GetAt(data->get_elementArray().GetSize()-1));
			return NULL;
		else
		{
			if (verkettung)
				return data->get_elementArray().get_element_at(select);
			else
			{
				Element* ele = NULL;
				while (select >= 0)
				{
					ele = data->get_elementArray().get_element_at(select);
					if (ele->get_verkettung() == 0)
						break;
					select--;	
				}
				return ele;
			}
		}
	}
	return NULL;
}

void GewinderohrEingabe::selectFitting(Element *xele)
{
	if (!xele)
	{
		// Datensatz makiert?
		int index = list.getSelectedItem();
		if (index == -1) 
			return;

		ele_INIT_SAFE(index);
		xele = ele;
	}

	int curtab = xele->get_verkettung() ? 1 : 0;
	tab.SetCurSel(curtab);
	CTreeCtrl &t = curtab == 1 ? tree_zus : tree;

	// Endeelement auswählen (Rohrstück ohne Fitting)
	// Elementids liegen in der Root, sollten sie wieder als Untergruppe erscheinen, muss hier die
	// Initialisierung geändert werden.
	HTREEITEM child = t.GetRootItem();
	HTREEITEM sel = NULL;
	if (xele->get_id() >= ROHR_Q1 && xele->get_id() <= ROHR_Q5)
	{
		for (; child;  child = t.GetNextSiblingItem(child))
		{
			Bauelemente::data *b = dynamic_cast<Bauelemente::data *>((CObject*)t.GetItemData(child));
			if (!b)
				continue;
			if (data->get_rohrEnde() == b->get_elemid())
			{
				sel = child;
				break;
				
			}
		}
	}

	// Fitting auswählen. Dazu muss die Rootebene durchgegangen werden und alle Items, die Childitems besitzen 
	// müssen gescannt werden. Sollte sich die Hierarchieebene ändern, muss hier die Initialisierung angepasst werden
	HTREEITEM root = t.GetRootItem();
	for (; root && !sel; root = t.GetNextSiblingItem(root))
	{
		for (child = t.GetChildItem(root); child; child = t.GetNextSiblingItem(child))
		{
			FormstueckeUndFittinge::data *f = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)t.GetItemData(child));
			if (!f)
				continue;
			if (xele->get_artikelnr() == f->get_artikelnr())
			{
				sel = child;
				break;
			}
		}
	}
	t.SelectItem(sel);
	t.EnsureVisible(sel);
	onSelchangeTab(NULL, NULL);
}

void GewinderohrEingabe::onSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	selectTab();
	
	if (pResult)
		*pResult = 0;
}

void GewinderohrEingabe::selectTab(int tabid, bool setfocus  )
{
	/*
	Zeigt den jeweiligen Tab an. -1 zeigt nur das Fenster zum aktiven Tab an
	*/
	int index = tabid;
	XASSERT(index <= MAX_TAB);
	if (tabid < 0)
		index = tab.GetCurSel();	
	else
		tab.SetCurSel(index);
	
	tree.ShowWindow(index ? SW_HIDE : SW_SHOW);
	tree_zus.ShowWindow(index ? SW_SHOW : SW_HIDE);
	if (setfocus)
	{
		if (!index)
		{
			tree.SetFocus();
		}
		else
		{
			tree_zus.SetFocus();
		}
	}
}

void GewinderohrEingabe::setAnlage(Element* elem, int index)
{
	///Flag fuer Anlage setzen
	if (index == -1) 
		return;

	if (index == 0)
		elem->set_makeAnlage(data->isAnlagOK(elem->get_anschlussDN(), elem->get_abstRel()-data->get_zmassLinks()-elem->getZmass(Element::RECHTS)));
	else
	{
		while (index > 0)
		{
			ele_INIT_SAFE(index-1);
			//jetzt ist ele vorheriges Element
			if (ele && !ele->get_verkettung())
			{
				elem->set_makeAnlage(data->isAnlagOK(elem->get_anschlussDN(), elem->get_abstRel()-ele->getZmass(Element::LINKS)-elem->getZmass(Element::RECHTS)));
				break;
			}
			index--;
		}
	}
}

void GewinderohrEingabe::setZMassLinksRechts(Element* ele)
{
	///ZMasse in den beiden abgeblendeten Editfeldern setzen
	int index = list.getSelectedItem();
	if (index == -1) 
		return;

	SetDlgItemInt(IDC_GROHR_ZMR, ele->getZmass(Element::RECHTS));

	int zml = data->get_zmassLinks();
	while (index > 0)
	{
		ele_INIT_SAFE(index-1);
		//jetzt ist ele vorheriges Element
		if (ele && !ele->get_verkettung())
		{
			zml = ele->getZmass(Element::LINKS);
			break;
		}
		index--;
	}
	SetDlgItemInt(IDC_GROHR_ZML, zml);
}

void GewinderohrEingabe::onSelchangeGrohr_dn()
{
	insertItems();
}

BOOL GewinderohrEingabe::PreTranslateMessage(MSG* pMsg)
{
	/*
	Hotkey Strg+Z detektieren. 
	*/
	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == 'Z')
		{
			short keystate = ::GetAsyncKeyState(VK_CONTROL);
			if (keystate < 0)
			{
				PostMessage(WM_COMMAND, WM_ZUSAETZLICHER_FITTING);
				return true;			
			}
		}
	}
	return  WaxPartForm::PreTranslateMessage(pMsg);
}

void GewinderohrEingabe::onZusaetzlicherFitting()
{
	// Direkt in den richtigen Baum springen. Wird per Hotkey aufgerufen (Strg+Z)
	selectTab(1, true);
}

int GewinderohrEingabe::getReferenceDN() const
{
	// Liefert die Nennweite des ausgewählten Bauteils,
	// bzw. wenn nichts ausgewählt wurde, die ausgewählte DN im Eingabefeld, sonst die dnvl 
	// des Rohres.
	int ret;
	CComboBox *b = (CComboBox *)GetDlgItem(IDC_GROHR_DN);
	int index = b ? b->GetCurSel() : CB_ERR;
	if (index != CB_ERR)
		ret = b->GetItemData(index);
	else
		ret = data->get_dnvl();
	return ret;
}

void GewinderohrEingabe::updateTabOrder(bool remove)
{
	// Tabulator für Position und Anzahl werden nur beim ersten Element freigeschaltet, damit sie aus der Tab-Order ab
	// dem zweiten Element raus sind (Tastaturhandling). ZMass Eingabe für links braucht man auch nur
	// einmal
	UINT remove_from_tab_order[] = 
	{  
		IDC_GROHR_ANZAHL_NEU, 
		IDC_GROHR_POSNR_NEU, 
		IDC_GROHR_ZM1, 
		IDC_GROHR_ZM2,
		IDC_GROHR_ZM3,
		IDC_GROHR_ZML_ENDE,
		IDC_GROHR_DN2,
		IDC_GROHR_DN3,
		0 };
	for (int i = 0; remove_from_tab_order[i]; i++)
	{
		DWORD wndlong = GetWindowLong(GetDlgItem(remove_from_tab_order[i])->m_hWnd, GWL_STYLE);
		wndlong = remove ? wndlong & ~WS_TABSTOP : wndlong | WS_TABSTOP;
		SetWindowLong(GetDlgItem(remove_from_tab_order[i])->m_hWnd, GWL_STYLE, wndlong);
	}
}

bool GewinderohrEingabe::isEleDNok(const FormstueckeUndFittinge::data * fitting, const int dn) const
{
	// Ein Fitting kann auch als Kopfstück eingesetzt werden, deshalb muss es nur eine Über-
	// Einstimmung bei den DNs geben, damit die Methode true liefert
	if (fitting->get_gruppenid() == FormstueckeUndFittinge::DOPPELNIPPEL || fitting->get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL)
		return true;
	
	const int dn_f[4] = 
	{
		fitting->get_dn1(),
		fitting->get_dn2(),
		fitting->get_dn3(),
		fitting->get_dn4()
	};

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (dn_f[j] && dn_f[j] == dn)
				return true;
		}
	}
	return false;
}

bool GewinderohrEingabe::passendesGewinde(const int rel_abstand, const Element *ele, int gruppenid) const
{
	XASSERT(ele);
	if (!ele)
		return true; // Im Fehlerfall sollen lieber alle Elemente auswählbar sein
	Element e;
	e.set_gruppenid(gruppenid);

	if (rel_abstand == 0) // Verkettung an voriges Element, zusätzlicher Fitting
	{	
		if (ele->hasEleInnenAussengewinde())
			return true;
		if (ele->hasEleInnengewinde() && e.hasEleAussengewinde())
			return true;
		if (ele->hasEleAussengewinde() && e.hasEleInnengewinde())
			return true;
	}
	else
	{
		if (e.hasEleInnenAussengewinde() || e.hasEleInnengewinde()) 
			return true;
	}						

	return false;
}
