#include "stdafx.h"


#include "resource.h"

#include "FormstueckeUndFittingeEingabe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(FormstueckeUndFittingeEingabe, WaxPartForm)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, onDblclkTree)
	ON_NOTIFY(NM_RETURN, IDC_TREE, onReturnTree)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, onEndlabeleditList)
	ON_BN_CLICKED(IDC_DELETE, onClickedDelete)
	ON_BN_CLICKED(IDC_EDIT, onClickedEdit)
	ON_BN_CLICKED(IDC_INSERT, onClickedInsert)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_NEW, onClickedNew)
	ON_BN_CLICKED(IDC_STAMM, onClickedStammdaten)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(FormstueckeUndFittingeEingabe, WaxPartForm)

FormstueckeUndFittingeEingabe::FormstueckeUndFittingeEingabe()
: WaxPartForm(IDD_FORMSTUECKEUNDFITTINGE, getStammdaten().getText("IDD_FORMSTUECKEUNDFITTINGE").c_str()), istInitialisiert(false)
{
}

BOOL FormstueckeUndFittingeEingabe::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return false;

	ListView_SetExtendedListViewStyle(get_list().m_hWnd, LVS_EX_FULLROWSELECT);
	CString bez;
	bez = getStammdaten().getText("IDS_ANZAHL").c_str();		get_list().InsertColumn(ANZAHL, bez, LVCFMT_RIGHT, 50);
	bez = getStammdaten().getText("IDS_BESCHREIBUNG").c_str();	get_list().InsertColumn(BESCHREIBUNG, bez, LVCFMT_LEFT, 250);
	bez = getStammdaten().getText("IDS_ARTNR").c_str();			get_list().InsertColumn(ARTNR, bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_GEWICHT").c_str();		get_list().InsertColumn(GEWICHT, bez, LVCFMT_RIGHT, 80);

	bez = getStammdaten().getText("IDS_PREIS").c_str();			get_list().InsertColumn(PREIS, bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_PULVERPREIS").c_str();	get_list().InsertColumn(PULVERPREIS, bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_EINKAUF").c_str();		get_list().InsertColumn(EK, bez, LVCFMT_RIGHT, 30);


	setWindowsText(IDC_INSERT, "IDS_INSERT");
	setWindowsText(IDC_EDIT, "IDS_EDIT");
	setWindowsText(IDC_DELETE, "IDS_DELETE");
	setWindowsText(IDC_NEW, "IDS_NEW");
	setWindowsText(IDC_STAMM, "IDS_BEZ_LADEN");


	return true;
}

//Datenaustausch
void FormstueckeUndFittingeEingabe::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, tree);
	DDX_Control(pDX, IDC_LIST, list);
	DDX_Control(pDX, IDC_CB, cb);

	list.SetRedraw(FALSE);
	CObList& materialList = get_data()->get_werkauftrag()->get_material().get_matlist();

	if (pDX->m_bSaveAndValidate)
	{
		get_data()->get_werkauftrag()->get_material().clean();
		// neue Liste erstellen
		for (int index = 0; index < get_list().GetItemCount(); index++) 
		{
			Material *material = new Material();

			material->set_anzahl(atoi(get_list().GetItemText(index, ANZAHL)));
			material->set_bezeichnung(get_list().GetItemText(index, BESCHREIBUNG));
			material->set_gewicht(list.getDouble(index, GEWICHT)/material->get_anzahl());
			material->set_artikelnr(atoi(get_list().GetItemText(index, ARTNR)));
#ifdef __WAX_FUER_DRITTE__		
			FormstueckeUndFittinge f;
			material->set_festPreis(f.getPreis(material->get_artikelnr(), FormstueckeUndFittinge::festPreis));
			material->set_pulverpreis(f.getPreis(material->get_artikelnr(), FormstueckeUndFittinge::pulverPreis));
#else
			material->set_festPreis(list.getDouble(index, PREIS)/material->get_anzahl());
			material->set_pulverpreis(list.getDouble(index, PULVERPREIS)/material->get_anzahl());
#endif
			material->set_einkauf((get_list().GetItemText(index, EK) == "x")?true:false);
			materialList.AddTail(material);
		}
	}
	else
	{
		get_list().DeleteAllItems();
		int index = 0;

		for (POSITION pos = materialList.GetHeadPosition(); pos != NULL;) 
		{
			Material *material = dynamic_cast<Material*>(materialList.GetNext(pos));
			XASSERT(material);
			if (!material)
				continue;

			index = get_list().insertInt(index, ANZAHL, material->get_anzahl());
			XASSERT(index >= 0);
			if (index < 0)
				continue;
			get_list().insertString(index,  BESCHREIBUNG, material->get_bezeichnung());
			get_list().insertInt(index,		ARTNR, material->get_artikelnr());
			get_list().insertDouble(index,  GEWICHT, material->get_gewicht()*material->get_anzahl());
#ifndef __WAX_FUER_DRITTE__		
			get_list().insertDouble(index,  PREIS, material->get_festPreis()*material->get_anzahl());
			get_list().insertDouble(index,  PULVERPREIS, material->get_pulverpreis()*material->get_anzahl());
#endif
			get_list().insertString(index,  EK, (material->get_einkauf())?"x":"-");
			index++;
		}
	}
	list.SetRedraw(TRUE);
}

//get Methode für Baum
CTreeCtrl& FormstueckeUndFittingeEingabe::get_tree()
{
	return tree;
}

//Zeigt an, dass im Baum doppelgeklickt wurde
void FormstueckeUndFittingeEingabe::onDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	insertMaterial(false);
	*pResult = 1;
}

//Zeigt an, dass im Baum Alt+Return gedrückt wurde
void FormstueckeUndFittingeEingabe::onReturnTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	insertMaterial(false);
	*pResult = 1;
}

//Wird aufgerufen, nachdem das Fenster in der Grösse geändert wurde
void FormstueckeUndFittingeEingabe::OnSize(UINT nType, int cx, int cy)
{
	WaxPartForm::OnSize(nType, cx, cy);

	const int AbstandUnten = 40;
	const int AbstandLinks = 10;
	const int AbstandOben = 30;
	const int Verteilung = 3; // Baum bekommt ein Drittel, Liste den Rest

	if (::IsWindow(get_tree().m_hWnd))
		get_tree().SetWindowPos( NULL, AbstandLinks, AbstandOben, cx/Verteilung, cy-AbstandUnten, SWP_SHOWWINDOW | SWP_NOZORDER);
	if (::IsWindow(get_list().m_hWnd))
		get_list().SetWindowPos( NULL, cx/Verteilung+AbstandLinks+1, AbstandOben, cx - cx/Verteilung - 2*AbstandLinks, cy-AbstandUnten, SWP_SHOWWINDOW | SWP_NOZORDER);
	if (::IsWindow(get_cb().m_hWnd))
		get_cb().SetDroppedWidth(cx - 2*AbstandLinks);
}

//get Methode für die Liste
ListCtrl& FormstueckeUndFittingeEingabe::get_list()
{
	return list;
}

//get Methode für die Combobox
CComboBox& FormstueckeUndFittingeEingabe::get_cb()
{
	return cb;
}

//Fügt das ausgewählte Material in die Liste ein. Wenn fromCB = true 
//ist, die Auswahl der Combobox benutzt
void FormstueckeUndFittingeEingabe::insertMaterial(bool fromCB) 
{
	FormstueckeUndFittinge::data *fitting = NULL;
	if (fromCB)
	{
		UINT index = get_cb().GetCurSel();
		if (index != CB_ERR)
			fitting = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)get_cb().GetItemDataPtr(index));
	}
	else
	{
		HTREEITEM item = get_tree().GetSelectedItem();
		if (item)
		{
			HTREEITEM child = get_tree().GetChildItem(item);
			if (child)
				return;
			fitting = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)get_tree().GetItemData(item));
		}
	}

	XASSERT(fitting);
	if (fitting)
	{
		int index = get_list().GetItemCount();
		for (int i = 0; i < index; i++)
		{
			if ((fitting->get_artikelnr() == atoi(get_list().GetItemText(i, 2))) && (fitting->get_kurztext() == get_list().GetItemText(i, 1)))
			{
				int anzahl = atoi(get_list().GetItemText(i, ANZAHL));
				CString s;
				s.Format("%d", anzahl+1);
				get_list().SetItemText(i, ANZAHL, s);
				calcPreisUndGewicht(i, fitting->get_festPreis(), fitting->get_pulverpreis(), fitting->get_gewicht(), anzahl+1);
				return;
			}
		}

		index = index > 0 ? index : 0;
		index = get_list().InsertItem(index, "1");
		get_list().SetItemText(index, BESCHREIBUNG, fitting->get_kurztext());
		CString txt;
		txt.Format("%-.6i", fitting->get_artikelnr());
		get_list().SetItemText(index, ARTNR, txt);
		
		get_list().insertDouble(index, GEWICHT, fitting->get_gewicht());
#ifndef __WAX_FUER_DRITTE__		
		get_list().insertDouble(index, PREIS, fitting->get_festPreis());
		get_list().insertDouble(index, PULVERPREIS, fitting->get_pulverpreis());
#endif
		get_list().insertString(index, EK, (fitting->get_einkauf())?"x":"-");

		get_list().selectItem(index);
		PostMessage(WM_COMMAND, BN_CLICKED<<16 | IDC_EDIT, (LPARAM)GetDlgItem(IDC_EDIT)->m_hWnd);
	}
	
}

//Zeigt an, dass der Benutzer die Editierung des Itemlabels beendet hat
void FormstueckeUndFittingeEingabe::onEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	int sel = get_list().getSelectedItem();
	// Datensatz merken
	Material* ma = readMaterial(sel);
	
	if (pDispInfo->item.pszText && strlen(pDispInfo->item.pszText) && atoi(pDispInfo->item.pszText))
	{
		get_list().SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
		*pResult = 1;
	}
	else
	{
		*pResult = 0; 
		delete ma;
		return;
	}

	int anzahl = atoi(get_list().GetItemText(sel, ANZAHL));
	// Preis/Gewicht neu berechnen 
	calcPreisUndGewicht(sel, ma->get_festPreis()/ma->get_anzahl(), ma->get_pulverpreis()/ma->get_anzahl(), ma->get_gewicht()/ma->get_anzahl(), anzahl);
	ma = readMaterial(sel);		
	delete ma;
}

//Zeigt an, dass der Button Stammdaten geklickt wurde
void FormstueckeUndFittingeEingabe::onClickedStammdaten()
{
	for (int index = 0; index < get_list().GetItemCount(); index++) 
	{
		int artnr = atoi(get_list().GetItemText(index, ARTNR));

		FormstueckeUndFittinge f;
		CString bez = f.getKurztext(artnr);
		if (bez.GetLength())
			get_list().SetItemText(index, BESCHREIBUNG, bez);
	}
}

//Zeigt an, dass der Button Delete geklickt wurde
void FormstueckeUndFittingeEingabe::onClickedDelete()
{
	int index = get_list().getSelectedItem();
	if (index >= 0)
		get_list().deleteItem(index);
}

//Zeigt an, dass der Button Edit geklickt wurde
void FormstueckeUndFittingeEingabe::onClickedEdit()
{
	int sel = get_list().getSelectedItem();
	
	// Datensatz merkern
	Material* ma = readMaterial(sel);

	// Datensatz bearbeiten
	if (WaxSys::isAV())
	{
		onEditNew();
	}
	else
	{
		get_list().SetFocus();
		ListView_EditLabel(get_list().m_hWnd, sel);
	}

	int anzahl = atoi(get_list().GetItemText(sel, ANZAHL));
	// Preis/Gewicht neu berechnen 
	calcPreisUndGewicht(sel, ma->get_festPreis()/ma->get_anzahl(), ma->get_pulverpreis()/ma->get_anzahl(), ma->get_gewicht()/ma->get_anzahl(), anzahl);
	ma = readMaterial(sel);		
	
	delete ma;
}

//Zeigt an, dass der Button Insert geklickt wurde
void FormstueckeUndFittingeEingabe::onClickedInsert()
{
	int sel = get_cb().GetCurSel();

	// Entweder Auswahl der Combobox einfügen oder aus der Liste
	insertMaterial(sel == CB_ERR ? false:true);
}

//Preis und Gewichtsberechung
void FormstueckeUndFittingeEingabe::calcPreisUndGewicht(int index, double preis, double pulverpreis, double gewicht, int anzahl)
{
	get_list().insertDouble(index, GEWICHT, gewicht*(double)anzahl);
#ifndef __WAX_FUER_DRITTE__		
	get_list().insertDouble(index, PREIS, preis*(double)anzahl);
	get_list().insertDouble(index, PULVERPREIS, pulverpreis*(double)anzahl);
#endif
}

//Initialisierung einer TV_INSERTSTRUCT Struktur zum Einfügen in 
//einen Treecontrol.
TV_INSERTSTRUCT FormstueckeUndFittingeEingabe::makeInsertStruct(HTREEITEM lastitem, UINT image, const CString &titel, BOOL hasChildren, void *code) const
{
	static char tit[255];
	strcpy(tit, titel.Left(254));
	TV_INSERTSTRUCT TreeCtrlItem;
	TreeCtrlItem.hParent = lastitem; 

	TreeCtrlItem.item.pszText = tit; 
	TreeCtrlItem.item.iImage = image; 
	TreeCtrlItem.item.iSelectedImage = image; 
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
	TreeCtrlItem.item.cChildren = hasChildren;
	TreeCtrlItem.item.lParam = (DWORD)(code);
	return TreeCtrlItem;
}

//Called when CWnd is to be hidden or shown.
void FormstueckeUndFittingeEingabe::OnShowWindow(BOOL bShow, UINT nStatus)
{
	WaxPartForm::OnShowWindow(bShow, nStatus);
	if (!istInitialisiert)
	{
		TV_INSERTSTRUCT TreeCtrlItem;
		char titel[255];

		CString grp, grplast;
		HTREEITEM parent = TVI_ROOT;
		HTREEITEM rootitem = TVI_ROOT;
		int elemid = 0;
		tree.SetRedraw(FALSE);
		cb.SetRedraw(FALSE);

		for (POSITION pos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); pos;)
		{
			FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(pos);

			// Elementid ist entweder Formstücke oder Fittinge
			if (f->get_elemid() != elemid)
			{
				elemid = f->get_elemid();
				for (POSITION eg = getStammdaten().get_bauelementgruppen().GetHeadPosition(); eg != NULL;)
				{
					BauelementGruppen::data *ele = (BauelementGruppen::data *) getStammdaten().get_bauelementgruppen().GetNext(eg);
					if (ele->get_elem_ID() == elemid)
					{ // Bezeichnung für Formstücke und Fittinge gefunden
						strcpy(titel, ele->get_bezeichnung().Left(254));
						TreeCtrlItem = makeInsertStruct(TVI_ROOT, 0, titel, TRUE, NULL);
						rootitem = get_tree().InsertItem(&TreeCtrlItem);
						TV_ITEM stateit;
						memset(&stateit, 0, sizeof(TV_ITEM));
						stateit.mask = TVIF_STATE;
						stateit.hItem = rootitem;
						stateit.stateMask = TVIS_BOLD;
						stateit.state = TVIS_BOLD;
						tree.SetItem(&stateit);			

						break;
					}
				}
			}

			if (f->get_gruppe() != grplast)
			{
				strcpy(titel, f->get_gruppe().Left(254));
				TreeCtrlItem = makeInsertStruct(rootitem, 0, titel, TRUE, NULL);
				grplast = f->get_gruppe();
				parent = get_tree().InsertItem(&TreeCtrlItem);
				if (parent)
					get_tree().EnsureVisible(parent);
			}
		
			if (f->get_elemid() == FORMSTUECKE &&
				(
#ifndef __WAX_FUER_DRITTE__	
				f->get_gruppenid() == FormstueckeUndFittinge::KUPPLUNG
				||
#endif
				f->get_gruppenid() == FormstueckeUndFittinge::BOGEN 
				|| f->get_gruppenid() == FormstueckeUndFittinge::T_STUECK) 
				&& !f->get_schwarz())
				continue;
						
			CString bez;
#ifndef __WAX_FUER_DRITTE__		
			bez.Format("%-70s        %-.6i      %.3f   %.2f", f->get_kurztext(), f->get_artikelnr(), f->get_gewicht(), f->get_festPreis());
#else
			bez.Format("%-70s        %-.6i      %.3f", f->get_kurztext(), f->get_artikelnr(), f->get_gewicht());
#endif
			strcpy(titel, bez.Left(254));
			int index = get_cb().AddString(bez);
			get_cb().SetItemDataPtr(index, f);
			TreeCtrlItem = makeInsertStruct(parent, 0, titel, FALSE, f);
			get_tree().InsertItem(&TreeCtrlItem);
		}
		tree.SetRedraw(TRUE);
		cb.SetRedraw(TRUE);
		istInitialisiert = true;
	}
}

void FormstueckeUndFittingeEingabe::onClickedNew()
{
	// Alle Selektionen zurücksetzen
	int insertafter = -1;
	POSITION pos = list.GetFirstSelectedItemPosition();
	while (pos)
	{
		insertafter = list.GetNextSelectedItem(pos);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, (UINT)~LVIS_SELECTED, LVIS_SELECTED, NULL);
	}

	// Item einfuegen
	CString txt = "1";
	int index = list.InsertItem(insertafter+1, txt);

	txt = getStammdaten().getText("IDS_FORMUNDFITT").c_str();
	list.SetItemText(index, BESCHREIBUNG, txt); 
	txt.Format("%d", 0);
	list.SetItemText(index, ARTNR, txt);
	txt.Format("%10.2f", 0);
	list.SetItemText(index, GEWICHT, txt);
	list.SetItemText(index, PREIS, txt);
	list.SetItemText(index, PULVERPREIS, txt);
	list.SetItemText(index, EK, "x");

	list.SetItemState(insertafter+1, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);

	// Default-F&F in Liste packen
	Material *ma = new Material;
	ma->set_anzahl(1);
	txt = getStammdaten().getText("IDS_FORMUNDFITT").c_str();
	ma->set_bezeichnung(txt);
	get_data()->get_werkauftrag()->get_material().get_matlist().AddTail(ma);
	writeMaterial(ma, index);
	
	// Eintrag editieren
	if (!onEditNew())
	{
		list.DeleteItem(insertafter+1);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, LVIS_SELECTED, LVIS_SELECTED, NULL);
	}
	//delete ma;
}

bool FormstueckeUndFittingeEingabe::onEditNew()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (!pos)
		return false;
	bool ok = false;
	int index = list.GetNextSelectedItem(pos);
	Material* ma = readMaterial(index);
	
	EditFormstueckeUndFittinge e(this, ma);
	while (!ok)
	{
		if (e.DoModal() == IDOK)
		{
			writeMaterial(ma, index);
			if (list.valueInListe(ARTNR, ma->get_artikelnr()))
			{
				CString str; str = getStammdaten().getText("IDS_ARTIKEL_IN_LISTE").c_str();
				AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				delete ma;
				return true;
			}
		}
		else 
			ok = true;
	}
	delete ma;
	return false;
}

void FormstueckeUndFittingeEingabe::writeMaterial(Material* mat, int index)
{
	CString txt = "";
	txt.Format("%i", mat->get_anzahl());
	list.SetItemText(index, ANZAHL, txt);

	list.SetItemText(index, BESCHREIBUNG, mat->get_bezeichnung());
		
	txt.Format("%.d", mat->get_artikelnr());
	list.SetItemText(index, ARTNR, txt);

	list.insertDouble(index, GEWICHT, mat->get_gewicht());
#ifndef __WAX_FUER_DRITTE__		
	list.insertDouble(index, PREIS, mat->get_festPreis());
	list.insertDouble(index, PULVERPREIS, mat->get_pulverpreis());
#endif
	list.insertString(index, EK, (mat->get_einkauf())?"x":"-");
}

Material* FormstueckeUndFittingeEingabe::readMaterial(int index)
{
	Material* ma = new Material;
	
	CString tmp = list.GetItemText(index, ANZAHL);
	ma->set_anzahl(atoi(tmp));
	
	tmp = list.GetItemText(index, BESCHREIBUNG);
	ma->set_bezeichnung(tmp);
	
	tmp = list.GetItemText(index, ARTNR);
	ma->set_artikelnr(atoi(tmp));

	ma->set_gewicht(list.getDouble(index, GEWICHT));
	ma->set_festPreis(list.getDouble(index, PREIS));
	ma->set_pulverpreis(list.getDouble(index, PULVERPREIS));
	ma->set_einkauf((list.GetItemText(index, EK) == "x")?true:false);
		
	return ma;
}
