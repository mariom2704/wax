#include "stdafx.h"





#include "..\wax\docver.h"

#include "..\waxui\ApaxEditArbeitsfolge.h"



IMPLEMENT_DYNCREATE(ArbeitEingabeForm, WaxPartForm)

BEGIN_MESSAGE_MAP(ArbeitEingabeForm, WaxPartForm)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RETURN,		IDC_TREE, onReturnTree)
	ON_NOTIFY(NM_DBLCLK,		IDC_TREE, onDblclkTree)
	ON_NOTIFY(NM_DBLCLK,		IDC_LIST, onDblclkList)
	ON_NOTIFY(LVN_COLUMNCLICK,	IDC_LIST, onColumnclickList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, onEndlabeleditList)
	ON_NOTIFY(NM_RCLICK,		IDC_LIST, onRclickList)

	ON_COMMAND(IDC_DELETE,	onClickedDelete)
	ON_COMMAND(IDC_EDIT,	onClickedEdit)
	ON_COMMAND(IDC_INSERT,	onClickedInsert)
	ON_COMMAND(IDC_AUTO,	onClickedAuto)
	ON_COMMAND(IDC_NEW,		onClickedNew)
	ON_COMMAND(IDC_UP,		onClickedUp) 
	ON_COMMAND(IDC_DOWN,	onClickedDown) 
	ON_COMMAND(IDC_APAX, onApax)
	ON_COMMAND(ID_APAX_LOAD, onApaxLoad)
	ON_COMMAND(ID_APAX_SAVE, onApaxSave)
END_MESSAGE_MAP()

//Konstruktor
ArbeitEingabeForm::ArbeitEingabeForm() : WaxPartForm(IDD_ARBEIT, getStammdaten().getText("IDD_ARBEIT").c_str()), istInitialisiert(false), m_bClearArbeitsfolgen(true)
{
	/**
	Konstruktor
	Resource id ist IDD_ARBEIT
	istInitialisiert wird auf false gesetzt. Erst beim ersten Anzeigen wird dann einmal der Baum mit den Stammdaten aufgebaut
	*/
}

//Destruktor
ArbeitEingabeForm::~ArbeitEingabeForm()
{
}

//Vergleichsfunktion für die Sortierung der Liste nach 
//Positionsnummern
int CALLBACK ArbeitEingabeForm::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	/**
	Vergleichsfunktion für die Sortierung in der Liste
	Es wird nach der ersten Spalte sortiert, d.h. nach Positionsnummern
	*/
	Arbeitsfolge *af1 = (Arbeitsfolge *)lParam1;
	Arbeitsfolge *af2 = (Arbeitsfolge *)lParam2;


	switch (lParamSort)
	{
		case POSNR:
			if (af1->get_nr() > af2->get_nr())
				return 1;
			else if (af2->get_nr() > af1->get_nr())
				return -1;
			break;
		case ANZ:
			if (af1->get_anz() > af2->get_anz())
				return 1;
			else if (af2->get_nr() > af1->get_nr())
				return -1;
			
	}
	
	//Daten sind gleich
	return 0;
}

//Wird aufgerufen, nachdem das Fenster in der Grösse geändert wurde
void ArbeitEingabeForm::OnSize(UINT nType, int cx, int cy)
{
	/**
	Fenstergrösse wurde geändert
	Der Baum und die Liste werden in ihren Abmessungen dem Fenster angepasst. Die Breitenverteilung ist festgelegt. 
	(Baum ein Viertel, Rest für die Liste). Die Buttons werden immer rechts vom Baum platziert.
	*/
	WaxPartForm::OnSize(nType, cx, cy);

	const int AbstandUnten = 40;
	const int AbstandLinks = 10;
	const int AbstandOben = 30;
	const int Verteilung = 4; // Baum bekommt ein Viertel, Liste den Rest
	const int ButtonAbstand = 5;

	if (::IsWindow(tree.m_hWnd))
		tree.SetWindowPos( NULL, AbstandLinks, 0, cx/Verteilung, cy-AbstandUnten+AbstandOben, SWP_SHOWWINDOW | SWP_NOZORDER);
	else
		return; 
	if (::IsWindow(list.m_hWnd))
		list.SetWindowPos( NULL, cx/Verteilung+AbstandLinks+1, AbstandOben, cx - cx/Verteilung - 2*AbstandLinks, cy-AbstandUnten, SWP_SHOWWINDOW | SWP_NOZORDER);
	else
		return;

	// Buttons IDC_INSERT etc. muessen mitskaliert werden: Der Abstand zum Baum ist immer fest

	CWnd *butt[] = 
	{
		GetDlgItem(IDC_DELETE),
		GetDlgItem(IDC_EDIT),
		GetDlgItem(IDC_INSERT),
		GetDlgItem(IDC_NEW),
		GetDlgItem(IDC_AUTO),
		GetDlgItem(IDC_APAX),
		NULL
	};
	
	WINDOWPLACEMENT wndpl; 
	butt[0]->GetWindowPlacement(&wndpl); 
	int xcx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;

	for (int i = 0; butt[i]; i++)
	{
		XASSERT(butt[i]);
		if (butt[i])
		{
			butt[i]->SetWindowPos( NULL, AbstandLinks + cx/Verteilung + (i+1)*ButtonAbstand + i*xcx, wndpl.rcNormalPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
		}
	}
}

//Initialisierung der Materialansicht (Spalten und Bezeichnungen)
void ArbeitEingabeForm::initList()
{
	/**
	Initialisierung der Materialansicht (Spalten und Bezeichnungen)
	*/

	CString str;
	ListView_SetExtendedListViewStyle(list.m_hWnd, LVS_EX_FULLROWSELECT);
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	list.InsertColumn(POSNR,	str,	LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_ANZAHL").c_str();
	list.InsertColumn(ANZ,		str,	LVCFMT_RIGHT, 50);
	str = getStammdaten().getText("IDS_TEXT").c_str();
	list.InsertColumn(TEXT,		str,	LVCFMT_LEFT, 250);
	str = getStammdaten().getText("IDS_ARBEITSPLATZ").c_str();
	list.InsertColumn(ARBPL,	str,	LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_PREISPROSTUNDE").c_str();
	list.InsertColumn(PREIS,	str,	LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_ZEIT").c_str();
	list.InsertColumn(ZEIT,		str,	LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_RUESTZEIT").c_str();
	list.InsertColumn(RUESTZEIT,str,	LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_CODE").c_str();
	list.InsertColumn(CODE,		str,	LVCFMT_RIGHT, 50);

	del_list();
	write_list();
}

//Dialog initialisieren
BOOL ArbeitEingabeForm::OnInitDialog()
{
	/**
	Dialoginitialisierung
	Hier werden mit initList die Spalten in der Liste erzeugt.
	Der Aufbau der Baumstruktur wird aus Performancegründen erst beim ersten Anzeigen vorgenommen in OnShowWindow
	Der Apaxbutton ist nur in der AV Version verfügbar
	*/
	if (!WaxPartForm::OnInitDialog())
		return FALSE;
	
	initList();
		
	GetDlgItem(IDC_APAX)->ShowWindow(WaxSys::isAV() ? SW_SHOW : SW_HIDE);

	setWindowsText(IDC_DELETE, "IDS_DELETE");
	setWindowsText(IDC_EDIT, "IDS_EDIT");
	setWindowsText(IDC_INSERT, "IDS_INSERT");
	setWindowsText(IDC_NEW, "IDS_NEW");
	setWindowsText(IDC_AUTO, "IDC_AUTO");
	setWindowsText(IDC_APAX, "IDC_APAX");


	
	return TRUE;
}

//Dialogdatenaustausch
void ArbeitEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	/**
	Achtung: Einträge dürfen nur mit writeArbeitsfolge und readArbeitsfolge hinzugefügt/ausgelesen werden, 
	da bei jedem Eintrag ein Pointer auf ein Afo Objekt mitverwaltet werden muss
	*/
	WaxPartForm::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE, tree);
	DDX_Control(pDX, IDC_LIST, list);
	
	if (!validWerkauftrag())
		return;

	if (pDX->m_bSaveAndValidate)// Speichern der Liste 
	{
		saveAfos(get_data()->get_werkauftrag()->get_arbeit());
	}

	else
	{
		// MFC Bug: OnInitDialog wird nach dem ersten DoDataExchange aufgerufen, so dass 
		// Dialogelemente, die noch initialisiert werden müssen, beim ersten DoDataExchange nicht
		// benutzt werden dürfen
		if (list.getColumnCount() > 1)
		{
			del_list();
			write_list();
		}
	}

}

void ArbeitEingabeForm::saveAfos(Arbeitsplan& a)
{
	/** 
	Liste mit Afos auslesen und im Arbeitsplan speichern
	*/
	for (POSITION pos = a.get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *af = dynamic_cast<Arbeitsfolge *>(a.get_arbeitsfolgen().GetNext(pos));
		XASSERT(af);
		if (af)	delete af;
	}
	a.get_arbeitsfolgen().RemoveAll();
	for (int i = 0; i < list.GetItemCount(); i++)
	{
		Arbeitsfolge *afo = new Arbeitsfolge;
		if (readArbeitsfolge(i, *afo))
			a.get_arbeitsfolgen().AddTail(afo);
	}
	a.sortAfos();
}

//Indicates the user clicked a button
void ArbeitEingabeForm::onClickedAuto()
{
	/**
	Wenn am Verteiler auch nur die Anzahl geaendert wird, 
	muss der Arbeitsplan neu generiert werden.
	Anzahl der Rohre ist in der Afo bei Anzahl und Zeit mit drin. 
	*/
	if (!validWerkauftrag())
		return;
	CString str; str = getStammdaten().getText("IDS_HINWEIS_AFO").c_str();
	
	if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
	{
		get_data()->get_werkauftrag()->get_arbeit().clean();
		m_bClearArbeitsfolgen = false;
		get_data()->createAutoArbeit();
		UpdateData(false);
	}
}

//Indicates the user clicked a button
void ArbeitEingabeForm::onClickedNew()
{
	// Alle Selektionen zurücksetzen
	int insertafter = -1;
	POSITION pos = list.GetFirstSelectedItemPosition();
	while (pos)
	{
		insertafter = list.GetNextSelectedItem(pos);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, (UINT)~LVIS_SELECTED, LVIS_SELECTED, NULL);
	}

	// Neue Positionsnummer berechnen, nach dem letzten selektierten Item
	int newposnr = atoi(list.GetItemText(insertafter, 0)) + 1;
	Arbeitsfolge afo;
	afo.set_nr(newposnr);
	CString str; str = getStammdaten().getText("IDS_NEW_AFO").c_str();
	afo.set_txt(str);

	// Item einfuegen
	int index = list.InsertItem(insertafter+1, afo.get_txt());
	list.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);
	writeArbeitsfolge(afo, index);
	
	// Eintrag editieren
	if (!editAfo())
	{
		del_listentry(insertafter+1);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, LVIS_SELECTED, LVIS_SELECTED, NULL);
	}
}

//Zeigt an, dass der Button Edit geklickt wurde
void ArbeitEingabeForm::onClickedEdit()
{
	editAfo();
}

//Zeigt an, dass der Button Delete geklickt wurde
void ArbeitEingabeForm::onClickedDelete()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	int index = 0;
	while (pos)
	{
		index = list.GetNextSelectedItem(pos);
		if (index >= 0)
			del_listentry(index);
		 pos = list.GetFirstSelectedItemPosition();
	}

	// Index darf nicht unter Null sein oder über max. Itemindex
	if (index == 0)
		index = 1;
	else if (index > list.GetItemCount())
		index = list.GetItemCount();
	index--;
	list.selectItem(index);
}

//Zeigt an, dass der Button Insert geklickt wurde
void ArbeitEingabeForm::onClickedInsert()
{
	/*
	Button Einfügen wurde gedrückt
	Fügt die ausgewählte Arbeitsfolge durch <mf .insertAFO> ein
	*/
	insertAFO();
}

//Zeigt an, dass im Baum doppelgeklickt wurde
void ArbeitEingabeForm::onDblclkTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	/*
	Arbeitsfolge im Baum wurde doppeltgeklickt
	Fügt die ausgewählte Arbeitsfolge durch <mf .insertAFO> ein
	*/
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	insertAFO();
}

//Zeigt an, dass der Benutzer die Editierung des Itemlabels beendet hat
void ArbeitEingabeForm::onEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult)
{
	/*
	Positionsnummer wurde bearbeitet
	setzt pResult auf 1 damit die Änderungen übernommen werden
	*/
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;
}

//Zeigt an, dass im Baum Alt+Return gedrückt wurde
void ArbeitEingabeForm::onReturnTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	insertAFO();
	*pResult = 1;
}


//Called when CWnd is to be hidden or shown.
void ArbeitEingabeForm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	/*
	Fenster wird angezeigt
	aus Performancegründen wird erst beim ersten Anzeigen des Fensters die Liste aufgebaut.
	@xref <mf .insertGewinderohrAfos> <mf .insertApAfos>
	*/
	WaxPartForm::OnShowWindow(bShow, nStatus);

	if (!istInitialisiert)
	{
		treeInsertApAfos();
	}
	istInitialisiert = true;
}

//Arbeitsfolge in Arbeitsplan einfügen
void ArbeitEingabeForm::insertAFO()
{
	/*
	Einfügen einer einzelnen Arbeitsfolge aus dem Baum in die Liste mit Arbeitsfolgen
	Bei Doppelklick (oder 'Einfügen' Button) wird der ausgewählte Eintrag aus dem Baum in die Liste übertragen
	*/
	Arbeitsfolgen::data *a = NULL;
	int insertafter = -1;

	// Alle Selektionen zurücksetzen
	POSITION pos = list.GetFirstSelectedItemPosition();
	while (pos)
	{
		insertafter = list.GetNextSelectedItem(pos);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, (UINT)~LVIS_SELECTED, LVIS_SELECTED, NULL);
	}

	// Neue Positionsnummer berechnen, nach dem letzten selektierten Item
	int newindex = atoi(list.GetItemText(insertafter, 0)) + 5;
	
	CString kurztext;

	// Item aus dem Baum auslesen
	HTREEITEM item = tree.GetSelectedItem();

	if (item)
	{
		HTREEITEM parent = tree.GetParentItem(item);
		if (!parent)
			return;
		a = dynamic_cast<Arbeitsfolgen::data *>((CObject*)tree.GetItemData(parent));
	}
	else
		return;

	int index = insertafter;
	a = dynamic_cast<Arbeitsfolgen::data *>((CObject*)tree.GetItemData(item));
	if (a)
	{
		CString posnr;
		posnr.Format("%i", newindex);
		index = list.InsertItem(insertafter+1, posnr );

		Arbeitsfolge afo;
		afo.set_nr(newindex);
		afo.set_txt(a->get_txt());
		afo.set_arbpl(a->get_arbeitsplatz());
		afo.set_ppstd(afo.getTarif(afo.getKostst(a->get_arbeitsplatz())));
		afo.set_zeit(0);
		afo.set_id(a->get_code());
		writeArbeitsfolge(afo, index);
	}
	else
		return;
	list.selectItem(index);
	// Eintrag editieren
	if (!editAfo())
	{ // Editieren abgebrochen, Afo löschen und letzten Eintrag markieren
		del_listentry(index);
		list.selectItem(insertafter < 0 ? 0 : insertafter);
	}
	list.SetFocus();
}

//Initialisierung des Treecontrols mit den Arbeitsfolgen der Tabelle 
//Arbeitsfolgen aus den Stammdaten
void ArbeitEingabeForm::treeInsertApAfos()
{
	/*
	Einfügen der AP Arbeitsfolgen in den Baum
	Jede Arbeitsfolge ist einer oder mehreren Sparten zugehörig
	@todo Arbeitsfolgen 'Allgemein' sind nicht definiert.
	*/
	tree.SetRedraw(FALSE);
	// Titelgliederung einfügen: Gewinderohrfertigung
	TV_INSERTSTRUCT TreeCtrlItembase;
	
	const int SECTION_COUNT = 11;
	CString str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11;
	str1 = getStammdaten().getText("IDS_ALLGEMEIN").c_str();
	str2 = getStammdaten().getText("IDS_WASSER").c_str();
	str3 = getStammdaten().getText("IDS_CO2").c_str();
	str4 = getStammdaten().getText("IDS_CO2_TIKKO").c_str();
	str5 = getStammdaten().getText("IDS_ARGON").c_str();
	str6 = getStammdaten().getText("IDS_HALTER").c_str();
	str7 = getStammdaten().getText("IDS_CO2HD_VERT65").c_str();
	str8 = getStammdaten().getText("IDS_CO2HD_VERT80").c_str();
	str9 = getStammdaten().getText("IDS_GEWINDEROHR").c_str();
	str10 = getStammdaten().getText("IDS_GEWINDEROHR_HD").c_str();
	str11 = getStammdaten().getText("IDS_NICHT_ZUGEORDNET").c_str();
	CString titel[SECTION_COUNT] = { str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11 };
	HTREEITEM htit[SECTION_COUNT]; // Für jede Überschrift ein Handle für Rootelemente
	memset(htit, 0, sizeof(htit));

	// Erstmal werden die Rootelemente mit den Titeln eingefügt
	for (int i = 0; i < SECTION_COUNT-1; i++)
	{
		TreeCtrlItembase = makeInsertStruct(TVI_ROOT, 0, titel[i], TRUE, NULL);
		htit[i] = tree.InsertItem(&TreeCtrlItembase);
		XASSERT(htit[i]);
		if (!htit[i]) return;
	}

	for (POSITION pos = getStammdaten().get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolgen::data *f = (Arbeitsfolgen::data *)getStammdaten().get_arbeitsfolgen().GetNext(pos);

		CString kurztext;
	
		HTREEITEM it[SECTION_COUNT];
		memset(it, 0, sizeof(it));
	
		// Flags werden abgefragt, jede Arbeitsfolge kann überall auftauchen, Reihenfolge muss mit der in htit übereinstimmen
		if (f->get_allgemein())				it[0] = htit[0];
		if (f->get_wasser())				it[1] = htit[1];
		if (f->get_co2())					it[2] = htit[2];
		if (f->get_co2tikko())				it[3] = htit[3];
		if (f->get_argon())					it[4] = htit[4];
		if (f->get_halter())				it[5] = htit[5];
		if (f->get_co2hdverteiler_dn65())	it[6] = htit[6];
		if (f->get_co2hdverteiler_dn80())	it[7] = htit[7];
		if (f->get_gewinderohr())			it[8] = htit[8];
		if (f->get_gewinderohrhd())			it[9] = htit[9];

		bool zugeordnet = false;
		int i;
		for (i = 0; i < SECTION_COUNT; i++)
		{
			if (it[i])
			{
				zugeordnet = true;
				break;
			}
		}
		if (!zugeordnet)
		{
			const int index = SECTION_COUNT-1;
			if (!htit[index])
			{
				TreeCtrlItembase = makeInsertStruct(TVI_ROOT, 0, titel[index], TRUE, NULL);
				htit[index] = tree.InsertItem(&TreeCtrlItembase);
			}
			it[index] = htit[SECTION_COUNT-1];
		}

		for (i = 0; i < SECTION_COUNT; i++)
		{ // Item unter allen gewünschten Titeln einfügen
			if (it[i])
			{
				TV_INSERTSTRUCT TreeCtrlItem;
				kurztext.Format("%s (af%i)", f->get_txt(), f->get_code());
				TreeCtrlItem = makeInsertStruct(it[i], 0, kurztext, FALSE, f);
				tree.InsertItem(&TreeCtrlItem);
			}		
		}
	}
	tree.SetRedraw();
}

//Indicates that one of the columns was clicked
void ArbeitEingabeForm::onColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	/*
	Sortierung nach Positionsnummern
	Jeder Eintrag erhält als ItemData die umgewandelte Positionsnummer. 
	*/
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	list.SortItems(CompareFunc, pNMListView->iSubItem);
	*pResult = 0;
}


void ArbeitEingabeForm::updateSpecialAF(Arbeitsfolge* afo)
{
	get_data()->get_werkauftrag()->get_arbeit().updateSpecialAF(afo, 
																get_data()->get_werkauftrag()->get_beschichtung(),
																get_data()->get_werkauftrag()->getGesGewicht(),
																get_data()->get_werkauftrag()->getMaxLaenge(),
																get_data()->get_werkauftrag()->get_status().get_wanr());
}

//Afo lesen
bool  ArbeitEingabeForm::readArbeitsfolge(int index, Arbeitsfolge &afo)
{
	DWORD itemdata = list.GetItemData(index);
	if (!itemdata)
		return false;
	
	Arbeitsfolge *listafo = dynamic_cast<Arbeitsfolge*>((CObject *)itemdata);
	XASSERT(listafo);
	if (!listafo)
		return false;

	afo.copy(*listafo);

	CString tmp = list.GetItemText(index, CODE);
	int id = atoi(tmp.Right(tmp.GetLength()-2));	
	afo.init(id); 

	tmp = list.GetItemText(index, POSNR);
	afo.set_nr(atoi(tmp));
	
	afo.set_anz(list.getDouble(index, ANZ));

	tmp = list.GetItemText(index, TEXT);
	afo.set_txt(tmp);
	
	tmp = list.GetItemText(index, ARBPL);
	afo.set_arbpl(tmp);

	Werkauftrag wa;
	
	afo.set_kostst(afo.getKostst(afo.get_arbpl()));
	afo.set_ppstd(afo.getTarif(afo.get_kostst()));

	afo.set_zeit(list.getDouble(index, ZEIT));
	afo.set_ruestzeit(list.getDouble(index, RUESTZEIT));
	return true;
}


//Afo schreiben
void ArbeitEingabeForm::writeArbeitsfolge(const Arbeitsfolge& af, int index)
{
	Arbeitsfolge afcp;
	afcp.copy(af);

	
	updateSpecialAF(&afcp);
	

	CString txt;
	txt.Format("%i", afcp.get_nr());
	list.SetItemText(index, POSNR, txt);

	list.insertDouble(index, ANZ, afcp.get_anz());
	list.SetItemText(index, TEXT, afcp.get_txt());
	
	txt = afcp.get_arbpl();

	list.SetItemText(index, ARBPL, txt);
	
	Werkauftrag wa;
	Arbeitsfolge afo;
	afo.set_id(afcp.get_id());

	list.insertDouble(index, PREIS, afcp.getTarif(afcp.getKostst(afcp.get_arbpl()))); //Vollkostentarif
	list.insertDouble(index, ZEIT, afcp.get_zeit());
	list.insertDouble(index, RUESTZEIT, afcp.get_ruestzeit());
	
	txt.Format("af%i", afcp.get_id());
	list.SetItemText(index, CODE, txt);


	delete dynamic_cast<Arbeitsfolge *>((CObject*)list.GetItemData(index));

	Arbeitsfolge *afonew = new Arbeitsfolge(afcp);
	list.SetItemData(index, (DWORD)afonew);
}

//Indicates that the user has double-clicked the left mouse button in the 
//control
void ArbeitEingabeForm::onDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	editAfo();	 
}

//Indicates that the user has clicked the right mouse button in the control
void ArbeitEingabeForm::onRclickList(NMHDR* pNMHDR, LRESULT* pResult)
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

	//int  menu_items[]   = { IDC_UP, IDC_DOWN, IDC_DELETE, IDC_EDIT, -1 };

	menu_items items[] = 
	{
		ADD_MENU_ITEM(IDC_UP)
		ADD_MENU_ITEM(IDC_DOWN)
		ADD_MENU_ITEM(IDC_DELETE)
		ADD_MENU_ITEM(IDC_EDIT)
		{-1, ""},
	};

	createPopupMenu(point, items);
	*pResult = 1;
}


void ArbeitEingabeForm::onClickedUp()
{
	list.moveSelectedItem();
}

void ArbeitEingabeForm::onClickedDown()
{
	list.moveSelectedItem(false);
}

BOOL ArbeitEingabeForm::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE)
		{
			onClickedDelete();
			return TRUE;
		}
	}
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		switch(pMsg->wParam)
		{
			case VK_UP:
			case VK_DOWN:
				list.moveSelectedItem(pMsg->wParam == VK_UP ? true : false);
				return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}



//Zeigt das Kontextmenü zur Auswahl der APAX Funktionen: Laden, Speichern, 
//Laden von Altwax Arbeitsplänen
void ArbeitEingabeForm::onApax()
{
	CMenu pop;
	pop.LoadMenu(IDR_APAX);
	CMenu *popup = pop.GetSubMenu(0);

	CRect r;
	GetDlgItem(IDC_APAX)->GetWindowRect(&r);
	CPoint pt(r.right, r.top);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this); 
	pop.DestroyMenu();	
}



//Laden eines Arbeitsplans
void ArbeitEingabeForm::onApaxLoad()
{
	CString file = getFileName(true);
	if (!file.GetLength())
		return;
	
	CString str; str = getStammdaten().getText("IDS_FRAGE_AFO").c_str();
	if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	if (loadApax(file))
		UpdateData(false);
}

//Speichern eines Arbeitsplans
void ArbeitEingabeForm::onApaxSave()
{
	CString file = getFileName(false);
	if (!file.GetLength())
		return;
	
	UpdateData(true);
	if (storeApax(file))
	{
		CString str; str = getStammdaten().getText("IDS_DATEI_GESPEICHERT").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
	}
}




//Zeigt den Datei laden/speichern Dialog an und liefert den ausgewählten 
//Dateinamen zurück.
CString ArbeitEingabeForm::getFileName(bool load, bool oldwax)
{
	char fname[1024];
	memset(fname, 0, 1024);

	CString initpath =WaxSys::para_str(oldwax ? DIR_WAXAPAX_OLD : DIR_WAXAPAX);
	if (!initpath.GetLength())
		initpath = ".";
	CString deffilter;
	XASSERT((oldwax && load) || !oldwax);
	CString fileext;
	if (oldwax)
	{
		CString str; str = getStammdaten().getText("IDS_STANDARDARBEITSPLAENE").c_str();
		deffilter.Format("%s (*.sap)|*.sap|Arbeitspläne (*.ap)|*.ap||", str);
		fileext = "apax";
	}
	else
	{
		CString str; str = getStammdaten().getText("IDS_ARBEITSPLAENE").c_str();
		deffilter.Format("%s (*.apax)|*.apax||", str);
		fileext = "apax";
	}

	CString title, abp, lad, sav; 
	abp = getStammdaten().getText("IDS_ARBEITSPLAN_LADEN_SPEICHERN").c_str();
	lad = getStammdaten().getText("IDS_LADEN").c_str();
	sav = getStammdaten().getText("IDS_SPEICHERN").c_str();
	title.Format(abp, load ? lad : sav);
	
	CFileDialog f(load, fileext, get_data()->get_werkauftrag()->get_bemerkung(), OFN_EXPLORER| OFN_NOCHANGEDIR , deffilter, NULL);
	f.m_ofn.lpstrInitialDir = initpath;
	f.m_ofn.lpstrTitle = (LPCSTR)title;
	f.m_ofn.lpstrFile = fname;
	f.m_ofn.nMaxFile = 1024;
	f.m_ofn.lpfnHook=NULL;
	f.m_ofn.hwndOwner = this->m_hWnd;
	f.m_ofn.Flags &= ~OFN_ENABLEHOOK | OFN_SHAREAWARE;

	CString ret;
	if (load)
	{
		if (GetOpenFileName(&f.m_ofn))
			ret = f.m_ofn.lpstrFile;
	}
	else
	{
		if (GetSaveFileName(&f.m_ofn))
			ret = f.m_ofn.lpstrFile;
	}

	Sleep(100);// Workaround für 'Öffnen schlägt fehl wg. Zugriffverletzung'
	return ret;
}



bool ArbeitEingabeForm::editAfo()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (!pos)
		return false;
	int index = list.GetNextSelectedItem(pos);
	Arbeitsfolge af;
	readArbeitsfolge(index, af);

	EditArbeitsfolge e(this, &af);
	//ApaxEditArbeitsfolge e(af, this);
	if (e.DoModal() == IDOK)
	{
		writeArbeitsfolge(af, index);
		list.SetFocus();
		return true;
	}
	return false;
}



//Löscht einen Eintrag in der Liste inklusive Arbeitsfolge, die dem Eintrag 
//über SetItemData() zugeordnet wurde.
bool ArbeitEingabeForm::del_listentry(int index)
{
	Arbeitsfolge *af = dynamic_cast<Arbeitsfolge *>((CObject*)list.GetItemData(index));
	if (!af)
		return false;
	if (af)
		delete af;
	list.DeleteItem(index);
	return true;
}

void ArbeitEingabeForm::del_list()
{
	int count = list.GetItemCount();
	if (!count)
		return;
	
	for (int index = count-1; index >= 0; index--)
	{
		Arbeitsfolge *af = dynamic_cast<Arbeitsfolge *>((CObject*)list.GetItemData(index));
		if (af)
			delete af;
	}
	list.DeleteAllItems();
}


void ArbeitEingabeForm::OnDestroy( )
{
	del_list();
}

void ArbeitEingabeForm::write_list( )
{
	//Liste erstmalig mit Afos fuellen 
	Werkauftrag *w = get_data()->get_werkauftrag();
	
	list.SetRedraw(FALSE);
	writeAfos(w->get_arbeit());
	list.SetRedraw();
	list.selectItem();
}

void ArbeitEingabeForm::writeAfos(Arbeitsplan& a)
{
	for (POSITION pos = a.get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *af = (Arbeitsfolge *)a.get_arbeitsfolgen().GetNext(pos);
		int index = list.InsertItem(list.GetItemCount(), "-");
		writeArbeitsfolge(*af, index);
		list.SetFocus();
	}
}



//Speichert den aktuellen Inhalt der Arbeitsfolgen, Materialliste und Sokos 
//in eine Datei
bool ArbeitEingabeForm::storeApax(const CString & file)
{
	TRY
	{
		CFile f(file, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive);
		
		BinaryArchive ar(&f, BinaryArchive::store);
		CString magic("WAX+APAX");
		ar << magic;
		ar << (CString)MAGIC; // aus docver.h, mit der Kennung der Version

		get_data()->get_werkauftrag()->get_arbeit().serialize(ar);
		get_data()->get_werkauftrag()->get_material().serialize(ar);
		get_data()->get_werkauftrag()->get_preis().serialize_soko(ar);
		ar.close();
		f.Close();
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH
	return true;
}



//Lädt einen Wax+ Arbeitsplan
bool ArbeitEingabeForm::loadApax(const CString& file)
{
	CFile f(file, CFile::modeRead | CFile::shareExclusive);
	BinaryArchive ar(&f, BinaryArchive::load);
	
	CString magic;
	ar >> magic;

	if (magic.Left(8) != "WAX+APAX")
	{
		CString str; str = getStammdaten().getText("IDS_KEIN_AP").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
		return false;
	}
	ar >> magic; // Magic String aus waxdoc.h mit Versionskennung

	CString version = magic.Mid(13, 3);
	ar.set_version((int)(atof(version)*10));
	
	get_data()->get_werkauftrag()->get_arbeit().serialize(ar);
	// Alle Sonderarbeitsfolgen auf Standard setzen
	get_data()->get_werkauftrag()->get_arbeit().updateSpecialAF(NULL, get_data()->get_werkauftrag()->get_beschichtung(), 0, 0, 0);

	get_data()->get_werkauftrag()->get_material().serialize(ar);
	get_data()->get_werkauftrag()->get_preis().serialize_soko(ar);
	ar.close();
	f.Close();
	return true;
}
