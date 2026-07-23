#include "stdafx.h"
#include "resource.h"

#include "childfrm.h"
#include "MainFrm.h"
#include "waxDoc.h"



#include <mxcontrols/FileLoader.h>

#include "StatusAktualisierung.h"
#include "docver.h"
#include "..\waxprj\BefehlUpdateStatistik.h"
#include "..\waxprj\GpSapAuftragsdaten.h"


#include <mapi.h>

#include "../waxprj/Fortschrittanzeige.h"
#include "../waxprj/GpInitialUpdateAfos.h"

#include <mxutil/strloader.h>
#include <mxcontrols/msgbox.h>
#include <mxcontrols/OpenFileDlg.h>



using namespace MxGui;
using namespace MxUtil;
using namespace MxFilesystem;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWaxDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaxDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, on_mail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, on_update_mail)
	ON_COMMAND(WAXID_PRJLEITKARTE, on_copy_prjleitkarte)
	ON_COMMAND(ID_STATUS_AKTUALISIEREN, OnStatusAktualisieren)
	ON_UPDATE_COMMAND_UI(ID_STATUS_AKTUALISIEREN, on_update_statusaktualisieren)
	ON_COMMAND(ID_FILE_CLOSE, on_close_file)
	ON_COMMAND(WAXID_UPDATESTATISTIK,on_updateStatistik)
	ON_COMMAND(WAXID_UPDATEKCNUMMER,on_updateKcNummer)
	ON_COMMAND(WAXID_UPDATEAFOS, on_update_afos)
END_MESSAGE_MAP()

CWaxDoc::CWaxDoc()
: prjfile(MAGIC)
{
	active_object = NULL;
	disable_autofreigabe = false;
	set_startdate();
	connect_signals();
	progress = NULL;
}


CWaxDoc::~CWaxDoc()
{	
	/**
	 Löscht alle Objekte aus objects und damit auch alle Unterobjekte
	*/
	for (int i = 0; i < objects.GetSize( ); i++)
		delete objects.GetAt(i);
}

void CWaxDoc::set_startdate()
{
	dtstart = MXDate::get_current_date();
}

bool CWaxDoc::set_active_object(CObject *object)
{
	/**
	Aktives Objekt setzen
	Diese Methode wird von CLeftView::OnRButtonDown und CLeftView::OnLButtonDown aufgerufen
	und liefert true wenn die Daten in der Eingabemaske (EingabeView) korrekt aktualisiert wurden
	*/
	WaxPart *oldobj = (WaxPart*)active_object;
	bool ret = true;
	
	if (object && object != oldobj)
	{
		active_object = (WaxPart*)object;
		CWnd *child = AfxGetApp()->GetMainWnd();
		CMainFrame *mfrm = dynamic_cast<CMainFrame*>(child);
		XASSERT(mfrm);
		child = mfrm->GetActiveFrame();
		CChildFrame* frm = dynamic_cast<CChildFrame*>(child);
		
		show_hinweis(oldobj);

		XASSERT(frm);
		ret = frm->onUpdateView();
		if (!ret)
		{
			active_object = oldobj;
			frm->SetFocus();
		}
	}
	return ret;
}


CMenu* CWaxDoc::GetMenu() const
{
	/**
	Liefert das Menü des aktuellen Objektes zurück
	*/
	if (active_object)
		return ((WaxPart*)active_object)->get_menu();
	return NULL;
}



void CWaxDoc::on_update_mail(CCmdUI* pCmdUI) 
{
	/**
	Abfrage, ob Email überhaupt verfügbar ist
	*/
	int mailavail = ::GetProfileInt(_T("MAIL"), _T("MAPI"), 0) != 0 &&
			SearchPath(NULL, _T("MAPI32.DLL"), NULL, 0, NULL, NULL) != 0;
	WaxPart *pt = dynamic_cast<WaxPart*>(active_object);
	if (!pt || dynamic_cast<PrjLeitkarteData*>(pt) || !pt->get_plausiOk())
	{
		mailavail = false;
	}
	pCmdUI->Enable(mailavail);
}


//Versendet eine Email mit der Projektdatei als Attachment
void CWaxDoc::on_mail() 
{
	/*
	Datei als Email senden
	Dieser Code ist aus dem MFC-Sourcecode kopiert und erweitert
	  Vorbelegung der Felder Empfänger und Betreff aus den Systemparametern:
	  EMAIL_SUBJECT -> Betreffzeile
	  EMAIL_ADRESS -> Email Adresse des Empfängers
	  EMAIL_DISPLAYNAME -> Angezeigter Name des Empfängers
	*/
	CWaitCursor wait;

	HINSTANCE m_hInstMail = NULL;
	if (m_hInstMail == NULL)
		m_hInstMail = ::LoadLibrary("MAPI32.DLL");

	if (m_hInstMail == NULL)
	{
		MsgBox::show(AFX_IDP_FAILED_MAPI_LOAD, MsgBox::Error);
		return;
	}
	XASSERT(m_hInstMail != NULL);

	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(m_hInstMail, "MAPISendMail");
	if (lpfnSendMail == NULL)
	{
		MsgBox::show(AFX_IDP_INVALID_MAPI_DLL, MsgBox::Error);
		::FreeLibrary(m_hInstMail);
		return;
	}
	XASSERT(lpfnSendMail != NULL);

	WaxPart *pt = dynamic_cast<WaxPart*>(active_object);
	XASSERT(pt);
	if (!pt)
		return;
	if (!pt->testLeitkarte(false))
		return;
	CString fn = WaxSys::GetTempFilename("wax").c_str();
	CFile f(fn, CFile::modeReadWrite | CFile::shareExclusive );
	pt->saveToFile(&f);
	f.Close();


	// prepare the file description (for the attachment)
	MapiFileDesc fileDesc;
	memset(&fileDesc, 0, sizeof(fileDesc));
	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = fn.GetBuffer(0);
	fileDesc.lpszFileName = fn.GetBuffer(0);

	MapiRecipDesc recip;
	CString smtp;
	smtp.Format("SMTP:%s", WaxSys::para_str(EMPFAENGER));
	CString displayname(WaxSys::para_str(DISPLAYNAME));
	memset(&recip, 0, sizeof(recip));
	recip.ulRecipClass = MAPI_TO;
	recip.lpszName = (LPSTR)(LPCTSTR)displayname;
	recip.lpszAddress = (LPSTR)(LPCTSTR)smtp;
	
		
	// prepare the message (empty with 1 attachment)
	MapiMessage message;
	CString subject(WaxSys::para_str(BETREFF));
	memset(&message, 0, sizeof(message));
	message.nFileCount = 1;
	message.lpFiles = &fileDesc;
	message.lpRecips = &recip;
	message.nRecipCount = 1;
	message.lpszSubject = (LPSTR)(LPCSTR)subject;

	// prepare for modal dialog box
	//AfxGetApp()->EnableModeless(FALSE);
	HWND hWndTop;
	CWnd* pParentWnd = CWnd::GetSafeOwner(NULL, &hWndTop);

	// some extra precautions are required to use MAPISendMail as it
	// tends to enable the parent window in between dialogs (after
	// the login dialog, but before the send note dialog).
	pParentWnd->SetCapture();
	::SetFocus(NULL);
	pParentWnd->m_nFlags |= WF_STAYDISABLED;

	int nError = lpfnSendMail(0, (ULONG)pParentWnd->GetSafeHwnd(),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

	// after returning from the MAPISendMail call, the window must
	// be re-enabled and focus returned to the frame to undo the workaround
	// done before the MAPI call.
	::ReleaseCapture();
	pParentWnd->m_nFlags &= ~WF_STAYDISABLED;

	pParentWnd->EnableWindow(TRUE);
	::SetActiveWindow(NULL);
	pParentWnd->SetActiveWindow();
	pParentWnd->SetFocus();
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
//	AfxGetApp()->EnableModeless(TRUE);

	if (nError != SUCCESS_SUCCESS &&
		nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
	{
		MsgBox::show(AFX_IDP_FAILED_MAPI_SEND, MsgBox::Error);
	}

	::FreeLibrary(m_hInstMail);
}

void CWaxDoc::get_updatelist(vector<const WaxPart*>& lst) const
{
	/**
	Erstellt eine Gesamtliste aller geänderten Werkaufträge im Projekt
	*/
	lst.clear();
	for (int i = 0; i < objects.GetSize(); i++)
	{
		const WaxPart* pt = dynamic_cast<const WaxPart*>(objects.GetAt(i));
		XASSERT(pt);
		if (pt)
			pt->get_changelist(lst, dtstart);
	}
}
	
bool CWaxDoc::is_modified() const
{
	vector<const WaxPart*> lst;
	get_updatelist(lst);
	
	return lst.size() ? true : false;	
}

//Prüfung, ob das Dokument geschlossen werden kann
BOOL CWaxDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	/**
	Speichern des Dokumentes
	Bevor das Dokument gespeichert wird, müssen alle Views ihre Daten aktualisieren
	*/
	update_all_views();

	if (is_modified())
	{
		//auto_freigabe(lst);
		// Beim Freigeben könnten Daten geändert worden sein. Die müssen natürlich auch mitgespeichert werden
		update_all_views();
	}

	// Speichern unter oder neue Datei speichern: Alte Datei schliessen (falls vorhanden) und neue öffnen
	if (prjfile.is_new_file() || prjfile.get_file_name() != (string)lpszPathName )
	{
		string fn = lpszPathName;
		if (!fn.length())
		{
			Filter flt(MAGIC);
			flt.set_dir(WaxSys::para_cstr(PROJEKTE));
			flt.set_extension("wax");
			flt.set_mask("Wax+ Projekte (*.wax)|*.wax||");
			OpenFileDlg ofn(flt);

			fn = ofn.get_file(true);
			if (!fn.length())
				return false;
		}

		int err = prjfile.open_file(fn, FileLoader::STORE);
		if (err)
		{
			if (err != FileLoader::ABORTED) 
				MsgBox::show(prjfile.get_error_text(err), MsgBox::Error);
			return false;
		}
	}

	XASSERT(!prjfile.is_new_file());

	TRY
	{
		int lst = 0;
		BinaryArchive* ar = prjfile.get_arch(FileLoader::STORE);
		while (lst < objects.GetSize())
		{
			if (!((WaxPart*)objects[lst])->get_deleted())
			{
				*ar << (int)1;
				ar->write_class(objects[lst]->GetRuntimeClass());
				((WaxPart*)objects[lst++])->serialize(*ar);
			}
			else
				lst++;
		}
		*ar << (int)0;
		prjfile.store();
		set_startdate(); // Änderungsdatum zurücksetzen
	}
	CATCH_ALL(e)
	{
		e->ReportError();
		return false;
	}
	END_CATCH_ALL
	
	
	return true;
}




void CWaxDoc::update_all_views()
{
	/**
	Durchläuft alle Ansichten/Views und sorgt für ein Speichern der Daten. Wird 
	lst übergeben, wird die Liste mit den Objektkten gefüllt, die geändert 
	wurden.
	*/

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		pView->UpdateData(TRUE);
	}
}

void CWaxDoc::UpdateAllViews(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CDocument::UpdateAllViews(pSender, lHint, pHint);
	ASSERT(pSender == NULL || !m_viewList.IsEmpty());
		// must have views if sent by one of them
/*
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		CHtmlView* html = dynamic_cast<CHtmlView*>(pView);
		if (html)
			html->Refresh();
	}
*/
}


void CWaxDoc::on_copy_prjleitkarte()
{
	/**
	Leitkarte für alle Objekte setzen
	Ruft für alle WaxPart Objekte in objects <mf WaxPart::setPrjLeitkarte> auf.
	Diese Methode darf nur von <c PrjLeitkarteForm> aufgerufen werden.
	Es wird geprüft, ob dies ein normales NL Projekt ist und überhaupt ein Objekt vorhanden ist.
	Falls nicht wird eine Assertion erzeugt. 
	*/
	if (objects.GetSize()<1 || !dynamic_cast<PrjLeitkarteData*>(objects.GetAt(0)))
		return;
	const Leitkarte& lt = ((PrjLeitkarteData*)objects.GetAt(0))->get_leitkarte();
	for (int i = 1; i < objects.GetSize(); i++)
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(objects.GetAt(i));
		XASSERT(pt);
		if (pt)
			pt->setPrjLeitkarte(lt);
	}
}



void CWaxDoc::DeleteContents()
{
	/**
	Inhalt des Dokumentes löschen
	*/
	CDocument::DeleteContents();
	for (int i = 0; i < objects.GetSize( ); i++)
		delete objects.GetAt(i);
	objects.RemoveAll();
}



BOOL CWaxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	XASSERT(!objects.GetSize());
	prjfile.close();
	objects.Add(new PrjLeitkarteData);
	objects.Add(new PrjGliederung);
	active_object = (WaxPart*)objects.GetAt(0);

	return TRUE;
}


void CWaxDoc::OnStatusAktualisieren()
{
	/**
	Aktualisiert den Status des gerade ausgewählten Werkauftrages
	*/
	StatusAktualisierung stat(objects, dynamic_cast<WaxPart*>(active_object));
	if (!stat.get_update_count())
		return;
	stat.DoModal();
	
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* vw = GetNextView(pos);
		vw->Invalidate();
		vw->UpdateData(FALSE);
		vw->PostMessage(WM_COMMAND, WAXID_DOTREEUPDATE);	
	}   
}

///Hier wird der Menüpunkt "Status aktualisieren" an oder abgeblendet.
void CWaxDoc::on_update_statusaktualisieren(CCmdUI* pCmdUI)
{
	/**
	Abgeblendet wird, wenn kein Objekt eine Aktualisierung nötig hat, d.h.
	wenn noch kein Wa in diesem Projekt freigegeben wurde.
	*/
	StatusAktualisierung stat(objects, NULL);
	pCmdUI->Enable(stat.get_update_count());
}

void CWaxDoc::on_close_file()
{
	/**
	Überschreibung von CDocument::OnFileClose. Leider wird dort nur das 
	Modified Flag geprüft, es wird aber nicht gesetzt (über CanCloseFrame).
	*/
	CFrameWnd *frm = dynamic_cast<CFrameWnd*>(AfxGetApp()->GetMainWnd());
	if (!frm)
		return; // Lieber nichts machen
	CDocument::OnFileClose();
}


BOOL CWaxDoc::IsModified()
{
	/*
	Prüfung, ob Änderungen abgespeichert werden müssen
	Bevor das Dokument geschlossen werden kann, müssen alle Ansichten ihre Daten speichern
	*/

	SetModifiedFlag(FALSE);

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		pView->UpdateData(TRUE);
	}

	if (is_modified())
		SetModifiedFlag();
	return CDocument::IsModified();
}


BOOL CWaxDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	XASSERT(prjfile.is_new_file());

	string fn = lpszPathName;
	if (!fn.length())
	{
		Filter flt(MAGIC);
		flt.set_dir(WaxSys::para_cstr(PROJEKTE));
		flt.set_extension("wax");
		flt.set_mask("Wax+ Projekte (*.wax)|*.wax|Wax+ Teilprojekte (*.wtp)|*.wtp");
		OpenFileDlg ofn(flt);

		fn = ofn.get_file();
		if (!fn.length())
			return false;
	}
	
	TRY
	{
		int error = prjfile.open_file(fn, FileLoader::LOAD | FileLoader::TRY_READONLY);
		if (!error)
		{
			if (WaxSys::isAV())
			{
				boost::filesystem::remove(fn + ".lock");
			}
			error = prjfile.load();
		}


		if (error)
		{
			if (error != FileLoader::ABORTED) // Abbruch durch Benutzer wird ignoriert
				MsgBox::show(prjfile.get_error_text(error), MsgBox::Error);
			return false;
		}

		

		BinaryArchive* ar = prjfile.get_arch(FileLoader::LOAD);
		int dummy;
		for (int i = 0; i < objects.GetSize( ); i++)
			delete objects.GetAt(i);
		objects.RemoveAll();
				
		while (1)
		{
			*ar >> dummy;
			if (dummy != 1 && dummy != WaxPart::FILETOC)
				break;// Letztes Objekt wurde gelesen oder Dateifehler
			if (dummy == WaxPart::FILETOC)
			{
			// Ein WaxPart wurde über Datei/Öffnen geladen. Jetzt muss eine Projektleitkarte 
			// und eine Defaultgliederung eingefügt werden

				objects.Add(new PrjLeitkarteData);
				objects.Add(new PrjGliederung);
			}

			UINT schema = 0;
			CObject *xload = ar->read_class(NULL, &schema)->CreateObject();
			WaxPart *load = dynamic_cast<WaxPart*>(xload);
			XASSERT(load);
			if (!load)
				break;

			if (load->IsKindOf(RUNTIME_CLASS(WaxPart)))
			{
				load->GetRuntimeClass()->m_wSchema = schema;
				load->set_version(schema);
			}
			
			load->serialize(*ar);
			
			if (dummy == WaxPart::FILETOC)
			{
				WaxPart *gliederung = (WaxPart*)objects.GetAt(1);
				load->set_parent(gliederung);
				gliederung->get_subobjects().AddTail(load);

				((PrjLeitkarteData*)objects.GetAt(0))->get_leitkarte().copy(load->get_leitkarte());
				dummy = 0;
				break; // Nur ein WaxPart pro Datei zulässig
			}
			else
				objects.Add(load);
		}
		if (dummy != 0)
		{
			MsgBox::show_error(getStammdaten().getText("IDS_ERROR12c"));
			return FALSE;
		}
	}
	CATCH_ALL(e)
	{
		prjfile.close();
		e->ReportError();
		return false;
	}
	END_CATCH_ALL

	if (prjfile.get_fileinfo().get_readonly())
		MsgBox::show(getStammdaten().getText("IDS_HINWEIS7"));
	
	active_object = (WaxPart*)objects.GetAt(0);
	return TRUE;
}



BOOL CWaxDoc::DoSave(LPCTSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;
	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		return FALSE;
	}

	
	// reset the title and change the document name
	if (bReplace)
		SetPathName(prjfile.get_file_name().c_str());

	return TRUE;        // success}
}


void CWaxDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	/**
	Ruft CDocument::SetPathName auf. Entweder mit dem Namen des geöffneten Files
	oder mit der Erweiterung "Kopie von", wenn die Datei schreibgeschützt geöffnet wurde
	*/

	UNREFERENCED_PARAMETER(lpszPathName);
	string copy_of;
	if (prjfile.get_fileinfo().get_readonly())
	{
		copy_of = getStammdaten().getText("IDS_KOPIE_VON");
		bAddToMRU = false;
	}

	CDocument::SetPathName((copy_of + prjfile.get_file_name()).c_str(), bAddToMRU);
}


void CWaxDoc::doAutosave(bool disable)
{
	disable_autofreigabe = disable;
	DoSave(prjfile.get_file_name().c_str(), true); 
	disable_autofreigabe = false;
}

const PrjLeitkarteData* CWaxDoc::get_prjleitkarte() const
{
	/**
	Liefert, falls vorhanden, die Projektleitkarte zurück
	*/
	const WaxPart *pat = dynamic_cast<WaxPart*>(objects.GetAt(0));
	if (pat) 
	{
		return dynamic_cast<const PrjLeitkarteData*>(pat); 
	}
	return NULL;
}

WaxPart* CWaxDoc::get_active_object() 
{ 
	return dynamic_cast<WaxPart*>(active_object); 
}


/**
Liste mit den Root WaxPart Objekten zurückgeben

Inhalt ist identisch mit get_objects. (ToDo: get_objects sollte gegen get_waxPartListe getauscht werden)
*/
WaxPartListe& CWaxDoc::get_waxPartListe()
{
	CObArray& arWaxPart = get_objects();


	//m_waxPartListe.resetListe();
	m_waxPartListe.RemoveAll();

	CObject *pWaxPart = NULL;
	for (int i = 0; i < arWaxPart.GetSize(); i++)
	{
		pWaxPart = objects.GetAt(i);
		if (!pWaxPart)
			continue;

		//m_waxPartListe.append(pWaxPart);
		m_waxPartListe.AddTail(pWaxPart);
	}

	return m_waxPartListe;
}



/**
Nach dem z.B. neue Wax-Aufträge in der AV eingelesen wurden, wird die Statistik in der 
DB aktualisiert.
*/
void CWaxDoc::on_updateStatistik()
{
	//Liste mit WaxPart Objekten
	WaxPartListe& listWaxParts = get_waxPartListe();
	
	if (listWaxParts.getSize() <= 0)
		return;

	//Liste mit allen Werkaufträgen aus den WaxPart Objekten (ohne Dummy Werkaufträge)
	WerkauftragListe& listWa = listWaxParts.getListeWerkauftraege(true,false);
	if (listWa.isEmpty())
		return;


	BefehlUpdateStatistik befehl;
	
	befehl.set_mitFortschrittsanzeige(true);
	befehl.set_werkauftraege(&listWa);

	befehl.start();
}


/**
KC Nummer aktualisieren
*/
void CWaxDoc::on_updateKcNummer()
{
	//Liste mit WaxPart Objekten
	WaxPartListe& listWaxParts = get_waxPartListe();
	
	if (listWaxParts.getSize() <= 0)
		return;

	//Liste mit allen Werkaufträgen aus den WaxPart Objekten
	WerkauftragListe& listWa = listWaxParts.getListeWerkauftraege(true, true);
	if (listWa.isEmpty())
		return;

	GpSapAuftragsdaten gpKc;
	if (!gpKc.readDatei())
		return;

	GpSapWittenbergeAuftragsdaten gpWittKc;
	if (!gpWittKc.readDatei())
		return;

	CString error; 
	//KC Aufträge neu zuweisen
	MxIterator iter(listWa);
	SammelWerkauftrag* sammelWerkauftrag = NULL;
	while (iter.next())
	{

		Werkauftrag* werkauftrag = dynamic_cast<Werkauftrag*>(listWa.getAt(iter));
		if (!werkauftrag)
			continue;

		if(dynamic_cast<SammelWerkauftrag*>(werkauftrag))
		{
			sammelWerkauftrag = dynamic_cast<SammelWerkauftrag*>(werkauftrag);
		}
		if(!werkauftrag->get_leitkarte().get_IsChoosenIppeNumbers())
		{
			__int64 sdnummer = werkauftrag->get_leitkarte().get_sdAuftragsnr();
			int sdPos = 0;

			if (sdnummer > 0 || !werkauftrag->get_GUID().IsEmpty() && sdnummer == 0 )
			{
				CString kcAuftrag;
				int kcPos = 0;
				CString psp;
				int nl = 0, pl = 0;
				CString strIppeMatNr; 
				COleDateTime dtWunschtermin;
				BOOL bFound = FALSE;
				if(!werkauftrag->get_GUID().IsEmpty())
				{
					bFound = gpWittKc.getKcNummer( werkauftrag->get_GUID(), sdPos, kcAuftrag, kcPos, psp, nl, pl, dtWunschtermin);
					if(bFound == TRUE)
					{
						werkauftrag->get_leitkarte().set_sdPositionsnr(sdPos);
						sammelWerkauftrag->get_leitkarte().set_sdPositionsnr(sdPos);
						
					}
				}
				else
				{
					sdPos = werkauftrag->get_leitkarte().get_sdPositionsnr();
					if (gpKc.getKcNummer(strIppeMatNr, sdnummer, sdPos, kcAuftrag, kcPos, psp, nl, pl, dtWunschtermin))
					{
						bFound = TRUE;					
					}
				}
				

				if(bFound == TRUE)
				{
					werkauftrag->get_leitkarte().set_kcAuftragsnr(kcAuftrag);
					werkauftrag->get_leitkarte().set_kcPositionsnr(kcPos);	
					werkauftrag->get_leitkarte().set_prj_produktlinie(pl);
					werkauftrag->get_leitkarte().set_prj_nl(nl);
					werkauftrag->get_leitkarte().set_prj_pspElement(psp);
					MXDate dtLocalWunschtermin;
					dtLocalWunschtermin = werkauftrag->get_leitkarte().get_wunschliefertermin();
					// Wunschtermin aus dem SAP File wird genommen
					werkauftrag->get_leitkarte().set_wunschliefertermin(dtWunschtermin);
					
					//nur wenn es sich um eine Position handelt, Werte in der DB aktualisieren. (Bei einem Sammelauftrag nichts machen)
					if (werkauftrag->get_waid() > 0)
					{
						Server server(DbBase::save, werkauftrag);
						if (!server.updateKcAuftragsdaten())
						{
							CString err;
							err.Format(getStammdaten().getText("err_updatekc").c_str(), werkauftrag->get_leitkarte().get_sdAuftragsnr(), werkauftrag->get_leitkarte().get_sdPositionsnr(), werkauftrag->get_posNr());
							error += "\n" + err;
						}
					}
				}
			}
		}
	}

	if (error.GetLength() > 0)
	{
		MsgBox::show(error.GetBuffer());
	}
}


void CWaxDoc::on_update_afos()
{
	WaxPartListe& listWaxParts = get_waxPartListe();
	
	if (listWaxParts.getSize() <= 0)
		return;

	/*
	//Liste mit allen Werkaufträgen aus den WaxPart Objekten
	WerkauftragListe& listWa = listWaxParts.getListeWerkauftraege(true, true);
	if (listWa.isEmpty())
		return;
	*/

	GpInitialUpdateAfos ua(listWaxParts);
	ua.writeAfos();
}

void CWaxDoc::show_hinweis(const WaxPart* const wp) const
{
	/**
	Zeigt Hinweise zur Eingabe von Werkaufträgen an.
	Hinweise verhindern nicht den Maskenwechsel. 
	*/
	if (!wp || wp->get_deleted())
		return;

	const Werkauftrag* const w = wp->get_werkauftrag_const();

	if (!w) 
		return;

	string hw = (LPCSTR)w->checkHinweis();
		
	if (!hw.length())
		return;
	
	string hinweis = boost::str( boost::format("%1%\r\n\r\n%2%") % (LPCSTR)wp->getBezeichnung() % hw );
	MsgBox::show(hinweis);
}

void CWaxDoc::connect_signals()
{
	prjfile.progress_init.connect( boost::bind(&CWaxDoc::load_start, this, _1, _2, _3) );
	prjfile.progress_step.connect( boost::bind(&CWaxDoc::load_step, this) );
	prjfile.progress_end.connect( boost::bind(&CWaxDoc::load_end, this) );
}


void CWaxDoc::load_start(bool is_loading, const string& file, int steps)
{
	cur_steps = 0;
	progress = new Fortschrittanzeige;
	progress->set_minWert(0);
	progress->set_maxWert(steps);
	CString txt;
	txt = getStammdaten().getText(is_loading ? "IDS_LOADING" : "IDS_STORING").c_str();
	progress->set_ueberschrift(txt);
	progress->setAnzeige1(file.c_str());
	progress->start();
}

void CWaxDoc::load_step()
{
	cur_steps++;
	if (progress)
		progress->nextStep(cur_steps);
}

void CWaxDoc::load_end()
{
	if (progress)
		progress->ende();
	delete progress;
	progress = NULL;
	cur_steps = 0;
}