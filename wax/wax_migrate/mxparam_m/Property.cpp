#include "stdafx.h"
#include "resource.h"
#include "passwd.h"
#include "Property.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(Property, CPropertySheet)
	ON_COMMAND(ID_STANDARD_SETZEN, set_default)
END_MESSAGE_MAP()


Property::Property(CWnd* pWndParent, MxParameter* para, bool check_passwd) : CPropertySheet(IDS_PARAMETEREINST, pWndParent),
ppAllgemein(para), ppAnsicht(para), ppProjekt(para), ppGewinderohr(para), 
ppGraphik(para), ppFreigabe(para), ppUser(para),  ppSysall(para)
{

	//�berschriften setzen
	ppAllgemein.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_ALLGEMEIN"));
    ppAnsicht.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_ANSICHT"));
	ppProjekt.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_PROJEKT"));
	ppGewinderohr.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_GEWINDEROHR"));
	ppGraphik.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_GRAFIK"));
	ppFreigabe.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_FREIGABESTATUS"));
	ppUser.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_USER"));
	ppSysall.setPageTitle(TextDataList::getInstance()->getText("IDD_PARA_SYSALL"));


	UNREFERENCED_PARAMETER(check_passwd);
	parameter = para;
	AddPage(&ppAllgemein);
	AddPage(&ppAnsicht);
	AddPage(&ppProjekt);
	AddPage(&ppGewinderohr);
	AddPage(&ppGraphik);
	AddPage(&ppFreigabe);
	AddPage(&ppUser);
	if (WaxPropPage::passwd_ok)  // bei erneuten Aufrufen mit vorher bereits eingegebem Passwort
	{
		AddPage(&ppSysall);
	}
		
}

Property::~Property()
{
}


BOOL Property::OnInitDialog( )
{
	CPropertySheet::OnInitDialog();

	RECT r;
	this->GetWindowRect(&r);
	ScreenToClient(&r);    

	r.bottom -= 10;
	r.top = r.bottom - 25;
	r.left = 10;
	r.right = 150;
	CString txt;
	txt = TextDataList::getInstance()->getText("ID_STANDARD_SETZEN").c_str();
	default_button.Create(txt, BS_TEXT | BS_PUSHBUTTON | WS_TABSTOP, r, this, ID_STANDARD_SETZEN);
	default_button.SetFont(this->GetFont());
	default_button.ShowWindow(SW_SHOW);

	return true;
}



bool Property::check_password()
{
	/**
	F�hrt eine Passwortabfrage aus. Das Passwort muss nur einmal richtig eingegeben werden.
	Liefert true zur�ck, wenn Passwortdialog mit Ok best�tigt wurde, unabh�ngig von der Richtigkeit
	der Eingabe. Wenn das Passwort falsch oder gar nicht eingegeben wurde, k�nnen nur die pers�nlichen
	Einstellungen bearbeitet werden.
	*/
	if (WaxPropPage::passwd_ok)
		return true;

	Passwd passwd(this, parameter->get_passwd());
	if (passwd.DoModal() != IDOK)
		return false;

	if (passwd.check_passwd())
	{
		AddPage(&ppSysall);
		WaxPropPage::passwd_ok = true;
	}
	return true;
}

void Property::set_default()
{
	/**
	Setzt die Eingabefelder der aktiven Einstellungsseite auf die Defaultwerte zur�ck
	*/
	WaxPropPage* prop = dynamic_cast<WaxPropPage*>(GetActivePage());
	prop->set_standard();
}