#pragma once
#include "resource.h"
#include "afxdtctl.h"

// DlgDate-Dialogfeld

class DlgDate : public CDialog
{
	DECLARE_DYNAMIC(DlgDate)

public:
	DlgDate(COleDateTime startdate, CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~DlgDate();

// Dialogfelddaten
	enum { IDD = IDD_DIALOG_DATE };

	COleDateTime get_date();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	COleDateTime date;
public:
protected:
	CMonthCalCtrl calendarCtl;
public:
	afx_msg void OnBnClickedOk();
};
