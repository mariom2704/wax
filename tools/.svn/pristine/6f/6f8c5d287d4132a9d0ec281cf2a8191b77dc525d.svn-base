#pragma once

#include "LicenseGenerator.h"

/**
Generischer Dialog zur Lizenzeingabe
*/
class Lizenzeingabe : public CDialog
{
public:
	Lizenzeingabe(CWnd* pParent, CWinApp *app);   // Standardkonstruktor
	virtual ~Lizenzeingabe();

	bool is_valid() const;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	void on_checkkey();
	
private:
	string mHwkennung;
	string mKey;
	bool mValid;
	
	CWinApp *mApp;

	void validate();
	const string load_key() const;
	const string get_hwkennung() const;
	void set_valid(bool val);

	LicenseGenerator lic;

	DECLARE_MESSAGE_MAP()
};
