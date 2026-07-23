#pragma once

#include <mxcontrols/listctrl.h>

/// Dialog zur Bearbeitung bzw. Auswahl von Wa Nummern aus der Datenbanktabelle 
/// gesperrte_wanr. Es kann aber auch manuell eine WA-Nummer eingegeben werden.
class WanrSperrung : public CDialog
{
public:
	/// Fügt val in die Datenbank ein
	bool lock();

	/// Entfernt val aus der Datenbank
	void unlock();

	/// Liefert den selektierten bzw. eingegebenen Wert
	int get_val();

	/// Liefert den eingegebenen bzw. ausgewählten Wert als String zurück.
	CString get_valstring();

	/// Entfernt die übergebene WA-Nummer aus der Liste mit Sperrungen
	void remove_val(int wanr);

	WanrSperrung(CWnd *parent, int value);

	virtual ~WanrSperrung();

private:
	ListCtrl list;

	int val;

	/// Liste mit Wa-Nummern und dem User
	map<int,string> locked_wanr;

public:
	virtual void DoDataExchange(CDataExchange* pDX);


	virtual BOOL OnInitDialog();

	virtual void OnOK();

protected:
	 void onSetwanr();
	
	DECLARE_MESSAGE_MAP()
};
