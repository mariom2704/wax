#include "stdafx.h"


#include "resource.h"
#include "SapSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(SapSearch, CDialog)
ON_BN_CLICKED(IDC_CHECK_IPPE_MAT_KNOTEN_SD, &SapSearch::OnBnClickedCheckIppe)
END_MESSAGE_MAP()

SapSearch::SapSearch(CWnd* parent) : CDialog(IDD_SEARCH_SAP, parent)
{
	SDPosNr			= 0;
	SDAuftragsNr	= 0;

	IsChoosenIppe	= false;
	IppeMatNr		= "";
}





void SapSearch::OnBnClickedCheckIppe()
{
	UpdateData();
	int iCheckedIsIppeMatKnoten = ((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->GetCheck();
	if(iCheckedIsIppeMatKnoten > 0)
	{
		if( SDAuftragsNr != 0)
		{
			if( AfxMessageBox(getStammdaten().getText("IDS_OUESTION_SDAUFTRAG_SDPOS_SEARCH").c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES )
			{
				IsChoosenIppe = true;
				CWnd* pWnd = GetDlgItem(IDC_STATIC_SD_IPPE_MAT);
				pWnd->SetWindowText(getStammdaten().getText("IDS_IPPEMATERIAL_NR").c_str());
				pWnd = GetDlgItem(IDC_AUFTRAGSNR);
				pWnd->ModifyStyle(ES_NUMBER, 0);
				pWnd->SendMessage(EM_LIMITTEXT, 10, 0);

				pWnd = GetDlgItem(IDC_STATIC_SD_IPPE_KNOTEN);
				pWnd->SetWindowText(getStammdaten().getText("IDS_IPPE_KNOTEN_NR").c_str());
				pWnd = GetDlgItem(IDC_POSNR);

				pWnd->ModifyStyle(ES_NUMBER, 0);
				pWnd->SendMessage(EM_LIMITTEXT, 40, 0);
				pWnd->EnableWindow(FALSE);
				SDAuftragsNr	= 0;
				SDPosNr		= 0;
				UpdateData(FALSE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->SetCheck(FALSE);
			}
		}
		else
		{
			IsChoosenIppe = true;
			CWnd* pWnd = GetDlgItem(IDC_STATIC_SD_IPPE_MAT);
			pWnd->SetWindowText(getStammdaten().getText("IDS_IPPEMATERIAL_NR").c_str());
			pWnd = GetDlgItem(IDC_AUFTRAGSNR);
			pWnd->ModifyStyle(ES_NUMBER, 0);
			pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
		
			pWnd = GetDlgItem(IDC_STATIC_SD_IPPE_KNOTEN);
			pWnd->SetWindowText(getStammdaten().getText("IDS_IPPE_KNOTEN_NR").c_str());
			pWnd = GetDlgItem(IDC_POSNR);
			pWnd->EnableWindow(FALSE);
			pWnd->ModifyStyle(ES_NUMBER, 0);
			pWnd->SendMessage(EM_LIMITTEXT, 40, 0);
			
			UpdateData(FALSE);
		}
		
	}
	else
	{
		
		if( !IppeMatNr.IsEmpty() )
		{
			if( AfxMessageBox(getStammdaten().getText("IDS_OUESTION_IPPEMAT_IPPEKNOTEN").c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES )
			{
				IsChoosenIppe = false;
				GetDlgItem(IDC_STATIC_SD_IPPE_MAT)->SetWindowText(getStammdaten().getText("IDS_SDNR").c_str());
				GetDlgItem(IDC_STATIC_SD_IPPE_KNOTEN)->SetWindowText(getStammdaten().getText("IDS_SDPOS").c_str());
				IppeMatNr	 = "";	
				CString strTemp;
				
				strTemp.Format( "%Id", SDAuftragsNr);
				CWnd* pWnd = GetDlgItem(IDC_AUFTRAGSNR);
				pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
				pWnd->ModifyStyle(0, ES_NUMBER);
				GetDlgItem(IDC_AUFTRAGSNR)->SetWindowText(strTemp);
				
				strTemp.Format("%ld", SDPosNr);
				pWnd = GetDlgItem(IDC_POSNR);
				pWnd->SendMessage(EM_LIMITTEXT, 4, 0);
				pWnd->ModifyStyle(0, ES_NUMBER);
				pWnd->EnableWindow(TRUE);
				pWnd->SetWindowText(strTemp);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_CHECK_IPPE_MAT_KNOTEN_SD))->SetCheck(TRUE);
			}
		}
		else
		{
			IsChoosenIppe = false;
			GetDlgItem(IDC_STATIC_SD_IPPE_MAT)->SetWindowText(getStammdaten().getText("IDS_SDNR").c_str());
			GetDlgItem(IDC_STATIC_SD_IPPE_KNOTEN)->SetWindowText(getStammdaten().getText("IDS_SDPOS").c_str());
			IppeMatNr		= "";	
			CString strTemp;
			
			strTemp.Format( "%Id", SDAuftragsNr);
			CWnd* pWnd = GetDlgItem(IDC_AUFTRAGSNR);
			pWnd->SendMessage(EM_LIMITTEXT, 10, 0);
			pWnd->ModifyStyle(0, ES_NUMBER);
			pWnd->SetWindowText(strTemp);
			
			strTemp.Format("%ld", SDPosNr);
			pWnd = GetDlgItem(IDC_POSNR);
			pWnd->EnableWindow(TRUE);
			pWnd->SendMessage(EM_LIMITTEXT, 4, 0);
			pWnd->ModifyStyle(0, ES_NUMBER);
			pWnd->SetWindowText(strTemp);
		}
	}

}


BOOL SapSearch::OnInitDialog()
{
	return CDialog::OnInitDialog();
}

void SapSearch::DoDataExchange(CDataExchange* pDX)
{
	if(IsChoosenIppe)
	{
		DDX_Text(pDX, IDC_AUFTRAGSNR,  IppeMatNr);
	}
	else
	{
		DDX_Text(pDX, IDC_AUFTRAGSNR, SDAuftragsNr);
		DDX_Text(pDX, IDC_POSNR,      SDPosNr);
	}
	
	CWnd::DoDataExchange(pDX);
}

void SapSearch::OnOK()
{
	CDialog::OnOK();
}

const int SapSearch::Get_SDAuftragsNr() const
{
	return SDAuftragsNr;
}

const int SapSearch::Get_SDPosNr() const
{
	return SDPosNr;
}
