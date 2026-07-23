#include "stdafx.h"


#include <mxutil/crc32.h>
#include "resource.h"
#include "befehlshowerror.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(WaxPartForm, CDialog)

BEGIN_MESSAGE_MAP(WaxPartForm, CDialog)
	ON_COMMAND(IDOK, OnOK)
	ON_COMMAND(IDCANCEL, OnOK)
	ON_COMMAND(WM_COMMITUPDATE, commitUpdate)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


//Geschützter Konstruktor
WaxPartForm::WaxPartForm(UINT idd, const CString& chapter, CWnd* pParent)
	: CDialog(idd, pParent), data(NULL), document(NULL), disableCheckPlausis(false), treeItemText(NULL)
{
	/**
	Konstruktor
	Setzt die Dialog ID und den Namen. Der Name wird definiert durch einen Eintrag in der Stringtable
	mit dem gleichen Wert wie die Dialogvorlage. Dieser String kann durch ein Newline getrennt eine nähere 
	Beschreibung enthalten, die dann als Tooltip angezeigt wird (todo)
	*/
	dlgid = idd;
	XASSERT(dlgid); // Konstruktor von WaxPartForm nicht aufgerufen????

	if (chapter.IsEmpty())
		name.LoadString(idd);
	else
		name = chapter;

	crc = 0;

	//Parameter bisher nur im Gewinderohrdialog wirksam 
	//muss in der entsprechenden Dialogverwaltung gesetzt werden über die
	//Abfrage von GF_DN_2_ZOLL oder GROHR_DN_2_ZOLL
	dn2zoll = false;
	av = WaxSys::isAV();
}

WaxPartForm::~WaxPartForm()
{
	delete treeItemText;
	if (crc)
		check_changes();
}




void WaxPartForm::DoDataExchange(CDataExchange* pDX)
{
	/**
	Datenaustausch mit den Dialogelementen
	In dieser Basisklasse z.Zt. keine Funktion
	*/
	CDialog::DoDataExchange(pDX);
}

const CString WaxPartForm::get_name(bool left) const
{
	/**
	Extrahiert den Namen des Dialogs aus name. Durch ein Newline kann 
	der Name in Überschrift und Popup Hilfe separiert werden. Mit left = true
	wird der Name geliefert, sonst die Popup Hilfe. Ist der Text nicht mit
	Newline aufgeteilt, wird immer der ganze Text geliefert.
	*/
	CString tit(name);
	int index = tit.Find('\n');
		
	if (index != -1)
		tit = left ? tit.Left(index) : tit.Right(tit.GetLength() - index - 1);
	return tit;
}


BOOL WaxPartForm::PreTranslateMessage(MSG* pMsg) 
{
	/**
	Vorverarbeitung von Messages
	Eingegende Messages an aktiven Tabdialog weiterleiten
  
	Ctrl + (Shift) + TAB führt zu einem Umschalten des Tabs
  
	Wenn der Fokus auf das erste Element gesetzt wird (durch Tastatureingabe)
	wird automatisch die Überschrift des Tabs aktiviert. Dies ist problematisch bei Comboboxen, da der Fokus auf
	das Editcontrol der Combobox gesetzt wird. Deshalb im Code die Abfrage:
	if (GetFocus == first) or (GetFocus->GetParent() == first))
	*/
	BOOL ret =  CDialog::PreTranslateMessage(pMsg);
	// allow sheet to translate Ctrl+Tab, Shift+Ctrl+Tab,
	//  Ctrl+PageUp, and Ctrl+PageDown
	// Tabumschaltung, funktioniert nur, wenn der Focus auf der Tabbezeichnung liegt
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		(pMsg->wParam == VK_TAB || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT))
	{
		if (GetAsyncKeyState(VK_SHIFT))
			GetParent()->SendMessage(WM_KEYDOWN, VK_LEFT, 0);
		else
			GetParent()->SendMessage(WM_KEYDOWN, VK_RIGHT, 0);
	}

	// Nach dem letzten Dialogelement den Focus auf die Tabbezeichnung setzen, damit der Tab 
	// per Tastatur umgeschaltet werden kann
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB )
	{
		CWnd *first = GetNextDlgTabItem((CWnd*) NULL, FALSE);
				
		if (GetFocus())
		{
			if (!first || (GetFocus() == first) || (GetFocus()->GetParent() == first))
			{
				GetParent()->SetFocus();
			}
		}
		else
		{
			TRACE("Kein Focus im Dialog!!\n");
		}
	}
	
	return ret;
}


void WaxPartForm::createForm(CWnd *pParent, WaxPart *pData, CDocument *pDocument)
{
	/**
	Dialog erzeugen
	Dient der Initialisierung des Dialog. Es wird der nichtmodale
	Dialog erzeugt, das Datenobjekte m_pData und der Pointers auf das Dokument m_pDocument wird gesetzt .
	*/
	set_data(pData);
	set_document(pDocument);
	Create(dlgid, pParent);
}

void WaxPartForm::OnOK()
{
	/**
	Ein OK dieses Dialogs führt nur zu einem Update der Daten
	*/
	UpdateData();	
}

void WaxPartForm::PreSubclassWindow() 
{
	/**
	Tooltips enablen
	für Tooltips und deren Implementations siehe MFC Doku
	*/
	CDialog::PreSubclassWindow();
	EnableToolTips(TRUE);
}

INT_PTR WaxPartForm::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	/**
	Tooltipfunktion
	*/
	int res = CDialog::OnToolHitTest( point, pTI );
	if (pTI && res != -1)
	{
		pTI->lpszText = (LPSTR)malloc(2048);
		pTI->uFlags |= TTF_ALWAYSTIP;
		CString out;
		out.LoadString(res);
		strcpy(pTI->lpszText, out.Left(2048));
	}
	return res;
}

//Zugriff auf das Documentobjekt
 CDocument* WaxPartForm::get_document() const
{
	/**
	Pointer auf das Dokument zurückliefern
	Leider hat nur die über diesem Dialog liegende View (<c CTabCtrlView>) Zugriff auf
	das Dokument. Diese Methode dient dazu über UpdateAllViews() ein globales Aktualisieren 
	anzustossen. 
	*/
	return document;
}


//Initialisierung einer TV_INSERTSTRUCT Struktur zum Einfügen in 
//einen Treecontrol.
TV_INSERTSTRUCT WaxPartForm::makeInsertStruct(HTREEITEM lastitem, UINT image, const CString& titel, BOOL hasChildren, void* code) 
{
	/**
	Diese Methode bereitet eine TV_INSERTSTRUCT für das Einfügen in ein TreeCtrl vor. 
	Wird erst in abgeleiteten Klassen benutzt.
	*/
	const int itemTextLen = 255;
	if (!treeItemText)
	{
		treeItemText = new char[itemTextLen];
	}
	strcpy(treeItemText, titel.Left(itemTextLen));
	TV_INSERTSTRUCT TreeCtrlItem;
	TreeCtrlItem.hParent = lastitem; 

	TreeCtrlItem.item.pszText = treeItemText; 
	TreeCtrlItem.item.iImage = image; 
	TreeCtrlItem.item.iSelectedImage = image; 
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
	TreeCtrlItem.item.cChildren = hasChildren;
	TreeCtrlItem.item.lParam = (DWORD)(code);
	return TreeCtrlItem;
}


//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
//wird.
bool WaxPartForm::check_plausis() 
{
	/**
	Wird in einer abgeleiteten checkPlausis Methode der Focus auf ein Dialogelement gesetzt, 
	so wird doUpdate (und damit checkPlausis) doppelt aufgerufen. Damit kommen alle Fehler
	meldungen doppelt. Um dies zu vermeiden, kann vor einem Aufruf von SetFocus oder AfxMessageBox disableCheckPlausis auf
	true gesetzt werden. Es muss dann nur am Anfang von checkPlausis abgeprüft werden, ob WaxPartForm::checkPlausis 
	false liefert. Siehe dazu Pumpenprobierleitung::checkChanges
	*/
	if (disableCheckPlausis)
	{
		disableCheckPlausis = false;
		return false;
	}

	if (get_data()->get_werkauftrag()->getErrorHandler().getErrorCount())
	{
		BefehlShowError error;
		error.getErrorHandler() = get_data()->get_werkauftrag()->getErrorHandler();
		error.start();
		if (error.getErrorHandler().isCritical())
		{
			disableCheckPlausis = true;
			return false;
		}
		return  true; // Bei Informationsmeldungen wird true zurückgegeben
	}

	return true;
}



//Prüft die Werkaufträge auf Änderungen und setzt das Flag, wenn nötig. Wird 
//vom Destruktor und TabCtrlView aufgerufen.
void WaxPartForm::check_changes()
{
	XASSERT(this->data);
	if (!data)
		return;

	// Muss immer durchgeführt werden,
	// sonst wird, das Aenderungsdatum nur bei der ersten Aenderung gesetzt  
	unsigned int crcsave = crc;
	initCrc();
	if (crc != crcsave)
	{
		data->get_werkauftrag()->set_aenderungsdatum();
		CString str; str = getStammdaten().getText("IDS_WERKAUFTRAG_GEAENDERT").c_str();
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
		// hier noch mal, weil bei set_change() das Aenderungsdatum neu gesetzt wird 
		//initCrc();
	}
}



//Initialisiert den CRC zu Prüfung auf Änderungen
void WaxPartForm::initCrc()
{
	CMemFile saveData(128000);
	BinaryArchive ar(&saveData, BinaryArchive::store);
	data->get_werkauftrag()->serialize(ar);
	
	ar.close();

	unsigned int size = (unsigned int)saveData.GetPosition();
	unsigned char *buffer = saveData.Detach();

	Crc32 crc32;
	crc32.addData(buffer, size);
	crc = crc32.get_crc();
	free(buffer);
}



BOOL WaxPartForm::OnInitDialog()
{
	/**
	Ist der Werkauftrag in Bearbeitung in der AV werden alle Dialogelemente abgeblendet
	*/
	if (!CDialog::OnInitDialog())
		return false;

	// Ohne Datenobjekt braucht die Maske gar nicht erst erscheinen
	WaxPart* pt = dynamic_cast<WaxPart*>(get_data());
	XASSERT(pt);
	if (!pt) 
		return false;
	
	DisableAllControls();

	return true;
}



BOOL WaxPartForm::DisableAllControls()
{
	if ((!this->get_data()->get_werkauftrag()->get_GUID().IsEmpty() && av) || (!av && (this->get_data()->get_werkauftrag()->get_status().get_wanr() > 0 ||
			    this->get_data()->get_werkauftrag()->get_status().get_status() == Status::Angebot ||
			    (this->get_data()->get_werkauftrag()->get_status().get_status() == Status::Freigegeben && this->get_data()->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer() > 0))))
	{

		if(!IsDebuggerPresent())
		{
			CWnd* wnd = GetDlgItem(IDC_ELEMENTE_ANSDN);
			if(wnd != NULL)
			{
				wnd->EnableWindow(FALSE);
			}
			
			
			CWnd *first = GetNextDlgTabItem( NULL);
			CWnd *next = first;
			do
			{
				next = GetNextDlgTabItem( next );
				next->EnableWindow(false);
			}
			while (first != next);
			return true;
		}
	}
	return false;
}



//Setzt disableCheckPlausis auf true
void WaxPartForm::disableCheck()
{
	disableCheckPlausis = true;
}


void WaxPartForm::set_disableCheckPlausis(bool wert)
{
	disableCheckPlausis = wert;
}


LRESULT WaxPartForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = NULL;
	try
	{
		ret = CWnd::WindowProc(message, wParam, lParam);
	}
	catch (Element::Error e)
	{
		AfxMessageBox(e.get_errorText().c_str(), MB_OK | MB_ICONERROR);
	}
	return ret;
}



//virtuelle Methode, nach einer Prüfung zurückliefert, ob der Tab eine 
//Kennzeichnung erhalten soll. Siehe PrjAnhangForm.
int WaxPartForm::get_highlight() const
{
	return 0;
}



//Standardmethode zum Lesen und Setzen der Positionsnummer
void WaxPartForm::ddx_posnr(CDataExchange *pDX, UINT id)
{
	CString posnr = get_data()->get_werkauftrag()->get_posNr();
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, id, posnr);
		get_data()->get_werkauftrag()->set_posNr(posnr);
	}
	else
		DDX_Text(pDX, id, posnr);
}



//Standardmethode zum Lesen und Setzen der Anzahl
void WaxPartForm::ddx_anzahl(CDataExchange* pDX, UINT id)
{
	int anzahl = get_data()->get_werkauftrag()->get_anzahl();
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, id, anzahl);
		get_data()->get_werkauftrag()->set_anzahl(anzahl);
	}
	else
		DDX_Text(pDX, id, anzahl);
}


void WaxPartForm::doUpdate()
{
	/**
	Workaround für fehlerhaftes Update bei Comboboxen und anderen Controls: Man wählt einen
	neuen Wert aus, bei DoDataExchange wird aber noch der alte Wert vom Control geliefert.
	Dies führt zu einem 'Nachlauf', bei jeder Auswahl wird der vorige Wert benutzt.
	Postet eine Updatemessage, die bewirkt, dass commitUpdate aufgerufen wird. 
	Dadurch wird erreicht, dass bei Comboboxen erst die internen Daten aktualisiert 
	werden und dann erst ein UpdateData erfolgt.
	*/
	if (get_data()->update_enabled())
		PostMessage(WM_COMMAND, WM_COMMITUPDATE);
}

void WaxPartForm::do_update_rohr_dn()
{
	/**
	siehe WaxPartForm::doUpdate
	*/
	if (get_data()->update_enabled())
		PostMessage(WM_COMMAND, WM_DNUPDATE);
}

void WaxPartForm::commitUpdate()
{
	/**
	Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
	checkPlausis true liefert.
	*/

	UpdateData();
	
	if (check_plausis())
	{
		get_data()->get_werkauftrag()->set_plausiOk(true);
		Rohrleitung *tmprohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
		if (tmprohr)
			tmprohr->createModell();
		get_data()->updateGrafic(this);
	}
	else
		get_data()->get_werkauftrag()->set_plausiOk(false);
	
}


bool WaxPartForm::validWerkauftrag() const
{
	/**
	Prüft auf gültige Initialisierung mit einem WaxPart und einem dazugehörigen Werkauftrag
	*/
	XASSERT(data);
	if (!data)
		return false; 

	const Werkauftrag *w = data->get_werkauftrag();
	XASSERT(w);
	if (!w)
		return false; 
	return true;
}


int WaxPartForm::selchange_rohr_dn(int iddn, int iddin)
{
	CComboBox* dn = (CComboBox*)GetDlgItem(iddn);
	CComboBox* din = (CComboBox*)GetDlgItem(iddin);
	Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	XASSERT(dn && din && rohr);
	if (!dn || !din || !rohr)
		return 0;

	UpdateData();
	int newdn = rohrquali_autoselect(iddn, iddin);
	if (newdn)
	{
		rohr->set_dnvl(newdn);
	}
	return newdn;
}



int WaxPartForm::rohrquali_autoselect(UINT id_dn, UINT id_din)
{
	/**
	Nichtstatische Methode, ruft rohrquali_autoselect(CComboBox* dn, CComboBox* din)
	auf.
	*/
	CComboBox* dn = (CComboBox*)GetDlgItem(id_dn);
	CComboBox* din = (CComboBox*)GetDlgItem(id_din);
	return rohrquali_autoselect(dn, din);
}


int WaxPartForm::rohrquali_autoselect(CComboBox* dn, CComboBox* din)
{
	/** 
	Wenn eine DN grösser 50 ausgewählt wurde, muss der Text in iddin für die
	Rohrqualität geändert werden. 
	Statische Version zur allgemeinen Verwendung (z.B. in SpanxRohrDialog).
	*/
	XASSERT(dn && din);
	if (!dn || !din)
		return 0;
	
	CString din_2440, din_2458;
	din_2440 = getStammdaten().getText("IDS_2440STD").c_str();
	din_2458 = getStammdaten().getText("IDS_2458STD").c_str();

	CString dnvalstr;
	dn->GetWindowText(dnvalstr);
	int dnval = atoi(dnvalstr);

	CString dinval;
	din->GetWindowText(dinval);
	int index = din->GetCurSel();
	
	if (index >= 0 && (dinval == din_2440 || dinval == din_2458))
	{
		CString newdinval = dnval <= 50 ? din_2440 : din_2458;
		// Wenn der richtige String bereits angezeigt wird, passiert nix
		if (dinval == newdinval)
			return dnval;

		din->DeleteString(index);
		// lt. Doku macht InsertString kein Sort
		index = din->InsertString(index, newdinval); 
		if (index >= 0)
		{
			din->SetItemData(index, Rohrleitung::RQ2440_2458GSCH);
			din->SetCurSel(index);
		}
	}
	return dnval;
}

BOOL WaxPartForm::OnEraseBkgnd(CDC* pDC)
{
	/**
	Zeigt als Hintergrund eine Bitmap mit dem Text 'Storniert' an
	*/
	if (!get_data() || !get_data()->get_werkauftrag())
		return false;
	
	if (get_data()->get_subobject_count())
	{
		
		for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos; )
		{
			WaxPart* pt = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
			if (!pt)
				continue;
			if (!pt->get_werkauftrag()->get_status().get_storniert())
				return false;

		}
	}
	
	else if (!get_data()->get_werkauftrag()->get_status().get_storniert())
		return false;

	CBitmap storno;
	storno.LoadBitmap(IDB_STORNIERT);
	BITMAP bmp;
	memset((void*)&bmp, 0, sizeof(BITMAP));
	storno.GetBitmap(&bmp);
	

	RECT r;
	GetWindowRect(&r);
	ScreenToClient(&r);

	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CBitmap* pbmpOld = (CBitmap*)dcMem.SelectObject(&storno); 
	ASSERT(pbmpOld) ;
	pDC->StretchBlt(0, 0, r.right, r.bottom, &dcMem, 0,
		0, bmp.bmWidth, bmp.bmWidth, SRCCOPY);
	dcMem.SelectObject(pbmpOld) ;
	
	return true;
}



WaxPart* WaxPartForm::get_data() 
{
	XASSERT(data);
	return data;
}

const WaxPart* WaxPartForm::get_data() const
{
	XASSERT(data);
	return data;
}

Rohrleitung* WaxPartForm::get_rohrleitung()
{
	/**
	Sicherer Zugriff auf die Rohrleitung. Im Fehlerfall wird eine CNotSupportedException
	erzeugt. 
	*/
	Rohrleitung* r = dynamic_cast<Rohrleitung*>(get_data()->get_werkauftrag());
	if (!r)
		AfxThrowNotSupportedException();
	return r;
}

void WaxPartForm::setWindowsText(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(getStammdaten().getText(txtId).c_str());
}

void WaxPartForm::setWindowsTextWithColon(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(getStammdaten().getTextWithColon(txtId).c_str());
}


void WaxPartForm::createPopupMenu(CPoint point, menu_items items[])
{
	CMenu menu; 
	menu.CreatePopupMenu();
	
	for (int i = 0; items[i].id != -1; i++) 
	{
		if (items[i].id != ID_SEPARATOR) 
		{
			menu.AppendMenu(MF_STRING, items[i].id, getStammdaten().getText(items[i].name).c_str());
		} else 
		{
			menu.AppendMenu(MF_SEPARATOR);
		}
	} 
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}



