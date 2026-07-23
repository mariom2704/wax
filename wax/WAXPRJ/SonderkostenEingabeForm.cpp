#include "stdafx.h"


#include "EditSoko.h"


IMPLEMENT_DYNCREATE(SonderkostenEingabeForm, WaxPartForm)

BEGIN_MESSAGE_MAP(SonderkostenEingabeForm, WaxPartForm)
	ON_BN_CLICKED(IDC_NEW,    OnClickedNew)
	ON_BN_CLICKED(IDC_EDIT,   OnClickedEdit)
	ON_BN_CLICKED(IDC_DELETE, OnClickedDelete)
	ON_BN_CLICKED(IDC_AUTO,   OnClickedAuto)
	ON_NOTIFY(NM_DBLCLK, IDC_SONDERKOSTEN, OnDblclkSonderkosten)
	ON_WM_SIZE()
END_MESSAGE_MAP()

SonderkostenEingabeForm::~SonderkostenEingabeForm()
{

}

SonderkostenEingabeForm::SonderkostenEingabeForm() : WaxPartForm(IDD_SONDERKOSTEN, getStammdaten().getText("IDD_SONDERKOSTEN").c_str())
{
	// Sonderkosten werden auch an anderer Stelle generiert, so dass sichergestellt
	// sein muss, dass nur gespeichert wird, wenn auch in dieser Maske geändert wurde
	set_dirty(false);

}

//Called after the size of CWnd has changed.
void SonderkostenEingabeForm::OnSize(UINT nType, int cx, int cy)
{
	WaxPartForm::OnSize(nType, cx, cy);

	const int AbstandUnten  = 40;
	const int AbstandLinks  = 10;
	const int AbstandOben   = 40;
	const int ButtonAbstand =  5;

	if (::IsWindow(list.m_hWnd))
		list.SetWindowPos( NULL, AbstandLinks, AbstandOben, cx - 2*AbstandLinks, cy-AbstandUnten, SWP_SHOWWINDOW | SWP_NOZORDER);
	else
		return;

	// Buttons müssen mitskaliert werden: Der Abstand ist immer fest
	CWnd *butt[4];
	butt[0] = GetDlgItem(IDC_NEW);
	butt[1] = GetDlgItem(IDC_EDIT);
	butt[2] = GetDlgItem(IDC_DELETE);
	butt[3] = GetDlgItem(IDC_AUTO);
	
	XASSERT(butt[0] && butt[1] && butt[2] && butt[3]);
	if (!butt[0] || !butt[1] || !butt[2] || !butt[3])
		return;

	WINDOWPLACEMENT wndpl; 
	butt[0]->GetWindowPlacement(&wndpl); 
	int xcx = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;

	butt[0]->SetWindowPos( NULL, AbstandLinks + ButtonAbstand, wndpl.rcNormalPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
	butt[1]->SetWindowPos( NULL, AbstandLinks + 2*ButtonAbstand + 1*xcx, wndpl.rcNormalPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
	butt[2]->SetWindowPos( NULL, AbstandLinks + 3*ButtonAbstand + 2*xcx, wndpl.rcNormalPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
	butt[3]->SetWindowPos( NULL, AbstandLinks + 4*ButtonAbstand + 3*xcx, wndpl.rcNormalPosition.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
}

BOOL SonderkostenEingabeForm::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return FALSE;
	
	ListView_SetExtendedListViewStyle(list.m_hWnd, LVS_EX_FULLROWSELECT);
	CString str;
	str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
	list.InsertColumn(POSNR,	str,		LVCFMT_RIGHT, 60);
	str = getStammdaten().getText("IDS_TEXT").c_str();
	list.InsertColumn(TEXT,		str,		LVCFMT_LEFT, 400);
	str = getStammdaten().getText("IDS_PREIS").c_str();
	list.InsertColumn(PREIS,	str,		LVCFMT_RIGHT, 80);
	str = getStammdaten().getText("IDS_FIX").c_str();
	list.InsertColumn(FIX,	str,		LVCFMT_RIGHT, 80);

	setWindowsText(IDC_NEW, "IDS_NEW");
	setWindowsText(IDC_EDIT, "IDS_EDIT");
	setWindowsText(IDC_DELETE, "IDS_DELETE");
	setWindowsText(IDC_AUTO, "IDC_AUTO");

	return TRUE;
}

void SonderkostenEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SONDERKOSTEN, list);

	XASSERT(get_data());
	if (!get_data())
		return; // Schwerer Fehler!
	Werkauftrag *w = get_data()->get_werkauftrag();
	XASSERT(w);
	if (!w)
		return; // Fast genauso schwerer Fehler!!
	
	CObList& sokolist = w->get_preis().get_sokolist();

	// Speichern der Liste 
	if (pDX->m_bSaveAndValidate)
	{
		if (is_dirty())
		{
			// Erstmal die alte Liste löschen
			w->get_preis().delete_sokolist(true);
			
			// dann neue rein
			for (int i = 0; i < list.GetItemCount(); i++)
				sokolist.AddTail(readSoko(i));
			set_dirty(false);
		}
	}
	// gespeicherte Liste anzeigen
	else 
	{
		// Aufbau der Liste mit Sonderkosten
		list.DeleteAllItems();
		list.SetRedraw(FALSE);
		
		for (POSITION pos = sokolist.GetHeadPosition(); pos;)
		{
			Preis::Sonderkosten* so = (Preis::Sonderkosten*)sokolist.GetNext(pos);
			int index = list.InsertItem(list.GetItemCount(), "-");
			writeSoko(so, index);
		}
		list.SetRedraw();
	}
}

Preis::Sonderkosten * SonderkostenEingabeForm::readSoko(int index)
{
	Preis::Sonderkosten *so = new Preis::Sonderkosten;
	
	CString tmp = list.GetItemText(index, POSNR);
	so->set_nr(atoi(tmp));
	
	tmp = list.GetItemText(index, TEXT);
	so->set_text(tmp);
	
	tmp = list.GetItemText(index, PREIS);
	so->set_preis(atof(tmp));
	
	tmp = list.GetItemText(index, FIX);

	if (tmp.GetLength())
		so->set_isfix(true);
	else
		so->set_isfix(false);
	
	return so;
}

void SonderkostenEingabeForm::writeSoko(Preis::Sonderkosten * soko, int index)
{
	CString txt = "";
	txt.Format("%i", soko->get_nr());
	list.SetItemText(index, POSNR, txt);

	list.SetItemText(index, TEXT, soko->get_text());
		
	txt.Format("%.2f", soko->get_preis());
	list.SetItemText(index, PREIS, txt);

	if (soko->get_isfix())
		txt = "X";
	else
		txt = "";
	list.SetItemText(index, FIX, txt);
}

//Indicates the user clicked a button
void SonderkostenEingabeForm::OnClickedNew()
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
	int newindex = atoi(list.GetItemText(insertafter, 0)) + 1;
	CString txt;
	txt.Format("%i", newindex);
	
	// Item einfuegen
	int index = list.InsertItem(insertafter+1, txt);

	txt = getStammdaten().getText("IDS_VK3ZUSATZPREIS").c_str();
	list.SetItemText(index, TEXT, txt); 
	txt.Format("%10.2f", 0);
	list.SetItemText(index, PREIS, txt);

	list.SetItemState(insertafter+1, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);

	// Default-Soko in Liste packen
	Preis::Sonderkosten *so = new Preis::Sonderkosten;
	so->set_nr(newindex);
	txt = getStammdaten().getText("IDS_VK3ZUSATZPREIS").c_str();
	so->set_text(txt);
	so->set_preis(0);
	so->set_isfix(true);
	Preis &p = get_data()->get_werkauftrag()->get_preis(); 
	p.get_sokolist().AddTail(so);
	writeSoko(so, index);
	
	// Eintrag editieren
	if (!onEdit())
	{
		list.DeleteItem(insertafter+1);
		list.SetItem(insertafter, 0, LVIF_STATE , NULL, 0, LVIS_SELECTED, LVIS_SELECTED, NULL);
	}
	set_dirty();
}

//Indicates the user clicked a button
void SonderkostenEingabeForm::OnClickedEdit()
{
	onEdit();
}

bool SonderkostenEingabeForm::onEdit()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (!pos)
		return false;
	int index = list.GetNextSelectedItem(pos);
	Preis::Sonderkosten *so = readSoko(index);
	EditSoko e(this, so);
	if (e.DoModal() == IDOK)
	{
		writeSoko(so, index);
		delete so;
		set_dirty();

		UpdateData(true);
		return true;
	}
	else
	{
		delete so;
		return false;
	}
}

//Indicates the user clicked a button
void SonderkostenEingabeForm::OnClickedDelete()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	int index = 0;
	while (pos)
	{
		index = list.GetNextSelectedItem(pos);
		if (index >= 0)
			list.DeleteItem(index);
		 pos = list.GetFirstSelectedItemPosition();
	}
	list.SetItem(index-1, 0, LVIF_STATE , NULL, 0, LVIS_SELECTED, LVIS_SELECTED, NULL);

	set_dirty();
	UpdateData(true);

	
}

//Indicates that the user has double-clicked the left mouse button in the 
//control
void SonderkostenEingabeForm::OnDblclkSonderkosten(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	OnClickedEdit();	 
}

//Indicates the user clicked a button
void SonderkostenEingabeForm::OnClickedAuto()
{
	/**
    Wenn am Verteiler auch nur die Anzahl geaendert wird, muss der Sokoliste neu generiert werden.
    */
	if (AfxMessageBox(getStammdaten().getText("IDS_ACHTUNG_SOKO").c_str(), MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
	{
		set_dirty();
		get_data()->createAutoSoko(false);
		UpdateData(false);
	}
}

void SonderkostenEingabeForm::set_dirty(bool val)
{
	dirty = val;
}

bool SonderkostenEingabeForm::is_dirty() const
{
	return dirty;
}