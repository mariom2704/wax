// updateAssist.h : Haupt-Header-Datei für die Anwendung UPDATEASSIST
//

#if !defined(AFX_UPDATEASSIST_H__D6D3FEFD_93C6_4235_9DCA_28202D62E3CC__INCLUDED_)
#define AFX_UPDATEASSIST_H__D6D3FEFD_93C6_4235_9DCA_28202D62E3CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CUpdateAssistApp:
// Siehe updateAssist.cpp für die Implementierung dieser Klasse
//

class CUpdateAssistApp : public CWinApp
{
public:
	CUpdateAssistApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CUpdateAssistApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CUpdateAssistApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_UPDATEASSIST_H__D6D3FEFD_93C6_4235_9DCA_28202D62E3CC__INCLUDED_)
