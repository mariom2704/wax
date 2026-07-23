
#include "stdafx.h"
#include "resource.h"
#include "wax.h"
#include <exdisp.h>


#include "..\waxprj\DialogWaxHtmlFile.h"
	#include "..\waxprj\DialogHtmlDruck.h"

#include "..\waxprj\Dialogdrucklistenverwaltung.h"
#import <shdocvw.dll>

#include "../rohrkonst/elementlist.h"
#include "..\waxprj\maschinenlisten.h"

				
								#include "SammelAuftragViaList.h"

#pragma warning(disable: 4192)
#pragma warning(disable: 4146)






IMPLEMENT_DYNAMIC(SammelAuftragViaList, CDialog)

SammelAuftragViaList::SammelAuftragViaList(MxCObList* listOfWerkauftrageLocal, CWnd* pParent /*=NULL*/)
	: CDialog(SammelAuftragViaList::IDD, pParent)
{
	vectorOfWerkauftrage = listOfWerkauftrageLocal;
	
	GetDocOnlyOnce = FALSE;
	DoPrint = FALSE;
}



SammelAuftragViaList::~SammelAuftragViaList()
{
	delete maschinenlisten;
}



void SammelAuftragViaList::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EXPLORER_WEB_CONTROL, waxBrowser);
	DDX_Control(pDX, IDC_LIST_AUFRAEGE, listControl);
	DDX_Control(pDX, IDC_BUTTON_GRAFIK_DRUCK, ButtonGrafikDruck);
	DDX_Control(pDX, IDC_BUTTON_SAEGE_NUT, ButtonSaegeNut);
	DDX_Control(pDX, IDC_BUTTON32060, ButtonSaege32060);
	DDX_Control(pDX, IDC_BUTTON_SAEGE32070,ButtonSaege32070);
	DDX_Control(pDX, IDC_BUTTON_38030, ButtonSaege38030_Hand);
	DDX_Control(pDX,  IDC_BUTTON_UEBERGABESTRANGROHRSTRASSE , Strangrohrstrasse);
	CDialog::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(SammelAuftragViaList, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &SammelAuftragViaList::OnBnClickedButtonGrafikDruck)
	ON_BN_CLICKED(IDC_BUTTON_SAEGE_NUT, &SammelAuftragViaList::OnBnClickedButtonSaegenut32020)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_AUFRAEGE, &SammelAuftragViaList::OnNMCustomdrawListAufraege)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AUFRAEGE, &SammelAuftragViaList::OnNMClickListAufraege)
	ON_BN_CLICKED(IDC_BUTTON32060, &SammelAuftragViaList::OnBnClickedButton32060)
	ON_BN_CLICKED(IDC_BUTTON_SAEGE32070, &SammelAuftragViaList::OnBnClickedButtonSaege32070)
	ON_BN_CLICKED(IDC_BUTTON_38030, &SammelAuftragViaList::OnBnClickedButton38030)
	ON_BN_CLICKED(IDC_RADIO_ALL, &SammelAuftragViaList::OnBnClickedRadioAll)
	ON_BN_CLICKED(IDC_RADIO_OD, &SammelAuftragViaList::OnBnClickedRadioOd)
	ON_BN_CLICKED(IDC_RADIO_WITT, &SammelAuftragViaList::OnBnClickedRadioWitt)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &SammelAuftragViaList::OnBnClickedButtonPrint)
	ON_MESSAGE(WM_GRID_CLICKED, OnStatusMessage)	
	ON_BN_CLICKED(IDC_BUTTON_ARBEITSPLAENE, &SammelAuftragViaList::OnBnClickedButtonArbeitsplaene)
	ON_BN_CLICKED(IDC_RADIO_NONE, &SammelAuftragViaList::OnBnClickedRadioNone)
	ON_BN_CLICKED(IDC_BUTTON_TRANFER_TO_SAP, &SammelAuftragViaList::OnBnClickedButtonTranferToSap)
	ON_BN_CLICKED(IDC_BUTTON_UEBERGABESTRANGROHRSTRASSE, &SammelAuftragViaList::OnBnClickedButtonUebergabestrangrohrstrasse)
END_MESSAGE_MAP()





void SammelAuftragViaList::OnDocumentCompleteExplorerScreen(LPDISPATCH pDisp, VARIANT FAR* URL)
{ 	
	if(GetDocOnlyOnce == FALSE)
	{
		IWebBrowser2Ptr webBrowser(pDisp);

		(*webBrowser).get_Document(&htmlDocDisp);
		
		
		GetDocOnlyOnce = TRUE;
	
	}
	if( DoPrint == TRUE)
	{
		DoPrint = FALSE;
		print(FALSE);
	}
}




BEGIN_EVENTSINK_MAP(SammelAuftragViaList, CDialog)
 ON_EVENT(SammelAuftragViaList, IDC_EXPLORER_WEB_CONTROL, 259 /* DocumentComplete */, OnDocumentCompleteExplorerScreen, VTS_DISPATCH VTS_PVARIANT)
 
END_EVENTSINK_MAP()



BOOL SammelAuftragViaList::OnInitDialog()
{
	CDialog::OnInitDialog();

	Strangrohr* strangrohr = dynamic_cast<Strangrohr*>( vectorOfWerkauftrage->GetHead());

	Strangrohrstrasse.EnableWindow(FALSE);
	if(strangrohr !=  NULL)
	{
		Strangrohrstrasse.EnableWindow(TRUE);
	}


	COleVariant vaURL(_T("about:blank"));
	
	waxBrowser.Navigate2(vaURL, NULL, NULL, NULL, NULL);

	listControl.SetExtendedStyle(LVS_EX_GRIDLINES |  LVS_EX_ONECLICKACTIVATE | LVS_EX_FULLROWSELECT |LVS_EX_CHECKBOXES | LVS_EX_TRACKSELECT);
	listControl.InsertColumn(0, "SDAuftrag", LVCFMT_LEFT , 130 , -1);
	listControl.InsertColumn(1, "Wanr", LVCFMT_LEFT , 90, -1);
	listControl.InsertColumn(2, "DN", LVCFMT_RIGHT , 80, -1);
	listControl.InsertColumn(3, "SDPos", LVCFMT_RIGHT , 50 , -1);

	listControl.DeleteAllItems();
	int i = 0;
	for (POSITION pos = vectorOfWerkauftrage->GetHeadPosition(); pos; )
	{
		Werkauftrag* pt = dynamic_cast<Werkauftrag*>(vectorOfWerkauftrage->GetNext(pos));
	
		Leitkarte& leitkarte = pt->get_leitkarte();
		CString temp;
		temp.Format("%I64d",leitkarte.get_sdAuftragsnr());
		listControl.InsertItem(i, temp);

		listControl.SetItemData(i,(DWORD_PTR)pt);

		Status& status = pt->get_status();
		temp.Format("%d", status.get_wanr());
		listControl.SetItemText(i, 1, temp);

		Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(pt);
		temp.Format("DN %d", rohr->get_dnvl());
		listControl.SetItemText( i, 2, temp);
		
		temp.Format("%d", leitkarte.get_sdPositionsnr());
		listControl.SetItemText( i, 3, temp);
		i++;
	}
	
	

	mehrfachdruckDlg.Create(IDD_DIALOG_MEHRFACHDRUCK, this);	
	mehrfachdruckDlg.SetListControl(&listControl, &waxBrowser);

	ButtonSaegeNut.EnableWindow(FALSE);
	ButtonSaege32060.EnableWindow(FALSE);
	ButtonSaege32070.EnableWindow(FALSE);
	ButtonSaege38030_Hand.EnableWindow(FALSE);

	AdjustWindows();

	maschinenlisten = NULL;
	
	return TRUE; 
}



void SammelAuftragViaList::AdjustWindows()
{
	CRect rectMehrfachdruckDlg;
	mehrfachdruckDlg.GetClientRect(rectMehrfachdruckDlg);

	
		CRect rectButtonGrafikDruck;
		ButtonGrafikDruck.GetClientRect(rectButtonGrafikDruck);


		mehrfachdruckDlg.SetWindowPos(NULL, waxBrowser.GetControlSite()->m_rect.left, waxBrowser.GetControlSite()->m_rect.top,
			rectMehrfachdruckDlg.Width(), rectMehrfachdruckDlg.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		//::AdjustWindowRectEx(rect,m_wndBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		
		waxBrowser.SetWindowPos(NULL, waxBrowser.GetControlSite()->m_rect.left, rectMehrfachdruckDlg.Height()+ rectButtonGrafikDruck.Height()+20,
			waxBrowser.GetControlSite()->m_rect.Width()+70, waxBrowser.GetControlSite()->m_rect.Height() - rectMehrfachdruckDlg.Height(), SWP_NOACTIVATE | SWP_NOZORDER);		
	
		CRect rectDruck;
	mehrfachdruckDlg.GetClientRect(rectDruck);
	iDialogHeight = rectDruck.Height();

CRect rectListcontrol;
		listControl.GetWindowRect(rectListcontrol);

		CRect rect;
		this->GetWindowRect(rect);
		iListTop =  rectListcontrol.top - rect.top;



}




void SammelAuftragViaList::OnBnClickedButtonGrafikDruck()
{
	MxCObList  vectorOfWerkauftrageLocal;
	for(int i = 0; i < listControl.GetItemCount(); i++)
	{
		if(listControl.GetCheck(i)== TRUE)
		{
			Werkauftrag* pt = (Werkauftrag*) listControl.GetItemData(i);
			vectorOfWerkauftrageLocal.AddTail(pt);
		}
	}

	
	WaxDruck::DialogDrucklistenVerwaltung dialogDrucklistenVerwaltung(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::GRAPHIKDRUCK, &vectorOfWerkauftrageLocal, FALSE);
	CString strURL = dialogDrucklistenVerwaltung.makeAusgabe(iHowManyToPrint);
	mehrfachdruckDlg.SetHowManyToPrint(iHowManyToPrint);
	
	COleVariant vanURL(_T("about:blank"));
	waxBrowser.Navigate2(vanURL, NULL, NULL, NULL, NULL);
	COleVariant vaURL((LPCTSTR)strURL);
	waxBrowser.Navigate2(vaURL, NULL, NULL, NULL, NULL);
	STARTUPINFO si; 
     PROCESS_INFORMATION pi; 
   
     ZeroMemory( &si, sizeof(si) ); 
     si.cb = sizeof(si); 
     ZeroMemory( &pi, sizeof(pi) ); 
/*
	 strURL = CString("C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe ") + strURL;
	
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

void SammelAuftragViaList::OnBnClickedButtonSaegenut32020()
{


	DialogExcel excel;

	
	if (WaxSys::isAV())
	{
		excel.setExcelTableOfficeVersion2010(false);
	}
	

	// Objekte der 1. Untergliederungsebene in Liste packen
	
	excel.set_walist(maschinenlisten->GetWerkauftraege32020());
	
	/*
	excel.initAction();
	pop.CreatePopupMenu();

	for (int i = 0; i < excel.get_aktionen().GetSize(); i++)
	{
		int id = excel.get_aktionen().GetAt(i);
		pop.AppendMenu(MF_STRING, id,  (LPCTSTR)excel.getName(id));
	}
	
	CPoint pt = ((CMainFrame*)AfxGetMainWnd())->getToolbarButton(ID_EXCEL);
	cmd = pop.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, this); 

	if (cmd != Excel::PREISTEST && cmd != Excel::AUFTRAGSTABELLE && cmd != Excel::ZUSAMMENFASSUNG_WERKAUFTRAEGE)
	{
		// Fuer Maschine muss Werkauftrag markiert sein
		if (wa->IsKindOf(RUNTIME_CLASS(PrjGliederung)) || wa->IsKindOf(RUNTIME_CLASS(PrjLeitkarteData)) || wa->IsKindOf(RUNTIME_CLASS(WpPreisanfrage)))
		{
			CString str; str = getStammdaten().getText("IDS_HINWEIS2a").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONINFORMATION);	
			return;
		}
		// Ist Werkauftrag unter Sammelauftrag muss dieser makiert sein
		if (wa->get_parent() && wa->get_parent()->IsKindOf(RUNTIME_CLASS(SammelAuftrag)))
		{
			CString str; str = getStammdaten().getText("IDS_HINWEIS1a").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			return;
		}
	}
	excel.set_currentAction(cmd);
	pop.DestroyMenu();
	*/
	UINT cmd = DialogExcel::EXCELAKTIONEN::SAEGENUTANLAGE;
	excel.makeAction(cmd);

	AfxMessageBox("Die Aufträge wurden übertragen", MB_OK);

}




bool SammelAuftragViaList::print(int iPrintWidthScroll)
{
	CStringArray aPath;
	aPath.Add(_T(""));
	
	// HTML-Document direkt drucken
	LPDISPATCH lpDispatch = htmlDocDisp;
	
	
	//lpDispatch = GetHtmlDocument(); 
	//(*webBrowser).get_Document(lpDispatch);
	XASSERT(lpDispatch);

	if (lpDispatch == NULL)
		return false;

	// Print ueber IOleCommandTarget		
	LPOLECOMMANDTARGET lpOleCommandTarget = NULL;
	lpDispatch->QueryInterface(IID_IOleCommandTarget, (void**)&lpOleCommandTarget);
	XASSERT(lpOleCommandTarget);

	if (lpOleCommandTarget == NULL)
		return false;

	lpOleCommandTarget->Exec(NULL, OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);

	lpOleCommandTarget->Release();
	//lpDispatch->Release();
	return true;
}






void SammelAuftragViaList::GetSpecificWekauftraege(SELECTEDWERK werk)
{
	

	ButtonSaegeNut.EnableWindow(FALSE);
	ButtonSaege32060.EnableWindow(FALSE);
	ButtonSaege32070.EnableWindow(FALSE);
	ButtonSaege38030_Hand.EnableWindow(FALSE);

	
	
	if(maschinenlisten != NULL)
	{
		delete maschinenlisten;
		maschinenlisten = NULL;
	}

	maschinenlisten = new WaxDruck::Maschinenlisten(NULL);

	for(int i = 0; i < listControl.GetItemCount(); i++)
	{
		listControl.SetCheck(i, FALSE);
	}


	if(werk == SELECTEDWERK::NONE)
	{
		return;
	}

	for(int i = 0; i < listControl.GetItemCount(); i++)
	{

						
			if(werk == SELECTEDWERK::ALL)
			{
				listControl.SetCheck(i, TRUE);
				Werkauftrag* werkauftrag = (Werkauftrag*) listControl.GetItemData(i);
				maschinenlisten->createMaschinenRohrleitung(werkauftrag);
			}
			else
			{
				if(werk == SELECTEDWERK::OLDESLOE)
				{
					Werkauftrag* werkauftrag = (Werkauftrag*) listControl.GetItemData(i);
					if(werkauftrag->get_GUID().IsEmpty() == TRUE)
					{
						listControl.SetCheck(i, TRUE);
						maschinenlisten->createMaschinenRohrleitung(werkauftrag);
					}
				}
				if(werk == SELECTEDWERK::WITTENBERGE)
				{
					Werkauftrag* werkauftrag = (Werkauftrag*) listControl.GetItemData(i);
					if(werkauftrag->get_GUID().IsEmpty() == FALSE)
					{
						listControl.SetCheck(i, TRUE);
						maschinenlisten->createMaschinenRohrleitung(werkauftrag);
					}
				}
			}
	}

	if(maschinenlisten->zuschnittliste_32020.GetSize() > 0)
	{
		ButtonSaegeNut.EnableWindow(TRUE);
	}
	if(maschinenlisten->zuschnittliste_32060.GetSize() > 0)
	{
		ButtonSaege32060.EnableWindow(TRUE);
	}

	if(maschinenlisten->zuschnittliste_32070.GetSize() > 0)
	{
		ButtonSaege32070.EnableWindow(TRUE);
	}

	if(maschinenlisten->zuschnittliste_38030_handsw.GetSize() > 0)
	{
		ButtonSaege38030_Hand.EnableWindow(TRUE);
	}
	
}



void SammelAuftragViaList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (::IsWindow(waxBrowser.m_hWnd))
	{
		CRect rectListcontrol;
		listControl.GetClientRect(rectListcontrol);

		CRect rectButtonGrafikDruck;
		ButtonGrafikDruck.GetClientRect(rectButtonGrafikDruck);

		CRect rectDruck;
		mehrfachdruckDlg.GetClientRect(rectDruck);
		CRect rect;
		GetClientRect(rect);
		
		rectListcontrol.right = 390;
		if(m_bShowPrintDialog)
		{
			rect.top = iDialogHeight + 4;
			rectDruck.bottom = iDialogHeight;
		}
		else
		{
			rectDruck.bottom = 25;
		//	rect.top = 27;
		}
		mehrfachdruckDlg.SetWindowPos(NULL, rectListcontrol.right + 35, rectButtonGrafikDruck.Height()+ 20,
			rectDruck.Width(), rectDruck.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

		//::AdjustWindowRectEx(rect,waxBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);

			if(m_bShowPrintDialog)
			{
				//::AdjustWindowRectEx(rect,waxBrowser.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
				waxBrowser.SetWindowPos(NULL, rectListcontrol.right + 15, rectButtonGrafikDruck.Height()+ rectDruck.Height(),
				rect.Width()-(rectListcontrol.right + 20), rect.Height()-rectButtonGrafikDruck.Height()-10, SWP_NOACTIVATE | SWP_NOZORDER);
			}
			
			else
			{
				waxBrowser.SetWindowPos(NULL, rectListcontrol.right + 15, rectButtonGrafikDruck.Height()+ 20 + rectDruck.Height(),
				rect.Width()-(rectListcontrol.right + 20), rect.Height()-rectButtonGrafikDruck.Height()-30, SWP_NOACTIVATE | SWP_NOZORDER);
			}
			
		listControl.SetWindowPos(NULL, 10, iListTop , rectListcontrol.Width(), rect.Height()+ 65, SWP_NOACTIVATE | SWP_NOZORDER);
	}
}






void SammelAuftragViaList::OnNMCustomdrawListAufraege(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    *pResult = 0;
  
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
         {
			
			
			Werkauftrag* werkauftrag = (Werkauftrag*) listControl.GetItemData(pLVCD->nmcd.dwItemSpec);
	
	
			pLVCD->clrTextBk = RGB(255, 255, 255);
			pLVCD->clrText = RGB(0, 0, 0);
			if(werkauftrag->get_GUID().IsEmpty() == FALSE )
			{
				pLVCD->clrTextBk = RGB(165, 255, 74);
			}
			
			*pResult = CDRF_DODEFAULT;
         }
}



void SammelAuftragViaList::OnNMClickListAufraege(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	MxCObList  vectorOfWerkauftrageLocal;
	
	if(pNMLV->iItem == -1)
		return;

	Werkauftrag* pt = (Werkauftrag*) listControl.GetItemData(pNMLV->iItem);
	vectorOfWerkauftrageLocal.AddTail(pt);
	
	
	WaxDruck::DialogDrucklistenVerwaltung dialogDrucklistenVerwaltung(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::GRAPHIKDRUCK, &vectorOfWerkauftrageLocal, FALSE);
	CString strURL = dialogDrucklistenVerwaltung.makeAusgabe(iHowManyToPrint);
	mehrfachdruckDlg.SetHowManyToPrint(iHowManyToPrint);
	
	COleVariant vanURL(_T("about:blank"));
	waxBrowser.Navigate2(vanURL, NULL, NULL, NULL, NULL);
	COleVariant vaURL((LPCTSTR)strURL);
	waxBrowser.Navigate2(vaURL, NULL, NULL, NULL, NULL);

	ButtonSaegeNut.EnableWindow(FALSE);
	ButtonSaege32060.EnableWindow(FALSE);
	ButtonSaege32070.EnableWindow(FALSE);
	ButtonSaege38030_Hand.EnableWindow(FALSE);

	PostMessage(WM_GRID_CLICKED, 0,0);

	*pResult = 0;
}




LRESULT SammelAuftragViaList::OnStatusMessage(WPARAM nIndex , LPARAM lPARAM)
{


	if(maschinenlisten != NULL)
	{
		delete maschinenlisten;
		maschinenlisten = NULL;
	}
	maschinenlisten = new WaxDruck::Maschinenlisten(NULL);
//	if(pNMLV->iSubItem == 0)
	{
		MxCObList  vectorOfWerkauftrageLocal;
		for(int i = 0; i < listControl.GetItemCount(); i++)
		{
			if(listControl.GetCheck(i)== TRUE)
			{
				Werkauftrag* werkauftrag = (Werkauftrag*) listControl.GetItemData(i);
				maschinenlisten->createMaschinenRohrleitung(werkauftrag);
			}
		}
	}



	if(maschinenlisten->zuschnittliste_32020.GetSize() > 0)
	{
		ButtonSaegeNut.EnableWindow(TRUE);
	}
	if(maschinenlisten->zuschnittliste_32060.GetSize() > 0)
	{
		ButtonSaege32060.EnableWindow(TRUE);
	}

	if(maschinenlisten->zuschnittliste_32070.GetSize() > 0)
	{
		ButtonSaege32070.EnableWindow(TRUE);
	}

	if(maschinenlisten->zuschnittliste_38030_handsw.GetSize() > 0)
	{
		ButtonSaege38030_Hand.EnableWindow(TRUE);
	}
	

	return 0;
};



void SammelAuftragViaList::OnBnClickedButton32060()
{
	generateCuttingList(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ZUSCHNITTSLISTE_32060);
}
		

void SammelAuftragViaList::generateCuttingList(int NumberOfList)
{
	MxCObList  vectorOfWerkauftrageLocal;

	WaxDruck::DialogDrucklistenVerwaltung* dialogDrucklistenVerwaltung = NULL;

	if(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ZUSCHNITTSLISTE_32060 == NumberOfList)
	{
		dialogDrucklistenVerwaltung = new WaxDruck::DialogDrucklistenVerwaltung(NumberOfList, maschinenlisten->GetWerkauftraege32060(), FALSE);
	}

	if(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ZUSCHNITTSLISTE_32070 == NumberOfList)
	{
		dialogDrucklistenVerwaltung = new WaxDruck::DialogDrucklistenVerwaltung(NumberOfList, maschinenlisten->GetWerkauftraege32070(), FALSE);
	}

	if(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ZUSCHNITTSLISTE_38030_HANDSW == NumberOfList)
	{
		dialogDrucklistenVerwaltung = new WaxDruck::DialogDrucklistenVerwaltung(NumberOfList, maschinenlisten->GetWerkauftraege38030(), FALSE);
	}

	dialogDrucklistenVerwaltung->SetMaschliste(maschinenlisten);
	CString strURL = dialogDrucklistenVerwaltung->makeAusgabe(iHowManyToPrint);
	mehrfachdruckDlg.SetHowManyToPrint(iHowManyToPrint);
	delete dialogDrucklistenVerwaltung;

	COleVariant vanURL(_T("about:blank"));
	waxBrowser.Navigate2(vanURL, NULL, NULL, NULL, NULL);
	COleVariant vaURL((LPCTSTR)strURL);
	waxBrowser.Navigate2(vaURL, NULL, NULL, NULL, NULL);
	/*
STARTUPINFO si; 
     PROCESS_INFORMATION pi; 
   
     ZeroMemory( &si, sizeof(si) ); 
     si.cb = sizeof(si); 
     ZeroMemory( &pi, sizeof(pi) ); 

	 strURL = CString("C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe ") + strURL;


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



void SammelAuftragViaList::OnBnClickedButtonSaege32070()
{
	generateCuttingList(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ZUSCHNITTSLISTE_32070);
}


void SammelAuftragViaList::OnBnClickedButton38030()
{
	generateCuttingList(WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ZUSCHNITTSLISTE_38030_HANDSW);
}



void SammelAuftragViaList::OnBnClickedRadioAll()
{
	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_RADIO_ALL));
	
	if (poButton->GetCheck()== TRUE)
	{
		GetSpecificWekauftraege(SELECTEDWERK::ALL);
	}
}



void SammelAuftragViaList::OnBnClickedRadioOd()
{
	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_RADIO_OD));
	
	if (poButton->GetCheck()== TRUE)
	{
		GetSpecificWekauftraege(SELECTEDWERK::OLDESLOE);
	}
}



void SammelAuftragViaList::OnBnClickedRadioWitt()
{
	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_RADIO_WITT));
	
	if (poButton->GetCheck() == TRUE)
	{
		GetSpecificWekauftraege(SELECTEDWERK::WITTENBERGE);
	}
}



void SammelAuftragViaList::OnBnClickedButtonPrint()
{
	WaxDruck::DialogDrucklistenVerwaltung dialogDrucklistenVerwaltung;
	int CurrentAusgabe = dialogDrucklistenVerwaltung.get_currentAusgabe();
	if( CurrentAusgabe == WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::GRAPHIKDRUCK )
	{
		mehrfachdruckDlg.onPrint();
	}
	else
	{
		print(FALSE);
	}
	
}



void SammelAuftragViaList::OnBnClickedButtonArbeitsplaene()
{
	
	MxCObList  vectorOfWerkauftrageLocal;
	for(int i = 0; i < listControl.GetItemCount(); i++)
	{
		if(listControl.GetCheck(i)== TRUE)
		{
			Werkauftrag* pt = (Werkauftrag*) listControl.GetItemData(i);
			vectorOfWerkauftrageLocal.AddTail(pt);
		}
	}

	WaxDruck::DialogDrucklistenVerwaltung dialogDrucklistenVerwaltung( WaxDruck::DialogDrucklistenVerwaltung::DRUCKLISTEN::ARBEITSPLAN, &vectorOfWerkauftrageLocal, FALSE);
	//dialogDrucklistenVerwaltung.set_currentAusgabe(2);
	CString strURL = dialogDrucklistenVerwaltung.makeAusgabe(iHowManyToPrint);
	mehrfachdruckDlg.SetHowManyToPrint(iHowManyToPrint);
	
	COleVariant vanURL(_T("about:blank"));
	waxBrowser.Navigate2(vanURL, NULL, NULL, NULL, NULL);
	COleVariant vaURL((LPCTSTR)strURL);
	waxBrowser.Navigate2(vaURL, NULL, NULL, NULL, NULL);
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




void SammelAuftragViaList::OnBnClickedRadioNone()
{
	CButton* poButton = (CButton*)(GetDlgItem(IDC_RADIO_NONE));
	
	if (poButton->GetCheck()== TRUE)
	{
		GetSpecificWekauftraege(SELECTEDWERK::NONE);
	}
}




void SammelAuftragViaList::OnBnClickedButtonTranferToSap()
{	

	MxCObList  vectorOfWerkauftrageLocal;
	for(int i = 0; i < listControl.GetItemCount(); i++)
	{
		if(listControl.GetCheck(i)== TRUE)
		{
			Sap sap;
			sap.set_currentAusgabe(Sap::WAXTOSAP);

			Werkauftrag* pt = (Werkauftrag*) listControl.GetItemData(i);
			sap.set_werkauftrag(pt);
			sap.makeAusgabe(Sap::WAXTOSAP);
		}
	}
	AfxMessageBox("Die Aufträge wurden nach SAP übertragen", MB_OK);
}





void SammelAuftragViaList::OnBnClickedButtonUebergabestrangrohrstrasse()
{

	
	for(int i = 0; i < listControl.GetItemCount(); i++)
	{
		if(listControl.GetCheck(i)== TRUE)
		{
			MxCObList  vectorOfWerkauftrageLocal;
			Werkauftrag* pt = (Werkauftrag*) listControl.GetItemData(i);
			vectorOfWerkauftrageLocal.AddTail(pt);
	
			DialogExcel excel;
			
			if (WaxSys::isAV())
			{
				excel.setExcelTableOfficeVersion2010(false);
			}
			

			
			excel.set_walist(&vectorOfWerkauftrageLocal);
			
			
			UINT cmd = DialogExcel::EXCELAKTIONEN::STRANGROHRANLAGE_1;
			excel.makeAction(cmd);
		}
	}

	AfxMessageBox("Die Aufträge wurden übertragen", MB_OK);

}
