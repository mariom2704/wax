#pragma once







class CImportSAPToWAX : public CDialog
{
	DECLARE_DYNAMIC(CImportSAPToWAX)

public:
	CImportSAPToWAX(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CImportSAPToWAX();
	BOOL m_bImported;

// Dialogfelddaten
	enum { IDD = IDD_DIALOG_SAP_WAX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSapDatenImportieren();
	static UINT ImportThread(LPVOID);

	CStatic m_SapToWaxImportFile, m_PlaceForStammDB;
	CString m_strSapToWaxImportFile,  m_strPlaceForStammDB;
	int m_iCountUpdates, m_iCountInsert;
	CString	m_strLogFile;
	BOOL m_bImportAbbrechen;
	CButton m_ImportAbbrechen, m_ImportStarten, m_Abort;
	CProgressCtrl m_ProgressControl;

	afx_msg void OnBnClickedButtonImportAbbrechen();
};
