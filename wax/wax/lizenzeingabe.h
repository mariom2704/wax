#pragma once

#include <lizenz/key.h>
#include <Lizenz/Decoder.h>
#include <Lizenz/Validator.h>
#include <Lizenz/DateUtil.h>

/**
Generischer Dialog zur Lizenzeingabe
*/
class Lizenzeingabe : public CDialog
{
	DECLARE_MESSAGE_MAP()
public:
	Lizenzeingabe(CWnd* pParent, const CWinApp *app);   
	virtual ~Lizenzeingabe();

	/// Liefert das mValid Flag
	bool is_valid() const;
	bool create_key_from_keyid();

	

protected:
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	void on_readLicenceFile();
	void readfile();
	
private:
	CWnd* m_pParent;
	CString WaxRegKey;
	bool bRegkeyOk;
	License::Validator::LicenseState StateLic;
	
	
	/// Entfernt den Lizenzschlüssel aus der Wax.ini
	void remove_license();


	/// Wird im Feld IDC_ERROR ausgegeben, wenn der Key ungültig ist
	//std::string error;
	const CWinApp *mApp;
	
	/// Laden des Lizenzkeys
	const CString load_key() const;



	License::Key key;
	License::Validator::LicenseState LicenseState;
	const string Lizenzeingabe::get_hwkennung() const ;
   
	void SetState( License::Validator::LicenseState lState)
	{
		LicenseState = lState;
	}
   
	vector<CString> FileContent; 

	void setWindowsText(int id, const string& txtId);
	void setWindowsTextWithColon(int id, const string& txtId);

public:
	afx_msg void OnBnClickedCreateLiceneceFile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
