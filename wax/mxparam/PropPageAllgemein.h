#ifndef __PROPPAGEGEALLGEMEIN_H__
#define __PROPPAGEGEALLGEMEIN_H__

#include "waxproppage.h"

/// PropPage Dialogfeld
class PropPageAllgemein : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstructor
	PropPageAllgemein(MxParameter *para);

	/// Destructor
	~PropPageAllgemein();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();

	/// Passwort
	void onPassword();
	
	/// Default-Werte setzen
	virtual void set_default();

private:
	enum SPRACHE 
	{  
		DEUTSCH = 0,	
		ENGLISCH = 1,
	};	


	/// Programmsprache
	int	paraSprache;
	
	/// Text der eingestellten Sprache
	CString sprache;

	/**
	Ein kleiner Dialog zur Eingabe eines neuen Passwortes
	*/
	class PasswdAendern : public CDialog
	{
	public:
		PasswdAendern(CWnd* pParent = NULL);   ///  Standardkonstruktor

		CString	newPassword;
		CString	password;
		
	protected:
		virtual void DoDataExchange(CDataExchange* pDX); 
		
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		DECLARE_MESSAGE_MAP()
	};
};

#endif

