#pragma once

class Material;

#include "WaxPartForm.h"
#include <mxcontrols/listctrl.h>

//Allgemeine Materialeingabe
class WAXPRJDLL MaterialEingabeForm : 
public WaxPartForm  
{
public:
	//Konstruktor
	MaterialEingabeForm();

	//Destruktor
	virtual ~MaterialEingabeForm();

private:
	void edit(bool showprofilreiter);

	//Legt die Spalten und die Reihenfolge in der Liste fest
	enum COLUMNS_IN_LIST 
	{
		ANZAHL = 0,
		POSNR = 1,
		LAENGE = 2,
		BESCHREIBUNG = 3,
		//Linkes und Rechtes Ende vom Profil
		PROFILE = 4,
		ARTNR = 5,
		GEWICHT = 6,
		GESAMTGEWICHT = 7,
		PREIS = 8,
		GESAMTPREIS = 9,
		PULVERPREIS = 10,
		GESAMTPULVERPREIS = 11,
		EINKAUF = 12,
		HALBZEUG = 13,
		ID = 14
	};

	DECLARE_DYNCREATE(MaterialEingabeForm)

	//{{AFX_VIRTUAL(MaterialEingabeForm)
	public:
	//Dialoginitialisierung
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(MaterialEingabeForm)
	//Zeigt an, dass der Button Delete geklickt wurde
	 void onClickedDelete();
	//Zeigt an, dass der Button Edit geklickt wurde
	 void onClickedEdit();
	//Zeigt an, dass der Button Insert geklickt wurde
	 void onClickedInsert();
	//Zeigt an, dass im Baum doppelgeklickt wurde
	 void onDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	//Zeigt an, dass der Benutzer die Editierung des Itemlabels beendet 
	//hat
	 void onEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	//Zeigt an, dass im Baum Alt+Return gedrückt wurde
	 void onReturnTree(NMHDR* pNMHDR, LRESULT* pResult);
	//Wird aufgerufen, nachdem das Fenster in der Grösse geändert wurde
	 void OnSize(UINT nType, int cx, int cy);
	//Called when CWnd is to be hidden or shown.
	 void OnShowWindow(BOOL bShow, UINT nStatus);
	//Indicates the user clicked a button
	 void onClickedEditlabel();

	 void onClickedNew();
	
	DECLARE_MESSAGE_MAP()

	//Datenaustausch
	virtual void DoDataExchange(CDataExchange* pDX);

	//Initialisierung der Materialansicht (Spalten und Bezeichnungen)
	virtual void initList();

private:
	//Der Baum zeigt das Material aus den Stammdaten an
	CTreeCtrl tree;

	//Die Liste zeigt das eingefügte Material an
	ListCtrl list;

	//Preis und Gewichtsberechung
	void calcPreisUndGewicht(int index, double preis, double pulverpreis, double gewicht, int anzahl, int laenge);

	//Fügt das ausgewählte Material in die Liste ein.
	void insertMaterial();

	bool onEditNew();

	//Fensterelemente wurden initialisiert
	bool istInitialisiert;

	//Liest die Profile aus den Stammdaten und fügt sie in den Baum ein
	void insertProfile(bool co2tikko = false, HTREEITEM root = NULL);

	///Liest das Sondermaterial den Stammdaten und fügt sie in den Baum ein
	void insertSondermaterial(bool co2tikko = false, HTREEITEM root = NULL);

	//Fügt die Tabelle Formstücke und Fittinge in den Materialbaum ein
	void insertFStuFitt(bool ensureVisible = false);

	//Fügt die Elementliste in den Materialbaum ein
	HTREEITEM insertElemente(bool co2tikko = false);

	//Liest die Angaben an der Stelle index aus und erzeugt daraus ein 
	//Materialobjekt
	Material* readMaterial(int index);

	//Schreibt die Daten eines Materials an die Stelle index in der Liste.
	void writeMaterial(int index, Material * mat);

	//Schreibt die Daten eines Materials an die Stelle index in der Liste.
	Material *  conv2Material(Profile::data  * dat  ) const;

	//Schreibt die Daten eines Materials an die Stelle index in der Liste.
	Material* conv2Material(Bauelemente::data* dat, const CString &kurztext) const;

	//Schreibt die Daten eines Materials an die Stelle index in der Liste.
	Material * conv2Material(FormstueckeUndFittinge::data* dat) const;

	//Schreibt die Daten eines Materials an die Stelle index in der Liste.
	Material * conv2Material(Sondermaterial::data* dat) const;
};
