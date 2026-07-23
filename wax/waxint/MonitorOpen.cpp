#include "stdafx.h"


#include "resource.h"
#include "GpAnlageSaegeNut.h"
#include "DlgDate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int POSNR	  = 0;
const int AUFTRAG = 1;			

const int TIMER_ID = 1000;
const int timeout = 30000;

BEGIN_MESSAGE_MAP(MonitorOpen, CDialog)
	ON_BN_CLICKED(IDC_STREET_DELETE, onClickedStreet_delete)
	ON_BN_CLICKED(IDC_STREET_DELETE2, onClickedStreet_delete2)
	ON_BN_CLICKED(IDC_STREET_ARCHIVE, onClickedStreet_archive)
	ON_BN_CLICKED(IDC_BUTTON_DATE, onClickedChangeDate)
	//ON_NOTIFY(NM_DBLCLK, IDC_WA_FREE, OnClickedStreet_delete)
	//ON_NOTIFY(NM_DBLCLK, IDC_WA_FINISHED, OnClickedStreet_archive)
	ON_WM_TIMER()
END_MESSAGE_MAP()

MonitorOpen::~MonitorOpen()
{
}

MonitorOpen::MonitorOpen(int anl) 
: CDialog(IDD_OPEN_MONITOR), listActive(0,0,0), listFinished(0,0,0), listFree(0,0,0), listStopped(0,0,0)
{
	anlage = anl;
	//Strings muessen in der gleichen Folge wie die Anlagennr. sein
	CString mon;
	windowText.Add(" ");
	mon = getStammdaten().getText("IDS_MONITOR2").c_str();
	windowText.Add(mon);
	mon = getStammdaten().getText("IDS_MONITOR3").c_str(); //VERTEILER_1
	windowText.Add(mon);
	mon = getStammdaten().getText("IDS_MONITOR4").c_str(); //STRANGROHR_1
	windowText.Add(mon);
	mon = getStammdaten().getText("IDS_MONITOR5").c_str();
	windowText.Add(mon);
	mon = getStammdaten().getText("IDS_MONITOR6").c_str();
	windowText.Add(mon);
	mon = getStammdaten().getText("IDS_MONITOR7").c_str();
	windowText.Add(mon);

	
	if (anlage == VERTEILER_1)
	{
		pathFree	 = WaxSys::para_str(DIR_VERTEILER_FREE);
		pathArchive  = WaxSys::para_str(DIR_VERTEILER_ARCHIVE);
		pathFinished = WaxSys::para_str(DIR_VERTEILER1_FINISHED);
		pfade.Add(WaxSys::para_str(DIR_VERTEILER_FREE));
		pfade.Add(WaxSys::para_str(DIR_VERTEILER1_ACTIVE));
		pfade.Add(WaxSys::para_str(DIR_VERTEILER1_STOPPED));
		pfade.Add(WaxSys::para_str(DIR_VERTEILER1_FINISHED));
	}
	else if (anlage == STRANGROHR_1)
	{
		pathFree	 = WaxSys::para_str(DIR_STRANG_FREE);
		pathArchive  = WaxSys::para_str(DIR_STRANG_ARCHIVE);
		pathFinished = WaxSys::para_str(DIR_STRANG1_FINISHED);
		pfade.Add(WaxSys::para_str(DIR_STRANG_FREE));
		pfade.Add(WaxSys::para_str(DIR_STRANG1_ACTIVE));
		pfade.Add(WaxSys::para_str(DIR_STRANG1_STOPPED));
		pfade.Add(WaxSys::para_str(DIR_STRANG1_FINISHED));
	}
	else if (anlage == STRANGROHR_2)
	{
		pathFree	 = WaxSys::para_str(DIR_STRANG_FREE);
		pathArchive  = WaxSys::para_str(DIR_STRANG_ARCHIVE);
		pathFinished = WaxSys::para_str(DIR_STRANG2_FINISHED);
		pfade.Add(WaxSys::para_str(DIR_STRANG_FREE));
		pfade.Add(WaxSys::para_str(DIR_STRANG2_ACTIVE));
		pfade.Add(WaxSys::para_str(DIR_STRANG2_STOPPED));
		pfade.Add(WaxSys::para_str(DIR_STRANG2_FINISHED));
	}
	else if (anlage == STRANGROHR_3)
	{
		pathFree	 = WaxSys::para_str(DIR_STRANG_FREE);
		pathArchive  = WaxSys::para_str(DIR_STRANG_ARCHIVE);
		pathFinished = WaxSys::para_str(DIR_STRANG3_FINISHED);
		pfade.Add(WaxSys::para_str(DIR_STRANG_FREE));
		pfade.Add(WaxSys::para_str(DIR_STRANG3_ACTIVE));
		pfade.Add(WaxSys::para_str(DIR_STRANG3_STOPPED));
		pfade.Add(WaxSys::para_str(DIR_STRANG3_FINISHED));
	}
	else if (anlage == STRANGROHR_4)
	{
		pathFree	 = WaxSys::para_str(DIR_STRANG_FREE);
		pathArchive  = WaxSys::para_str(DIR_STRANG_ARCHIVE);
		pathFinished = WaxSys::para_str(DIR_STRANG4_FINISHED);
		pfade.Add(WaxSys::para_str(DIR_STRANG_FREE));
		pfade.Add(WaxSys::para_str(DIR_STRANG4_ACTIVE));
		pfade.Add(WaxSys::para_str(DIR_STRANG4_STOPPED));
		pfade.Add(WaxSys::para_str(DIR_STRANG4_FINISHED));
	}
}

CString MonitorOpen::getName(int id, bool hotkey)
{
	CString str; str = getStammdaten().getText("IDS_UNBEKANNTE_AKTION").c_str();

	if (id < windowText.GetSize())
	{
		CString s;
		if (hotkey)
			s = "&";
		else 
			s = "";
				
		str.Format(windowText.GetAt(id), s);
	}
	return str;
}

BOOL MonitorOpen::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

BOOL MonitorOpen::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;
	
	SetWindowText(getName(anlage, false).GetBuffer(10));

	initList(&listFree); 
	initList(&listActive); 
	initList(&listStopped); 
	initList(&listFinished); 

	if (anlage == SAEGENUTANLAGE)
	{
		GetDlgItem(IDC_STREET_ARCHIVE)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_DATE)->EnableWindow(false);
	}

	UpdateData();

	SetTimer(TIMER_ID, timeout, NULL);
	
	return true;
}

void MonitorOpen::initList(ListCtrl* list)
{
	CString str;
	str = getStammdaten().getText("IDS_AUFTRAEGE").c_str();
	
	if (anlage == SAEGENUTANLAGE)
	{
		list->InsertColumn(0, str, LVCFMT_LEFT,  70);

		str = getStammdaten().getText("IDS_ORT").c_str();
		list->InsertColumn(1, str, LVCFMT_LEFT,  70);

		str = getStammdaten().getText("IDS_DATUM").c_str();
		list->InsertColumn(2, str, LVCFMT_LEFT,  65);

		str = getStammdaten().getText("IDS_FERT_DATE").c_str();
		list->InsertColumn(3, str, LVCFMT_LEFT,  65);

		str = getStammdaten().getText("IDS_SDAUFTRAG").c_str();
		list->InsertColumn(4, str, LVCFMT_LEFT,  70);
	}
	else
	{
		list->InsertColumn(0, str, LVCFMT_LEFT,  140);
	}

}

void MonitorOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WA_FREE,	  listFree);
	DDX_Control(pDX, IDC_WA_ACTIVE,	  listActive);
	DDX_Control(pDX, IDC_WA_STOPPED,  listStopped);
	DDX_Control(pDX, IDC_WA_FINISHED, listFinished);

	searchForFiles();
}

void MonitorOpen::OnOK()
{
	KillTimer(TIMER_ID);
	CDialog::OnOK();
}

void MonitorOpen::searchForFiles()
{

	if (anlage == SAEGENUTANLAGE)
	{
		//Werte aus Db lesen
		readSaegeNutWerte();
		return;
	}
	
	//Exceldateien einlesen

	CStdioFile file;
	CFileException e;
	CString fn = "";
		
	for (int i=0; i<4; i++)
	{
		fn = pfade.GetAt(i);
		fn += "*.*"; 
		
		WIN32_FIND_DATA data;
		HANDLE handle = 0;
		handle = FindFirstFile(fn, &data);
		CStringArray array;
		DWORD skip_flags = FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_TEMPORARY;
		if (handle != INVALID_HANDLE_VALUE) 
		{
			if (!(data.dwFileAttributes & skip_flags))
				array.Add(data.cFileName);	

			while (FindNextFile(handle, &data)) 
			{
				if (!(data.dwFileAttributes & skip_flags))
					array.Add(data.cFileName);	
			}
		} 
		else
		{
			//if (GetLastError() != 2)
			//CString err = "Kann Pfad nicht öffnen!";
			
			//((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);
		}
		FindClose(handle);

		if (i==0)		waFree.Copy(array);
		else if (i==1)	waActive.Copy(array);
		else if (i==2)	waStopped.Copy(array);
		else if (i==3)	waFinished.Copy(array);
	}


	// Neu fuellen mit Auftraegen
	fillList(&waActive, &listActive);
	fillList(&waFinished, &listFinished);
	fillList(&waFree, &listFree);
	fillList(&waStopped, &listStopped);
}

/**
Werte der Sägenutanlage aus der Db einlesen
*/
void MonitorOpen::readSaegeNutWerte()
{
	GpAnlageSaegeNut gpSaegenut(NULL);

	MXDate date;

	//Freigegebene Aufträge
	list<DbAnlage::kopfsatz> data;
	gpSaegenut.readAuftraege(GpAnlage::FreigabeSegmentRohr, data, date);
	fillList(data, &listFree);
	
	//Aufträge in Arbeit
	data.clear();
	gpSaegenut.readAuftraege(GpAnlage::InBearbeitungSaegeNut, data, date);
	fillList(data, &listActive);

	//unterbrochene und stornierte Aufträge
	data.clear();
	gpSaegenut.readAuftraege(GpAnlage::UnterbrochenSaegeNut, data, date);
	fillList(data, &listStopped);
	gpSaegenut.readAuftraege(GpAnlage::StorniertSaegeNut, data, date);
	fillList(data, &listStopped);


	//fertige Aufträge 
	COleDateTimeSpan span(31);
	date = MXDate::get_current_date();
	date = date - span;
	data.clear();
	gpSaegenut.readAuftraege(GpAnlage::FertigSaegeNut, data, date);
	fillList(data, &listFinished);

}

void MonitorOpen::fillList(list<DbAnlage::kopfsatz> &data, ListCtrl* ctlList)
{
	ctlList->SetRedraw(false);
	ctlList->DeleteAllItems();

	MxString str;
	CString test;

	list<DbAnlage::kopfsatz>::iterator iter = data.begin();
	int count = 0;
	while (iter != data.end())
	{
		str = MxString("{0}").arg((double)iter->wanr,0);
		int index = ctlList->InsertItem(count, str.getString());
		
		ctlList->SetItemText(index,1,iter->ort.getString());

		str = MxString(iter->date.Format("%d.%m.%y"));
		ctlList->SetItemText(index,2,str.getString());

		str = MxString(iter->fertDate.Format("%d.%m.%y"));
		ctlList->SetItemText(index,3,str.getString());
		
		str = MxString("{0}").arg((double)iter->sdAuftrag,0);
		ctlList->SetItemText(index,4,str.getString());

		count++;
		iter++;
	}

	ctlList->SetRedraw(true);
}


void MonitorOpen::fillList(CStringArray* array, ListCtrl* list)
{
	list->SetRedraw(FALSE);
	list->DeleteAllItems();

	for (int i=0; i < array->GetSize(); i++) 
	{
		CString pos, auftrag;

		auftrag = array->GetAt(i);	
		pos.Format("%d", i+1); 
		//int index = list->InsertItem(list->GetItemCount(), pos);
		//list->SetItemText(index, AUFTRAG, auftrag);
		list->InsertItem(list->GetItemCount(), auftrag);
	}
	list->SetRedraw(TRUE);
}

/*
//Indicates the user clicked a button
void MonitorOpen::OnClickedStreet_delete(NMHDR * pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	onClickedStreet_delete();
}
*/

void MonitorOpen::onClickedStreet_delete()
{
	CStringArray items;
	listFree.getSelectedItems(&items, 0);
	if (items.GetSize() == 0)
	{
		AfxMessageBox(getStammdaten().getText("IDS_HINWEIS3b").c_str(), MB_OK | MB_ICONINFORMATION);
		return;
	}

	deleteAuftraege(items, pathFree);
}

/**
gestoppte Aufträge löschen
*/
void MonitorOpen::onClickedStreet_delete2()
{
	CStringArray items;
	listStopped.getSelectedItems(&items, 0);
	if (items.GetSize() == 0)
	{
		AfxMessageBox(getStammdaten().getText("IDS_HINWEIS3b").c_str(), MB_OK | MB_ICONINFORMATION);
		return;
	}

	deleteAuftraege(items, "");
}

void MonitorOpen::deleteAuftraege(const CStringArray &items,const CString &path)
{
	KillTimer(TIMER_ID);

	if (anlage == SAEGENUTANLAGE)
	{
		//wanr ermitteln
		for (int i=0; i < items.GetSize(); i++)
		{
			CString wert = items[i];

			int wanr = 0;
			if (!wert.IsEmpty())
				wanr = atoi(wert);

			//Eintrag in DB löschen
			deleteSaegeNutAuftrag(wanr);
		}

	}
	else if (!path.IsEmpty())
	{
		//Dateien löschen
		for (int i=0; i < items.GetSize(); i++)
		{
			CString fn = path + items[i];
		
			if (!DeleteFile(fn))
				ERRORMSG1(fn);
		}
	}
	
	searchForFiles();

	SetTimer(TIMER_ID, timeout, NULL);
}


void MonitorOpen::deleteSaegeNutAuftrag(int wanr)
{
	GpAnlageSaegeNut gpSaegenut(NULL);	
	gpSaegenut.cleanTables(wanr);
}

/*
//Indicates the user clicked a button
void MonitorOpen::OnClickedStreet_archive(NMHDR * pNMHDR, LRESULT *pResult)
{	
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = 1;

	onClickedStreet_archive();
}
*/


void MonitorOpen::onClickedStreet_archive()
{
	int select;
	if ((select = listFinished.getSelectedItem()) < 0)
	{
		AfxMessageBox(getStammdaten().getText("IDS_ERROR13b").c_str(), MB_OK | MB_ICONINFORMATION);
		return;
	}

	KillTimer(TIMER_ID);
	CString fnar = pathArchive  + waFinished.GetAt(select);
	CString fnfi = pathFinished + waFinished.GetAt(select);

	if (!MoveFile(fnfi, fnar))
		ERRORMSG1(fnfi);

	searchForFiles();
	SetTimer(TIMER_ID, timeout, NULL);

	//nächsten Eintrag markieren
	if (listFinished.GetItemCount() > select)
		listFinished.selectItem(select);
	else if (listFinished.GetItemCount() > select - 1)
		listFinished.selectItem(select - 1);

	listFinished.SetFocus();
}

//Called after each interval specified in SetTimer.
void MonitorOpen::OnTimer(UINT_PTR nIDEvent)
{
	UNREFERENCED_PARAMETER(nIDEvent);

	searchForFiles();
}


void MonitorOpen::onClickedChangeDate()
{
	CStringArray items;
	listFree.getSelectedItems(&items, 0);
	if (items.GetSize() == 0)
	{
		AfxMessageBox(getStammdaten().getText("IDS_ERROR13b").c_str(), MB_OK | MB_ICONINFORMATION);
		return;
	}


	//Wanr holen		
	int wanr = atoi(items[0]);

	//Spalte Datum holen
	CStringArray dates;
	listFree.getSelectedItems(&dates, 3);
	if (dates.GetSize() == 0)
		return;

	//Datum abfragen (erstes Datum als vorgabe nehmen)
	int day = atoi(dates[0].Mid(0,2));
	int month = atoi(dates[0].Mid(3,2));
	int year = atoi(dates[0].Mid(6));
	year += 2000;
	COleDateTime fertDate(year, month, day, 0,0,0);

	DlgDate dlg(fertDate);
	if (dlg.DoModal() != IDOK)
		return;

	fertDate = dlg.get_date();

	MXDate mxDate;
	mxDate.SetDateTime(fertDate.GetYear(), fertDate.GetMonth(), fertDate.GetDay(),0, 0, 0);

	DbAnlage dbAnlage(NULL);
	for (int i=0; i < items.GetSize(); i++)
	{
		wanr = atoi(items[i]);

		//Datum neu in Db schreiben	
		if (!dbAnlage.updateFertDatum(wanr, mxDate))
		{
			AfxMessageBox(getStammdaten().getText("IDS_ERROR13b").c_str(), MB_OK | MB_ICONINFORMATION);
			return;
		}
	}

	//Listen aktualisieren
	searchForFiles();
}