// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_BESCHICHTUNGEINGABEFORM_3C1DFFF80017_INCLUDED
#define _INC_BESCHICHTUNGEINGABEFORM_3C1DFFF80017_INCLUDED

#include "WaxPartForm.h"
#include "BeschichtungEingabe.h"

/**
Klasse zur Eingabe der Beschichtung
Diese Klasse implementiert eine Eingabemaske für die Beschichtungseingabe und 
benutzt als Hilfsklasse BeschichtungEingabe
*/
class WAXPRJDLL BeschichtungEingabeForm : public WaxPartForm  
{
	DECLARE_DYNCREATE(BeschichtungEingabeForm)
	DECLARE_MESSAGE_MAP()
public:
	BeschichtungEingabeForm();

	virtual ~BeschichtungEingabeForm();

	enum KONSTANTE 
	{
		//Maximale Anzahl von Zeilen im Bemerkungsfeld
		MAXLINE_BEMERKUNG = 13
	};

	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

private:
	CEdit edBemerkung;

	CComboBox verzinkerei;

	//Für diesen Auftrag muss ein Zeugnis erstellt werden
	bool zeugnis;

	// Es musse eine Zulassungsstelle gewaehlt werden
	int m_KollilisteExport;

	//Bemerkungsfeld
	CString bemerkung;

	//Rohr wird im Sichtbereich angebracht, nur gültig für Rohrleitungen
	bool sichtbereich;

	int rohrKFarbe;

	//Beschichtungscontrol
	BeschichtungEingabe beschichtung;
	
	void on_autosoko();

	
protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);

	virtual void OnOK();

	void onKFarbe();
	
public:
	afx_msg void OnBnClickedSichtbereich();
};

#endif /* _INC_BESCHICHTUNGEINGABEFORM_3C1DFFF80017_INCLUDED */
