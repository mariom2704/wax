#include "stdafx.h"


#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int POSNR		 = 0;
const int SAPAUFTRAG   = 1;
const int SAPAUFTRAGII = 2;

const int LEN_AUFTRAGSNR  =  12;
const int LEN_PROJEKTNR   =   8;
const int LEN_PSP_ELEMENT =  20;
const int LEN_SD_AUFTRNR  =  10;
const int LEN_SD_POSINR   =   6;
const int LEN_LIEFDAT     =   8;
const int LEN_SD_MATNR    =  18;
const int LEN_SACHBEARB   =  12;
const int LEN_BEZEICHNUNG =  20;
const int LEN_DEBITORENNR =  10;
const int LEN_AUFTRAGSBEZ =  35;
const int LEN_KEY         =   3;
const int LEN_PLZ         =  10;
const int LEN_ORT         =  35;
const int LEN_NL          =  12;
const int LEN_BAU         =  12;
const int LEN_AUSSTELLER  =  12;
const int LEN_DATUM       =   8;
const int LEN_KC_AUFTRNR  =  10;
const int LEN_KC_POSINR   =   6;
const int LEN_IS_NEXT_GENERATION = 1;
const int LEN_IPPE_MATNR		 = 18;
const int LEN_IPPE_KNOTEN_NR	 = 40;


const int POS_NEXT_GENERATION	= 267;
const int POS_IPPE_MATNR		= 268;
const int POS_IPPE_KNOTEN_NR	= 286;
const int POS_ENDE				= 326;




BEGIN_MESSAGE_MAP(SapfileOpen, CDialog)
	ON_BN_CLICKED(IDC_SEARCH, OnClickedSearch)
END_MESSAGE_MAP()

SapfileOpen::SapfileOpen() : CDialog(IDD_OPEN_SAP), list(0,0,0)
{
	select = -1;
	bIgnoreSelection = false;
}

SapfileOpen::~SapfileOpen()
{

}

BOOL SapfileOpen::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

BOOL SapfileOpen::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;
	
	CString str;
	if (sapfile == Sap::DELETEWAXFROMSAP)
	{
		GetDlgItem(IDC_SEARCH)->EnableWindow(FALSE);
		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(POSNR, str, LVCFMT_RIGHT, 50);
		str = getStammdaten().getText("IDS_AUFTRAEGE").c_str();
		list.InsertColumn(SAPAUFTRAG, str, LVCFMT_LEFT, 200);
	}
	else
	{
		str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
		list.InsertColumn(POSNR, str, LVCFMT_RIGHT, 50);
		str = getStammdaten().getText("IDS_AUFTRAEGE").c_str();
		list.InsertColumn(SAPAUFTRAG, str, LVCFMT_LEFT, 1600);
		str = getStammdaten().getText("IDS_AUFTRAEGE").c_str();
		list.InsertColumn(SAPAUFTRAGII, str, LVCFMT_LEFT, 400);
	}

	if (sapfile == Sap::DELETEWAXFROMSAP)
		GetDlgItem(IDC_SEARCH)->EnableWindow(FALSE);
	
	
	UpdateData(false);
	
	return true;
}

void SapfileOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SAPAUFTRAEGE, list);


	if (!pDX->m_bSaveAndValidate) 
	{	
		if (!list.getColumnCount())
			return;

		if (sapfile == Sap::SAPTOWAX_FILE || sapfile == Sap::SAPTOWAX_ALL)
			fillStringBox();
		else if (sapfile == Sap::DELETEWAXFROMSAP)
			fillStringBox2();

		if (sapfile == Sap::SAPTOWAX_FILE || sapfile == Sap::SAPTOWAX_ALL)
		{
			bool bIsSearch = false;
			if(bIsNextGeneration)
			{
				if (strIppeMatNr.IsEmpty())
				{
					bIsSearch = true;
					OnClickedSearch();
				}
			}
			else
			{
				if (aufnr == 0 || posnr == 0)
				{
					bIsSearch = true;
					OnClickedSearch();
				}
			}

			if(!bIsSearch)
			{
				if(!bIsNextGeneration)
				{
					select = checkSDNr();
					if(select == -1)
					{
						CString msg, str; str = getStammdaten().getText("IDS_HINWEIS6a").c_str();
						msg.Format(str, aufnr, posnr);  
						AfxMessageBox(msg, MB_ICONERROR | MB_OK);
					}
				}
				else
				{
					select = checkIppeNr();
					if(select == -1)
					{
						CString msg, str; str = getStammdaten().getText("IDS_HINWEIS6a").c_str();
						msg.Format(str, aufnr, posnr);  
						AfxMessageBox(msg, MB_ICONERROR | MB_OK);
					}
				}
			}
			
		}
	}
}



void SapfileOpen::OnOK()
{
	if (sapfile == Sap::SAPTOWAX_FILE || sapfile == Sap::SAPTOWAX_ALL)
	{
		if(!bIgnoreSelection)
		{
			if (sapauftraege.GetSize() > 0)
				select = list.getSelectedItem();
		}
	}
	else if (sapfile == Sap::DELETEWAXFROMSAP)
	{
		wanummer->RemoveAll();
		/*
		CObList* items = list.getSelectedItems();
		for (POSITION pos = items->GetHeadPosition(); pos;)
		{
			CString* str = dynamic_cast<CString*>(items->GetNext(pos));
			wanummer->Add(*str);
		}
		*/
		list.getSelectedItems(wanummer, 1);
	}
	CDialog::OnOK();
}

int SapfileOpen::openDialog(bool bIsNextGeneration, CString strIppeMatNr,  __int64 aufnr, int posnr, int sapfile) 
{
	this->sapfile = sapfile;
	this->aufnr = aufnr;
	this->posnr = posnr;
	this->bIsNextGeneration = bIsNextGeneration;
	this->strIppeMatNr = strIppeMatNr;
	
	
	if (DoModal() == IDOK) 
	{
		if (select == -1)
			select = 0;
		
		return select;
	}
	return -1;
}

void SapfileOpen::openDialog(CStringArray* wanummer, int sapfile) 
{
	this->sapfile = sapfile;
	this->wanummer = wanummer;
	
	if (DoModal() != IDOK) 
	{
		wanummer->RemoveAll();
	}
}

void SapfileOpen::fillStringBox2()
{
	// Selektiertes Element suchen und merken
/*
	int curselect;
	if (select == -1)
	{
		curselect = list.getSelectedItem();
		if (curselect == -1)
			curselect = 0;
	}
	else
		curselect = select;
*/
	list.SetRedraw(FALSE);
	// Löschen aller Strings
	list.DeleteAllItems();

	// Neu füllen mit Sapauftraegen
	for (int i=0; i<wanummer->GetSize(); i++) 
	{
		CString pos, auftrag;

		auftrag = wanummer->GetAt(i);	
		pos.Format("%d", i+1); 
		
		int index = list.GetItemCount();
		index = index > 0 ? index : 0;
		index = list.InsertItem(index, "1");
	
		list.SetItemText(index, POSNR,		pos);
		list.SetItemText(index, SAPAUFTRAG,	auftrag);
	}
	// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
	// sonst den letzten in der Liste
	//curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
	//list.selectItem(curselect);
	
	list.SetRedraw(TRUE);
}

/*
@mfunc Füllt die Listbox mit Strings. 
*/
void SapfileOpen::fillStringBox(int select)
{
	CStdioFile file;
	CFileException e;
	CString fn = "", str;

	CString path(WaxSys::para_str(DIR_SAPTOWAX));
	fn = path;

	CString search;
	if(bIsNextGeneration)
	{
		search = strIppeMatNr;
	}
	else
	{
		search.Format("%.10I64u%.6i", aufnr, posnr);
	}

	if (file.Open(fn, CStdioFile::modeRead | CStdioFile::shareDenyNone, &e))
	{	

		while (	file.ReadString(str) ) 
		{
			sapauftraege.Add(str);
		}
		file.Close();
		
		// Selektiertes Element suchen und merken
		int curselect;
		if (select == -1)
		{
			curselect = list.getSelectedItem();
			if (curselect == -1)
				curselect = 0;
		}
		else
			curselect = select;
		list.SetRedraw(FALSE);
		// Löschen aller Strings
		list.DeleteAllItems();

		// Neu füllen mit Sapauftraegen
		for (int i=0; i<sapauftraege.GetSize(); i++) 
		{
			CString pos, auftrag;

			auftrag = sapauftraege.GetAt(i);	
			pos.Format("%d", i+1); 
			
			int index = list.GetItemCount();
			index = index > 0 ? index : 0;
			index = list.InsertItem(index, "1");
		
			list.SetItemText(index, POSNR,		pos);
			list.SetItemText(index, SAPAUFTRAG,	auftrag);
			if(auftrag.GetLength() > POS_NEXT_GENERATION)
			{
				CString strTemp = auftrag.Mid(POS_NEXT_GENERATION+1, auftrag.GetLength()- 1 - POS_NEXT_GENERATION);
				list.SetItemText(index, SAPAUFTRAGII, strTemp);
			}
			
			if (!curselect && auftrag.Find(search) != -1)
				curselect = index;
		}
		// Wenn ein Eintrag vorhanden ist, den vorher selektierten anwählen, 
		// sonst den letzten in der Liste
		curselect = curselect >= list.GetItemCount() ? list.GetItemCount()-1 : curselect;
		list.selectItem(curselect);
		list.EnsureVisible(curselect, true);
		list.SetRedraw(TRUE);
	}
	else 
	{
		CString txt, str; str = getStammdaten().getText("IDS_ERROR4b").c_str();
		txt.Format(str, fn);
		AfxMessageBox(txt, MB_OK | MB_ICONINFORMATION);
	}
}



const CStringArray& SapfileOpen::get_sapauftraege() const
{
	return sapauftraege;
}


int SapfileOpen::findEintragDependIppeMatNummer(CString strIppeMatNummer, CString &zeile)
{
	if (sapauftraege.GetSize() == 0)
		return false;

	int iLength = strIppeMatNummer.GetLength();
	for(int i = iLength; i < 18; i++)
	{
		strIppeMatNummer = CString("0") + strIppeMatNummer;
	}


	CString strLocalIppeMatNummer, strLocalIppeKnotenNr;

	for (int i = 0; i < sapauftraege.GetSize(); i++)
	{	
		getText(sapauftraege.GetAt(i), POS_IPPE_KNOTEN_NR - POS_IPPE_MATNR, POS_IPPE_MATNR, strLocalIppeMatNummer, true);
		getText(sapauftraege.GetAt(i), POS_ENDE - POS_IPPE_KNOTEN_NR, POS_IPPE_KNOTEN_NR, strLocalIppeKnotenNr, true);
	
		if (strLocalIppeMatNummer == strIppeMatNummer)
		{
			zeile = sapauftraege.GetAt(i);
			return i;
		}
	}
	
	return -1;
}



bool SapfileOpen::getText(const CString &text, int offset, int pos, CString &result, bool trim)
{
	result = text.Mid(pos, offset);

	if (trim)
	{
		result.TrimLeft();
		result.TrimRight();
	}

	return true;
}



int SapfileOpen::checkIppeNr()
{
	CString zeile;
	
	int bFindIppe = -1;
	if(strIppeMatNr.IsEmpty())
	{
		return bFindIppe;
	}

	bFindIppe = findEintragDependIppeMatNummer(strIppeMatNr, zeile);

	list.selectItem(bFindIppe);
	if (bFindIppe > -1)
	{
		return  bFindIppe;
	}
	return -1;
}



int SapfileOpen::checkSDNr()
{
	if (sapauftraege.GetSize() > 0)
	{
		for (int i = 0; i < sapauftraege.GetSize(); i++)
		{
			CString str = sapauftraege.GetAt(i);

			CString txt = " ";
			// SD-Auftragsnr.
			int j;
			for (j=0; j<LEN_SD_AUFTRNR; j++) {
				txt.SetAt(j, str.GetAt(j+LEN_AUFTRAGSNR+LEN_PROJEKTNR+LEN_PSP_ELEMENT));
				txt += '\0';
			}
			__int64 value1 = _atoi64(txt);
			
			txt = " ";
			// SD-Positionsnr.
			for (j=0; j<LEN_SD_POSINR; j++) {
				txt.SetAt(j, str.GetAt(j+LEN_AUFTRAGSNR+LEN_PROJEKTNR+LEN_PSP_ELEMENT+LEN_SD_AUFTRNR));
				txt += '\0';
			}
			int value2 = atoi(txt);

			if (value2 == posnr && value1 == aufnr)
			{
				list.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);
				return  i;
			}
		}
	}
	return -1;
}

//Indicates the user clicked a button
void SapfileOpen::OnClickedSearch()
{
	SapSearch ss(this);
	if (ss.DoModal() == IDOK || (aufnr != 0 && posnr != 0))
	{
		bIsNextGeneration = ss.GetIsChoosenIppe();
		if(!bIsNextGeneration)
		{
			aufnr = ss.Get_SDAuftragsNr();		
			posnr = ss.Get_SDPosNr();
		
			select = checkSDNr();
			bIgnoreSelection = true;
			if(select == -1)
			{

				if(AfxMessageBox("Es wurde kein Eintrag gefunden\n Suche wiederholen ??", MB_YESNO) == IDNO)
				{
					PostMessage(WM_CLOSE);
				}
			}
			else
			{
				PostMessage(WM_COMMAND, IDOK, 0);
			}
		}
		else
		{
			strIppeMatNr	= ss.GetIppeMatNr();

			select = checkIppeNr();
			bIgnoreSelection = true;
			if(select == -1)
			{
				if(AfxMessageBox("Es wurde kein Eintrag gefunden\n Suche wiederholen ??", MB_YESNO) == IDNO)
				{
					PostMessage(WM_CLOSE);
				}
			}
			else
			{
				PostMessage(WM_COMMAND, IDOK, 0);
			}
			
		}
	}
}
