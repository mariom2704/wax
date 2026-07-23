// TxtReplaceDlg.h : Header-Datei
//

#if !defined(AFX_TXTREPLACEDLG_H__28536FF7_2A55_11D5_9803_0050DA35C3DB__INCLUDED_)
#define AFX_TXTREPLACEDLG_H__28536FF7_2A55_11D5_9803_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReplacementStatistics.h"

/////////////////////////////////////////////////////////////////////////////
// CTxtReplaceDlg Dialogfeld

class CTxtReplaceDlg : public CDialog
{
private:
	//@cmember Ersetzungen an einer Zeile durchführen
	CString applyOnLine(const CString str);

	//@cmember Suchstring
	CString	m_toFind;
	//@cmember Ersetzungsstring
	CString	m_replaceWith;
	//@cmember Ausgabe des Status
	CString	m_status;
	//@cmember Ganze Zeile löschen in der der Suchtext 
	//gefunden wurde
	BOOL	m_bDeleteLine;
	//@cmember Nur ganzes Wort ersetzen, gilt nicht für 
	//Zeilenlöschen
	BOOL	m_bganzesWort;
	//@cmember Sourcetextpfad
	static CString	m_path;
	static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData);
	void applyOnFile(CString fname);
	void udpateRegistry(bool bSave);
	void doReplacement(CFileFind& fFind);
	//@cmember Handle auf Programm-Icon
	HICON m_hIcon;
	//@cmember Statistikobjekt
	ReplacementStatistics m_statistics;

// Konstruktion
public:
	//@cmember Konstruktor
	CTxtReplaceDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

	
// Dialogfelddaten
	//{{AFX_DATA(CTxtReplaceDlg)
	enum { IDD = IDD_TXTREPLACE_DIALOG };
	BOOL	m_createBackup;
	//}}AFX_DATA

	//@cmember Sourcetextpfad

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CTxtReplaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CTxtReplaceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBrowse();
	afx_msg void OnCheckDeleteline();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_TXTREPLACEDLG_H__28536FF7_2A55_11D5_9803_0050DA35C3DB__INCLUDED_)
