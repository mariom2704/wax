#pragma once
#include <exdisp.h>
#import <shdocvw.dll>
#include "WaxBrowser.h"
#include "DialogMehrfachdruckDlg.h"


class PrintSelection
{
	public:
	enum SelectionPrint
	{
		grafic,
		arbeitsplaene,
		vk3
	};
	SelectionPrint	selectionPrint;
	CString			selectionPrintName;
};


class SammelAuftragViaList : public CDialog
{
	DECLARE_DYNAMIC(SammelAuftragViaList)

	enum SELECTEDWERK
	{
		OLDESLOE,
		WITTENBERGE,
		ALL,
		NONE
	};
	

	void GetSpecificWekauftraege(SELECTEDWERK Werk);

	BOOL DoPrint;
	BOOL GetDocOnlyOnce;

public:
	
	DialogMehrfachdruckDlg mehrfachdruckDlg;

	void SetDoPrint(BOOL DoPrintLocal)
	{
		DoPrint = DoPrintLocal;
	}

	IDispatchPtr htmlDocDisp;

	void AdjustWindows();

	MxCObList*  vectorOfWerkauftrage;

	SammelAuftragViaList(MxCObList* listOfWerkauftrage, CWnd* pParent = NULL);
	virtual ~SammelAuftragViaList();


	enum { IDD = IDD_SAMMELAUFTRAGVIALIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	
	afx_msg void OnDocumentCompleteExplorerScreen(LPDISPATCH pDisp, VARIANT FAR* URL);
	DECLARE_MESSAGE_MAP()


    DECLARE_EVENTSINK_MAP()
   
	bool m_bShowPrintDialog;


public:

	
	CWaxBrowser waxBrowser;
	CListCtrl	listControl;
	CButton		checkBox, ButtonGrafikDruck, ButtonSaegeNut, ButtonSaege32060, ButtonSaege32070,  ButtonSaege38030_Hand, Strangrohrstrasse;
	
	WaxDruck::Maschinenlisten* maschinenlisten;
	
	
	LRESULT OnStatusMessage(WPARAM , LPARAM );

	int iDialogHeight;
	int iListTop;

	void SetShowPrintDialog(bool showDialog)
	{
		m_bShowPrintDialog = showDialog;
		CRect rectListcontrol;
		GetClientRect(rectListcontrol);
		OnSize(2,  rectListcontrol.Height(), rectListcontrol.Width());
		/*
			SetWindowPos(NULL, rectListcontrol.left, rectListcontrol.top +10,
			rectListcontrol.Width(), rectListcontrol.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
			*/
	}

	bool GetShowPrintDialog()
	{
		return m_bShowPrintDialog;
		
	}

	virtual BOOL OnInitDialog();
	bool print(int iPrintWidthScroll);
	void generateCuttingList(int NumberOfCuttingList);
	int iHowManyToPrint;
	afx_msg void OnBnClickedButtonGrafikDruck();
	afx_msg void OnBnClickedButtonSaegenut32020();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg void OnNMCustomdrawListAufraege(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListAufraege(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton32060();
	afx_msg void OnBnClickedButtonSaege32070();
	
	
	afx_msg void OnBnClickedButton38030();
	
	afx_msg void OnBnClickedRadioAll();
	afx_msg void OnBnClickedRadioOd();
	afx_msg void OnBnClickedRadioWitt();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonArbeitsplaene();
	afx_msg void OnBnClickedRadioNone();
	afx_msg void OnBnClickedButtonTranferToSap();
	afx_msg void OnBnClickedButtonUebergabestrangrohrstrasse();
};
