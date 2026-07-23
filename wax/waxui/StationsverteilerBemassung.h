////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2001 Minimax GmbH
// 
// StationsverteilerBemassung.h: Headerdatei
//
// Developer: Peter Riechard & Henning Kluetz
//
// Date		: 28.11.2001
//     
////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDD_STATVERT_BEMASSUNG_H__7B647081_BA35_11D4_AF39_D961547234C6__INCLUDED_)
#define AFX_IDD_STATVERT_BEMASSUNG_H__7B647081_BA35_11D4_AF39_D961547234C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StretchBitmapControl\StretchBitmapControl.h"

//Eingabe der Muffen des Stationsverteilers
class WAXUIDLL StationsverteilerBemassung : 
public WaxPartForm  
{
private:

	CStretchBitmapControl m_StretchBitmapControl;

	int extraAbgObenDN;

	int extraAbgObenID;

	int extraAbgObenLaenge;

	int extraAbgObenAbsAbst;

	DECLARE_DYNCREATE(StationsverteilerBemassung)

	int elementDN[6];

	int	elementID;
	
	int	elementLaenge;

	int elementRelAbst[6];

	int	elementlEnde;
	
	int	elementrEnde;

	int extraAbgID;
	
	int	extraAbgAbsAbst;

	int	extraAbgDN;
	
	int	extraAbgLaenge;
	
	int	rohrDN;
	
	int	rohrLaenge;
	
	int rohrDruckstufe;

	
	
	//Rohrdatenobjekt, siehe <c Rohrdaten>
	StationsverteilerData *rohr;

	void init();



public:
	//Konstruktor
	StationsverteilerBemassung();   
	
	//Destruktor
	virtual ~StationsverteilerBemassung();
	
	//set rohrRestlaenge
	void set_rohrRestlaenge();

	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) 
	//aufgerufen wird.
	virtual bool check_plausis();

	//Dialog initialisieren
	virtual BOOL OnInitDialog();
	
protected:
	//Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
	//Plausicheck true liefert.
	virtual void commitUpdate();


	//Enabled und disabled die Dialogelemente in Abhängigkeit der Eingaben
	void setCtrlStatus();
	
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(StationsverteilerBemassung)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(StationsverteilerBemassung)
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_IDD_STATVERT_BEMASSUNG_H__7B647081_BA35_11D4_AF39_D961547234C6__INCLUDED_
