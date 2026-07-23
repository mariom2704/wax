#pragma once

class GrohrAuswahl : public CDialog  
{
	DECLARE_MESSAGE_MAP()
private:
	CObList * walist;

	ListCtrl list;

	bool isInit;

	enum ATTRIBUTES 
	{
		CHECK = 0,

		POSNR = 1,

		LFDNR = 2,

		ANZAHL = 3,

		LAENGE = 4,

		NENNWEITE = 5,

		TEXT = 6,

		LST_WINKEL = 7,

		VERKETTUNG = 8
	};
/*
	enum COLUMNS_IN_LIST 
	{
		POSNR = 0,
		LFDNR = 1,
		ANZAHL = 2,
		DN = 3,
		LAENGE = 4,
		KFARBE = 5,
		LINKSGEW = 6,
		RECHTSGEW = 7,
		BESCH = 8,
		LINKS = 9,
		RECHTS = 10
	};
*/
	void init_list();

	void fill_list(int select = -1);

	bool setCheck(CString check, CString posnr, int lfdnr);

public:

	GrohrAuswahl(CObList* walist);

	virtual ~GrohrAuswahl();

	public:
	virtual void OnCancel();


	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();

protected:
	//Indicates the user finished editing a item label
	 void onEndlabeleditGrohrlist(NMHDR* pNMHDR, LRESULT* pResult);
	//Indicates that the user has double-clicked the left mouse button in the 
	//control
	 void onDblclkGrohrlist(NMHDR* pNMHDR, LRESULT* pResult);
	
	

};

