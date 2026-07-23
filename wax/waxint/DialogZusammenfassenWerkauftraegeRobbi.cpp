#include "stdafx.h"


#include "DialogZusammenfassenWerkauftraegeRobbi.h"
#include "..\waxprj\BefehlShowError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(DialogZusammenfassenWerkauftraegeRobbi, CDialog)

DialogZusammenfassenWerkauftraegeRobbi::DialogZusammenfassenWerkauftraegeRobbi(CWnd* pParent /*=NULL*/)
	: CDialog(DialogZusammenfassenWerkauftraegeRobbi::IDD, pParent)
	, m_waNr(0)
{
	m_gpUebergabeStrasse = NULL;
}

DialogZusammenfassenWerkauftraegeRobbi::~DialogZusammenfassenWerkauftraegeRobbi()
{
}

void DialogZusammenfassenWerkauftraegeRobbi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WANR, m_waNr);
	DDX_Control(pDX, IDC_LIST1, m_ctrlWerkauftraege);
	DDX_Control(pDX, IDC_EDIT_WANR, m_ctrlEdit);
}


BEGIN_MESSAGE_MAP(DialogZusammenfassenWerkauftraegeRobbi, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD,OnClickedAddWaNr)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnNMRclickList1)
	ON_COMMAND(IDS_ELEM_DELETE,	OnDelete)
	ON_COMMAND(IDS_ELEM_PASTE,	OnPaste)
	ON_MESSAGE(WM_ENTER_PRESSED, OnKeyDownWaEingabe)
END_MESSAGE_MAP()



BOOL DialogZusammenfassenWerkauftraegeRobbi::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  hier zusätzliche Initialisierung hinzufügen.


	CString str;
	ListView_SetExtendedListViewStyle(m_ctrlWerkauftraege.m_hWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	str = getStammdaten().getText("IDS_WANR").c_str();
	m_ctrlWerkauftraege.InsertColumn(WANR,str,LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_WA_BEZ").c_str();
	m_ctrlWerkauftraege.InsertColumn(BEZ,str,LVCFMT_RIGHT, 150);
	str = getStammdaten().getText("IDS_WA_ANZMUFFEN").c_str();
	m_ctrlWerkauftraege.InsertColumn(ANZMUFFEN,str,LVCFMT_RIGHT, 80);
	str = getStammdaten().getText("IDS_DNROHR").c_str();
	m_ctrlWerkauftraege.InsertColumn(DNROHR,str,LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_ROHRQUALI").c_str();
	m_ctrlWerkauftraege.InsertColumn(ROHRQUALI,str,LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_MUFFENLAENGE").c_str();
	m_ctrlWerkauftraege.InsertColumn(MUFFENLAENGE,str,LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_DNMUFFE").c_str();
	m_ctrlWerkauftraege.InsertColumn(DNMUFFE,str,LVCFMT_RIGHT, 60);


	return TRUE;  // return TRUE unless you set the focus to a control
	// AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}



void DialogZusammenfassenWerkauftraegeRobbi::set_gpUebergabeStrStrasse(GpUebergabeStrangrohrstrasse *gpWa)
{
	m_gpUebergabeStrasse = gpWa;
}





///Listcontrol mit Werkaufträgen füllen
void DialogZusammenfassenWerkauftraegeRobbi::fillListe()
{
	CString str;

	m_ctrlWerkauftraege.DeleteAllItems();

	int waNr = 0;

	MxIterator iter(*m_gpUebergabeStrasse->get_werkauftraege());
	int counter = 0;
	while (iter.next())
	{
		Strangrohr *wa =  dynamic_cast<Strangrohr*>(m_gpUebergabeStrasse->get_werkauftraege()->getAt(iter));
		if (!wa)
			continue;

		//Auftragswerte zur WA NR nur einmal in die Liste schreiben
		if (waNr != wa->get_status().get_wanr())
		{
			waNr = wa->get_status().get_wanr();
			str.Format("%d",waNr);
			int index = m_ctrlWerkauftraege.addItem(counter, WANR, str);
			counter++;
			
			//Bezeichnung
			m_ctrlWerkauftraege.SetItemText(index, BEZ, wa->get_leitkarte().get_prj_auftragsbez());

			//Anz Position
			str.Format("%d",m_gpUebergabeStrasse->get_werkauftraege()->getAnzahlMuffenVonWaNr(waNr));
			m_ctrlWerkauftraege.SetItemText(index, ANZMUFFEN, str);

			//DN Rohr
			str.Format("%d",wa->get_dnvl());
			m_ctrlWerkauftraege.SetItemText(index, DNROHR, str);

			//Rohrquali
			m_ctrlWerkauftraege.SetItemText(index, ROHRQUALI, wa->getRohrQualikurztext());

			//Muffenlaenge
			str.Format("%d",wa->getMuffenLaenge());
			m_ctrlWerkauftraege.SetItemText(index, MUFFENLAENGE, str);

			//DN Muffe
			if (wa->get_muffenID() == BOHR24)
                str.Format("%.2f",wa->get_durchmesser());
			else
				str.Format("%d",wa->get_muffenDN());
			m_ctrlWerkauftraege.SetItemText(index, DNMUFFE, str);
		}
	}
}

/**
Werkauftrag an interne Liste hängen. Dieser wird nur angehängt, wenn er noch nicht in der Liste steht.
*/
bool DialogZusammenfassenWerkauftraegeRobbi::addWerkauftrag(const CUIntArray &arNewWa)
{
	//Errorliste zurücksetzen
	m_gpUebergabeStrasse->clearErrorList();


	//Zu den WA Nummern die Werkaufträge ermitteln
	m_gpUebergabeStrasse->appendWerkauftraege(arNewWa);


	//Wenn Fehler aufgetreten sind, diese anzeigen	
	BefehlShowError error;
	error.appendError(m_gpUebergabeStrasse->getErrorListe());
	error.start();

	return true;
}

/**
Werkauftrag aus Liste löschen und Array löschen
*/
void DialogZusammenfassenWerkauftraegeRobbi::OnDelete()
{
	
	CStringArray arDelString;

	m_ctrlWerkauftraege.getSelectedItems(&arDelString,0);

	//Selektierte Items aus der Liste löschen
	for (int i=0; i<arDelString.GetSize(); i++)
	{
		MxIterator iter(*m_gpUebergabeStrasse->get_werkauftraege()),
				   iterMerken;
		while (iter.next())
		{
			Werkauftrag *wa =  dynamic_cast<Werkauftrag*>(m_gpUebergabeStrasse->get_werkauftraege()->getAt(iter));
			if (!wa)
				continue;

			long waNr = wa->get_status().get_wanr();

			if (waNr == atol(arDelString[i]))
			{
				//Iterrator speichern und ein Element zurücksetzen, sonst würde der Iterator auf einem nicht definierten Element stehen
				iterMerken = iter;
				iter.back();
				m_gpUebergabeStrasse->get_werkauftraege()->deleteAt(iterMerken,true);
			}
		}
	}

	//Liste im Control neu schreiben
	fillListe();

	checkPlausis();
}


/**
Eingaben überprüfen
*/
bool DialogZusammenfassenWerkauftraegeRobbi::checkPlausis()
{
	if (!m_gpUebergabeStrasse)
		return false;
	
	m_gpUebergabeStrasse->clearErrorList();

	if (!m_gpUebergabeStrasse->checkPlausis())
	{
		BefehlShowError error;
		error.appendError(m_gpUebergabeStrasse->getErrorListe());

		error.start();

		return false;
	}

	return true;
}

///Button OK wurde gedrückt
void DialogZusammenfassenWerkauftraegeRobbi::OnBnClickedOk()
{
	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.

	if (!checkPlausis())
		return;

	OnOK();
}

///rechte Maustaste wurde gedrückt
void DialogZusammenfassenWerkauftraegeRobbi::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{

	// Pointer auf NMHDR ist generisch: Er muss auf die richtige Struktur
	// gecastet werden. 
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Der Punkt im Clientwindow ist in ptAction gespeichert
	CPoint point( pNMListView->ptAction.x, pNMListView->ptAction.y);

	// Wenn nicht auf ein Item geklickt wurde, ist iItem gleich -1
	// Menü soll nur angezeigt werden, wenn ein Item angeklickt wurde
	//if (pNMListView->iItem == -1) 
	//	return;

	// relative Koordinaten auf absolute umrechnen
	m_ctrlWerkauftraege.ClientToScreen(&point);
	createPopupMenu(point,(pNMListView->iItem == -1 ? false : true));


	*pResult = 0;
}

/**
Popup Menü aufbauen
*/
void DialogZusammenfassenWerkauftraegeRobbi::createPopupMenu(CPoint point,bool isItemSelected)
{
	CMenu menu; 
	CString txt;
	menu.CreatePopupMenu();

	int  menu_items[]   = { IDS_ELEM_PASTE, ID_SEPARATOR, IDS_ELEM_DELETE, -1 };

	for (int i = 0; menu_items[i] != -1; i++) 
	{
		if (menu_items[i] != ID_SEPARATOR) 
		{
			txt.LoadString(menu_items[i]);
			menu.AppendMenu(MF_STRING, menu_items[i], txt);
		} 
		else 
		{
			menu.AppendMenu(MF_SEPARATOR);
		}
	} 

	//evt. Menüitems ausschalten
	if (!::IsClipboardFormatAvailable(CF_TEXT)) 
		menu.EnableMenuItem(IDS_ELEM_PASTE,MF_GRAYED);

	if (!isItemSelected)
		menu.EnableMenuItem(IDS_ELEM_DELETE,MF_GRAYED);


	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

/**
Werte aus Zwischenablage in die Liste kopieren
*/
void DialogZusammenfassenWerkauftraegeRobbi::OnPaste()
{
	HGLOBAL   hglb; 
    
	LPTSTR    lptstr; 

	CUIntArray arWa;


	if (!::IsClipboardFormatAvailable(CF_TEXT)) 
		return; 
	if (!::OpenClipboard(this->m_hWnd)) 
		return; 

	hglb = ::GetClipboardData(CF_TEXT); 
	if (hglb != NULL) 
	{ 
		lptstr = (LPTSTR)GlobalLock(hglb); 
		if (lptstr != NULL) 
		{ 
			CString zeile = lptstr;

			stringToInArray(zeile,arWa);
			
			::GlobalUnlock(hglb); 
		} 
	} 
	::CloseClipboard(); 


	if (!arWa.IsEmpty())
	{
		//Werkaufträge in Werkauftragsliste schreiben
		addWerkauftrag(arWa);

		//Listcontrol aktualisieren
		fillListe();

		//Plausis durchführen
		checkPlausis();
	}
}


/**
String mit "/n" in Array convertieren
*/
void DialogZusammenfassenWerkauftraegeRobbi::stringToInArray(const CString &str, CUIntArray &ar)
{
	int		wert,
			counter = 0,
			counterMerken = 0;

	CString subString;

	ar.RemoveAll();

	if (str.IsEmpty())
		return;

	while ((counter = str.Find("\n",counterMerken)) >= 0)
	{
		subString = str.Mid(counterMerken,counter-counterMerken);

		wert = atoi(subString);

		if (wert > 0)
			ar.Add(wert);

		counterMerken = counter + 1;
	}

}

/**
Im Editfeld Wa Nummer wurde Enter gedrückt
*/
LRESULT DialogZusammenfassenWerkauftraegeRobbi::OnKeyDownWaEingabe(WPARAM, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	addWaNrFromEditFeld();

	return NULL;
}


///Button Hinzufügen Wa Nummer wurde gedrückt
void DialogZusammenfassenWerkauftraegeRobbi::OnClickedAddWaNr()
{
	addWaNrFromEditFeld();
}


/**
Den im Editfeld eingegebenen Werkauftrag zur Liste hinzufügen
*/
void DialogZusammenfassenWerkauftraegeRobbi::addWaNrFromEditFeld()
{
	UpdateData();

	if (m_waNr <= 0)
	{
		//Falsche Wa nr
		return;
	}

	CUIntArray ar;
	ar.Add(m_waNr);

	//neuen Eintrag in Liste mit Werkaufträgen	
	addWerkauftrag(ar);
		
	//Listcontrol neu füllen
	fillListe();

	//Plausis durchführen
	checkPlausis();

	return;
}