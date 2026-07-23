#include "stdafx.h"
#include "resource.h"
#include <mxodbc\mxodbc.h>
#include "..\waxint\ServerDBOpen.h"

#include "..\waxint\WaLoader.h"
#include "Auftragstest.h"

#include <algorithm>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Auftragstest, CDialog)
	ON_BN_CLICKED(IDC_AKTUALISIEREN, on_aktualisieren)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, on_column_sort)
END_MESSAGE_MAP()


Auftragstest::Auftragstest(CWnd* pParent) : CDialog(IDD_AUFTRAGSTEST, pParent)
{
}

Auftragstest::~Auftragstest()
{
}

BOOL Auftragstest::DestroyWindow( )
{
	clear_list();
	return CDialog::DestroyWindow();
}

void Auftragstest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, list);
}

BOOL Auftragstest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString tmp;
	tmp = getStammdaten().getText("IDS_AKTUALISIEREN").c_str();
	SetDlgItemText(IDC_AKTUALISIEREN, tmp);
	tmp = getStammdaten().getText("IDS_ABBRECHEN").c_str();
	SetDlgItemText(IDCANCEL, tmp);
	tmp = getStammdaten().getText("IDS_AUFTRAEGE").c_str();
	SetWindowText(tmp);

	init_list();
	on_aktualisieren();
	
	return TRUE;  
}

void Auftragstest::clear_list()
{
	for (int i = 0; i < get_list().GetItemCount(); i++)
		delete dynamic_cast<Werkauftrag*>((CObject*)get_list().GetItemData(i));
	get_list().DeleteAllItems();
}

void Auftragstest::load_waids(vector<int>& waids)
{
	/**
	Lädt alle Aufträge ohne WA Nummer und fügt die waid der Liste hinzu
	*/
	MXTable status(WaxSys::getServerdb(), "status");

	DECLARE_COL(Int, waid, status);
	DECLARE_COL(Int, wanr, status);

	status.load("wanr = 0 AND storniert = 0", NULL, true);
	for(;!status.isEof();status.moveNext())
	{
		waids.push_back(waid.data);
	}
	sort(waids.begin(), waids.end());
}

void Auftragstest::on_aktualisieren() 
{
	/**
	Löscht die Anzeige und erstellt sie neu.
	Dazu werden aus der Statustabelle alle neuen Werkaufträge geladen und dann aus der Werkauftragstabelle
	die Einträge (Sammelaufträge) erstellt.
	*/
	CWaitCursor w;
	clear_list();

	vector<int> waids;
	load_waids(waids);

	MXTable werkauftrag(WaxSys::getServerdb(), "werkauftrag");
	DECLARE_COL(Int, waid, werkauftrag);
	DECLARE_COL(Int, nl, werkauftrag);
	DECLARE_COL(BigInt, sdauftragsnr, werkauftrag);
	DECLARE_COL(Long, sdmaterialnr, werkauftrag);
	DECLARE_COL(String, classid, werkauftrag);
	DECLARE_COL(String, bauabschnitt, werkauftrag);

	vector<int> abgearbeitet;
	for (vector<int>::iterator i = waids.begin(); i != waids.end(); i++)
	{
		// in abgearbeitet werden alle waids, die benutzt wurden abgespeichert, damit keine doppelten 
		// Einträge vorgenommen werden
		vector<int>::iterator found;
		for (found = abgearbeitet.begin(); found != abgearbeitet.end(); found++)
		{
			if (*found == *i)
				break;
		}
		if (found != abgearbeitet.end()) 
			continue;


		CString str;
		str.Format("waid = %i", *i);
		werkauftrag.load((LPCSTR)str, "", true);
		if (!werkauftrag.get_count())
		{
			// Fehler: Es existiert kein Eintrag in Werkauftragstabelle. 
			// Solche Einträge gibt es in der Testdatenbank. Zu prüfen wäre, ob es auch solche in der 
			// Produktivdatenbank gibt


			//Abfrage mit einem join über Tabelle status u. werkauftrag würde das Problem lösen


			werkauftrag.close();
			continue;
		}

		/// Sammelaufträge laden
		str.Format("sdauftragsnr = %I64d and sdmaterialnr = %i", sdauftragsnr.data, sdmaterialnr.data);
		
		werkauftrag.close();
		werkauftrag.load((LPCSTR)str, "waid", true);

		// Ein eigentlich unmöglicher Fehler
		if (!werkauftrag.get_count())
			continue;
		
		CString psp;
		
		// Klassenname für den Werkauftrag erstellen
		CString classnm(*classid.data);
		
		WaxPart *pt = ServerInterface::WaLoader::dyncreate_object((LPCSTR)*classid.data);
		if (pt)
			classnm = pt->get_werkauftrag()->get_stdbezeichnung();
		delete pt;

		Werkauftrag *wa = new Werkauftrag;
		wa->set_bemerkung(classnm);
		wa->set_anzahl(werkauftrag.get_count());
		wa->set_waid(waid.data);
		wa->get_leitkarte().set_prj_nl(nl.data);
		wa->set_bezeichnung(*bauabschnitt.data);
		wa->get_leitkarte().set_sdAuftragsnr(sdauftragsnr.data);
		wa->get_leitkarte().set_sdMaterialnummer(sdmaterialnr.data);

		add_wa(wa);

		for (;!werkauftrag.isEof(); werkauftrag.moveNext())
		{
			// alle Positionen dieses Auftrages als abgearbeitet markieren
			abgearbeitet.push_back(waid.data);
		}	
		werkauftrag.close();
	}
}


void Auftragstest::add_wa(Werkauftrag *wa) 
{
	/**
	Fügt die Daten eines 'Werkauftrages' in die Liste ein.
	Der Pointer auf das Objekt wird als Itemdata hinterlegt, damit die Sort Funktion
	darauf zurückgreifen kann.
	*/
	int nl = wa->get_leitkarte().get_prj_nl();
	string bauabschnitt = (LPCSTR)wa->get_bezeichnung();
	__int64 sdauftragsnr = wa->get_leitkarte().get_sdAuftragsnr();
	int sdmaterialnr = wa->get_leitkarte().get_sdMaterialnummer();

	char tmp[50];
	int i = 1;
	int item = get_list().GetItemCount();
	sprintf(tmp, "%i", nl);
	item = get_list().InsertItem(item, tmp, 0);
	get_list().SetItemText(item, i++, wa->get_bemerkung());	
	
	get_list().SetItemText(item, i++, bauabschnitt.c_str());
	
	sprintf(tmp, "%i", wa->get_anzahl());
	get_list().SetItemText(item, i++, tmp);
	
	sprintf(tmp, "%I64d", sdauftragsnr);
	get_list().SetItemText(item, i++, tmp);
	sprintf(tmp, "%i", sdmaterialnr);
	get_list().SetItemText(item, i++, tmp);
	get_list().SetItemData(item, (DWORD)wa);
}

ListCtrl& Auftragstest::get_list()
{
	return list;
}

void Auftragstest::init_list()
{
	const int SIZE = 40;
	CString tmp;
	tmp = getStammdaten().getText("IDS_NL").c_str();
	int i = 0;
	get_list().addColumn(tmp, i++, SIZE);
	
	tmp = getStammdaten().getText("IDS_CLASSID").c_str();
	get_list().addColumn(tmp, i++, SIZE, LVCFMT_LEFT);

	tmp = getStammdaten().getText("IDS_BAUABSCHNITT").c_str();
	get_list().addColumn(tmp, i++, SIZE*2, LVCFMT_LEFT);

	tmp = getStammdaten().getText("IDS_POSITIONEN").c_str();
	get_list().addColumn(tmp, i++, SIZE, LVCFMT_LEFT);

	tmp = getStammdaten().getText("IDS_SDNR").c_str();
	get_list().addColumn(tmp, i++, SIZE, LVCFMT_LEFT);
	
	tmp = getStammdaten().getText("IDS_SDMATNR").c_str();
	get_list().addColumn(tmp, i++, SIZE, LVCFMT_LEFT);
}

//Indicates that one of the columns was clicked
void Auftragstest::on_column_sort(NMHDR* pNMHDR, LRESULT* pResult)
{
	/*
	Sortierung 
	*/
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	get_list().SortItems(sort_func, pNMListView->iSubItem);
	*pResult = 0;
}


int CALLBACK Auftragstest::sort_func(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	/**
	Vergleichsfunktion für die Sortierung in der Liste
	*/
	Werkauftrag *wa1 = (Werkauftrag*)lParam1;
	Werkauftrag *wa2 = (Werkauftrag*)lParam2;

	__int64 u1 = 0, u2 =0;
	int i1 = 0, i2 = 0; 
	string s1, s2;
	switch (lParamSort)
	{
		case NL:
			i1 = wa1->get_leitkarte().get_prj_nl();
			i2 = wa2->get_leitkarte().get_prj_nl();
			break;
		case ANZAHL:
			i1 = wa1->get_anzahl();
			i2 = wa2->get_anzahl();
			break;
		case SDNR:
			u1 = wa1->get_leitkarte().get_sdAuftragsnr();
			u2 = wa2->get_leitkarte().get_sdAuftragsnr();
			break;
		case SDMATNR:
			i1 = wa1->get_leitkarte().get_sdMaterialnummer();
			i2 = wa2->get_leitkarte().get_sdMaterialnummer();
			break;
		case CLASSID:
			s1 = (LPCSTR)wa1->get_bemerkung();
			s2 = (LPCSTR)wa2->get_bemerkung();
			break;
		case BAUABSCHNITT:
			s1 = (LPCSTR)wa1->get_bezeichnung();
			s2 = (LPCSTR)wa2->get_bezeichnung();
			break;
	}
	if (s1.length() || s2.length())
	{
		return s1.compare(s2);
	}
	if (i1 == i2)
		return 0;
	if (u1 == u2)
		return 0;
    return u1 > u2 ? 1 : -1;
}