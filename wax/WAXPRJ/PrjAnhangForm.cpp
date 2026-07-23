#include "stdafx.h"
#include "resource.h"
#include "..\wax\waxDoc.h"
#include "..\wax\MainFrm.h"
#include "..\wax\ChildFrm.h"

		#include "PrjAnhangForm.h"


BEGIN_MESSAGE_MAP(PrjAnhangForm, WaxPartForm)
	ON_BN_CLICKED(IDC_ANHANG_EINFUEGEN, onClickedAnhang_einfuegen)
	ON_BN_CLICKED(IDC_ANHANG_EINSEHEN, onClickedAnhang_einsehen)
	ON_BN_CLICKED(IDC_ANHANG_LOESCHEN, onClickedAnhang_loeschen)
	ON_BN_CLICKED(IDC_CHECK_SHOW, onShowAnhang)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(PrjAnhangForm, WaxPartForm)


CString PrjAnhangForm::m_strCurrentPath = _T("");

PrjAnhangForm::PrjAnhangForm() : WaxPartForm(IDD_ANHANG, getStammdaten().getText("IDD_ANHANG").c_str())
{
	
}

PrjAnhangForm::~PrjAnhangForm()
{
}

void PrjAnhangForm::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);
	CString s;
	if (!pDX->m_bSaveAndValidate)
	{
		GetDlgItem(IDC_ANHANG_BEZEICHNUNG)->SetWindowText(get_data()->get_werkauftrag()->get_anhang()->get_bezeichnung());
		GetDlgItem(IDC_FILELIST)->SetWindowText(get_data()->get_werkauftrag()->get_anhang()->get_filename());
		s.Format("%i Bytes", get_data()->get_werkauftrag()->get_anhang()->get_len());
		GetDlgItem(IDS_DATEIGROESSE)->SetWindowText(s);
		GetDlgItem(IDS_GROSSEDATEI)->ShowWindow(get_data()->get_werkauftrag()->get_anhang()->get_len() > 100000 ? SW_SHOW : SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_ANHANG_BEZEICHNUNG)->GetWindowText(s);
		get_data()->get_werkauftrag()->get_anhang()->set_bezeichnung(s);
	}
	GetDlgItem(IDC_ANHANG_LOESCHEN)->EnableWindow( get_data()->get_werkauftrag()->get_anhang()->get_data() ? TRUE : FALSE );
	GetDlgItem(IDC_ANHANG_EINSEHEN)->EnableWindow( get_data()->get_werkauftrag()->get_anhang()->get_data() ? TRUE : FALSE );
	
	CButton *bt = (CButton*)GetDlgItem(IDC_CHECK_SHOW);
	if (pDX->m_bSaveAndValidate) 
		get_data()->get_werkauftrag()->get_anhang()->set_showAnhang(bt->GetCheck() ? true : false);
	else
		bt->SetCheck(get_data()->get_werkauftrag()->get_anhang()->get_showAnhang());
}

//Dialog initialisieren
BOOL PrjAnhangForm::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return FALSE;

	setWindowsTextWithColon(IDS_STATIC_BE, "IDS_BEZEICHNUNG");
	setWindowsTextWithColon(IDC_STATIC_DAT, "IDS_DATEI");
	setWindowsTextWithColon(IDC_STATIC_DAT_GROESSE, "IDS_DATEIGROESSE");
	setWindowsTextWithColon(IDS_STATIC_BE, "IDS_BEZEICHNUNG");

	setWindowsText(IDC_STATIC_INFO_ANHANG, "IDC_STATIC_INFO_ANHANG");
	setWindowsText(IDS_GROSSEDATEI, "IDS_GROSSEDATEI");

	setWindowsText(IDC_ANHANG_EINFUEGEN, "IDS_DATEI_EINFUEGEN");
	setWindowsText(IDC_ANHANG_EINSEHEN, "IDS_DATEI_ANZEIGEN");
	setWindowsText(IDC_ANHANG_LOESCHEN, "IDS_DATEI_LOESCHEN");
	setWindowsText(IDC_CHECK_SHOW, "IDC_CHECK_SHOW");

	if(dynamic_cast<ApaxWerkauftrag*>(get_data()->get_werkauftrag()))
	{
		CButton *bt = (CButton*)GetDlgItem(IDC_CHECK_SHOW);
		bt->SetCheck(true);
		bt->EnableWindow(false);
	}

	return TRUE;
}

//Indicates the user clicked a button
void PrjAnhangForm::onClickedAnhang_einfuegen()
{

	CString deffilter, gl;
	gl = getStammdaten().getText("IDS_GRAFIK_FILE").c_str();
	deffilter.Format("%s(pdf; dwf; jpg)|*.pdf;*.dwf;*.jpg||", gl);


	CString filefilter;
	char fname[1024];
	memset(fname, 0, 1024);
	
		
	if(m_strCurrentPath.IsEmpty())
	{
		MxBase::Env env;
		m_strCurrentPath = ".";
	}
	
	CFileDialog f(TRUE, NULL, NULL, 
		OFN_EXPLORER , deffilter, NULL);
	f.m_ofn.lpstrInitialDir = m_strCurrentPath;
	f.m_ofn.lpstrFile= fname;
	f.m_ofn.nMaxFile = 1024;
	f.m_ofn.lpfnHook=NULL;
	f.m_ofn.Flags &= ~OFN_ENABLEHOOK;
	f.m_ofn.hwndOwner = this->m_hWnd;

	if (GetOpenFileName(&f.m_ofn))
	{
		Sleep(100); // Workaround für 'Öffnen schlägt fehl wg. Zugriffverletzung'
		TRY
		{
			CString strTemp(f.m_ofn.lpstrFile);
			if(strTemp.Find(".pdf") == -1 && strTemp.Find(".dwf") == -1 && strTemp.Find(".jpg") == -1)
			{
				CString str = getStammdaten().getText("IDS_DATEI_ZU_DATEI_ENDUNG").c_str();
				AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);
				return;
			}
			
			get_data()->get_werkauftrag()->get_anhang()->setfile(f.m_ofn.lpstrFile);
		}
		CATCH(CNotSupportedException, e)
		{
			CString filetoolong, str;
			str = getStammdaten().getText("IDS_DATEI_ZU_GROSS").c_str();
			filetoolong.Format(str, get_data()->get_werkauftrag()->get_anhang()->MAXLEN_ANHANG / 1024);
			AfxMessageBox(filetoolong, MB_OK | MB_ICONINFORMATION);
		}
		END_CATCH
		UpdateData(FALSE);
	}
	else
	{
		int error = CommDlgExtendedError();
		TRACE("commdlg error beim öffnen: %i\n", error);
	}
	
	
	TCHAR szAppPath[1000];
	GetCurrentDirectory(1000, szAppPath);
	m_strCurrentPath = szAppPath;
	

	onShowAnhang();
}

//Indicates the user clicked a button
void PrjAnhangForm::onClickedAnhang_einsehen()
{
	TRY
	{
		CString fn;
		Werkauftrag::Anhang* file = get_data()->get_werkauftrag()->get_anhang();

		fn = WaxSys::GetTempFilename((LPCSTR)file->get_filename()).c_str();
		CFile f(fn, CFile::modeCreate | CFile::modeWrite);
		f.Write(file->get_data(), file->get_len());
		f.Close();

		if ((DWORD)ShellExecute(NULL, "open", fn, NULL, NULL, SW_SHOWNORMAL) < 31)
		{
			CString str; str = getStammdaten().getText("IDS_DATEI_STARTEN").c_str();
			AfxMessageBox(str);
		}
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
	}
	END_CATCH

	
}



//Indicates the user clicked a button
void PrjAnhangForm::onClickedAnhang_loeschen()
{
	CString str; str = getStammdaten().getText("IDS_ANHANG_LOESCHEN").c_str();
	if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		get_data()->get_werkauftrag()->get_anhang()->set_data(NULL, 0);
		get_data()->get_werkauftrag()->get_anhang()->set_filename("");
		UpdateData(FALSE);
		CChildFrame* pChildFrame =(CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
		bool bShowgraifcforApax = true;
		if(dynamic_cast<ApaxWerkauftrag*>(get_data()->get_werkauftrag()))
		{
			 bShowgraifcforApax = false;
		}
		pChildFrame->SendMessage(WM_NAVIGATE_ANHANG, bShowgraifcforApax);
	}
}





//virtuelle Methode, nach einer Prüfung zurückliefert, ob der Tab eine 
//Kennzeichnung erhalten soll. Siehe PrjAnhangForm.
int PrjAnhangForm::get_highlight() const
{
	CString size;
	GetDlgItem(IDS_DATEIGROESSE)->GetWindowText(size);
	return atoi(size) ? true : false;
}


void PrjAnhangForm::onShowAnhang()
{
	commitUpdate();
	CChildFrame* pChildFrame =(CChildFrame*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive();
	
	pChildFrame->SendMessage(WM_NAVIGATE_ANHANG, get_data()->get_werkauftrag()->get_anhang()->get_showAnhang());
}