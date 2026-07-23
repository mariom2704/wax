#include "stdafx.h"


#include "resource.h"
#include "ApaxPosition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ApaxPosition, WaxPartForm)
	ON_EN_CHANGE(IDC_APAX_POSNR,		doUpdate)	
	ON_EN_KILLFOCUS(IDC_APAX_ANZAHL,	doUpdate)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(ApaxPosition, WaxPartForm)

ApaxPosition::~ApaxPosition()
{

}

ApaxPosition::ApaxPosition() : WaxPartForm(IDD_APAXPOSITION)
{

}

BOOL ApaxPosition::OnInitDialog()
{
	if (!WaxPartForm::OnInitDialog())
		return false;

	if (!WaxSys::isAV())
	{
		GetDlgItem(IDC_APAX_VK3)->ShowWindow(SW_HIDE); //nur in der Av sichtbar
		GetDlgItem(IDC_APAX_LABEL)->ShowWindow(SW_HIDE);
	}

	setWindowsTextWithColon(IDC_STATIC_POS, "IDS_POS_KURZ");
	setWindowsTextWithColon(IDC_APAX_LABEL2, "IDS_ANZAHL");
	setWindowsTextWithColon(IDC_STATIC_VK3_PREIS, "IDC_STATIC_VK3_PREIS");


	return true;
}

void ApaxPosition::DoDataExchange(CDataExchange* pDX)
{
	WaxPartForm::DoDataExchange(pDX);

	ddx_anzahl(pDX, IDC_APAX_ANZAHL);
	ddx_posnr(pDX,  IDC_APAX_POSNR);


	ApaxWerkauftrag *apax = getApaxWerkauftrag();
	CString vk3;
	if (pDX->m_bSaveAndValidate)
	{
		DDX_Text(pDX, IDC_APAX_VK3, vk3);
		apax->set_vk3Preis(atof(vk3));
		//Preislisten erzeugen
		apax->createModell();
	}
	else
	{
		CString vk3;
		vk3.Format("%.2f", apax->get_vk3Preis());
		DDX_Text(pDX, IDC_APAX_VK3, vk3);
	}

}


/**
Zugriff auf das Entity Objekt Strangrohr aus WaxPart
*/
ApaxWerkauftrag* ApaxPosition::getApaxWerkauftrag()
{
	//WaxPart Objekt holen
	WaxPart* rohrData = dynamic_cast<WaxPart*>(get_data());
	XASSERT(rohrData);
	if (!rohrData)
		return NULL;


	//Aus dem WaxPart das Gewinderohr holen
	ApaxWerkauftrag* apax = dynamic_cast<ApaxWerkauftrag*>(rohrData->get_werkauftrag());
	XASSERT(apax);

	return apax;
}

bool ApaxPosition::check_plausis()
{
	if (!WaxPartForm::check_plausis())
		return false; 

	ApaxWerkauftrag* apax = getApaxWerkauftrag();
	CString err = apax->checkPlausis();
	if (err.GetLength() > 0)
	{
		CWnd* wnd = GetFocus();
		disableCheck();
		AfxMessageBox(err, MB_OK | MB_ICONINFORMATION);
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		if (wnd)
			wnd->SetFocus();
		return false;
	}

	CString str; str = getStammdaten().getText("IDS_EINGABEN_KORREKT").c_str();
	((CFrameWnd*)AfxGetMainWnd())->SetMessageText(str);
	return true;
}