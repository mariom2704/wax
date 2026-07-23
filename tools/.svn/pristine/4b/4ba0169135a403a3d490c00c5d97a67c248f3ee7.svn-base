// DriveIdDlg.h : Header-Datei
//

#if !defined(AFX_DRIVEIDDLG_H__B473DA9A_2290_11D5_97FA_0050DA35C3DB__INCLUDED_)
#define AFX_DRIVEIDDLG_H__B473DA9A_2290_11D5_97FA_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDriveIdDlg Dialogfeld

class CDriveIdDlg : public CDialog
{
// Konstruktion
public:
	CDriveIdDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(CDriveIdDlg)
	enum { IDD = IDD_DRIVEID_DIALOG };
	CString	m_driveID;
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CDriveIdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(CDriveIdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeLaufwerke();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void initCB();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_DRIVEIDDLG_H__B473DA9A_2290_11D5_97FA_0050DA35C3DB__INCLUDED_)
