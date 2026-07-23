#if !defined(AFX_PreisanfrageForm_H__F5638843_B6DB_11D4_9783_0050DA35C3DB__INCLUDED_)
#define AFX_PreisanfrageForm_H__F5638843_B6DB_11D4_9783_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//Bearbeitungsklasse
class PreisanfrageForm : 
public WaxPartForm  
{
private:
	FlexMaskedEdit editPSP;

public:
	//Standardkonstruktor
	PreisanfrageForm(   );   

	DECLARE_DYNCREATE(PreisanfrageForm)

	//{{AFX_DATA(PreisanfrageForm)
	enum { IDD = IDD_PREISANFRAGE };
		// HINWEIS: Der Klassen-Assistent fügt hier Datenelemente ein
	//}}AFX_DATA

	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(PreisanfrageForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	
	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(PreisanfrageForm)
	//Speichert die Bezeichnung des Werkauftrages als neuen Standard ab
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDtnDatetimechangeDatetimepickerGueltigbis(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_PreisanfrageForm_H__F5638843_B6DB_11D4_9783_0050DA35C3DB__INCLUDED_
