// DB_Zugriff.h : Hauptheaderdatei für die DB_Zugriff-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CDB_ZugriffApp:
// Siehe DB_Zugriff.cpp für die Implementierung dieser Klasse
//

class CDB_ZugriffApp : public CWinApp
{
public:
	CDB_ZugriffApp();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CDB_ZugriffApp theApp;
