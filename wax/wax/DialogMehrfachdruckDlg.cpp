#include "stdafx.h"
#include "resource.h"

#import <shdocvw.dll>
#include <exdisp.h>

#include "WaxHtmlView.h"
#include "WaxSplitterWnd.h"
#include "..\WAXPRJ\JScriptDruck.h"
#include "ChildFrm.h"
#include "JScriptViewTemplate.h"
#include "WaxDoc.h"
#include "SammelAuftragViaList.h"


#include "..\waxprj\DialogWaxHtmlFile.h"
	#include "..\waxprj\DialogHtmlDruck.h"

#include "..\waxprj\MehrfachDialogdrucklistenverwaltung.h"
#import <shdocvw.dll>

#include "SammelAuftragViaList.h"

				#include "DialogMehrfachdruckDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(DialogMehrfachdruckDlg, CDialog)
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
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CLOSE, onClickedOpen_Cloose)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()




void DialogMehrfachdruckDlg::onClickedOpen_Cloose()
{		
	SammelAuftragViaList* pParent = dynamic_cast<SammelAuftragViaList*>(GetParent());
	pParent->SetShowPrintDialog( ! pParent->GetShowPrintDialog() );
	pParent->ShowWindow(SW_HIDE);
	pParent->ShowWindow(SW_SHOW);
}



void DialogMehrfachdruckDlg::onPrintAll()
{
	onPrint(-1);
}




void DialogMehrfachdruckDlg::onPrint( int iAllorNotAll )
{
	

	if(iAllorNotAll == -1)
	{
		UpdateData();
	}
	m_InternProgressControl.SetRange32(0, LISTCOUNT);
	m_InternProgressControl.SetPos(0);
	m_InternProgressControl.SetStep(1);
	m_InternProgressControl.StepIt();
//	bool bScrollForPrint = false;
	CStringArray aPath;
	for (int i = 0; i < LISTCOUNT; i++)
	{
		if(data.selected[i])
		{
			
			if(data.selected[BELIEBIGERTEXT] && BELIEBIGERTEXT == i)
			{
				aPath.Add(data.regkey[i] + data.text);
			}
			else
			{
				if(data.selected[AWFERTIGUNG] && AWFERTIGUNG == i)
				{
					CString strTemp =  data.regkey[i] + CString(_T("<br/>Firma ")) + data.strFirma;
					strTemp = strTemp + "|";
						CString strT;
						strT.Format("%d", data.anzahl[i]);
						strTemp += strT;
						aPath.Add(strTemp);
				}
				else
				{
					if(0 ==i)
					{
						CString strTemp = data.selected[VKABRECHNUNG] ? getStammdaten().getText("IDS_VKABRECHNUNG").c_str() : getStammdaten().getText("IDS_ABRECHNUNGAUFWAND").c_str();
						
						strTemp = strTemp + "|";
						CString strT;
						strT.Format("%d", data.anzahl[i]);
						strTemp += strT;
						aPath.Add(strTemp);
					}
					else
					{
						
						CString strTemp;
						strTemp = data.regkey[i] + "|";
						CString strT;
						strT.Format("%d", data.anzahl[i]);
						strTemp += strT;
						aPath.Add(strTemp);
					}
				}
			}
		
			
		}
		m_InternProgressControl.StepIt();
	}
	m_InternProgressControl.SetPos(0);
	

	MxCObList  vectorOfWerkauftrageLocal;
	for(int i = 0; i < listControl->GetItemCount(); i++)
	{
		if(listControl->GetCheck(i)== TRUE)
		{
			Werkauftrag* pt = (Werkauftrag*) listControl->GetItemData(i);
			vectorOfWerkauftrageLocal.AddTail(pt);
		}
	}

	WaxDruck::MehrfachDialogDrucklistenVerwaltung dialogDrucklistenVerwaltung(&vectorOfWerkauftrageLocal, FALSE);
	int iHowManyToPrint = 0;
	CString strURL = dialogDrucklistenVerwaltung.makeAusgabe(iHowManyToPrint, &aPath);
	//mehrfachdruckDlg.SetHowManyToPrint(iHowManyToPrint);
	
	//COleVariant vanURL(_T("about:blank"));
	//waxBrowser->Navigate2(vanURL, NULL, NULL, NULL, NULL);
	COleVariant vaURL((LPCTSTR)strURL);
	waxBrowser->Navigate2(vaURL, NULL, NULL, NULL, NULL);

	SammelAuftragViaList* pSammelAuftragViaList   =  dynamic_cast<SammelAuftragViaList*>(GetParent());
				
	pSammelAuftragViaList->SetDoPrint(TRUE);
	
	STARTUPINFO si; 
     PROCESS_INFORMATION pi; 
   
     ZeroMemory( &si, sizeof(si) ); 
     si.cb = sizeof(si); 
     ZeroMemory( &pi, sizeof(pi) ); 

	 strURL = CString("C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe ") + strURL;
	
/*
	CreateProcess( NULL,   // No module name (use command line) 
         (LPSTR)(LPCTSTR)strURL,        // Command line 
         NULL,           // Process handle not inheritable 
         NULL,           // Thread handle not inheritable 
         FALSE,          // Set handle inheritance to FALSE 
         0,              // No creation flags 
         NULL,           // Use parent's environment block 
         NULL,           // Use parent's starting directory 
         &si,            // Pointer to STARTUPINFO structure 
         &pi );           // Pointer to PROCESS_INFORMATION structure 
	*/	

}



DialogMehrfachdruckDlg::~DialogMehrfachdruckDlg()
{}



void DialogMehrfachdruckDlg::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	data.dodataexchange(this, pDX);
	CString str, str2; 
	str = getStammdaten().getText("IDS_VKABRECHNUNG").c_str();
	str2 = getStammdaten().getText("IDS_ABRECHNUNGAUFWAND").c_str();
	DDX_Text(pDX, IDC_VKABRECHNUNG, m_Vk3 ?  str : str2);
	DDX_Control(pDX, IDC_PROGRESS, m_InternProgressControl);
}



void DialogMehrfachdruckDlg::SetVk3(bool Vk3)
{
	m_Vk3 = Vk3;
	UpdateData(FALSE);
}



DialogMehrfachdruckDlg::reginit::reginit(bool vk3)
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



DialogMehrfachdruckDlg::reginit::~reginit()
{
	for (int i = 0; i < LISTCOUNT; i++)
	{
		AfxGetApp()->WriteProfileInt("MEHRFACHDRUCK", regkey[i], selected[i]);
		AfxGetApp()->WriteProfileInt("MEHRFACHDRUCK", regkey[i] + "_anzahl", anzahl[i]);
		AfxGetApp()->WriteProfileString("MEHRFACHDRUCK", "firma", strFirma);
		AfxGetApp()->WriteProfileString("MEHRFACHDRUCK", "text", text);
	}
}



void DialogMehrfachdruckDlg::reginit::dodataexchange(CWnd* wnd, CDataExchange* pDX)
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


const CString DialogMehrfachdruckDlg::reginit::get_regkey(int index) const
{
	if (index == AWFERTIGUNG)
		return regkey[index] + (CString)" " + strFirma;
	else if (index == BELIEBIGERTEXT)
		return regkey[index] + (CString)" " + text;
	return regkey[index];
}



void DialogMehrfachdruckDlg::reginit::initdata(int liste, UINT id, UINT anzahlid, const CString & text)
{
	dlgid_anzahl[liste] = anzahlid;
	regkey[liste] = text;
	dlgid[liste] = id;
	anzahl[liste] = 0;
}



void DialogMehrfachdruckDlg::onClickedPrint_vkabrechnung()
{
	SetSpecificPrint(VKABRECHNUNG);
}

void DialogMehrfachdruckDlg::onClickedPrint_zeugnisstelle()
{
	SetSpecificPrint(ZEUGNISSTELLE);
}

void DialogMehrfachdruckDlg::onClickedPrint_schweisserei()
{
	SetSpecificPrint(SCHWEISSEREI);
}

void DialogMehrfachdruckDlg::onClickedPrint_gewinderohr()
{
	SetSpecificPrint(GEWINDEROHR);
}

void DialogMehrfachdruckDlg::onClickedPrint_pulveranlage()
{
	SetSpecificPrint(PULVERANLAGE);
}

void DialogMehrfachdruckDlg::onClickedPrint_grundierer()
{
	SetSpecificPrint(GRUNDIERER);	
}

void DialogMehrfachdruckDlg::onClickedPrint_buendler1()
{
	SetSpecificPrint(BUENDLER1);
}

void DialogMehrfachdruckDlg::onClickedPrint_buendler2()
{
	SetSpecificPrint(BUENDLER2);
}


void DialogMehrfachdruckDlg::onClickedPrint_versand1()
{
	SetSpecificPrint(VERSAND1);
}


void DialogMehrfachdruckDlg::onClickedPrint_awfertigung()
{
	SetSpecificPrint(AWFERTIGUNG);
}

void DialogMehrfachdruckDlg::onClickedPrint_robbi1()
{
	
	SetSpecificPrint(ROBBI1);
}

void DialogMehrfachdruckDlg::onClickedPrint_robbi2()
{
	SetSpecificPrint(ROBBI2);
}

void DialogMehrfachdruckDlg::onClickedPrint_beliebig()
{
	SetSpecificPrint(BELIEBIGERTEXT);
}


void DialogMehrfachdruckDlg::SetSpecificPrint(LISTEN iWichPrint)
{
	UpdateData();
	for (int i = 0; i < LISTCOUNT; i++)
	{
		data.selected[i] = false;
	}
	data.selected[iWichPrint] = true;
	onPrint(iWichPrint);
}


void DialogMehrfachdruckDlg::OnLButtonUp(UINT nFlags, CPoint point)
{

	if(point.y < 25)
	{
		
		SammelAuftragViaList* pParent = dynamic_cast<SammelAuftragViaList*>(GetParent());
		pParent->SetShowPrintDialog( ! pParent->GetShowPrintDialog() );
		pParent->ShowWindow(SW_HIDE);
		pParent->ShowWindow(SW_SHOW);
	}
	CDialog::OnLButtonUp(nFlags, point);
}


void DialogMehrfachdruckDlg::OnOK()
{
	CPoint point;
	point.y = 10;
	OnLButtonUp( 0, point);
}
