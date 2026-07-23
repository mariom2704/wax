#include "stdafx.h"


#include "resource.h"
#include "MaterialEingabe.h"


BEGIN_MESSAGE_MAP(MaterialEingabeForm, WaxPartForm)
	ON_BN_CLICKED(IDC_DELETE, onClickedDelete)
	ON_BN_CLICKED(IDC_EDIT, onClickedEdit)
	ON_BN_CLICKED(IDC_INSERT, onClickedInsert)
	ON_BN_CLICKED(IDC_NEW, onClickedNew)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, onDblclkTree)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, onEndlabeleditList)
	ON_NOTIFY(NM_RETURN, IDC_TREE, onReturnTree)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_EDITLABEL, onClickedEditlabel)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(MaterialEingabeForm, WaxPartForm)

//Konstruktor
MaterialEingabeForm::MaterialEingabeForm()
: WaxPartForm(IDD_MATERIAL, getStammdaten().getText("IDD_MATERIAL").c_str()), istInitialisiert(false)
{
}

//Destruktor
MaterialEingabeForm::~MaterialEingabeForm()
{
}

//Dialoginitialisierung
BOOL MaterialEingabeForm::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	setWindowsText(IDC_INSERT, "IDS_INSERT");
	setWindowsText(IDC_EDIT, "IDS_EDIT");
	setWindowsText(IDC_DELETE, "IDS_DELETE");
	setWindowsText(IDC_NEW, "IDS_NEW");

	initList();

	DisableAllControls();
	return TRUE;
}

//Zeigt an, dass der Button Delete geklickt wurde
void MaterialEingabeForm::onClickedDelete()
{
	int sel = list.getSelectedItem();
	if (sel != -1)
	{
		bool sellast = false;
		if (sel)
			if (list.GetItemCount()-1 == sel) //letztes Item
				sellast = true;

		list.DeleteItem(sel);
		ListView_SetItemState(list.m_hWnd, sellast ? sel -1 : sel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

//Zeigt an, dass der Button Edit geklickt wurde
void MaterialEingabeForm::onClickedEdit()
{
	// Beim Doppelklicken muss der Profilreiter nicht angezeigt werden, dies passiert nur beim Einfügen
	edit(false);
}

void MaterialEingabeForm::edit(bool showprofilreiter)
{
	/**
	Editiert die aktuell ausgewählte Position
	*/
	int sel = list.getSelectedItem();

	list.SetFocus();
	Material *mat = readMaterial(sel);
	MaterialEingabe matedit(mat, showprofilreiter);
	
	if (matedit.DoModal() == IDOK)
	{
		writeMaterial(sel, mat);
	}
	delete mat;
}


void MaterialEingabeForm::onClickedNew()
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

	list.SetItemText(index, POSNR, get_data()->get_werkauftrag()->get_posNr());
	txt = getStammdaten().getText("IDS_MAT").c_str();
	list.SetItemText(index, BESCHREIBUNG, txt); 
	txt.Format("%d", 0);
	list.SetItemText(index, ARTNR, txt);
	txt.Format("%10.2f", 0);
	list.SetItemText(index, GEWICHT, txt);
	list.SetItemText(index, PREIS, txt);
	list.SetItemText(index, PULVERPREIS, txt);
	list.SetItemText(index, EINKAUF, "X");

	list.SetItemState(insertafter+1, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);

	// Default-Material in Liste packen
	Material *ma = new Material;
	if (list.GetItemCount() == 0 || insertafter == -1) 
		ma->set_posnr(get_data()->get_werkauftrag()->get_posNr());
	else
		ma->set_posnr(list.GetItemText(insertafter, POSNR));
	
	ma->set_anzahl(1);
	txt = getStammdaten().getText("IDS_MAT").c_str();
	ma->set_bezeichnung(txt);
	ma->set_einkauf(true);
	get_data()->get_werkauftrag()->get_material().get_matlist().AddTail(ma);
	writeMaterial(index, ma);
	
	// Eintrag editieren
	if (!onEditNew())
	{
		list.DeleteItem(insertafter+1);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, LVIS_SELECTED, LVIS_SELECTED, NULL);
	}
}

bool MaterialEingabeForm::onEditNew()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (!pos)
		return false;
	bool ok = false;
	int index = list.GetNextSelectedItem(pos);
	Material* ma = readMaterial(index);
	
	MaterialEingabe e(ma);
	while (!ok)
	{
		if (e.DoModal() == IDOK)
		{
			writeMaterial(index, ma);
			if (ma->get_id() == NOARTNR && ma->get_artikelnr() == 0)
			{
				delete ma;
				return true;
			}

			if (ma->get_id() != NOARTNR && ma->get_artikelnr() == 0)
			{
				CString str; str = getStammdaten().getText("err_artikelnr_0").c_str();
				AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
				continue;
			}

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

//Zeigt an, dass der Button Insert geklickt wurde
void MaterialEingabeForm::onClickedInsert()
{
	insertMaterial();
}

//Zeigt an, dass im Baum doppelgeklickt wurde
void MaterialEingabeForm::onDblclkTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	insertMaterial();
}

//Zeigt an, dass der Benutzer die Editierung des Itemlabels beendet 
//hat
void MaterialEingabeForm::onEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	if (pDispInfo->item.pszText && strlen(pDispInfo->item.pszText) && atoi(pDispInfo->item.pszText))
	{
		double gewicht = atof(list.GetItemText(pDispInfo->item.iItem, GEWICHT));
		double preis = atof(list.GetItemText(pDispInfo->item.iItem, PREIS));
		double pulverpreis = atof(list.GetItemText(pDispInfo->item.iItem, PULVERPREIS));
		double laenge = atof(list.GetItemText(pDispInfo->item.iItem, LAENGE));
		list.SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
		int anzahl = atoi(list.GetItemText(pDispInfo->item.iItem, ANZAHL));
		calcPreisUndGewicht(pDispInfo->item.iItem, preis, pulverpreis, gewicht, anzahl, (int)laenge);
		*pResult = 1;
	}
	else
	{
		*pResult = 0; 
	}
}

//Zeigt an, dass im Baum Alt+Return gedrückt wurde
void MaterialEingabeForm::onReturnTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	insertMaterial();
}

//Wird aufgerufen, nachdem das Fenster in der Grösse geändert wurde
void MaterialEingabeForm::OnSize(UINT nType, int cx, int cy)
{
	WaxPartForm::OnSize(nType, cx, cy);

	const int AbstandUnten = 40;
	const int AbstandLinks = 10;
	const int AbstandOben = 30;
	const int Verteilung = 3; // Baum bekommt ein Drittel, Liste den Rest
	const int ButtonAbstand = 5;

	if (::IsWindow(tree.m_hWnd))
		tree.SetWindowPos( NULL, AbstandLinks, 0, cx/Verteilung, cy-AbstandUnten+AbstandOben, SWP_SHOWWINDOW | SWP_NOZORDER);
	else
		return;
	if (::IsWindow(list.m_hWnd))
		list.SetWindowPos( NULL, cx/Verteilung+AbstandLinks+1, AbstandOben, cx - cx/Verteilung - 2*AbstandLinks, cy-AbstandUnten, SWP_SHOWWINDOW | SWP_NOZORDER);
	else
		return;
	// Buttons IDC_INSERT, IDC_DELETE und IDC_EDIT müssen mitskaliert werden: Der Abstand zum Baum ist immer fest

	CWnd *butt[4];
	butt[0] = GetDlgItem(IDC_INSERT);
	butt[1] = GetDlgItem(IDC_EDIT);
	butt[2] = GetDlgItem(IDC_DELETE);
	butt[3] = GetDlgItem(IDC_NEW);
	
	XASSERT(butt[0] && butt[1] && butt[2] && butt[3]);
	if (!butt[0] || !butt[1] || !butt[2] || !butt[3])
		return;

	WINDOWPLACEMENT wndpl; 
	butt[0]->GetWindowPlacement(&wndpl); 
	int xcx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;

	for (int i = 0; i < 4; i++)
		butt[i]->SetWindowPos( NULL, AbstandLinks + cx/Verteilung + (i+1)*ButtonAbstand + i*xcx, wndpl.rcNormalPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
}

//Preis und Gewichtsberechung
void MaterialEingabeForm::calcPreisUndGewicht(int index, double preis, double pulverpreis, double gewicht, int anzahl, int laenge)
{
	CString str;
	double gesamtgewicht = laenge > 1.0 ? gewicht * laenge / 1000.0 * anzahl : gewicht * anzahl;
	list.insertDouble(index, GESAMTGEWICHT, gesamtgewicht);
	double gesamtpreis = laenge > 1.0 ? preis * laenge / 1000.0 * anzahl : preis * anzahl;
	list.insertDouble(index, GESAMTPREIS, gesamtpreis);
	double gesamtpulverpreis = pulverpreis * anzahl;
	list.insertDouble(index, GESAMTPULVERPREIS, gesamtpulverpreis);
}

//Fügt das ausgewählte Material in die Liste ein.
void MaterialEingabeForm::insertMaterial( )
{
	FormstueckeUndFittinge::data *f = NULL;
	Bauelemente::data *b = NULL;
	Profile::data *p = NULL;
	Sondermaterial::data *s = NULL;

	CString kurztext;

	HTREEITEM item = tree.GetSelectedItem();
	if (item)
	{
		f = dynamic_cast<FormstueckeUndFittinge::data *>((CObject*)tree.GetItemData(item));
		if (!f)
			b = dynamic_cast<Bauelemente::data *>((CObject*)tree.GetItemData(item));
		if (!b)
			p = dynamic_cast<Profile::data *>((CObject*)tree.GetItemData(item));
		if (!p)
			s = dynamic_cast<Sondermaterial::data *>((CObject*)tree.GetItemData(item));

		kurztext = tree.GetItemText(item);
	}

	Material *mat = NULL;
	if (f || b || p || s)
	{
		int artnr = 0;

		if (f)
		{
			mat = conv2Material(f);
		}
		else if (b)
		{
			artnr = b->get_artikelnr();
			int index = kurztext.Find(")", 0);
			if (index > 0)
				kurztext = kurztext.Left(index+1);
			mat = conv2Material(b, kurztext);
		}
		else if (p)
		{
			mat = conv2Material(p);	
		}
		else
		{
			mat = conv2Material(s);	
		}


		int index = list.GetItemCount();
		for (int i = 0; i < index; i++)
		{
			if ((atoi(list.GetItemText(i, 2)) == 0) && (mat->get_artikelnr() == atoi(list.GetItemText(i, 5))) && (mat->get_kurztext() == list.GetItemText(i, 3)))
			{
				int anzahl = atoi(list.GetItemText(i, ANZAHL));
				CString s;
				s.Format("%d", anzahl+1);
				list.SetItemText(i, ANZAHL, s);
				calcPreisUndGewicht(i, mat->get_sapPreis(), mat->get_pulverpreis(), mat->get_gewicht(), anzahl+1, mat->get_mass1());
				delete mat;
				return;
			}
		}
	
		index = list.getSelectedItem();
		if (list.GetItemCount() == 0 || index == -1) 
			mat->set_posnr(get_data()->get_werkauftrag()->get_posNr());
		else
		{
			Material *m = readMaterial(index);
			mat->set_posnr(m->get_posnr());
		}
	
		index = list.InsertItem(index+1, "");
		writeMaterial(index, mat);
		list.selectItem(index);

		if (p)
		{
			// Editieren und Profilreiter zeigen, wenns denn ein Profil ist
			edit(false);
		}
		else
		{
			ListView_SetItemState(list.m_hWnd, index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			PostMessage(WM_COMMAND, BN_CLICKED<<16 | IDC_EDITLABEL, (LPARAM)GetDlgItem(IDC_EDIT)->m_hWnd);
		}
		delete mat;
	}
}

//Datenaustausch
void MaterialEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, tree);
	DDX_Control(pDX, IDC_LIST, list);

	CObList& materialList = get_data()->get_werkauftrag()->get_material().get_matlist();

	list.SetRedraw(FALSE);
	if (pDX->m_bSaveAndValidate)
	{
		get_data()->get_werkauftrag()->get_material().clean();
		// neue Liste erstellen
		for (int index = 0; index < list.GetItemCount(); index++) 
		{
			materialList.AddTail(readMaterial(index));
		}
	}
	else
	{
		if (list.getColumnCount())
		{
			list.DeleteAllItems();
			int index = 0;

			for (POSITION pos = materialList.GetHeadPosition(); pos != NULL;) 
			{
				Material *material = dynamic_cast<Material*>(materialList.GetNext(pos));
				XASSERT(material);
				if (!material)
					continue;

				index = list.InsertItem(index, "");
				writeMaterial(index++, material);
			}
		}
	}
	list.SetRedraw(TRUE);
}

//Initialisierung der Materialansicht (Spalten und Bezeichnungen)
void MaterialEingabeForm::initList()
{
	ListView_SetExtendedListViewStyle(list.m_hWnd, LVS_EX_FULLROWSELECT);
	CString bez;
	bez = getStammdaten().getText("IDS_MAT_POSNR").c_str();			list.InsertColumn(POSNR,			bez, LVCFMT_RIGHT, 50);
	bez = getStammdaten().getText("IDS_MAT_ANZAHL").c_str();		list.InsertColumn(ANZAHL,			bez, LVCFMT_RIGHT, 50);
	bez = getStammdaten().getText("IDS_MAT_LAENGE").c_str();		list.InsertColumn(LAENGE,			bez, LVCFMT_RIGHT, 50);
	bez = getStammdaten().getText("IDS_BESCHREIBUNG").c_str();		list.InsertColumn(BESCHREIBUNG, 	bez, LVCFMT_LEFT, 250);
	bez = getStammdaten().getText("IDS_PROFILE").c_str();			list.InsertColumn(PROFILE,			bez, LVCFMT_RIGHT, 80);
	bez = getStammdaten().getText("IDS_ARTNR").c_str();				list.InsertColumn(ARTNR,			bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_GEWICHT").c_str();			list.InsertColumn(GEWICHT,			bez, LVCFMT_RIGHT, 80);
	bez = getStammdaten().getText("IDS_GESAMTGEWICHT").c_str();		list.InsertColumn(GESAMTGEWICHT,	bez, LVCFMT_RIGHT, 80);
	bez = getStammdaten().getText("IDS_PREIS").c_str();				list.InsertColumn(PREIS,			bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_GESAMTPREIS").c_str();		list.InsertColumn(GESAMTPREIS,		bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_PULVERPREIS").c_str();		list.InsertColumn(PULVERPREIS,		bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_GESAMTPULVERPREIS").c_str();	list.InsertColumn(GESAMTPULVERPREIS,bez, LVCFMT_RIGHT, 60);
	bez = getStammdaten().getText("IDS_EINKAUF").c_str();			list.InsertColumn(EINKAUF,			bez, LVCFMT_RIGHT, 20);
	bez = getStammdaten().getText("IDS_HALBZEUG").c_str();			list.InsertColumn(HALBZEUG,			bez, LVCFMT_RIGHT, 20);
	bez = getStammdaten().getText("IDS_ID").c_str();				list.InsertColumn(ID,				bez, LVCFMT_RIGHT, 80);

	list.setDblclkMsg(IDC_EDIT);
}

//Called when CWnd is to be hidden or shown.
void MaterialEingabeForm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	WaxPartForm::OnShowWindow(bShow, nStatus);

	if (!istInitialisiert)
	{
		// Ertst alle...
		//Liest die Bauelemente aus den Stammdaten und fügt sie in den Baum ein
		insertElemente();
		//Liest die Formstuecke und Fittinge aus den Stammdaten und fügt sie in den Baum ein
		insertFStuFitt();
		//Liest die Profile aus den Stammdaten und fügt sie in den Baum ein
		insertProfile();
		//Liest das Sondermaterial den Stammdaten und fügt sie in den Baum ein
		insertSondermaterial();

		// ...dann nur CO2Tikko
		HTREEITEM root = insertElemente(true);
		insertSondermaterial(true, root);

		istInitialisiert = true;
	}
}

//Fügt die Elementliste in den Materialbaum ein
HTREEITEM MaterialEingabeForm::insertElemente(bool co2tikko)
{
	TV_INSERTSTRUCT TreeCtrlItem;
	char titel[255];

	CString grp, grplast;
	HTREEITEM parent = TVI_ROOT;
	HTREEITEM rootitem = TVI_ROOT;
	int elemid = -1;

	tree.SetRedraw(FALSE);

	if (co2tikko)
	{
		CString str; str = getStammdaten().getText("IDS_CO2_TIKKO").c_str();
		strcpy(titel, str);
		TreeCtrlItem = makeInsertStruct(TVI_ROOT, 0, titel, TRUE, NULL);
		parent = rootitem = tree.InsertItem(&TreeCtrlItem);
	}

	for (int i=0; i<2; i++)
	{

		for (POSITION pos = getStammdaten().get_bauelemente().GetHeadPosition(); pos;)
		{
			Bauelemente::data *f = (Bauelemente::data *)getStammdaten().get_bauelemente().GetNext(pos);

			if (((i == 0) && (f->get_elemid() == ROHR_Q0 || f->get_elemid() == ROHR_Q3 || f->get_elemid() == ROHR_Q5
			               || f->get_elemid() == ROHR_Q6 || f->get_elemid() == ROHR_Q7 || f->get_elemid() == ROHR_Q8
						   || f->get_elemid() == ROHR_Q9 || f->get_elemid() == ROHR_Q10|| f->get_elemid() == ROHR_Q11 || f->get_elemid() == ROHR_Q12 || f->get_elemid() == ROHR_Q13)) 
						   || ((i == 1) && (f->get_elemid() != Element::LEER   && f->get_elemid() != GLATT  && f->get_elemid() != NUT
						  && f->get_elemid() != GEW  && f->get_elemid() != SNAHT  && f->get_elemid() != SCHWEISS
						  && f->get_elemid() != BOHR24  && f->get_elemid() != BOHR50 && f->get_elemid() != EINZ
						  && f->get_elemid() != ROHR_Q0 && f->get_elemid() != ROHR_Q1
						  && f->get_elemid() != ROHR_Q3 
						  && f->get_elemid() != ROHR_Q5 && f->get_elemid() != ROHR_Q6
						  && f->get_elemid() != ROHR_Q7 && f->get_elemid() != ROHR_Q8
						  && f->get_elemid() != ROHR_Q9 && f->get_elemid() != ROHR_Q10
						  && f->get_elemid() != ROHR_Q11 && f->get_elemid() != ROHR_Q12 
						  && f->get_elemid() != ROHR_Q13 && f->get_elemid() != ROHR_Q14 && f->get_elemid() != ROHR_Q15)))
			{
			

				if(	f->get_elemid() >= FLANSCH_COII || f->get_elemid() == ROHR_Q16 || f->get_elemid() == ROHR_Q18)
				{
					//Ignore chemical and COII HD stuff 
					continue;
				}

				if (co2tikko && !f->get_co2tikko())
					continue;

				if (f->get_elemid() != elemid)
				{
					elemid = f->get_elemid();
					for (POSITION eg = getStammdaten().get_bauelementgruppen().GetHeadPosition(); eg != NULL;)
					{
						BauelementGruppen::data *ele = (BauelementGruppen::data *) getStammdaten().get_bauelementgruppen().GetNext(eg);
						if (ele->get_elem_ID() == elemid)
						{ // Bezeichnung für Formstücke und Fittinge gefunden
							strcpy(titel, ele->get_bezeichnung().Left(254));
		
							if (co2tikko)
							{
								TreeCtrlItem = makeInsertStruct(rootitem, 0, titel, TRUE, NULL);
								parent = tree.InsertItem(&TreeCtrlItem);
							}
							else
							{
								TreeCtrlItem = makeInsertStruct(TVI_ROOT, 0, titel, TRUE, NULL);
								parent = rootitem = tree.InsertItem(&TreeCtrlItem);
							}
							break;
						}
					}
				}

				CString kurztext, kurztext2;
				kurztext.Format("%s (DN%i", titel, f->get_dn());
				if (f->get_dnvl())
					kurztext2.Format("/DNVL%i)", f->get_dnvl());
				else
					kurztext2 = ")";
				CString bez;
				kurztext += kurztext2;
				bez.Format("%-70s        %-.6i      %.3f   %.2f", kurztext, f->get_artikelnr(), f->get_gewicht(), f->get_sapPreis());
				char tit[255];
				strcpy (tit, bez.Left(254));
				TreeCtrlItem = makeInsertStruct(parent, 0, tit, FALSE, f);
				tree.InsertItem(&TreeCtrlItem);
			}
		}
	}
	tree.SetRedraw(TRUE);
	return rootitem;
}

//Fügt die Tabelle Formstücke und Fittinge in den Materialbaum ein
void MaterialEingabeForm::insertFStuFitt(bool ensureVisible)
{
	TV_INSERTSTRUCT TreeCtrlItem;
	char titel[255];

	CString grp, grplast;
	HTREEITEM parent = TVI_ROOT;
	HTREEITEM rootitem = TVI_ROOT;
	int elemid = 0;
	
	tree.SetRedraw(FALSE);
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
					rootitem = tree.InsertItem(&TreeCtrlItem);
					break;
				}
			}
		}

		if (f->get_gruppe() != grplast)
		{
			strcpy(titel, f->get_gruppe().Left(254));
			TreeCtrlItem = makeInsertStruct(rootitem, 0, titel, TRUE, NULL);
			grplast = f->get_gruppe();
			parent = tree.InsertItem(&TreeCtrlItem);
			if (ensureVisible && parent)
				tree.EnsureVisible(parent);					
		}
	
		CString bez;

		bez.Format("%-50s        %-.6i      %.3f   %.2f", f->get_kurztext(), f->get_artikelnr(), f->get_gewicht(), f->get_sapPreis());
		strcpy(titel, bez.Left(254));
		TreeCtrlItem = makeInsertStruct(parent, 0, titel, FALSE, f);
		tree.InsertItem(&TreeCtrlItem);
	}
	tree.SetRedraw(TRUE);
}

//Liest die Profile aus den Stammdaten und fügt sie in den Baum ein
void MaterialEingabeForm::insertProfile(bool co2tikko, HTREEITEM root)
{
	TV_INSERTSTRUCT TreeCtrlItem;
	char titel[255];

	CString grp, grplast, str;
	HTREEITEM parent = TVI_ROOT;
	HTREEITEM rootitem = TVI_ROOT;

	tree.SetRedraw(FALSE);
	str = getStammdaten().getText("IDS_PROFIL").c_str();
	strcpy(titel, str);

	if (co2tikko)
		parent = rootitem = root;

	TreeCtrlItem = makeInsertStruct(parent, 0, titel, TRUE, NULL);
	parent = rootitem = tree.InsertItem(&TreeCtrlItem);
	
	for (POSITION pos = getStammdaten().get_profile().GetHeadPosition(); pos;)
	{
		Profile::data *f = (Profile::data *)getStammdaten().get_profile().GetNext(pos);

		if (co2tikko && !f->get_co2tikko())
			continue;

		CString kurztext;
		CString bez;
		bez.Format("%-50s        %-.6i      %.3f   %.2f", f->get_kurztext(), f->get_artikelnr(), f->get_sapPreis());
		char tit[255];
		strcpy (tit, bez.Left(254));
		TreeCtrlItem = makeInsertStruct(parent, 0, tit, FALSE, f);
		tree.InsertItem(&TreeCtrlItem);
	}
	tree.SetRedraw(TRUE);
}

///Liest das Sondermaterial den Stammdaten und fügt sie in den Baum ein
void MaterialEingabeForm::insertSondermaterial(bool co2tikko, HTREEITEM root)
{
	TV_INSERTSTRUCT TreeCtrlItem;
	char titel[255];

	CString grp, grplast;
	HTREEITEM parent = TVI_ROOT;
	HTREEITEM rootitem = TVI_ROOT;

	tree.SetRedraw(FALSE);

	CString str; str = getStammdaten().getText("IDS_SONDERMATERIAL").c_str();
	strcpy(titel, str);
	
	if (co2tikko)
		parent = rootitem = root;
	
	TreeCtrlItem = makeInsertStruct(parent, 0, titel, TRUE, NULL);
	parent = rootitem = tree.InsertItem(&TreeCtrlItem);
	
	for (POSITION pos = getStammdaten().get_sondermaterial().GetHeadPosition(); pos;)
	{
		Sondermaterial::data *s = (Sondermaterial::data *)getStammdaten().get_sondermaterial().GetNext(pos);

		if (co2tikko && !s->get_co2tikko())
			continue;

		CString kurztext;
		CString bez;
		bez.Format("%-50s        %-.6i      %.3f   %.2f", s->get_kurztext(), s->get_artikelnr(), s->get_preis(), s->get_gewicht());
		char tit[255];
		strcpy (tit, bez.Left(254));
		TreeCtrlItem = makeInsertStruct(parent, 0, tit, FALSE, s);
		tree.InsertItem(&TreeCtrlItem);
	}
	tree.SetRedraw(TRUE);
}

//Indicates the user clicked a button
void MaterialEingabeForm::onClickedEditlabel()
{
	int sel = list.getSelectedItem();
	list.SetFocus();
	ListView_EditLabel(list.m_hWnd, sel);
}

//Liest die Angaben an der Stelle index aus und erzeugt daraus ein 
//Materialobjekt
Material* MaterialEingabeForm::readMaterial(int index)
{
	Material *material = new Material;

	material->set_posnr(list.GetItemText(index, POSNR));
	material->set_anzahl(atoi(list.GetItemText(index, ANZAHL)));
	material->set_mass1(atoi(list.GetItemText(index, LAENGE)));
	material->set_bezeichnung(list.GetItemText(index, BESCHREIBUNG));
	material->set_artikelnr(atoi(list.GetItemText(index, ARTNR)));
	material->set_gewicht(list.getDouble(index, GEWICHT));
	material->set_sapPreis(list.getDouble(index, PREIS));
	material->set_pulverpreis(list.getDouble(index, PULVERPREIS));
	material->set_einkauf(list.GetItemText(index, EINKAUF).GetLength() ? true : false);
	material->set_halbzeug(list.GetItemText(index, HALBZEUG).GetLength() ? true : false);
	material->set_id(atoi(list.GetItemText(index, ID)));

	CString txt = list.GetItemText(index, PROFILE);

	if (txt.GetLength() && (txt.GetAt(0) == 'F' || txt.GetAt(0) == 'U' || txt.GetAt(0) == 'L'))
	{
		material->set_profilkennung((CString)txt.GetAt(0));
		txt.SetAt(0, ' ');
		material->set_mass2(EditProfile::decodeMass2String(txt));
	}
	return material;
}

//Schreibt die Daten eines Materials an die Stelle index in der Liste.
void MaterialEingabeForm::writeMaterial(int index, Material * mat)
{
	CString txt;
	list.SetItemText(index, POSNR, mat->get_posnr());
	txt.Format("%i", mat->get_anzahl());
	list.SetItemText(index, ANZAHL, txt);
	txt.Format("%i", mat->get_mass1());
	list.SetItemText(index, LAENGE, txt);
	list.SetItemText(index, BESCHREIBUNG, mat->get_bezeichnung());
	txt.Format("%-.6i", mat->get_artikelnr());
	list.SetItemText(index, ARTNR, txt);
	
	list.insertDouble(index, GEWICHT, mat->get_gewicht());
	list.insertDouble(index, PREIS, mat->get_sapPreis());
	list.insertDouble(index, PULVERPREIS, mat->get_pulverpreis());

	list.SetItemText(index, EINKAUF, mat->get_einkauf() ? "X" : "");
	list.SetItemText(index, HALBZEUG, mat->get_halbzeug() ? "X" : "");

	txt = EditProfile::encodeMass2String(mat->get_mass2());
	if (mat->get_profilkennung() == Material::USTAHL)
		txt = "U " + txt;
	else if (mat->get_profilkennung() == Material::LSTAHL)
		txt = "L " + txt;
	else if (mat->get_profilkennung() == Material::FSTAHL)
		txt = "F " + txt;
	else
		txt = "";
	list.SetItemText(index, PROFILE, txt);
	

	calcPreisUndGewicht(index, mat->get_sapPreis(), mat->get_pulverpreis(), mat->get_gewicht(), mat->get_anzahl(), mat->get_mass1());

	txt.Format("%i", mat->get_id());
	list.SetItemText(index, ID, txt);
}

//Schreibt die Daten eines Materials an die Stelle index in der Liste.
Material*  MaterialEingabeForm::conv2Material(Profile::data* dat) const
{
	XASSERT(dat);
	if (!dat)
		return NULL;
	Material *mat = new Material;
	mat->set_id(dat->get_elemid());
	mat->set_profilkennung(dat->get_typ());
	mat->set_artikelnr(dat->get_artikelnr());
	mat->set_bezeichnung(dat->get_kurztext());
	mat->set_gewicht(dat->get_gewicht());
	mat->set_sapPreis(dat->get_sapPreis());
	mat->set_einkauf(dat->get_einkauf());
	mat->set_halbzeug(dat->get_halbzeug());
	mat->set_mass1(1000); // Defaultlänge 1 m
	mat->set_mass2(EditProfile::decodeMass2String("90 1A 90")); // Rechtes, linkes Ende gerade
	return mat;	
}

//Schreibt die Daten eines Materials an die Stelle index in der Liste.
Material* MaterialEingabeForm::conv2Material(Bauelemente::data* dat, const CString &kurztext) const
{
	XASSERT(dat);
	if (!dat)
		return NULL;
	Material *mat = new Material;
	mat->set_id(dat->get_elemid());
	mat->set_artikelnr(dat->get_artikelnr());
	mat->set_bezeichnung(kurztext);
	mat->set_gewicht(dat->get_gewicht());
	mat->set_sapPreis(dat->get_sapPreis());
	mat->set_einkauf(dat->get_einkauf());
	mat->set_halbzeug(dat->get_halbzeug());
	return mat;	
}

//Schreibt die Daten eines Materials an die Stelle index in der Liste.
Material * MaterialEingabeForm::conv2Material(FormstueckeUndFittinge::data* dat) const
{
	XASSERT(dat);
	if (!dat)
		return NULL;
	Material *mat = new Material;
	mat->set_id(dat->get_elemid());
	mat->set_artikelnr(dat->get_artikelnr());
	mat->set_bezeichnung(dat->get_kurztext());
	mat->set_gewicht(dat->get_gewicht());
	mat->set_sapPreis(dat->get_sapPreis());
	mat->set_pulverpreis(dat->get_pulverpreis());
	mat->set_einkauf(dat->get_einkauf());
	mat->set_halbzeug(dat->get_halbzeug());
	return mat;	
}

//Schreibt die Daten eines Materials an die Stelle index in der Liste.
Material * MaterialEingabeForm::conv2Material(Sondermaterial::data* dat) const
{
	XASSERT(dat);
	if (!dat)
		return NULL;
	Material *mat = new Material;
	mat->set_id(dat->get_elemid());
	mat->set_artikelnr(dat->get_artikelnr());
	mat->set_bezeichnung(dat->get_kurztext());
	mat->set_gewicht(dat->get_gewicht());
	mat->set_sapPreis(dat->get_preis());
	mat->set_einkauf(dat->get_einkauf());
	mat->set_halbzeug(dat->get_halbzeug());
	return mat;	
}
