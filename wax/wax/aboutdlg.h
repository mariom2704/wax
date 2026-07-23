#pragma once

/**
Dieser Dialog zeigt die Versionsinfo an und Kontaktadressen/Telefonnummern vom Support
*/
class AboutDlg : public CDialog
{
public:
	AboutDlg();

	CString	version;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
};




