#include "stdafx.h"
#include "resource.h"


#include "WaxHtmlView.h"
#include "WaxSplitterWnd.h"
#include "..\WAXPRJ\JScriptDruck.h"
#include "ChildFrm.h"
#include "JScriptViewTemplate.h"
#include "WaxDoc.h"

				#include "MehrfachdruckDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MehrfachdruckDlg, CDialog)
	ON_BN_CLICKED(IDC_PRINT, onPrintAll)
	ON_BN_CLICKED(IDC_PRINT_VKABRECHNUNG, onClickedPrint_vkabrechnung)
	ON_BN_CLICKED(IDC_PRINT_ZEUGNISSTELLE, onClickedPrint_zeugnisstelle)
	ON_BN_CLICKED(IDC_PRINT_SCHWEISSEREI, onClickedPrint_schweisserei)
	ON_BN_CLICKED(IDC_PRINT_GEWINDEROHR, onClickedPrint_gewinderohr)
	ON_BN_CLICKED(IDC_PRINT_PULVERANLAGE, onClickedPrint_pulveranlage)
	ON_BN_CLICKED(IDC_PRINT_GRUNDIERER, onClickedPrint_grundierer)
	ON_BN_CLICKED(IDC_PRINT_BUENDLER1, onClickedPrint_buendler1)
	ON_BN_CLICKED(IDC_PRINT_BUENDLER2, onClickedPrint_buendler2)
	ON_BN_CLICKED(IDC_PRINT_VERSAND1, onClickedPrint_versand1)
	ON_BN_CLICKED(IDC_PRINT_AWFERTIGUNG, onClickedPrint_awfertigung)
	ON_BN_CLICKED(IDC_PRINT_ROBBI1, onClickedPrint_robbi1)
	ON_BN_CLICKED(IDC_PRINT_ROBBI2, onClickedPrint_robbi2)
	ON_BN_CLICKED(IDC_PRINT_BELIEBIG, onClickedPrint_beliebig)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()






void MehrfachdruckDlg::onPrintAll()
{
	CHtmlJScriptCoreView* pJScriptViewTemplate = (CHtmlJScriptCoreView*)GetParent();
	WaxPart* pt = const_cast<WaxPart*>(pJScriptViewTemplate->GetMainSplitter()->GetCentralDoc()->get_active_object());
	if (!pt->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
	{
		CString msg, str, str2; 
		str = getStammdaten().getText("IDS_FRAGE3").c_str();
		str2 = getStammdaten().getText("IDS_EINZELAUFTRAG").c_str();
		msg.Format(str, str2);
		if (AfxMessageBox(msg, MB_YESNO) == IDNO)
		{
			return;
		}
	}
	onPrint(-1);
}


void MehrfachdruckDlg::onPrint(int iAllorNotAll)
{

	if(iAllorNotAll == -1)
	{
		UpdateData();
	}
	m_InternProgressControl.SetRange32(0, LISTCOUNT);
	m_InternProgressControl.SetPos(0);
	m_InternProgressControl.SetStep(1);
	m_InternProgressControl.StepIt();
	bool bScrollForPrint = false;
	for (int i = 0; i < LISTCOUNT; i++)
	{
		if(data.selected[i])
		{
			CStringArray aPath;
			if(data.selected[BELIEBIGERTEXT] && BELIEBIGERTEXT == i)
			{
				aPath.Add(data.regkey[i] + data.text);
			}
			else
			{
				if(data.selected[AWFERTIGUNG] && AWFERTIGUNG == i)
				{
					aPath.Add( data.regkey[i] + CString(_T("<br/>Firma ")) + data.strFirma);
				}
				else
				{
					if(0 ==i)
					{
						CString str = data.selected[VKABRECHNUNG] ? getStammdaten().getText("IDS_VKABRECHNUNG").c_str() : getStammdaten().getText("IDS_ABRECHNUNGAUFWAND").c_str();
						aPath.Add(str);
					}
					else
					{
						aPath.Add(data.regkey[i]);
					}
				}
			}
			
			((CHtmlJScriptCoreView*)GetParent())->ExecuteJavaScriptFunction("SetDruckHeader", aPath);
			
			for(int j = 0; j < data.anzahl[i]; j++)
			{
				Sleep(2000);
				if(!bScrollForPrint)
				{
					((CHtmlJScriptCoreView*)GetParent())->print(TRUE);
					bScrollForPrint = true;
				}
				else
				{
					((CHtmlJScriptCoreView*)GetParent())->print(FALSE);
				}
			}
		}
		m_InternProgressControl.StepIt();
	}
	m_InternProgressControl.SetPos(0);
}



MehrfachdruckDlg::~MehrfachdruckDlg()
{}



void MehrfachdruckDlg::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	data.dodataexchange(this, pDX);
	CString str, str2; 
	str = getStammdaten().getText("IDS_VKABRECHNUNG").c_str();
	str2 = getStammdaten().getText("IDS_ABRECHNUNGAUFWAND").c_str();
	DDX_Text(pDX, IDC_VKABRECHNUNG, m_Vk3 ?  str : str2);
	DDX_Control(pDX, IDC_PROGRESS, m_InternProgressControl);
}



void MehrfachdruckDlg::SetVk3(bool Vk3)
{
	m_Vk3 = Vk3;
	UpdateData(FALSE);
}



MehrfachdruckDlg::reginit::reginit(bool vk3)
{
	
	CString str, str2; 
	str = getStammdaten().getText("IDS_VKABRECHNUNG").c_str();
	str2 = getStammdaten().getText("IDS_ABRECHNUNGAUFWAND").c_str();
	initdata(VKABRECHNUNG, IDC_VKABRECHNUNG, IDC_N_VKABRECHNUNG, vk3 ? str : str2);

	str = getStammdaten().getText("IDS_ZEUGNISSTELLE").c_str();
	initdata(ZEUGNISSTELLE, IDC_ZEUGNISSTELLE, IDC_N_ZEUGNISSTELLE, str);
	str = getStammdaten().getText("IDS_SCHWEISSEREI").c_str();
	initdata(SCHWEISSEREI, IDC_SCHWEISSEREI, IDC_N_SCHWEISSEREI, str);
	str = getStammdaten().getText("IDS_GEWINDEROHRFERTIGUNG").c_str();
	initdata(GEWINDEROHR, IDC_GEWINDEROHR,  IDC_N_GEWINDEROHR, str);
	str = getStammdaten().getText("IDS_PULVERANLAGE").c_str();
	initdata(PULVERANLAGE, IDC_PULVERANLAGE,  IDC_N_PULVERANLAGE, str);
	str = getStammdaten().getText("IDS_AUSWAERTSFERTIGUNG").c_str();
	initdata(AWFERTIGUNG, IDC_AWFERTIGUNG,  IDC_N_AWFERTIGUNG, str);
	str = getStammdaten().getText("IDS_GRUNDIERER").c_str();
	initdata(GRUNDIERER, IDC_GRUNDIERER, IDC_N_GRUNDIERER, str);
	str = getStammdaten().getText("IDS_BUENDLER").c_str();
	str2.Format(str, 1);
	initdata(BUENDLER1, IDC_BUENDLER1,  IDC_N_BUENDLER1, str2);
	str = getStammdaten().getText("IDS_BUENDLER").c_str();
	str2.Format(str, 2);
	initdata(BUENDLER2, IDC_BUENDLER2,  IDC_N_BUENDLER2, str2);
	str = getStammdaten().getText("IDS_VERSAND").c_str();
	initdata(VERSAND1, IDC_VERSAND1, IDC_N_VERSAND1, str);
	str = getStammdaten().getText("IDS_ROBBI").c_str();
	initdata(ROBBI1, IDC_ROBBI1, IDC_N_ROBBI1, str);
	str = getStammdaten().getText("IDS_ROBBI").c_str();
	str2 = getStammdaten().getText("IDS_QW").c_str();
	initdata(ROBBI2, IDC_ROBBI2, IDC_N_ROBBI2, str+str2);
	initdata(BELIEBIGERTEXT, IDC_BELIEBIG, IDC_N_BELIEBIG, " ");

	for (int i = 0; i < LISTCOUNT; i++)
	{
		selected[i] = AfxGetApp()->GetProfileInt("MEHRFACHDRUCK", regkey[i], 0) ? true :false;
		anzahl[i] = AfxGetApp()->GetProfileInt("MEHRFACHDRUCK", regkey[i] + "_anzahl", 1);
		strFirma = AfxGetApp()->GetProfileString("MEHRFACHDRUCK", "firma", "");
		text = AfxGetApp()->GetProfileString("MEHRFACHDRUCK", "text", "");
	}	
}



MehrfachdruckDlg::reginit::~reginit()
{
	for (int i = 0; i < LISTCOUNT; i++)
	{
		AfxGetApp()->WriteProfileInt("MEHRFACHDRUCK", regkey[i], selected[i]);
		AfxGetApp()->WriteProfileInt("MEHRFACHDRUCK", regkey[i] + "_anzahl", anzahl[i]);
		AfxGetApp()->WriteProfileString("MEHRFACHDRUCK", "firma", strFirma);
		AfxGetApp()->WriteProfileString("MEHRFACHDRUCK", "text", text);
	}
}



void MehrfachdruckDlg::reginit::dodataexchange(CWnd* wnd, CDataExchange* pDX)
{
	for (int i = 0; i < LISTCOUNT; i++)
	{
		DDX_Text(pDX, dlgid_anzahl[i], anzahl[i]);

		CButton *but = (CButton*)wnd->GetDlgItem(dlgid[i]);
		if (pDX->m_bSaveAndValidate)
		{	
			selected[i] = but->GetCheck() ? true : false;
		}
		else
		{
			but->SetCheck(selected[i] ? 1 : 0);
		}
	}

	DDX_Text(pDX, IDC_FIRMA, strFirma);
	DDX_Text(pDX, IDC_TEXT, text);
	
	
/*
	if(pDX->m_bSaveAndValidate)
	{
		CString str, str2; 
		str = getStammdaten().getText("IDS_VKABRECHNUNG").c_str();
		str2 = getStammdaten().getText("IDS_ABRECHNUNGAUFWAND").c_str();
		regkey[VKABRECHNUNG] = selected[VKABRECHNUNG] ? str : str2;
	}
	*/
}


const CString MehrfachdruckDlg::reginit::get_regkey(int index) const
{
	if (index == AWFERTIGUNG)
		return regkey[index] + (CString)" " + strFirma;
	else if (index == BELIEBIGERTEXT)
		return regkey[index] + (CString)" " + text;
	return regkey[index];
}



void MehrfachdruckDlg::reginit::initdata(int liste, UINT id, UINT anzahlid, const CString & text)
{
	dlgid_anzahl[liste] = anzahlid;
	regkey[liste] = text;
	dlgid[liste] = id;
	anzahl[liste] = 0;
}



void MehrfachdruckDlg::onClickedPrint_vkabrechnung()
{
	SetSpecificPrint(VKABRECHNUNG);
}

void MehrfachdruckDlg::onClickedPrint_zeugnisstelle()
{
	SetSpecificPrint(ZEUGNISSTELLE);
}

void MehrfachdruckDlg::onClickedPrint_schweisserei()
{
	SetSpecificPrint(SCHWEISSEREI);
}

void MehrfachdruckDlg::onClickedPrint_gewinderohr()
{
	SetSpecificPrint(GEWINDEROHR);
}

void MehrfachdruckDlg::onClickedPrint_pulveranlage()
{
	SetSpecificPrint(PULVERANLAGE);
}

void MehrfachdruckDlg::onClickedPrint_grundierer()
{
	SetSpecificPrint(GRUNDIERER);	
}

void MehrfachdruckDlg::onClickedPrint_buendler1()
{
	SetSpecificPrint(BUENDLER1);
}

void MehrfachdruckDlg::onClickedPrint_buendler2()
{
	SetSpecificPrint(BUENDLER2);
}


void MehrfachdruckDlg::onClickedPrint_versand1()
{
	SetSpecificPrint(VERSAND1);
}


void MehrfachdruckDlg::onClickedPrint_awfertigung()
{
	SetSpecificPrint(AWFERTIGUNG);
}

void MehrfachdruckDlg::onClickedPrint_robbi1()
{
	
	SetSpecificPrint(ROBBI1);
}

void MehrfachdruckDlg::onClickedPrint_robbi2()
{
	SetSpecificPrint(ROBBI2);
}

void MehrfachdruckDlg::onClickedPrint_beliebig()
{
	SetSpecificPrint(BELIEBIGERTEXT);
}


void MehrfachdruckDlg::SetSpecificPrint(LISTEN iWichPrint)
{
	UpdateData();
	for (int i = 0; i < LISTCOUNT; i++)
	{
		data.selected[i] = false;
	}
	data.selected[iWichPrint] = true;
	onPrint(iWichPrint);
}


void MehrfachdruckDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(point.y < 25)
	{
		CHtmlJScriptCoreView* pParent =(CHtmlJScriptCoreView*)GetParent();
		pParent->SetShowPrintDialog( ! pParent->GetShowPrintDialog() );
		pParent->ShowWindow(SW_HIDE);
		pParent->ShowWindow(SW_SHOW);
	}
	CDialog::OnLButtonUp(nFlags, point);
}


void MehrfachdruckDlg::OnOK()
{
	CPoint point;
	point.y = 10;
	OnLButtonUp( 0, point);
}