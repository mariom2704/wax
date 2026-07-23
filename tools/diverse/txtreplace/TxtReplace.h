// TxtReplace.h : Haupt-Header-Datei für die Anwendung TXTREPLACE
//

#if !defined(AFX_TXTREPLACE_H__28536FF5_2A55_11D5_9803_0050DA35C3DB__INCLUDED_)
#define AFX_TXTREPLACE_H__28536FF5_2A55_11D5_9803_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CTxtReplaceApp:
// Siehe TxtReplace.cpp für die Implementierung dieser Klasse
//

class CTxtReplaceApp : public CWinApp
{
public:
	CTxtReplaceApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CTxtReplaceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CTxtReplaceApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_TXTREPLACE_H__28536FF5_2A55_11D5_9803_0050DA35C3DB__INCLUDED_)
