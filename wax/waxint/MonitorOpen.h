#pragma once

#include <mxcontrols/listctrl.h>
#include "DbAnlage.h"

class WAXINT MonitorOpen : public CDialog  
{
	DECLARE_MESSAGE_MAP()
public:
	MonitorOpen(int anl);

	virtual ~MonitorOpen();

	enum ANLAGEN 
	{
		SAEGENUTANLAGE = 1,
		VERTEILER_1 = 2,
		STRANGROHR_1 = 3,
		STRANGROHR_2 = 4,
		STRANGROHR_3 = 5,
		STRANGROHR_4 = 6,
	};

	ListCtrl listActive;

	ListCtrl listFree;

	ListCtrl listStopped;

	ListCtrl listFinished;
	
	CString getName(int id, bool hotkey);

private:
	CStringArray waFree;

	CStringArray waFinished;

	CStringArray waActive;

	CStringArray waStopped;

	int anlage;

	CStringArray windowText;

	CString	pathFree;
	
	CString	pathArchive;
	
	CString pathFinished;

	CStringArray pfade;

	void initList(ListCtrl* list);

	void searchForFiles();

	void readSaegeNutWerte();

	void fillList(CStringArray* array, ListCtrl* list);
	void fillList(list<DbAnlage::kopfsatz> &data, ListCtrl* ctlList);

	void deleteAuftraege(const CStringArray &items,const CString &path);

public:
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	virtual BOOL DestroyWindow();

	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	//Indicates the user clicked a button
	//void OnClickedStreet_delete(NMHDR * pNMHDR, LRESULT *pResult);

	void onClickedStreet_delete();
	void onClickedStreet_delete2();
	void onClickedChangeDate();
	void deleteSaegeNutAuftrag(int wanr);
	
	//Indicates the user clicked a button
	//void OnClickedStreet_archive(NMHDR * pNMHDR, LRESULT *pResult);

	void onClickedStreet_archive();
	
	//Called after each interval specified in SetTimer.
	void OnTimer(UINT_PTR IDEvent);
	
};
