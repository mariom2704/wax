#pragma once


class WaxHtmlView;
class CChildFrame;
class CJScriptViewTemplate;


class DialogMehrfachdruckDlg : public CDialog  
{
	DECLARE_MESSAGE_MAP()
private:

	bool m_bDialogWrapped;
	CProgressCtrl m_InternProgressControl;
	bool m_Vk3;
	
	
	
	void onPrintAll();
	int HowManyToPrint;

	CListCtrl*	listControl;
	CWaxBrowser* waxBrowser;

public:

	void onPrint(int iWhichButton = -1);

	void SetHowManyToPrint(int HowManyToPrintLocal)
	{
		HowManyToPrint = HowManyToPrintLocal;
	};
	void SetVk3(bool Vk3);
	/// Listenindex
	enum LISTEN 
	{
		VKABRECHNUNG = 0,

		ZEUGNISSTELLE = 1,

		SCHWEISSEREI = 2,

		ROBBI1 = 3,

		ROBBI2 = 4,

		GEWINDEROHR = 5,

		PULVERANLAGE = 6,

		GRUNDIERER = 7,

		BUENDLER1 = 8,

		BUENDLER2 = 9,

		VERSAND1 = 10,

		BELIEBIGERTEXT =11,

		AWFERTIGUNG =12,

		/// Zähler für die Listen. Muss immer der höchste Wert sein
		LISTCOUNT =13
	};


	class reginit 
	{
	private:
	/// Initialisiert die Datenelemente
		void initdata(int liste, UINT id, UINT anzahlid, const CString & text);

	public:

		reginit(bool vk3 = false);

		virtual ~reginit();

	
		int anzahl[LISTCOUNT];
		UINT dlgid[LISTCOUNT];
		CString regkey[LISTCOUNT];
		bool selected[LISTCOUNT];

	public:
		CString text;

		const CString get_regkey(int index) const;

		

		int dlgid_anzahl[LISTCOUNT];

		CString strFirma;

		void dodataexchange(CWnd* wnd, CDataExchange* pDX);
	};


	DialogMehrfachdruckDlg():m_bDialogWrapped(true){};


	void SetListControl(CListCtrl* listControlLocal, CWaxBrowser* waxBrowserLocal)
	{
		listControl = listControlLocal;
		waxBrowser  = waxBrowserLocal;
	};


	virtual ~DialogMehrfachdruckDlg();

	virtual void DoDataExchange(CDataExchange* pDX);


protected:

	
	
	
	
private:


	/// reginit Struktur für die Dialogdaten
	reginit data;

	

	/// Druckt eine einzige bestimmte Ausgabe.
	void printList(int counter);



	void onClickedOpen_Cloose();
	/// Druck aller angekreuzten Listen per Batch
	 void onClickedPrint();
	/// Indicates the user clicked a button
	 void onClickedPrint_vkabrechnung();
	/// Indicates the user clicked a button
	 void onClickedPrint_zeugnisstelle();
	/// Indicates the user clicked a button
	 void onClickedPrint_schweisserei();
	/// Indicates the user clicked a button
	 void onClickedPrint_gewinderohr();
	/// Indicates the user clicked a button
	 void onClickedPrint_pulveranlage();
	/// Indicates the user clicked a button
	 void onClickedPrint_grundierer();
	/// Indicates the user clicked a button
	 void onClickedPrint_buendler1();
	/// Indicates the user clicked a button
	 void onClickedPrint_buendler2();
	/// Indicates the user clicked a button
	 void onClickedPrint_versand1();
	/// Indicates the user clicked a button
	 void onClickedPrint_awfertigung();
	/// Indicates the user clicked a button
	 void onClickedPrint_robbi1();
	/// Indicates the user clicked a button
	 void onClickedPrint_robbi2();
	/// Indicates the user clicked a button
	 void onClickedPrint_beliebig();

	 void SetSpecificPrint(LISTEN iWichPrint);
	

	 
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	virtual void OnOK();
};

