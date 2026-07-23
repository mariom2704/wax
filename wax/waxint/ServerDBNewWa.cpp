#include "stdafx.h"


#include "resource.h"
#include "ServerDBNewWa.h"
#include <mxutil/ddx_extension.h>

#include "WaLoader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ServerDBNewWa, ServerDBOpen)
	ON_BN_CLICKED(IDC_CHECK_OHNE_KC_EINLESEN, &ServerDBNewWa::OnBnClickedCheckMitKC)
END_MESSAGE_MAP()


ServerDBNewWa::items::items(const CString& classname, int dlgid,  CString* var  )
{
	this->classname = classname;
	this->dlgid = dlgid;
	this->var = var;
}


void ServerDBNewWa::OnBnClickedCheckMitKC()
{
	m_ReadOnlyRecordsWithGUID = ((CButton*)GetDlgItem(IDC_CHECK_OHNE_KC_EINLESEN))->GetCheck();
}


ServerDBNewWa::~ServerDBNewWa()
{
	for (POSITION pos = itemlist.GetHeadPosition(); pos;)
	{
		delete (items*)itemlist.GetNext(pos);
	}
}

ServerDBNewWa::ServerDBNewWa(Status::STATUS status) : ServerDBOpen(status)
{
	m_ReadGUIDFile = TRUE;
	if (status == Status::Preisanfrage)
		this->sortPreisanfrage = true;

	nl = -1;
	/*
	In itemlist werden alle Sachbearbeiterzuweisungen eingetragen, so dass bei Änderungen nur diese
	Liste angepasst werden muss
	*/
	m_lpszTemplateName = MAKEINTRESOURCE(IDD_OPEN_DATABASE_NEWWA);

	itemlist.AddTail(new items("VerteilerData", IDC_SACHBEARBEITER_VERTEILER, &sachbearbeiter_verteiler));
	itemlist.AddTail(new items("StrangrohrData", IDC_SACHBEARBEITER_STRANGROHR, &sachbearbeiter_strangrohr));
	itemlist.AddTail(new items("StationsverteilerData", IDC_SACHBEARBEITER_STATIONSVERTEILER, &sachbearbeiter_stationsverteiler));
	itemlist.AddTail(new items("GewinderohrData", IDC_SACHBEARBEITER_GEWINDEROHR, &sachbearbeiter_gewinderohr));
	itemlist.AddTail(new items("HandelslaengeData", IDC_SACHBEARBEITER_HANDELSLAENGEN, &sachbearbeiter_handelslaengen));
	// Diese beiden auch zu Verteiler ordnen
	itemlist.AddTail(new items("PumpenverteilerData", NULL, &sachbearbeiter_verteiler));
	itemlist.AddTail(new items("PumpenprobierleitungData", NULL, &sachbearbeiter_verteiler));

	// Sonstiges muss am Ende stehen, weil das in initObject erwartet wird
	itemlist.AddTail(new items("Sonstiges", IDC_SACHBEARBEITER_SONSTIGES, &sachbearbeiter_sonstiges));


	if (status != Status::Preisanfrage)
	{
		// Sachbearbeiterzuweisungen aus Registry laden
		for (POSITION pos = itemlist.GetHeadPosition(); pos;)
		{
			items *it = (items*)itemlist.GetNext(pos);
			if (it->dlgid != NULL)
				*it->var = AfxGetApp()->GetProfileString("SACHBEARBEITER_SEGMENT", it->classname, "");
		}
	}
}

BOOL ServerDBNewWa::DestroyWindow()
{
	if (status != Status::Preisanfrage)
	{
		// Sachbearbeiterzuweisungen in Registry speichern
		for (POSITION pos = itemlist.GetHeadPosition(); pos;)
		{
			items *it = (items*)itemlist.GetNext(pos);
			if (it->dlgid != NULL)
				AfxGetApp()->WriteProfileString("SACHBEARBEITER_SEGMENT", it->classname, *it->var);
		}
	}
	
	return CWnd::DestroyWindow();
}

//Dialogelemente initialisieren
BOOL ServerDBNewWa::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;
	// Sachbearbeiterliste initialisieren
	for (POSITION pos = itemlist.GetHeadPosition(); pos;)
	{
		items *it = (items*)itemlist.GetNext(pos);
		if (it->dlgid == NULL)
			continue;

		CComboBox* users = (CComboBox*)GetDlgItem(it->dlgid);
		WaxSys::user_list(users);
		if (users->SelectString(-1, *it->var) == CB_ERR)
		{
			users->InsertString(0, *it->var);
			// und nochmal selektieren, leere Strings kann man einfügen aber nicht selektieren
			if (it->var->GetLength())
				users->SelectString(-1, *it->var);
			else
				users->SetWindowText("");
		}
		
		//Bei Preisanfrage keine Eingabe möglich
		if (status == Status::Preisanfrage)
			GetDlgItem(it->dlgid)->EnableWindow(false);
	}

	getStammdaten().initNL_ComboBox(&cb_nl);
	CString str; str = getStammdaten().getText("IDS_ALLE_NIEDERLASSUNGEN").c_str();
	int index = cb_nl.AddString(str);
	cb_nl.SetItemData(index, -1); // Alle Niederlassungen entspricht NlId = -1
	cb_nl.SelectString(-1, str);

	((CButton*)(GetDlgItem(IDC_SORT_WA)))->SetCheck(1);

	createProgressWnd();

	if (status == Status::Preisanfrage)
		this->SetWindowTextA(getStammdaten().getText("IDS_NEW_PA_EINLESEN").c_str());
	else
		this->SetWindowTextA(getStammdaten().getText("IDS_NEW_WA_EINLESEN").c_str());

	return true;
}

//Erzeugen des SQL Select Strings aus den Angaben in der Maske
const CString ServerDBNewWa::createSQLSelect() const
{
	/**
	Select ist Wa-Nummer ist Null ODER SachbearbeiterAV nicht gesetzt
	Optional kann eine Niederlassung ausgewählt werden
	*/
	CString sql("(wanr = 0 OR sachbesegment IS NULL OR sachbesegment = '')");
	if (nl >= 0)
	{
		CString sel_nl;
		sel_nl.Format(" AND nl = %i", nl);
		sql += sel_nl; 
	}
	return sql;
}

//Erzeugen des SQL Sort Strings aus den Angaben in der Maske
const CString ServerDBNewWa::createSQLSort(CString tablename) const
{
	return "waid";
}



void ServerDBNewWa::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NL, cb_nl);
	
	ddx_cbdata(pDX, IDC_NL, nl);
	
	// Sachbearbeiterliste initialisieren

	for (POSITION pos = itemlist.GetHeadPosition(); pos;)
	{
		items *it = (items*)itemlist.GetNext(pos);
		if (it->dlgid != NULL)
			DDX_Text(pDX, it->dlgid, *it->var);
	}
}



//Import starten
void ServerDBNewWa::OnOK()
{
#ifdef DEBUG
	UpdateData();
	for (POSITION pos = itemlist.GetHeadPosition(); pos;)
	{
		items *it = (items*)itemlist.GetNext(pos);
		CString strSachbearb = *it->var;
		if(!strSachbearb.IsEmpty())
		{
			if(IDNO == AfxMessageBox("Es ist ein Feld gefüllt,\nwollen Sie die Daten trotzdem einlesen??????", MB_YESNO))
			{
				return;
			}
			else
			{
				break;
			}
		}
	}
#endif

	ServerDBOpen::OnOK();
}


bool ServerDBNewWa::serialize_project(CObList* project)
{
	return ServerDBOpen::serialize_project(project);
}

// Initialisierung von neu geladenen Objekten
void ServerDBNewWa::initObject(WaxPart *pt)
{
	/**
	Bei neuegeladenen Werkaufträgen werden die Sachbearbeiter automatisch
	gesetzt
	*/
	ServerDBOpen::initObject(pt);
	initSachbearbeiter(pt);
	updateStatus(pt);
}

void ServerDBNewWa::initSachbearbeiter(WaxPart* pt)
{
	/**
	@todo Funktion von Dialog trennen
	*/
	CString classname = pt->GetRuntimeClass()->m_lpszClassName;
	for (POSITION pos = itemlist.GetHeadPosition(); pos;)
	{
		items *it = (items*)itemlist.GetNext(pos);

		// Wenn classname gefunden wurde oder wenn das Ende der Liste erreicht wurde (sonstiges)
		if (classname == it->classname || !pos)
		{
			pt->get_werkauftrag()->get_status().set_sachbearbeiterSegment(*it->var);
			break;
		}
	}

	// Rekursiv alle Unterpositionen setzen
	for (POSITION upos = pt->get_subobjects().GetHeadPosition(); upos != NULL; )
	{
		WaxPart* upt = dynamic_cast<WaxPart*>(pt->get_subobjects().GetNext(upos));
		if (!upt)
			continue;
		initSachbearbeiter(upt);
	}
}



BOOL ServerDBNewWa::ResetWaNr(WaxPart* pt)
{
	waloader->get_serverdb().set_werkauftrag(pt->get_werkauftrag());
	return waloader->get_serverdb().ResetWanr();
}





void ServerDBNewWa::updateStatus(WaxPart* pt)
{
	/**
	Schreibt den Status: Sachbearbeiter und WA Nummer werden gesetzt.
	Aber nur wenn der Sachbearbeiter auch durch initObject gesetzt wurde
	*/

	if (!pt->get_werkauftrag()->get_status().get_sachbearbeiterSegment().GetLength())
		return;
		
	waloader->get_serverdb().set_werkauftrag(pt->get_werkauftrag());
	if (!waloader->get_serverdb().updateStatus(true))
	{
		CString msg, str; str = getStammdaten().getText("IDS_ERROR8b").c_str();
		msg.Format(str, pt->get_werkauftrag()->get_leitkarte().get_prj_auftragsbez());
		AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
	}
}

