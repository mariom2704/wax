#include "stdafx.h"


#include "beschichtungeingabe.h"


#define ID_VERZINKEREI 126
#define ID_RAL 125
#define ID_BESCHICHTUNGEN 123

BEGIN_MESSAGE_MAP(BeschichtungEingabe, CWnd)
	ON_CBN_SELCHANGE(ID_BESCHICHTUNGEN, onSelchange)
	ON_CBN_SELCHANGE(ID_VERZINKEREI, on_select_verzinkerei)
	ON_BN_CLICKED(ID_RAL, onClicked)  
	ON_COMMAND_RANGE(WM_USER+1000, WM_USER+19000, onSetRal)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// Alle Einträge in der Combobox haben einen Pointer auf eine Beschichtungsart 
// aus den Stammdaten. Diese Methode liefert das Objekt des ausgewählten 
// Eintrages. Ist hier als Makro definiert, weil sonst Probleme bei der Einbindung
// des Stammdatenheaders entstehen.
#define getBeschichtungsart(xxxx) {\
xxxx = NULL;\
int index = arten.GetCurSel(); \
if (index >= 0) \
{ \
	CObject *tmp = (CObject*)(arten.GetItemDataPtr(index));\
	if (tmp) {\
		xxxx = dynamic_cast<Beschichtungen::data*>(tmp);\
		}\
}}\
	



BeschichtungEingabe::BeschichtungEingabe()
{
}


//Datenaustausch der Dialogelemente mit dem Beschichtungsobjekt
void BeschichtungEingabe::doDataExchange(CDataExchange* pDX, Beschichtung *beschichtung)
{
	if (!IsWindow(this->m_hWnd))
		return;
	if (pDX->m_bSaveAndValidate)
	{
		Beschichtungen::data* besch;
		getBeschichtungsart(besch);
		XASSERT(besch);
		if (besch)
		{
			CString stral;
			raleingabe.GetWindowText(stral);
			int ral = atoi(stral) ? atoi(stral) : -1 ;
			beschichtung->set_typ(besch->get_index());
			beschichtung->set_ral(ral);
			beschichtung->set_bezeichnung(besch->get_bezeichnung());
		}

		Verzinkung::data* z = dynamic_cast<Verzinkung::data*>((CObject*)zinkerei.GetItemDataPtr(zinkerei.GetCurSel()));
		beschichtung->set_verzinkerei(z ? z->get_nr() : Beschichtung::DEFAULT_VERZINKEREI);

	}
	else
	{
		// Falls Text in anderer Sprache abgespeichert ist als jetzt eingestellt,
		// wird der Text noch mal aus den Stammdaten geladen.
		if (arten.SelectString(-1, beschichtung->get_bezeichnung()) == CB_ERR)
		{
			Beschichtungen be;
			CString bez = be.getBezeichnung(beschichtung->get_typ());
			if (bez.GetLength())
			{
				beschichtung->set_bezeichnung(bez);
				arten.SelectString(-1, beschichtung->get_bezeichnung());
			}
		}

		onSelchange();
		CString ral;
		ral.Format("%i", beschichtung->get_ral());
		raleingabe.SetWindowText(ral == "-1" ? "-" : ral);

		for (int sel = 0; sel < zinkerei.GetCount(); sel++)
		{
			Verzinkung::data* z = dynamic_cast<Verzinkung::data*>((CObject*)zinkerei.GetItemDataPtr(sel));
			if (z && z->get_nr() == beschichtung->get_verzinkerei())
			{
				zinkerei.SetCurSel(sel);
				break;
			}
		}
	}
}

//Erzeugen des Controls
bool BeschichtungEingabe::create(CWnd* parent, UINT dlgid)
{
// Koordinaten mitgeben ist sehr mühsam, deshalb kann man ein Dummy-Control
	// in den Dialog einbauen, dessen Abmessungen benutzt werden um die Dialogelemente 
	// anzuzeigen.
	RECT rw;
	this->dlgid = dlgid;
	CWnd *item = parent->GetDlgItem(dlgid);
	bool enabled = item->IsWindowEnabled() ? true : false;
	
	if (item)
	{
		item->GetWindowRect(&rw);
		item->GetDesktopWindow()->MapWindowPoints(parent, &rw);
	}
	else 
		return false;

	// Rechteck, in dem die Controls erzeugt werden
	rw.top += 10;
	rw.left += 5;
	rw.bottom += 35;
	rw.right -= 5;

	// Dummy Elternfenster erzeugen, damit die Window Messages abgefangen werden können
	if (!Create("Static", "-", WS_CHILD | WS_GROUP | WS_TABSTOP | (enabled ? 0 : WS_DISABLED), rw, parent,122))
		return false;
	ShowWindow(SW_SHOW);
	GetWindowRect(&rw);
	ScreenToClient(&rw);

	// Combobox bekommt den halben Platz zugewiesen
	CRect comborect;
	int sizex = rw.right - (rw.right-rw.left)/2;
	comborect.SetRect(rw.left, rw.top, sizex, rw.bottom+200);
	arten.Create(CBS_DROPDOWNLIST| WS_CHILD, comborect, this, ID_BESCHICHTUNGEN);
	arten.SetFont(parent->GetFont());
	arten.SetDroppedWidth(50);
	arten.ShowWindow(SW_SHOW);

	// Combobox mit Beschichtungsarten initialisieren
	for (POSITION pos = getStammdaten().get_beschichtungen().GetHeadPosition(); pos != NULL; )
	{
		Beschichtungen::data* besch = (Beschichtungen::data* )getStammdaten().get_beschichtungen().GetNext(pos);
		int index = arten.AddString(besch->get_bezeichnung());
		arten.SetItemDataPtr(index, besch);
	}

	// Jetzt den Text "RAL:" neben dem Editfeld erzeugen
	CRect txtrect;
	sizex += 15; // 15 Pixel Mitte Rechts verschieben
	txtrect.SetRect(sizex, rw.top+5, sizex+30, rw.bottom);
	if (!txt.Create("&Ral:", WS_VISIBLE | WS_CHILD | (enabled ? 0 : WS_DISABLED), txtrect, this))
		return false;
	txt.SetFont(parent->GetFont());

	// Jetzt das Editfeld erzeugen
	CRect editrect;
	sizex += 30;
	editrect.SetRect(sizex, rw.top, sizex+40, rw.top+22);
	raleingabe.Create(WS_TABSTOP  | WS_VISIBLE | WS_CHILD | WS_BORDER| (enabled ? 0 : WS_DISABLED), editrect,this, 124);
	raleingabe.SetFont(parent->GetFont());
	
	
	// Jetzt den Auswahlbutton erzeugen
	CRect butrect;
	sizex += 42;
	butrect.SetRect(sizex, rw.top, sizex+20, rw.top+22);
	ralbut.Create("...", WS_TABSTOP   | WS_VISIBLE | WS_CHILD| (enabled ? 0 : WS_DISABLED), butrect,this, ID_RAL);
	ralbut.SetFont(parent->GetFont());

	sizex += 25;
	butrect.SetRect(sizex, rw.top, sizex+80, rw.bottom+200);
	//CComboBox
	zinkerei.Create(CBS_DROPDOWNLIST| WS_CHILD, butrect, this, ID_VERZINKEREI);
	zinkerei.SetFont(parent->GetFont());
	zinkerei.ShowWindow(WaxSys::isAV() ? SW_SHOW : SW_HIDE);

	int index = 0;
	for (POSITION pos = getStammdaten().get_verzinkung().get_lst().GetHeadPosition(); pos != NULL; )
	{
		Verzinkung::data* z = (Verzinkung::data* )getStammdaten().get_verzinkung().get_lst().GetNext(pos);
		index = zinkerei.InsertString(index, z->get_name());
		zinkerei.SetItemDataPtr(index, z);
	}
	
	EnableToolTips(TRUE);

	return true;
}

void BeschichtungEingabe::initMenu(CMenu *menu)
{
	UNREFERENCED_PARAMETER(menu);
}

//Beschichtung auf Plausibilität prüfen
bool BeschichtungEingabe::check_plausis()
{
	/**
	Nur wenn ral mit 0 vorbelegt ist (gepulvert), kann auch eine Ralnummer eingetragen werden.
	Nur dann muss hier auch geprüft werden. Eine Prüfung der Ralnummern aus den Stammdaten 
	(Ralnummer ist vorbelegt und nicht änderbar) wird aber zur Sicherheit trotzdem vorgenommen.
	*/

	Beschichtungen::data* besch;
	getBeschichtungsart(besch);
	if (!besch)
	{
		CString str; str = getStammdaten().getText("IDS_BESCHICHTUNG_WAEHLEN").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONERROR);
		return false;
	}
	if (besch->get_ral() != -1) // -1 heisst: Keine Farbe möglich
	{
		CString ralnr;
		raleingabe.GetWindowText(ralnr);
		int iral = atoi(ralnr);
		
		Ral ral;
		CString str; str = getStammdaten().getText("IDS_UNBEKANNT").c_str();
		if (ral.getBezeichnung(iral) != str)
			return true;

		str = getStammdaten().getText("IDS_UNGUELTIGE_RAL").c_str();
		AfxMessageBox(str, MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}


//Indicates a new combo box list item is selected
void BeschichtungEingabe::onSelchange()
{
	Beschichtungen::data* besch;
	getBeschichtungsart(besch);
	if (!besch)
		return;

	Beschichtung b(besch->get_index(), besch->get_ral(), besch->get_bezeichnung());
	zinkerei.ShowWindow(b.isVerzinkt() && WaxSys::isAV() ? SW_SHOW : SW_HIDE);
	
	// Verzinkereiauswahl mit neuer Generierung der Sokos erfolgt in der AV Version nur nach manueller Auswahl 
	// der Verzinkung, in der NL Version ist die Verzinkerei ausgeblendet, so das nur Selchange bleibt
	if (b.isVerzinkt() && !WaxSys::isAV())
		on_select_verzinkerei();
	switch (besch->get_ral())
	{
		case 0: // Beschichtung mit Usereingabe
		{
			CString ralnr;
			raleingabe.SetWindowText(ralnr);
			raleingabe.EnableWindow(TRUE);
			ralbut.EnableWindow(TRUE);
			raleingabe.SetFocus();
		}
		break;
		case -1: // Beschichtung ohne Farbe
			ralbut.EnableWindow(FALSE);
			raleingabe.EnableWindow(FALSE);
			raleingabe.SetWindowText("-");
		break;

		default: // Beschichtung mit Ral
		{
			raleingabe.EnableWindow(FALSE);
			ralbut.EnableWindow(FALSE);
			CString ralnr;
			ralnr.Format("%i", besch->get_ral());
			raleingabe.SetWindowText(ralnr);
		}
		break;
	}			
}



//Indicates the user clicked a button
void BeschichtungEingabe::onClicked()
{
	CMenu pop;
	//pop.LoadMenu(IDR_RAL);
	UINT uid = WM_USER+1000;
	CMenu mnral[9];

	pop.CreatePopupMenu();
	int i;
	for (i = 0; i < 9; i++)
	{
		mnral[i].CreateMenu();
	}
	
	for (POSITION pos = getStammdaten().get_ral().GetHeadPosition(); pos != NULL; )
	{
		Ral::data* ral = (Ral::data* )getStammdaten().get_ral().GetNext(pos);
		CString item;
		item.Format("RAL %i (%s)", ral->get_id(), ral->get_bezeichnung());
		int submenu = ral->get_id()/1000 - 1;
		mnral[submenu].AppendMenu(MF_STRING, uid+ral->get_id(),  (LPCTSTR)item);
	}
	
	for (i = 0; i < 9; i++)
	{
		CString tit;
		tit.Format("RAL %i000", i+1);
		pop.InsertMenu( i, MF_POPUP, (UINT)mnral[i].m_hMenu, tit);
	}
	
	CRect r;
	ralbut.GetWindowRect(&r);
	CPoint pt(r.right, r.top);
	pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this); 
	pop.DestroyMenu();	

	
}



void BeschichtungEingabe::onSetRal(UINT pCmdUI)
{
	CString ralnr;
	ralnr.Format("%i", pCmdUI - WM_USER - 1000);
	raleingabe.SetWindowText(ralnr);	
}



INT_PTR BeschichtungEingabe::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int res = CWnd::OnToolHitTest(point, pTI);
	if (res == 124)
	{
		CString ralnr;
		raleingabe.GetWindowText(ralnr);	
		int iral = atoi(ralnr);
		if (iral)
		{
			Ral ral;
			ralnr = ral.getBezeichnung(iral);
			
			pTI->lpszText = (LPSTR)malloc(2048);
			pTI->uFlags |= TTF_ALWAYSTIP;
			strcpy(pTI->lpszText, ralnr);
			
			((CFrameWnd*)AfxGetMainWnd())->SetMessageText(ralnr);
			return res;
		}
	}
	return res;	

}



//Called after CWnd gains the input focus.
void BeschichtungEingabe::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	arten.SetFocus();	
}

void BeschichtungEingabe::on_select_verzinkerei()
{
	GetParent()->PostMessage(WM_COMMAND, WM_AUTOSOKO);

}