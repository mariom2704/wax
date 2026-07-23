#if !defined _AUFTRAGSTEST_H__
#define _AUFTRAGSTEST_H__

#pragma once

/**
Dialog zur Anzeige von nicht bearbeiteten Aufträgen
*/
class Auftragstest : public CDialog
{
	DECLARE_MESSAGE_MAP()
public:
	/// Standardkonstruktor
	Auftragstest(CWnd* pParent = NULL);   

	virtual ~Auftragstest();
protected:
	/// DDX/DDV-Unterstützung
	virtual void DoDataExchange(CDataExchange* pDX);    

	/// Dialoginitialisierung
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
private:
	enum LISTCOLS
	{
		NL = 0,
		CLASSID = 1,
		BAUABSCHNITT = 2,
		ANZAHL = 3,
		SDNR = 4,
		SDMATNR = 5,
	};
	/// Aufträge werden in einem Baum angezeigt
	ListCtrl list;

	/// Zugriff auf das Treecontrol
	ListCtrl& get_list();

	/// Treecontrol initialisieren
	void init_list();

	/// Aufträge einlesen
	void load_waids(vector<int>& waids);

	/// Werkauftrag in die Liste einfügen
	void add_wa(Werkauftrag *wa);

	/// Liste löschen
	void clear_list();

	/// Anzeige updaten
	void on_aktualisieren();

	void on_column_sort(NMHDR* pNMHDR, LRESULT* pResult);

	static int CALLBACK sort_func(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};


#endif 
