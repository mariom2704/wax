#include "stdafx.h"

#include "resource.h"

#include "PropPageAllgemein.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PropPageAllgemein, WaxPropPage)
	ON_BN_CLICKED(IDC_PARA_SPRACHEDEUTSCH, on_update)
	ON_BN_CLICKED(IDC_PARA_SPRACHEENGLISCH, on_update)
	ON_EN_CHANGE(IDC_PARA_AUTOSAVE, on_update)
	ON_BN_CLICKED(IDC_PASSWORD, onPassword)
END_MESSAGE_MAP()

/// Konstructor
PropPageAllgemein::PropPageAllgemein(MxParameter *para) : WaxPropPage(IDD_PARA_ALLGEMEIN, para)
{
	sprache = parameter->val_str(LANGUAGE);
	if (sprache == LANGUAGE_DEUTSCH) paraSprache = DEUTSCH; 
	else if (sprache == LANGUAGE_ENGLISCH) paraSprache = ENGLISCH; 

	ADD_PARA_STR(AUTOSAVE);
	ADD_PARA_XX(IDC_PARA_SPRACHEDEUTSCH, LANGUAGE);
	ADD_PARA_XX(IDC_PARA_SPRACHEENGLISCH, LANGUAGE);
}

/// Destructor
PropPageAllgemein::~PropPageAllgemein()
{
}


/// Default-Werte setzen
void PropPageAllgemein::set_default()
{
	WaxPropPage::set_default();
	paraSprache = DEUTSCH;
	sprache = parameter->def_val_str(LANGUAGE);
}


/// Dialog initialisieren
BOOL PropPageAllgemein::OnInitDialog() 
{
	if (!WaxPropPage::OnInitDialog())
		return false;
	
	setWindowsText(IDC_STATIC_AUTOSAVE, "IDC_STATIC_AUTOSAVE");
	setWindowsTextWithColon(IDC_STATIC_ASAVE, "IDC_STATIC_ASAVE");
	setWindowsText(IDC_STATIC_SPRACHE, "IDS_SPRACHE");
	setWindowsText(IDC_PARA_SPRACHEDEUTSCH, "IDS_DEUTSCH");
	setWindowsText(IDC_PARA_SPRACHEENGLISCH, "IDS_ENGLISCH");
	setWindowsText(IDC_PASSWORD, "IDC_PASSWORD");

	GetDlgItem(IDC_PASSWORD)->EnableWindow(passwd_ok);
	return true;  
}

/// Datenaustausch
void PropPageAllgemein::DoDataExchange(CDataExchange* pDX)
{
	WaxPropPage::DoDataExchange(pDX);
	
	BOOL checkDeutsch  = (paraSprache == DEUTSCH)  ? 1 : 0;
	BOOL checkEnglisch = (paraSprache == ENGLISCH) ? 1 : 0;
	
	DDX_Check(pDX, IDC_PARA_SPRACHEDEUTSCH,  checkDeutsch);
	DDX_Check(pDX, IDC_PARA_SPRACHEENGLISCH, checkEnglisch);

	paraSprache = (checkDeutsch == 1) ? DEUTSCH : ENGLISCH; 
}

/// Allgemeine Einstellungen in Datenbank eintragen
BOOL PropPageAllgemein::OnApply() 
{
	if (WaxPropPage::OnApply())
		return true;

	if (paraSprache == DEUTSCH)
	{
		::SetThreadLocale(MAKELCID(MAKELANGID(LANG_GERMAN,SUBLANG_DEFAULT),SORT_DEFAULT));
		sprache = LANGUAGE_DEUTSCH;
	}
	else if (paraSprache == ENGLISCH)
	{
		::SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT),SORT_DEFAULT));
		sprache = LANGUAGE_ENGLISCH;
	}
	parameter->save_para(LANGUAGE, sprache);

	return true;
}

void PropPageAllgemein::onPassword() 
{
	PasswdAendern pa;
	if (pa.DoModal() == IDOK)
	{
		CString newpasswd;
		newpasswd.Format("UPDATE Password SET Password = '%s' WHERE Name = 'SYSTEM'", pa.password);
		try
		{
			parameter->get_db()->ExecuteSQL(newpasswd);
		}
		catch (CDBException* e)
		{
			e->Delete();
		}
	}
}

BEGIN_MESSAGE_MAP(PropPageAllgemein::PasswdAendern, CDialog)
END_MESSAGE_MAP()

PropPageAllgemein::PasswdAendern::PasswdAendern(CWnd* pParent)//=NULL
	: CDialog(IDD_PASSWD_AENDERN, pParent)
{
}

void PropPageAllgemein::PasswdAendern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEWPASSWORD, newPassword);
	DDV_MaxChars(pDX, newPassword, 15);
	DDX_Text(pDX, IDC_PASSWORD, password);
	DDV_MaxChars(pDX, password, 15);
}

///  Behandlungsroutinen f�r Nachrichten PasswdAendern 
void PropPageAllgemein::PasswdAendern::OnOK() 
{
	UpdateData(TRUE);
	if (!newPassword.GetLength()) 
		return;
	if (newPassword != password )
	{
		AfxMessageBox(IDS_PASSWORD_FALSCH_BESTAETIGT);
		GetDlgItem(IDC_NEWPASSWORD)->SetFocus();
		return;
	}
	CDialog::OnOK();
}

BOOL PropPageAllgemein::PasswdAendern::OnInitDialog() 
{
	if (!CDialog::OnInitDialog())
		return false;

	this->SetWindowText(TextDataList::getInstance()->getText("IDD_PASSWD_AENDERN").c_str());

	GetDlgItem(IDC_STATIC_NEWPWD)->SetWindowText(TextDataList::getInstance()->getTextWithColon("IDC_STATIC_NEWPWD").c_str());
    GetDlgItem(IDC_STATIC_NEWPWD_OK)->SetWindowText(TextDataList::getInstance()->getTextWithColon("IDC_STATIC_NEWPWD_OK").c_str());

	return true;
}