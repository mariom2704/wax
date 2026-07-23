#include "stdafx.h"


#include "ServerDBOpen.h"
#include "resource.h"

#include "../wax/docver.h"
#include <mxcontrols/fileloader.h>


#include "WaLoader.h"
#include "GpSammelauftragCreator.h"
#include "GpCadConverter.h"
#include "GpStatusUpdate.h"
#include "../waxprj/WpPreisanfrage.h"

#include <mxcontrols/msgbox.h>
using namespace MxGui;
using namespace MxFilesystem;
using namespace ServerInterface;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_STEPIT (WM_USER+232)

BEGIN_MESSAGE_MAP(ServerDBOpen, CDialog)
	ON_BN_CLICKED(IDC_ENABLE_FREIGABE_VON, OnClickedEnable_freigabe_von)
	ON_BN_CLICKED(IDC_ENABLE_FREIGABE_BIS, OnClickedEnable_freigabe_bis)
	ON_BN_CLICKED(IDC_SELECT_NEW, OnClickedSelect_new)
	ON_BN_CLICKED(IDC_SELECT_EGAL, OnClickedSelect_egal)
	ON_BN_CLICKED(IDC_SELECT_INBEARBEITUNG, OnClickedSelect_inbearbeitung)
	ON_BN_CLICKED(IDC_SELECT_WA, OnClickedSelect_wa)
	ON_MESSAGE(WM_STEPIT, OnLoadNext)
END_MESSAGE_MAP()

//Konstruktor
ServerDBOpen::ServerDBOpen(Status::STATUS status) : CDialog(IDD_OPEN_DATABASE), load_storniert(false), m_ReadGUIDFile(FALSE), m_ReadOnlyRecordsWithGUID(FALSE)
{
	this->status = status;
	waloader = new WaLoader();
	sortPreisanfrage = false;
}


//Destruktor
ServerDBOpen::~ServerDBOpen()
{
	delete waloader;
}

LRESULT ServerDBOpen::OnLoadNext( WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);
	//CStatic *txt = (CStatic*)freigabefortschritt.GetDlgItem(IDC_MSG);
	WaxPart* cur = dynamic_cast<WaxPart*>((CWnd*)wparam);

	if (cur)
		setProgressText(cur->get_werkauftrag_const()->get_bezeichnung() + " Posnr: " + cur->get_werkauftrag_const()->get_posNr());
	stepProgress();

	return 0;
}

void ServerDBOpen::setProgressText(const CString& str)
{
	CStatic *txt = (CStatic*)freigabefortschritt.GetDlgItem(IDC_MSG);
	txt->SetWindowText(str); 
}


//Dialogelemente initialisieren
BOOL ServerDBOpen::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;
	((CButton*)GetDlgItem(IDC_ALLE_NL))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_SELECT_NEW))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_SORT_NL))->SetCheck(TRUE);


	CComboBox *nllist = (CComboBox*)GetDlgItem(IDC_NL);
	WaxSys::user_list((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER));
	for (POSITION pos = getStammdaten().get_niederlassungen().GetHeadPosition();pos;)
	{
		Niederlassungen::data *nl = (Niederlassungen::data*)getStammdaten().get_niederlassungen().GetNext(pos);
		if (nl->get_bezeichnung().GetLength())
		{
			CString nlsel;
			nlsel.Format("%s (%i)", nl->get_bezeichnung(), nl->get_nlnummer());
			int index = nllist->AddString(nlsel);
			nllist->SetItemData(index, nl->get_nlnummer());
		}
	}

	((CComboBox*)GetDlgItem(IDC_NL))->SetDroppedWidth(220);
	initItems();

	createProgressWnd();

	return TRUE;
}

void ServerDBOpen::set_status(Status::STATUS val)
{
	status = val;
}

void ServerDBOpen::createProgressWnd()
{
	CString str; 
	str = getStammdaten().getText("IDS_LADEFORTSCHRITT").c_str();
	freigabefortschritt.Create(str);
	freigabefortschritt.ShowWindow(false);
	freigabefortschritt.CenterWindow();
}

//Import starten
void ServerDBOpen::OnOK()
{
	string errorid = "";
	UpdateData();

	freigabefortschritt.ShowWindow(SW_SHOW);
	freigabefortschritt.SetForegroundWindow();
	
	CString str;
	CStatic *txt = (CStatic*)freigabefortschritt.GetDlgItem(IDC_MSG);
	str = getStammdaten().getText("IDS_VERBINDUNG").c_str();
	txt->SetWindowText(str);

	waloader->config((LPCSTR)createSQLSelect(), (LPCSTR)createSQLSort("werkauftrag"), WaxSys::get_username(), 
		load_storniert, status, (DWORD)this->m_hWnd, WM_STEPIT);

	WaLoader::RETVAL retval = waloader->prepare_load(m_ReadGUIDFile, m_ReadOnlyRecordsWithGUID);
	switch(retval)
	{
		case WaLoader::OK:
			break;
		case WaLoader::SELECT_RETURNED_NULL:
			errorid = "IDS_ERROR9b";
			break;
		case WaLoader::NO_SERVER_CONNECT:
			errorid = "IDS_ERROR6";
		default:
			errorid = "IDS_ERROR10b"; // Unbekannter Fehler, Systemadministrator verständigen
			break;
	}
	
	if (errorid.length() > 0) 
	{
		freigabefortschritt.ShowWindow(false);
		string msg;
		msg = getStammdaten().getText(errorid);
		AfxMessageBox(msg.c_str(), MB_OK | MB_ICONINFORMATION);
		return; // Wenn Fehler auftreten, wird der Dialog nicht beendet
	}
	// @todo Fehlerliste von WaLoader anzeigen

	makeProject();

	freigabefortschritt.ShowWindow(false);

	CDialog::OnOK();
}


//Zugriff auf den Dateinamen des erstellten Projektes
const CString& ServerDBOpen::get_fname() const
{
	return fname;
}


//Erstellt ein neues Projekt aus der Datenbank
bool ServerDBOpen::makeProject()
{
	CObList serverobjects;
	
	initProgress(waloader->get_count());

	vector<WaxPart*> wplist = waloader->load_objects();

	// Werkaufträge aus der CAD importieren
	GpCadConvert cadconv;
	CString faultMessage = cadconv.convert(wplist);
	if(!faultMessage.IsEmpty())
	{
		AfxMessageBox(faultMessage, MB_OK | MB_ICONINFORMATION);
	}

	for (vector<WaxPart*>::iterator it = wplist.begin(); it != wplist.end(); it++)
	{
		WaxPart* wpit = *it;

		setProgressText((CString)"Statusupdate: " + wpit->get_werkauftrag_const()->get_bezeichnung() + " Posnr: " + wpit->get_werkauftrag_const()->get_posNr());

		initObject(wpit);

		stepProgress();
	}

	GpSammelauftragCreator sammelcreator;
	wplist = sammelcreator.create(wplist, this->sortPreisanfrage ? GpSammelauftragCreator::Preisanfrage : GpSammelauftragCreator::SdNummer, *this);

	initProgress(wplist.size());
	
	for (vector<WaxPart*>::iterator it = wplist.begin(); it != wplist.end(); it++)
	{
		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(*it);
		if (sammel)
		{
			//Für den Sammelauftrag evt die Preise einlesen
			if (sammel->get_leitkarte().get_preisanfrage().get_nummer() > 0)
			{
				Server serv(Server::load, sammel->get_werkauftrag(), NULL);	
				serv.updatePreisanfrage();
			}
		}

		serverobjects.AddTail(*it);
	}

	//createSammelauftrag(serverobjects);
	CObList *project = sortProject(&serverobjects);	
	XASSERT(project);

	if (!project)
		return false;
	return serialize_project(project);
}



void ServerDBOpen::initProgress(int size)
{
	CProgressCtrl *prog = (CProgressCtrl*)freigabefortschritt.GetDlgItem(IDC_PROG_LADE);

	prog->SetPos(0);
	prog->SetRange(0, size);
	prog->SetStep(1);
}

void ServerDBOpen::stepProgress()
{
	CProgressCtrl *prog = (CProgressCtrl*)freigabefortschritt.GetDlgItem(IDC_PROG_LADE);
	prog->StepIt();
}


//Erzeugen des SQL Select Strings aus den Angaben in der Maske
const CString ServerDBOpen::createSQLSelect() const
{
	const int selmax = 5;
	CString selections[selmax];
	CButton* nl = (CButton*)GetDlgItem(IDC_EINZELNL);
	// Einzelne Niederlassung ausgewählt
	if (nl->GetCheck())
	{
		CComboBox* nlbx = (CComboBox*)GetDlgItem(IDC_NL);
		int nlnr = nlbx->GetItemData(nlbx->GetCurSel());
		if (nlnr == CB_ERR)
		{
			// In der Debug
			CString str;
			nlbx->GetWindowText(str); // Text wie eingetragen übernehmen#
			selections[0].Format("nl = %s", str);
		}
		else selections[0].Format("nl = %i", nlnr);
	}

	// Sachbearbeiter ausgewählt
	CString sachbearbeiterav;
	GetDlgItem(IDC_SACHBEARBEITER)->GetWindowText(sachbearbeiterav);
	
	if (sachbearbeiterav.GetLength())
	{
		CString str, str2; 
		str = getStammdaten().getText("IDS_KEIN_SACHBEARBEITER").c_str();
		str2 = getStammdaten().getText("IDS_ALLE_SACHBEARBEITER").c_str();
		if (sachbearbeiterav == str)
			selections[1] = "sachbesegment IS NULL";
		else if (sachbearbeiterav != str2)
			selections[1].Format("sachbesegment = '%s'", sachbearbeiterav);
	}

	// Button 'neue Werkaufträge ohne WA Nummer'
	if (((CButton*)GetDlgItem(IDC_SELECT_NEW))->GetCheck())
	{
		selections[2] = "wanr = 0";
	}
	// Button 'Werkaufträge in Bearbeitung'
	else if (((CButton*)GetDlgItem(IDC_SELECT_INBEARBEITUNG))->GetCheck())
	{
		selections[2] = "NOT wanr = 0";
	}
	// Button 'Werkaufträge in Bearbeitung'
	else if (((CButton*)GetDlgItem(IDC_SELECT_WA))->GetCheck())
	{
		CString wanr;
		GetDlgItem(IDC_WANUMMER)->GetWindowText(wanr);
		if (!atoi(wanr))
		{
			CString str; str = getStammdaten().getText("IDS_ERROR12b").c_str();
			AfxMessageBox(str);
			AfxThrowUserException();
		}
		selections[2].Format("wanr = %s", wanr);
	}

	MXDate freigabe_von, freigabe_bis;
	((CDateTimeCtrl*)GetDlgItem(IDC_FREIGABE_VON))->GetTime(freigabe_von);
	((CDateTimeCtrl*)GetDlgItem(IDC_FREIGABE_BIS))->GetTime(freigabe_bis);


	// Freigabezeitraum von-bis ausgewählt
	
	if (((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_VON))->GetCheck() && ((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_BIS))->GetCheck())
	{
		selections[3].Format("freigabedatum >= %i AND freigabedatum <= %i", freigabe_von.toNornamlizedInt(false), freigabe_bis.toNornamlizedInt(true));
	}
	else if (((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_VON))->GetCheck())
	{
		selections[3].Format("freigabedatum = %i", freigabe_von.toNornamlizedInt(false));
	}
	else if (((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_BIS))->GetCheck())
	{
		selections[3].Format("freigabedatum = %i", freigabe_bis.toNornamlizedInt(true));
	}

	// Die verschiedenen Selects zusammensetzen (UND verknüpft)
	CString sqlsel;
	for (int i = selmax-1; i >= 0; i--)
	{
		if (selections[i].GetLength())
		{
			if (sqlsel.GetLength()) 
				sqlsel = selections[i] + " AND " + sqlsel;
			else
				sqlsel = selections[i];
		}
	}
	return sqlsel;
}


//Erzeugen des SQL Sort Strings aus den Angaben in der Maske
const CString ServerDBOpen::createSQLSort(CString tablename) const
{
	/*
	Sortierung festlegen
	Diese Sortierung gilt nur für das Auslesen der Tabellen und dient nur der Vorsortierung/Performance.
	Die eigentliche Sortierung wird durch <mf .sortProject> vorgenommen.
	*/
	CString ret;
	if (!tablename.CompareNoCase("status"))
	{ 
		ret = "waid";
	}
	else if (!tablename.CompareNoCase("werkauftrag"))
	{
		if (((CButton*)GetDlgItem(IDC_SORT_WA))->GetCheck())
			ret = "classid, nl";
		else if (((CButton*)GetDlgItem(IDC_SORT_NL))->GetCheck())
			ret = "nl, classid";
		else
			ret =  "waid, classid";
	}

	TRACE("Sortierung: %s\n", ret);
	return ret;
}


BOOL ServerDBOpen::DestroyWindow()
{
	// Niederlassung
	CString nlsel ;
	GetDlgItem(IDC_NL)->GetWindowText(nlsel);
	AfxGetApp()->WriteProfileString("AV", "NL", ((CButton*)GetDlgItem(IDC_ALLE_NL))->GetCheck() ? "-1" : nlsel);

	//Sachbearbeiter
	CString sachb;
	((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER))->GetWindowText(sachb);
	AfxGetApp()->WriteProfileString("AV", "Sachbearbeiter", sachb);
	
	// Anordnung, Sortierung
	if (((CButton*)GetDlgItem(IDC_SORT_NL))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "Sortierung", 0);
	else if (((CButton*)GetDlgItem(IDC_SORT_WA))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "Sortierung", 1);
	else if (((CButton*)GetDlgItem(IDC_SORT_SACHBEARBEITER))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "Sortierung", 2);

	// WA Auswahl
	if (((CButton*)GetDlgItem(IDC_SELECT_NEW))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "wasel", 0);
	else if (((CButton*)GetDlgItem(IDC_SELECT_EGAL))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "wasel", 1);
	else if (((CButton*)GetDlgItem(IDC_SELECT_INBEARBEITUNG))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "wasel", 2);
	else if (((CButton*)GetDlgItem(IDC_SELECT_WA))->GetCheck())
		AfxGetApp()->WriteProfileInt("AV", "wasel", 3);
	
	CString wanr;
	GetDlgItem(IDC_WANUMMER)->GetWindowText(wanr);
	AfxGetApp()->WriteProfileString("AV", "wanr", wanr);

	// Freigabedatum
	AfxGetApp()->WriteProfileInt("AV", "freigabe_von_enabled", ((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_VON))->GetCheck());
	AfxGetApp()->WriteProfileInt("AV", "freigabe_bis_enabled", ((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_BIS))->GetCheck());

	return CWnd::DestroyWindow();
}



void ServerDBOpen::initItems()
{
	// Niederlassung
	CString nlsel = AfxGetApp()->GetProfileString("AV", "NL", "-1");
	if (nlsel == "-1")
	{
		((CButton*)GetDlgItem(IDC_ALLE_NL))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_EINZELNL))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_EINZELNL))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_ALLE_NL))->SetCheck(0);		
		((CComboBox*)GetDlgItem(IDC_NL))->SelectString(-1, nlsel);
	}
	//Sachbearbeiter
	CString sachb;
	sachb = AfxGetApp()->GetProfileString("AV", "Sachbearbeiter", "");
	((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER))->SetWindowText(sachb);
	
	// Anordnung, Sortierung
	int sort = AfxGetApp()->GetProfileInt("AV", "Sortierung", 0);
	((CButton*)GetDlgItem(IDC_SORT_NL))->SetCheck(sort == 0 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_SORT_WA))->SetCheck(sort == 1 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_SORT_SACHBEARBEITER))->SetCheck(sort == 2 ? TRUE : FALSE);

	// WA Auswahl
	int wasel = AfxGetApp()->GetProfileInt("AV", "wasel", 0);
	((CButton*)GetDlgItem(IDC_SELECT_NEW))->SetCheck(wasel == 0 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_SELECT_EGAL))->SetCheck(wasel == 1 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_SELECT_INBEARBEITUNG))->SetCheck(wasel == 2 ? TRUE : FALSE);
	((CButton*)GetDlgItem(IDC_SELECT_WA))->SetCheck(wasel == 3 ? TRUE : FALSE);
	CString wanr = AfxGetApp()->GetProfileString("AV", "wanr");
	GetDlgItem(IDC_WANUMMER)->SetWindowText(wanr);
	GetDlgItem(IDC_WANUMMER)->EnableWindow(wasel == 3);


	// Freigabedatum
	MXDate freigabe_von, freigabe_bis;

	freigabe_von = MXDate::GetCurrentTime();
	freigabe_bis = MXDate::GetCurrentTime();
	COleDateTimeSpan yesterday;
	yesterday.SetDateTimeSpan(-1, 0, 0, 0);
	freigabe_von += yesterday;

	((CDateTimeCtrl*)GetDlgItem(IDC_FREIGABE_VON))->SetTime(freigabe_von);
	((CDateTimeCtrl*)GetDlgItem(IDC_FREIGABE_BIS))->SetTime(freigabe_bis);

	((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_VON))->SetCheck(AfxGetApp()->GetProfileInt("AV", "freigabe_von_enabled", 0));
	((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_BIS))->SetCheck(AfxGetApp()->GetProfileInt("AV", "freigabe_bis_enabled", 0));


	OnClickedEnable_freigabe_von();
	OnClickedEnable_freigabe_bis();

	// Auswahl der Sachbearbeiter erweitern um "Kein Sachbearbeiter zugeordnet"
	CString str;
	str = getStammdaten().getText("IDS_KEIN_SACHBEARBEITER").c_str();
	((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER))->AddString(str);
	str = getStammdaten().getText("IDS_ALLE_SACHBEARBEITER").c_str();
	((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER))->AddString(str);
}



//Checkbox zum Enablen/Disablen des 'Freigabedatum von' DateControls. Ein 
//freigeschaltetes Freigabedatum geht dann in die SQL Select Anweisung ein.
void ServerDBOpen::OnClickedEnable_freigabe_von()
{
	BOOL enable = ((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_VON))->GetCheck();
	GetDlgItem(IDC_FREIGABE_VON)->EnableWindow(enable);
}



//Checkbox zum Enablen/Disablen des 'Freigabedatum bis' DateControls.
void ServerDBOpen::OnClickedEnable_freigabe_bis()
{
	BOOL enable = ((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_BIS))->GetCheck();
	GetDlgItem(IDC_FREIGABE_BIS)->EnableWindow(enable);
}



//Odfasdfadsfbjekte sortieren nach NL, WANR usw.

//Objekte sortieren nach NL, WANR usw.
CObList *  ServerDBOpen::sortProject(CObList * list)
{
	int order = IDC_SORT_NL;

	if (((CButton*)GetDlgItem(IDC_SORT_WA))->GetCheck())
		order = IDC_SORT_WA;
	else if (((CButton*)GetDlgItem(IDC_SORT_SACHBEARBEITER))->GetCheck())
		order = IDC_SORT_SACHBEARBEITER;
	CObList *sorted = NULL, *ret;
	ret = new CObList;
	switch (order)
	{
		case IDC_SORT_WA: 
			if (!sortPreisanfrage)
				sorted = doClassSort(list);
			else 
				sorted = doPreisanfrageSort(list);
			break;
		case IDC_SORT_NL:
			sorted = doNlSort(list);
			break;
		case IDC_SORT_SACHBEARBEITER:
			sorted = doSachbearbeiterSort(list);
			break;
		default:
			XASSERT(FALSE);
	}

	PrjGliederung *g = new PrjGliederung;
	CString str; str = getStammdaten().getText("IDS_DATENBANKABFRAGE").c_str();
	g->get_werkauftrag()->set_bezeichnung(str);
	g->kommentar.Format("Select: %s\r\nSort:%s", createSQLSelect(), createSQLSort("werkauftrag"));
	ret->AddHead(g);
	if (sorted)
	{
		for (POSITION pos = sorted->GetHeadPosition(); pos; )
		{
			g->addChild((WaxPart*)sorted->GetNext(pos), false);
		}
		
		sorted->RemoveAll();
		delete sorted;
	}
	
	
	return ret;
}



//Sortiert die Werkaufträge nach Klassen
CObList* ServerDBOpen::doClassSort(CObList* list)
{
	CObList *newlst = new CObList;
	// Zuerst wird die Liste nach Klassen durchsucht
	CStringList runtimeclasses;
	for (POSITION pos = list->GetHeadPosition(); pos; )
	{  
		CObject *obj = list->GetNext(pos);
		XASSERT(obj); if (!obj) continue;

		WaxPart *pt = dynamic_cast<WaxPart *>(obj);
		XASSERT(pt); if (!pt) continue;
		CString classname = pt->GetRuntimeClass()->m_lpszClassName;
		if (classname == "SammelAuftrag")
		{
			classname = ((WaxPart *)pt->get_subobjects().GetHead())->GetRuntimeClass()->m_lpszClassName;
		}
		if (runtimeclasses.Find(classname) == NULL)
			runtimeclasses.AddTail(classname);
	}

	// Jetzt nach gefundenen Klassen sortieren
	for (POSITION x = runtimeclasses.GetHeadPosition(); x; )
	{
		CString classname = runtimeclasses.GetNext(x);
		PrjGliederung *g = NULL;
		for (POSITION pos = list->GetHeadPosition(); pos; )
		{  
			POSITION plast = pos;
			CObject *obj = list->GetNext(pos);
			if (!obj) continue;

			WaxPart *pt = dynamic_cast<WaxPart *>(obj);
			XASSERT(pt); if (!pt) continue;
			
			CString ptclassname = (CString)pt->GetRuntimeClass()->m_lpszClassName;
			CString gliederungsbez = pt->get_werkauftrag()->get_stdbezeichnung();
			if (ptclassname == "SammelAuftrag")
			{
				ptclassname = ((WaxPart *)pt->get_subobjects().GetHead())->GetRuntimeClass()->m_lpszClassName;
				gliederungsbez = ((WaxPart *)pt->get_subobjects().GetHead())->get_werkauftrag()->get_stdbezeichnung();
			}
				
			if (classname == ptclassname)
			{// Eintrag gefunden
				if (!g) // Noch kein Eintrag für diese Klasse
				{
					g = new PrjGliederung;
					g->get_werkauftrag()->set_bezeichnung(gliederungsbez);
					g->kommentar = classname;
				}
				g->addChild(pt, false);
				list->SetAt(plast, (CObject*) NULL); 
			}
		}
		if (g)
			newlst->AddTail(g);
	}
	return newlst;
}

//Sortiert die Werkaufträge nach Niederlassungen
CObList* ServerDBOpen::doNlSort(CObList* list)
{
	CObList *newlst = new CObList;
	// Alle Niederlassungen der Reihe nach durchgehen
	for (POSITION p = getStammdaten().get_niederlassungen().GetHeadPosition(); p;)
	{
		Niederlassungen::data *dat = (Niederlassungen::data *)getStammdaten().get_niederlassungen().GetNext(p);
		PrjGliederung *g = NULL;
		for (POSITION pos = list->GetHeadPosition(); pos; )
		{ // Liste mit Werkaufträgen durchgehen, ob von dieser NL etwas dabei ist
			POSITION plast = pos;
			CObject *obj = list->GetNext(pos);
			if (!obj)
				continue;
			WaxPart *pt = (dynamic_cast<WaxPart *>(obj));
			XASSERT(pt); if (!pt) continue;
			if (pt->get_leitkarte().get_prj_nl() == dat->get_nlnummer())
			{// Eintrag gefunden
				if (!g) // Noch kein Eintrag für diese NL
				{
					g = new PrjGliederung;
					g->get_leitkarte().set_prj_nl(dat->get_nlnummer());
					CString bez; 
					bez.Format("%s (%i)", dat->get_bezeichnung(), dat->get_nlnummer());
					g->get_werkauftrag()->set_bezeichnung(bez);
					g->kommentar = getStammdaten().getText("IDS_NEUE_FREIGABEN").c_str();
				}
				g->addChild(pt, false);
				list->SetAt(plast, (CObject*) NULL); 
			}
		}
		if (g)
			newlst->AddTail(g);
	}

	// Prüfen, ob alle Einträge abgearbeitet wurden
	PrjGliederung *g = NULL;
	for (POSITION pos = list->GetHeadPosition(); pos; )
	{ 
		CObject *obj = list->GetNext(pos);
		if (!obj)
			continue;
		WaxPart *pt = (dynamic_cast<WaxPart *>(obj));
		XASSERT(pt); if (!pt) continue;
		if (!g) // Noch kein Eintrag für diese NL
		{
			g = new PrjGliederung;
			g->get_leitkarte().set_prj_nl(pt->get_werkauftrag()->get_leitkarte().get_prj_nl());
			CString bez;
			bez.Format("???? (%i)", pt->get_werkauftrag()->get_leitkarte().get_prj_nl());
			g->get_werkauftrag()->set_bezeichnung(bez);
			g->kommentar = getStammdaten().getText("IDS_NEUE_FREIGABEN").c_str();
		}
		g->addChild(pt, false);
	}
	if (g)
		newlst->AddTail(g);
	return newlst;
}



//Sortiert die Werkaufträge nach Preisanfragen
CObList* ServerDBOpen::doPreisanfrageSort(CObList* list)
{
	CObList *newlst = new CObList;
	// Zuerst wird die Liste nach Preisanfragen durchsucht
	
	std::list<int> paNummern;
	for (POSITION pos = list->GetHeadPosition(); pos; )
	{  
		CObject *obj = list->GetNext(pos);
		XASSERT(obj); if (!obj) continue;

		WaxPart *pt = dynamic_cast<WaxPart *>(obj);
		XASSERT(pt); if (!pt) continue;
		int paNr = pt->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer();

		//Prüfen ob Nummer schon in der Liste steht
		bool found = false;
		std::list<int>::iterator iter = paNummern.begin();
		while (iter != paNummern.end())
		{
			if (paNr == *iter)
			{
				found = true;
				break;
			}

			iter++;
		}
		
		if (!found)
			paNummern.push_back(paNr);
	}

	//Gültigbis bei Preisanfage
	MXDate gueltigBis;
	int frist = 14;
	COleDateTimeSpan diff(frist, 0, 0, 0);	
	gueltigBis = gueltigBis.get_current_date();
	gueltigBis += diff;

	// Jetzt nach gefundenen PaNummern sortieren
	std::list<int>::iterator iter = paNummern.begin();
	while (iter != paNummern.end())
	{
		int paNr1 = *iter;

		WpPreisanfrage *g = NULL;
		for (POSITION pos = list->GetHeadPosition(); pos; )
		{  
			POSITION plast = pos;
			CObject *obj = list->GetNext(pos);
			if (!obj) continue;

			WaxPart *pt = dynamic_cast<WaxPart *>(obj);
			XASSERT(pt); if (!pt) continue;
			
			int paNr2 = pt->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer();
			if (paNr1 == paNr2)
			{// Eintrag gefunden
				if (!g) // Noch kein Eintrag für diese NL
				{
					g = new WpPreisanfrage;
					g->kommentar = "";
					
					//Daten von 1. Position übernehmen
					g->get_werkauftrag()->get_leitkarte().copy(pt->get_werkauftrag()->get_leitkarte());
					g->get_werkauftrag()->get_leitkarte().get_preisanfrage().set_nummer(paNr1);
					g->get_werkauftrag()->get_leitkarte().get_preisanfrage().set_gueltigBis(gueltigBis);
				}
				g->addChild(pt, false);
				list->SetAt(plast, (CObject*) NULL); 
			}
		}
		if (g)
		{
			//Datum noch an Unterordner übergeben
			g->copyGueltigBisToChilds(g, gueltigBis);

			newlst->AddTail(g);
		}

		iter++;
	}
	return newlst;
}






//Sortiert die Liste mit Werkaufträgen nach Sachbearbeitern
CObList* ServerDBOpen::doSachbearbeiterSort(CObList* list)
{
	CObList *newlst = new CObList;
	// Zuerst wird die Liste nach Sachbearbeitern durchsucht
	CStringList sachbearbeiter;
	for (POSITION pos = list->GetHeadPosition(); pos; )
	{  
		CObject *obj = list->GetNext(pos);
		XASSERT(obj); if (!obj) continue;

		WaxPart *pt = dynamic_cast<WaxPart *>(obj);
		XASSERT(pt); if (!pt) continue;
		CString sachbe = pt->get_werkauftrag()->get_status().get_sachbearbeiterSegment();
		if (sachbearbeiter.Find(sachbe) == NULL)
			sachbearbeiter.AddTail(sachbe);
	}

	// Jetzt nach gefundenen Sachbearbeitern sortieren
	for (POSITION x = sachbearbeiter.GetHeadPosition(); x; )
	{
		CString avtyp = sachbearbeiter.GetNext(x);
		PrjGliederung *g = NULL;
		for (POSITION pos = list->GetHeadPosition(); pos; )
		{  
			POSITION plast = pos;
			CObject *obj = list->GetNext(pos);
			if (!obj) continue;

			WaxPart *pt = dynamic_cast<WaxPart *>(obj);
			XASSERT(pt); if (!pt) continue;
			
			CString sachbe = (CString)pt->get_werkauftrag()->get_status().get_sachbearbeiterSegment();
			if (sachbe == avtyp)
			{// Eintrag gefunden
				if (!g) // Noch kein Eintrag für diese NL
				{
					g = new PrjGliederung;
					CString bez;
					bez.Format("%s", pt->get_werkauftrag()->get_status().get_sachbearbeiterSegment());
					if (bez.IsEmpty())
						bez = getStammdaten().getText("IDS_NICHT_ZUGEORDNET").c_str();
					g->get_werkauftrag()->set_bezeichnung(bez);
					g->kommentar = "";

				}
				g->addChild(pt, false);
				list->SetAt(plast, (CObject*) NULL); 
			}
		}
		if (g)
			newlst->AddTail(g);
	}
	return newlst;

}


//Indicates the user clicked a button
void ServerDBOpen::OnClickedSelect_new()
{
	GetDlgItem(IDC_WANUMMER)->EnableWindow(false);	
}

//Indicates the user clicked a button
void ServerDBOpen::OnClickedSelect_egal()
{
	GetDlgItem(IDC_WANUMMER)->EnableWindow(false);	
}

//Indicates the user clicked a button
void ServerDBOpen::OnClickedSelect_inbearbeitung()
{
	GetDlgItem(IDC_WANUMMER)->EnableWindow(false);	
}

//Indicates the user clicked a button
void ServerDBOpen::OnClickedSelect_wa()
{
	GetDlgItem(IDC_WANUMMER)->EnableWindow(true);
	((CComboBox*)GetDlgItem(IDC_SACHBEARBEITER))->SetCurSel(0);
	((CButton*)GetDlgItem(IDC_EINZELNL))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_ALLE_NL))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_VON))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_ENABLE_FREIGABE_BIS))->SetCheck(0);
}

// Initialisierung von neu geladenen Objekten
void ServerDBOpen::initObject(WaxPart *pt)
{
	/*
	Aufruf erfolgt in loadObject nach der Initialisierung aus der Datenbank.
	Default wird der Status eingelesen
	*/
	GpStatusUpdate statup;

	statup.readStatus(pt);
}






bool ServerDBOpen::serialize_project(CObList* project)
{
	bool ret = true;
	TRY
	{
		FileLoader prj(MAGIC);
		fname = WaxSys::GetTempDir() + "Werkaufträge.wax";
		int err = prj.open_file((LPCSTR)fname, FileLoader::STORE);
		if (err)
		{
			MsgBox::show_error(prj.get_error_text(err));
			return false;
		}
		BinaryArchive *ar = prj.get_arch(FileLoader::STORE);
			
		for (POSITION pos = project->GetHeadPosition(); pos; )
		{
			WaxPart *pt = dynamic_cast<WaxPart *>(project->GetNext(pos));
			XASSERT(pt);
			if (!pt)
				continue;
			*ar << (int)1;
			ar->write_class(pt->GetRuntimeClass());
			pt->serialize(*ar);
		}
		*ar << (int)0;
		prj.store(false, false);
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		ret = false;
	}
	END_CATCH_ALL

	for (POSITION pos = project->GetHeadPosition(); pos; )
		delete project->GetNext(pos);
	delete project;
	return ret;
}



void ServerDBOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate)
	{
		CButton *storno = (CButton*)GetDlgItem(IDC_STORNIERT);
		XASSERT(storno);
		if (storno)
		// Storniertflag wird nur hier benutzt, bisher
			load_storniert = storno->GetCheck() ? true : false;
	}
}


