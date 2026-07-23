#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_GEWINDEROHREINGABE_3C15C4C403E5_INCLUDED
#define _INC_GEWINDEROHREINGABE_3C15C4C403E5_INCLUDED

//Haupteingabemaske für Gewinderohre
class WAXUIDLL GewinderohrEingabe : public WaxPartForm  
{
//public
public:
	//Konstruktor
	GewinderohrEingabe();

	//Destruktor
	virtual ~GewinderohrEingabe();


	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	virtual BOOL OnInitDialog();

	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	void OnSize(UINT nType, int cx, int cy);
	void OnNew();
	void OnChange();
	void OnCopy();
	void OnDelete();
	void OnCut();
	void OnPaste();
	
	//Indicates that the user has clicked the right mouse button in the control
	 void OnRclickGrohr_list(NMHDR* pNMHDR, LRESULT* pResult);
	//Indicates that the user has clicked the left mouse button in the control
	 void OnClickGrohr_list(NMHDR* pNMHDR, LRESULT* pResult);
	//Indicates that a key has been pressed
	 void OnKeydownGrohr_list(NMHDR* pNMHDR, LRESULT* pResult);
	//Indicates that the control has the input focus and the user has pressed the 
	//enter key
	 void onGrohr_tree(NMHDR* pNMHDR, LRESULT* pResult);
	//Indicates the user clicked a button
	 void onClickedGrohr_zm();
	//Indicates a new combo box list item is selected
	 void onSelchangeGrohr_dn2();
	//Indicates a new combo box list item is selected
	 void onSelchangeGrohr_dn3();
	//Indicates the edit control is losing the input focus
	 void OnKillfocusGrohr_laenge();
	//Indicates that the selection has changed from one item to another
	 void onSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//Indicates a new combo box list item is selected
	 void onSelchangeGrohr_dn();
	 void onZusaetzlicherFitting();
	//Indicates the display is updated after text changes
	 void onChangeGrohr_zml_ende();
	
	DECLARE_MESSAGE_MAP()

private:
	/// Prüfung ausführen und Statustext setzen
	const CString doPlausiCheck();

	int dnRohr;

	BOOL m_bErzeugt;
	
	//Setzt  zmassLinks des Gewinderohrs und das Editfeld für die ZMass-Eingabe. 
	//Wenn das erste Rohrstück angewählt ist, wird auch hier die ZMass-Angabe 
	//gesetzt.
	void setZMassRohranfang(int zmassLinks);

	//Prüft, ob die DN Angaben des ausgewählten Elementes mit den DN Angaben des 
	//Fittingobjektes zusammenpassen.
	bool isEleDNok(const FormstueckeUndFittinge::data * fitting, const int dn) const;

	// Prüft, ob Element passendes Gewinde für gruppenid hat
	bool passendesGewinde(const int rel_abstand, const Element *ele, int gruppenid) const;

	//Nimmt alle Eingabefelder aus der Tab-Ordnung heraus, die nur beim ersten 
	//Rohrstück eingegeben werden.
	void updateTabOrder(bool remove);

	//Fügt ein neues Rohrstück ein
	bool doInsert(Element* ele, int index);

	//Setzt die Eingabefelder
	void elem_setFields();
	
	//Füllt die Liste mit Elementen
	void fillElementList(int select = -1, bool redraw_graphic = false);
	
	//Zeigt das Kontexmenü an
	void createPopupMenu(CPoint point);

	//Gibt die DN des ausgewählten Elements zurück, bzw. der Rohrdn.
	int getReferenceDN() const;

	//Setzt die Felder mit dem rechten und linken ZMass des ausgewählten Elementes
	void setZMassLinksRechts(Element* ele);

	//Definitionen der Tabs mit Fittingen und zus. Fittingen
	enum FITTINGTABS 
	{
		//Momentan ausgewählter Tab
		CUR_TAB = -1,
		//Erster Tab: Fittinge
		FITTINGS = 0,
		//Zweiter Tab: zusätzliche Fittinge
		ZUS_FITTINGS = 1,
	
		MAX_TAB = 2
	};

	void setLaengeNull();

	//Wrapper zur Auswahl eines Tabs. Fenster werden manuell umgeblendet
	void selectTab(int tabid = CUR_TAB, bool setfocus = false);

	//Wrappermethode zur Initialisierung des Baumes
	void insertItems();

	// Fuegt die Elementliste in den Materialbaum ein
	void insertElemente();

	// Fuegt die F&F-Liste in den Materialbaum ein
	void insertFittinge(bool ensureVisible = false);

	//Der Baum zeigt das Material aus den Stammdaten an
	CTreeCtrl tree_zus;

	//TabControl für die Fittinglisten
	CTabCtrl tab;

	//Selektiert den richtigen Fitting, je nach Auswahl in der Liste
	void selectFitting(Element *xele = NULL);

	//zu bearbeitendes Gewinderohr
	Gewinderohr* data;

	//Liefert das in der Liste selektierte Element aus dem Elementarray oder NULL
	Element* getSelectedElement(bool verkettung = true);

	//Checkbox für Kopfstueck-Flag
	CButton kopfstueck;

	enum ATTRIBUTES 
	{
		ANZAHL = 0,
		NENNWEITE = 1,
		LAENGE = 2,
		TEXT = 3,
		WINKEL_ = 4,
		VERKETTUNG = 5,
		ZML = 6,
		ZMR = 7
	};

	//Der Baum zeigt das Material aus den Stammdaten an
	CTreeCtrl tree;

	//Die Liste zeigt das eingefügte Material an
	ListCtrl list;

	void setAnlage(Element* elem, int index);

	DECLARE_DYNCREATE(GewinderohrEingabe)

};

#endif /* _INC_GEWINDEROHREINGABE_3C15C4C403E5_INCLUDED */
