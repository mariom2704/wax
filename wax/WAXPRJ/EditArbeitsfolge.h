#pragma once

#include "WaxPartForm.h"
#include <mxcontrols/floatedit.h>

//Arbeitsfolge editieren
//Dieser Dialog editiert die Attribute einer Arbeitsfolge.
//Die Ids der Dialogelemente sind über Variablen festgelegt, da diese Klasse mit 
//einer anderen Maske in Sondermz abgeleitet wurde.
class WAXPRJDLL EditArbeitsfolge : 
public WaxPartForm  
  
{
public:
	//Konstruktor
	EditArbeitsfolge(CWnd* parent, Arbeitsfolge* af);


	//{{AFX_VIRTUAL(EditArbeitsfolge)
	public:
	virtual void OnOK();


	//Daten aus Eingabefeldern lesen
	virtual void DoDataExchange(CDataExchange* pDX);


	//Dialog initialisieren
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	afx_msg void OnBnClickedAendern();

private:
	int sap;

	int isTextgeaendert;

	CString bez;

	//Prüft die Eingabe der Menge und der Zeit auf Änderung. 
	//Führt eine Neuberechnung der Zeit durch, falls Menge geändert wurde.
	void checkPreisUndAnzahl();

	FloatEditCtrl preisctrl;

	FloatEditCtrl anzahlctrl;

	FloatEditCtrl zeitctrl;

	FloatEditCtrl ruestzeitctrl;

	FloatEditCtrl einzelzeitctrl;

	FloatEditCtrl preis1ctrl;

	int posnr;

	double zeit;

	CString txt;

	CString  arbpl;

	double tarif;

	double anzahl;

	int code;

	double ruestzeit;

	double einzelzeit;

	double preis;


protected:
	UINT sapID;

	UINT isTextgeaendertID;

	int ruestzeitID;

	int einzelzeitID;

	UINT posnrID;
	UINT txtID;
	UINT arbplID;
	UINT preisID;
	UINT anzahlID;
	UINT codeID;
	UINT zeitID;
	UINT bezID;
	UINT preis1ID;

	//Zeiger auf die zu bearbeitende Arbeitsfolge
	Arbeitsfolge* paf;

	
	//{{AFX_MSG(EditArbeitsfolge)
	//Indicates a new combo box list item is selected
	 void onSelchangeArbpl();
	 void onKillfocusEinzelzeit();

	 virtual void loadTexte();
	
	DECLARE_MESSAGE_MAP()
};
