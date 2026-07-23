#pragma once

#include <mxcontrols/listctrl.h>
#include "waxproppage.h"

#include <string>

/**
Erweiterte Bearbeitung aller Systemparameter
*/
class PropPageSysAll : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	PropPageSysAll(MxParameter *para);

	/// Destructor
	~PropPageSysAll();

	virtual BOOL OnApply();

protected:
	///  DDX/DDV-Unterstützung
	virtual void DoDataExchange(CDataExchange* pDX);    

    virtual BOOL OnInitDialog();
	
private:
	CButton	but_bearbeiten;
	ListCtrl plist;

	/// Liste initialisieren
	void init_list();

	/// Liste löschen
	void clear_list();

	/// Default-Werte setzen
	virtual void set_default();

	/// Eintrag bearbeiten
	void on_bearbeiten();

	/// Anzeigestd::string für den Gültigkeitsbereich ermitteln
	std::string get_namespace_str(Parameter::VALID_NAMESPACE nmsp) const;

	void on_dblclick(NMHDR* pNMHDR, LRESULT* pResult);
	void on_column_clicked(NMHDR* pNMHDR, LRESULT* pResult);
	static int CALLBACK sort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
};



