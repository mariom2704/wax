#pragma once

#include "EditFormstueckeUndFittinge.h"

/**
Eingabe der Materialliste Formstücke und Fittinge
Dialog ID ist IDD_FORMSTUECKEUNDFITTINGE
*/
class WAXUIDLL FormstueckeUndFittingeEingabe : public WaxPartForm  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(FormstueckeUndFittingeEingabe)
private:
	///  Fensterelemente wurden initialisiert, das passiert erst, wenn der Dialog auch angezeigt wird.
	///  Dadurch wird die Performance bei der Projektanzeige maximiert
	bool istInitialisiert;

	/// Initialisierung einer TV_INSERTSTRUCT Struktur zum Einfügen in 
	/// einen Treecontrol.
	TV_INSERTSTRUCT makeInsertStruct(HTREEITEM lastitem, UINT image, const CString &titel, BOOL hasChildren, void *code) const;

	/// Index der einzelnen Felder in der Listbox.

	enum COLUMNS_IN_LIST 
	{
	
		ANZAHL = 0,

		BESCHREIBUNG = 1,

		ARTNR = 2,

		GEWICHT = 3,

		PREIS = 4,

		PULVERPREIS = 5,

		EK = 6
	};

	/// Preis und Gewichtsberechung
	void calcPreisUndGewicht(int index, double preis, double pulverpreis, double gewicht, int anzahl);

	/// Fügt das ausgewählte Material in die Liste ein. Wenn fromCB = true 
	/// ist, die Auswahl der Combobox benutzt
	void insertMaterial(bool fromCB = false);

	CComboBox cb;

	ListCtrl list;

	CTreeCtrl tree;

	

	bool onEditNew();

public:
	void writeMaterial(Material* mat, int index);

	Material* readMaterial(int index);

	/// Konstruktor
	FormstueckeUndFittingeEingabe();

	/// Dialoginitialisierung
	virtual BOOL OnInitDialog();

protected:
	/// get Methode für die Combobox
	CComboBox& get_cb();

	/// get Methode für die Liste
	ListCtrl& get_list();

	/// get Methode für Baum
	CTreeCtrl& get_tree();

	/// Datenaustausch
	virtual void DoDataExchange(CDataExchange* pDX);

	
	/// Zeigt an, dass im Baum doppelgeklickt wurde
	void onDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	/// Zeigt an, dass im Baum Alt+Return gedrückt wurde
	void onReturnTree(NMHDR* pNMHDR, LRESULT* pResult);
	/// Wird aufgerufen, nachdem das Fenster in der Grösse geändert wurde
	void OnSize(UINT nType, int cx, int cy);
	/// Zeigt an, dass der Benutzer die Editierung des Itemlabels beendet 
	/// hat
	void onEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	/// Zeigt an, dass der Button Delete geklickt wurde
	void onClickedDelete();
	/// Zeigt an, dass der Button Edit geklickt wurde
	void onClickedEdit();
	/// Zeigt an, dass der Button Insert geklickt wurde
	void onClickedInsert();
	/// Called when CWnd is to be hidden or shown.
	void OnShowWindow(BOOL bShow, UINT nStatus);
	/// Indicates the user clicked a button
	void onClickedNew();
	/// Zeigt an, dass der Button Stammdaten geklickt wurde
	void onClickedStammdaten();
	
	
};

