#pragma once
#include <map>
/**
Generischer Passwortdialog
*/
class EditParameter : public CDialog  
{
DECLARE_MESSAGE_MAP()
public:
	EditParameter(CWnd* parent, Parameter *para);

	/// Destruktor
	virtual ~EditParameter();

protected:
	BOOL OnInitDialog();

	void OnOK();

	void DoDataExchange(CDataExchange* pDX);

private:
	
	/// Zuordnung Radiobuttons zu Gültigkeitsbereich
	std::map<UINT, int> idc2namesp;

	/// Der zu bearbeitende Parameter
	Parameter *par;

	/// Wert aus dem Eingabefeld
	CString val;
};

