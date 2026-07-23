#if !defined(AFX_PRJGLIEDERUNGFORM_H__F5638843_B6DB_11D4_9783_0050DA35C3DB__INCLUDED_)
#define AFX_PRJGLIEDERUNGFORM_H__F5638843_B6DB_11D4_9783_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//Bearbeitungsklasse
class PrjGliederungForm : 
public WaxPartForm  
{
private:
	FlexMaskedEdit editPSP;

public:
	//Standardkonstruktor
	PrjGliederungForm(   );   

	DECLARE_DYNCREATE(PrjGliederungForm)

	//{{AFX_DATA(PrjGliederungForm)
	enum { IDD = IDD_GLIEDERUNG };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(PrjGliederungForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(PrjGliederungForm)
	//Speichert die Bezeichnung des Werkauftrages als neuen Standard ab
	 void OnSaveAsStandard();
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PRJGLIEDERUNGFORM_H__F5638843_B6DB_11D4_9783_0050DA35C3DB__INCLUDED_
