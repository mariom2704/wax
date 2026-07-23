#include "stdafx.h"

#include "waxproppage.h"

#include "resource.h"
#include <shlobj.h>
#include "Mxparameter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(WaxPropPage, CPropertyPage)
END_MESSAGE_MAP()


bool WaxPropPage::passwd_ok  = false;

WaxPropPage::WaxPropPage( UINT nIDTemplate, MxParameter *para) : CPropertyPage(nIDTemplate)
{
	parameter = para;
}

WaxPropPage::~WaxPropPage()
{
}

void WaxPropPage::setPageTitle(const string& str)
{
	m_psp.dwFlags |= PSP_USETITLE;
    m_psp.pszTitle = str.c_str();
}

/// F�r die Aktivierung der Tooltips �berschrieben
void WaxPropPage::PreSubclassWindow() 
{
	CPropertyPage::PreSubclassWindow();
	EnableToolTips(TRUE);
}

BOOL WaxPropPage::OnInitDialog()
{
	/**
	Dialog initialisieren: ruft enable_all_items auf
	*/
	if (!CPropertyPage::OnInitDialog())
		return false;
	enable_all_items();
	return true;
}


void WaxPropPage::set_default_val(UINT idc, int para)
{
	if (!GetDlgItem(idc)->IsWindowEnabled())
		return;
	GetDlgItem(idc)->SetWindowText(parameter->def_val_str(para));
}

INT_PTR WaxPropPage::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
	/**
	Testet das Dialogitem, ob es in der Liste mit Parametern eingetragen ist. Ist dies der Fall,
	wird der Bemerkungstext des Parameters angezeigt.
	Falls das Item kein Parameter ist, wird versucht einen String anzuzeigen, der dieselbe ID wie
	das Item hat.
	*/
	int id = CPropertyPage::OnToolHitTest(point, pTI);
	if ( id != -1)
	{
		pTI->lpszText = (LPSTR)malloc(2048);
		pTI->uFlags |=  TTF_ALWAYSTIP;
		map<UINT, int>::const_iterator it = paralst.find((UINT)id);
		if (it != paralst.end())
		{
			if (parameter->getBemerkung(it->second).length())
			{
                strcpy(pTI->lpszText, parameter->getBemerkung(it->second).c_str());
				return id;
			}
		}
		
		CString strid;
		strid.Format("ID_%i", id);		
		CString str = TextDataList::getInstance()->getText(strid.GetBuffer(), true).c_str();

		strcpy(pTI->lpszText, str);
	}
	return id;
}

void WaxPropPage::set_standard()
{
	/**
	Standardeinstellungen wiederherstellen �ber set_default
	*/
	// Default-Werte setzen
	set_default();
	UpdateData(FALSE);
	on_update();
}



void WaxPropPage::on_update()
{
	/**
	Auslesen der Eingabefelder und setzen des modified-Flags
	*/
	UpdateData();
	SetModified(true);
}

BOOL WaxPropPage::OnApply() 
{
	// Speichert alle Strings aus strpara
	for (map<UINT, int>::iterator it = strpara.begin(); it != strpara.end(); it++)
	{
		if (GetDlgItem(it->first)->IsWindowEnabled())
		{
			CString itemtext;
			GetDlgItem(it->first)->GetWindowText(itemtext);
			parameter->save_para(it->second, (LPCSTR)itemtext);
		}
	}

#ifdef __WAX_FUER_DRITTE__
	CString strEMail = WaxSys::getEmailfromDefaultUser().c_str();
	parameter->save_para(EMPFAENGER, (LPCSTR)strEMail);
	
	CString strNL_KENNUNG = WaxSys::getNLFromDefaultUser().c_str();
	parameter->save_para(IB_NL_KENNUNG, (LPCSTR)strNL_KENNUNG);
#endif
	
	SetModified(false);
	return false;
}


bool WaxPropPage::browse_path(const CWnd *wnd, const int idEdit)
{
	/**
	Auswahl eines Verzeichnisses
	*/
	CString initpath;
	wnd->GetDlgItem(idEdit)->GetWindowText(initpath);
	
	BROWSEINFO bi;
	LPITEMIDLIST pidl;
	char ach[MAX_PATH+255];

	bi.hwndOwner      = NULL;
	bi.pidlRoot       = NULL;/// (LPCSTR)m_Suchpfad;
	bi.pszDisplayName = ach;
	bi.lpszTitle      = NULL;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS;
	bi.lpfn           = (BFFCALLBACK)WaxPropPage::browseCallbackProc;
	bi.lParam         = (LPARAM)(LPCSTR)initpath;
	bi.iImage         = 0;

	pidl = SHBrowseForFolder(&bi);

	if (pidl)
	{
		SHGetPathFromIDList(pidl, ach);
		IMalloc *pm;
		SHGetMalloc(&pm);
		if (pm)
		{
			pm->Free(pidl);
			pm->Release();
		}
		wnd->GetDlgItem(idEdit)->SetWindowText(ach);
		return true;
	}
	return false;
}

bool WaxPropPage::enable_item(CWnd *wnd, UINT item, UINT par)
{
	/**
	Disabled ein Dialogelement, wenn der Parameter nicht f�r den User freigegeben
	ist und das Passwort nicht eingegeben wurde
	*/
	if (passwd_ok)
		return true;
	CWnd *witem = wnd->GetDlgItem(item);
	if (witem)
	{
		if (parameter->get_para_const(par)->get_valid_namespace() == Parameter::LOCATION)
		{
			witem->EnableWindow(FALSE);
			return false;
		}
	}
	return true;
}

bool WaxPropPage::browse_file(const CWnd *wnd, const int idEdit)
{
	/**
	Auswahl einer Datenbankdatei
	*/
	CString initfile;
	wnd->GetDlgItem(idEdit)->GetWindowText(initfile);
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	char fname[_MAX_PATH];
	memset(fname, NULL, _MAX_PATH);

	CString fileendung;
	fileendung.LoadString(IDS_BROWSEFILE);
	CFileDialog f(TRUE, "mdb", NULL, OFN_EXPLORER|OFN_FILEMUSTEXIST |OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR , fileendung, this);
	f.m_ofn.lpstrInitialDir = (LPCSTR)initfile;

	if (f.DoModal() == IDOK)
	{
		wnd->GetDlgItem(idEdit)->SetWindowText(f.m_ofn.lpstrFile);
		return true;
	}
	return false;
}

int CALLBACK WaxPropPage::browseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	UNREFERENCED_PARAMETER(lpData);
	UNREFERENCED_PARAMETER(lParam);

	if (uMsg == BFFM_INITIALIZED)
	{
		::PostMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;

}

void WaxPropPage::set_default()
{
	/**
	Setzt alle Eingabefelder, die in der strpara map enthalten sind auf die Defaultwerte
	wenn das Eingabefeld enabled ist
	*/
	map<UINT, int>::iterator it;
	for (it = strpara.begin(); it != strpara.end(); it++)
	{
		if (GetDlgItem(it->first)->IsWindowEnabled())
			GetDlgItem(it->first)->SetWindowText(parameter->def_val_str(it->second));
	}
}

void WaxPropPage::enable_all_items()
{
	/**
	Ruft f�r jeden Stringparameter in strpara und paralst die Methode enable_item auf
	*/
	map<UINT, int>::iterator it;
	for (it = strpara.begin(); it != strpara.end(); it++)
	{
		enable_item(this, it->first, it->second);
		GetDlgItem(it->first)->SetWindowText(parameter->val_str(it->second));
	}
	for (it = paralst.begin(); it != paralst.end(); it++)
		enable_item(this, it->first, it->second);
}


void WaxPropPage::setWindowsText(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(TextDataList::getInstance()->getText(txtId).c_str());
}

void WaxPropPage::setWindowsTextWithColon(int id, const string& txtId)
{
	GetDlgItem(id)->SetWindowText(TextDataList::getInstance()->getTextWithColon(txtId).c_str());
}