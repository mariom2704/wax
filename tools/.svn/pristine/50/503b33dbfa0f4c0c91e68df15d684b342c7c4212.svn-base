// FormatResourceDotHDlg.h : Header-Datei
//

#if !defined(AFX_FORMATRESOURCEDOTHDLG_H__EE572667_4F70_11D5_9840_0050DA35C3DB__INCLUDED_)
#define AFX_FORMATRESOURCEDOTHDLG_H__EE572667_4F70_11D5_9840_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFormatResourceDotHDlg Dialogfeld

class CFormatResourceDotHDlg : public CDialog
{
// Konstruktion
public:
	CFormatResourceDotHDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CFormatResourceDotHDlg)
	enum { IDD = IDD_FORMATRESOURCEDOTH_DIALOG };
	int		m_iCommand;
	int		m_iControls;
	int		m_iStrings;
	CString	m_File;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CFormatResourceDotHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	int m_iAPSOffset;
	int m_lineCounter;
	void writeAPSEntries(void);
	void writeFile();
	void processString(CString str);

	// @cmember Liste mit Kopfzeilen
	CStringList m_strLstHead;

	// @cmember Liste mit Fusszeilen
	CStringList m_strLstFoot;

	// @cmember Liste mit nicht einordbaren Strings
	CStringList m_strLst;

	// @cmember Liste mit ID_ Strings
	CStringList m_strLstID;

	// @cmember Liste mit IDR_ Strings
	CStringList m_strLstIDR;

	// @cmember Liste mit IDC_ Strings
	CStringList m_strLstIDC;

	// @cmember Liste mit IDI_ Strings
	CStringList m_strLstIDI;

	// @cmember Liste mit IDS_ Strings
	CStringList m_strLstIDS;

	// @cmember Liste mit IDD_ Strings
	CStringList m_strLstIDD;

	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CFormatResourceDotHDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFile();
	afx_msg void OnButtonEnumerate();
	afx_msg void OnChangeEditStringtable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_FORMATRESOURCEDOTHDLG_H__EE572667_4F70_11D5_9840_0050DA35C3DB__INCLUDED_)
