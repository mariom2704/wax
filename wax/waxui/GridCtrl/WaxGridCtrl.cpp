#include "stdafx.h"
#include "WaxGridCellCombo.h"
#include "WaxGridCtrl.h"

CWaxGridCtrl::CWaxGridCtrl(void)
{}



BEGIN_MESSAGE_MAP(CWaxGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CWaxGridCtrl)
	ON_MESSAGE(WM_LBUTTONDOWN_POSTED, OnLButtonDownPosted)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CWaxGridCtrl::~CWaxGridCtrl(void)
{
}

void CWaxGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	 CGridCtrl::OnLButtonDown(nFlags, point);
	 m_bLMouseButtonDown = FALSE;
};
BOOL CWaxGridCtrl::SetColumnCount(int nCols)
{
	CGridCtrl::SetColumnCount(nCols);
	iChoosenConboBoxGegenflanschTyp.SetSize(nCols-1);
	return TRUE;
}


LRESULT CWaxGridCtrl::OnLButtonDownPosted(WPARAM wParam /*iIndex*/, LPARAM lParam /*m_iStoredIndex*/ )
{
	CCellID cellFocusCell = GetFocusCell();

	if(cellFocusCell.row == 1)
	{
		if(cellFocusCell.col == 1)
		{
			if( wParam < 6 )
			{
				CString strTempAbstand = GetItemText( 2, cellFocusCell.col);
				int iTempAbstand = atoi(strTempAbstand);
				if(iTempAbstand > 100)
				{
					SetItemText( 2, cellFocusCell.col, CStringConvert(iTempAbstand));
				}
				else
				{
					SetItemText( 2, cellFocusCell.col, _T("100"));
				}
			}
			else
			{
				CString strTempAbstand = GetItemText( 2, cellFocusCell.col);
				int iTempAbstand = atoi(strTempAbstand);
				if(iTempAbstand > 150)
				{
					SetItemText( 2, cellFocusCell.col, CStringConvert(iTempAbstand));
				}
				else
				{
					SetItemText( 2, cellFocusCell.col, _T("150"));
				}
			}
		}

		if(wParam > 3)
		{
			if(GetItemState(3, cellFocusCell.col) & GVIS_READONLY)
			{
				SetItemState(3, cellFocusCell.col, GetItemState(3, cellFocusCell.col) & ~GVIS_READONLY);
				SetGegenflanschType(cellFocusCell.col, 0);
			}
		}
		else
		{
			SetCellType( 3, cellFocusCell.col, RUNTIME_CLASS(CGridCell));
			SetItemText( 3, cellFocusCell.col, _T(""));
			SetItemState(3, cellFocusCell.col, GetItemState(3, cellFocusCell.col) | GVIS_READONLY);
			SetItemState(4, cellFocusCell.col, GetItemState(4, cellFocusCell.col) | GVIS_READONLY);
		}
	}

	if(cellFocusCell.row == 3)
	{
		if(wParam > 2)
		{
			SetItemState(4, cellFocusCell.col, GetItemState(4, cellFocusCell.col) & ~GVIS_READONLY);
		}
		else
		{
			SetItemState(4, cellFocusCell.col, GetItemState(4, cellFocusCell.col) | GVIS_READONLY);
			SetItemText( 4, cellFocusCell.col, _T(""));
		}
		SetGegenflanschTypeFromSelectionChange(cellFocusCell.col, wParam);
	}

	Invalidate();
	return 0;
}


void CWaxGridCtrl::SetGegenflanschTypeFromSelectionChange(int col, int iSlectedIndex)
{
	iChoosenConboBoxGegenflanschTyp[col-1] = 0;

	switch(iSlectedIndex)
	{

		case 1:
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_COII_SCHWARZ_ABGANG;
		break;
		
		case 2:
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_COII_ABGANG;
		break;

		case 3:
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_GEWINDE_COII;
		break;

		case 4:
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_GEWINDE_MUFFE_COII;
		break;

	}
}

void CWaxGridCtrl::SetGegenflanschType(int col, int FlanschType)
{  
	SetCellType(3,col, RUNTIME_CLASS(CWaxGridCellCombo));
	
	CStringArray options;
	CString strText = getStammdaten().getText("IDS_KEIN_FLANSCH").c_str();
	options.Add(_T(strText));
	strText = getStammdaten().getText("IDS_FLANSCH_SCHWARZ").c_str();
	options.Add(_T(strText));
	strText = getStammdaten().getText("IDS_FLANSCH").c_str();
	options.Add(_T(strText));
	strText = getStammdaten().getText("IDS_FlANSCH_GEWINDE").c_str();
	options.Add(_T(strText));
	strText = getStammdaten().getText("IDS_FLANSCH_MUFFE_GEWINDE").c_str();
	options.Add(_T(strText));
	
	CGridCellCombo *pCell = (CGridCellCombo*) GetCell(3, col);
	pCell->SetOptions(options);

	CString strChooseFType =  getStammdaten().getText("IDS_KEIN_FLANSCH").c_str();
	iChoosenConboBoxGegenflanschTyp[col-1] = 0;

	switch(FlanschType)
	{

		case GEGENFLANSCH_COII_SCHWARZ_ABGANG:
			strChooseFType = getStammdaten().getText("IDS_FLANSCH_SCHWARZ").c_str();;
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_COII_SCHWARZ_ABGANG;
		break;
		
		case GEGENFLANSCH_COII_ABGANG:
			strChooseFType = getStammdaten().getText("IDS_FLANSCH").c_str();
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_COII_ABGANG;
		break;

		case GEGENFLANSCH_GEWINDE_COII:
			strChooseFType =getStammdaten().getText("IDS_FlANSCH_GEWINDE").c_str();
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_GEWINDE_COII;
		break;

		case GEGENFLANSCH_GEWINDE_MUFFE_COII:
			strChooseFType = getStammdaten().getText("IDS_FLANSCH_MUFFE_GEWINDE").c_str();
			iChoosenConboBoxGegenflanschTyp[col-1] = GEGENFLANSCH_GEWINDE_MUFFE_COII;
		break;

	}
	SetItemText(3, col, strChooseFType);

	return;
}



int CWaxGridCtrl::GetGegenflanschType(int iCol)
{ 
	return iChoosenConboBoxGegenflanschTyp[iCol-1];
}



void CWaxGridCtrl::ResetScrollBars()
{
		CGridCtrl::ResetScrollBars();
    EnableScrollBars(SB_VERT, FALSE); 

	return;
}




