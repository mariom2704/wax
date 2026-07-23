#if !defined(AFX_EINSTELLUNGEN_H__CE1A44D3_22E9_11D3_987C_00600819298A__INCLUDED_)
#define AFX_EINSTELLUNGEN_H__CE1A44D3_22E9_11D3_987C_00600819298A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Einstellungen.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld Einstellungen 

class Einstellungen : public CDialog
{
// Konstruktion
public:
	Einstellungen(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(Einstellungen)
	enum { IDD = IDD_DIALOG_VERZEICHNIS };
	CString	m_Verzeichnis;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(Einstellungen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(Einstellungen)
	virtual void OnOK();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_EINSTELLUNGEN_H__CE1A44D3_22E9_11D3_987C_00600819298A__INCLUDED_
