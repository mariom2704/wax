// Copyright (C) 2001 Minimax GmbH
// 
// StationsverteilerEingabe.h: Headerdatei
//
// Developer: Peter Riechard & Henning Kluetz
//
// Date		: 28.11.2001
//     
////////////////////////////////////////////////////////////////////////////////
/*	
@doc StationsverteilerEingabeH SonderMZdll
@module StationsverteilerEingabe.h - Definition der Eingabemaske 
für einen Stationsverteiler |
In diesem Modul wird die von WaxPartForm abgeleitete Klasse 
StationsverteilerEingabe definiert
@developer Peter Riechard & Henning Kluetz
@head3 Elemente |
@index | StationsverteilerEingabeH
*/

#include "..\rohrkonst\Stationsverteiler.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_STATIONSVERTEILEREINGABE_3C1DEF120002_INCLUDED
#define _INC_STATIONSVERTEILEREINGABE_3C1DEF120002_INCLUDED

//Klasse zur Eingabe von StationsverteilerEingabe
//Diese Klasse implementiert eine Eingabemaske für den Stationsverteiler
class DLLINTERFACE StationsverteilerEingabe : 
public WaxPartForm  
{
private:
	DECLARE_DYNCREATE(StationsverteilerEingabe)

	int rohrDruckstufe;

	int rohrKFarbe;

	Stationsverteiler *getStationsverteiler();

public:
	//Konstruktor
	StationsverteilerEingabe();

	//Destruktor
	virtual ~StationsverteilerEingabe();

	virtual bool check_plausis();


	//{{AFX_VIRTUAL(StationsverteilerEingabe)
	//Dialog initialisieren
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	//{{AFX_MSG(StationsverteilerEingabe)
	
	DECLARE_MESSAGE_MAP()
public:
	 void OnCbnSelchangeComboVertAbg();
};

#endif /* _INC_STATIONSVERTEILEREINGABE_3C1DEF120002_INCLUDED */
