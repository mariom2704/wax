#ifndef __PASSWD__
#define __PASSWD__



/**
Generischer Passwortdialog
*/
class MXPARAMDLL Passwd : public CDialog  
{
DECLARE_MESSAGE_MAP()
public:
	Passwd(CWnd* parent, string passwd_from_db);

	//Destruktor
	virtual ~Passwd();

	/// Vergleicht passwd mit dbpasswd
	bool check_passwd() const;

protected:
	BOOL OnInitDialog();

	void OnOK();

	void DoDataExchange(CDataExchange* pDX);

private:

	

	/// Eingegebenes Passwort
	CString passwd;

	/// Eingabefeld für Passwort
	CEdit ed_passwd;
	
	/// Passwort aus der Datenbank
	string dbpasswd;
};

#endif
