/// PropPageUser.h : Header-Datei
/// Eigenschaftenseite PropPageUser

#ifndef __PROPPAGEGEUSER_H__
#define __PROPPAGEGEUSER_H__

#include "waxproppage.h"

/**
Benutzerverwaltung
Wurde das Passwort eingegeben, können alle Benutzereinträge bearbeitet werden.
*/
class PropPageUser : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	PropPageUser(MxParameter *para);

	/// Destructor
	~PropPageUser();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung

	virtual BOOL OnInitDialog();
	
	/// Default-Werte setzen
	virtual void set_default();
	

private:
	void on_loeschen();
	void on_selchange();
	
	CComboBox userlst;
	CString login;
	CString benutzername;
	CString durchwahl;
	CString email;
};

#endif ///  __PROPPAGEGEUSER_H__

