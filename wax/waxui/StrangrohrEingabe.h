////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2001 Minimax GmbH
// 
// GewinderohrEingabe.h: Headerdatei
//
// Developer: Peter Riechard & Henning Kluetz
//
// Date		: 28.11.2001
//     
////////////////////////////////////////////////////////////////////////////////
/*	
Datei: StrangrohrEingabe.h in Waxuidll
Module: StrangrohrEingabe.h - Definition der Eingabemaske für ein Srangrohr
In diesem Modul wird die von WaxPartForm abgeleitete Klasse StrangrohrEingabe definiert
*/

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_STRANGROHREINGABE_3ACD89C000EF_INCLUDED
#define _INC_STRANGROHREINGABE_3ACD89C000EF_INCLUDED

///Klasse zur Eingabe der Rohrdaten
class WAXUIDLL StrangrohrEingabe : 
public WaxPartForm  
{
///private
private:
	CString fertigungshinweis;

	int rohrAnfang;

	int rohrEnde;

	int rohrQuali;

	int rohrKFarbe;

	int rohrDN;

	int rohrLaenge;

	bool trockenanlage;
	bool m_Ueberlaenge;
	int m_iAuswaertsfertigung;
	

	///Rohrdatenobjekt, siehe <c Rohrdaten>
	//StrangrohrData* rohr;

	Strangrohr* getStrangrohr();


///public
public:
	///Konstruktor
	StrangrohrEingabe();
	
	///Destruktor
	virtual ~StrangrohrEingabe();

	///Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird
	virtual bool check_plausis();

	///Indicates the edit control is losing the input focus
	virtual void commitUpdate();

	DECLARE_DYNCREATE(StrangrohrEingabe)

///protected
protected:
	//{{AFX_VIRTUAL(StrangrohrEingabe)
	///Datenaustausch	
	virtual void DoDataExchange(CDataExchange* pDX);

	///Dialog initialisierung
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(StrangrohrEingabe)
	///Indicates a new combo box list item is selected
	void OnSelchangeStrangrohr_dn();
	afx_msg void OnBnClickedCheckUeberlaenge();
	
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOnEis();
};

#endif /* _INC_STRANGROHREINGABE_3ACD89C000EF_INCLUDED */
