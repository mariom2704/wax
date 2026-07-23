// TestWaxGlServer.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// TestWaxGlServer:
// Siehe TestWaxGlServer.cpp für die Implementierung dieser Klasse
//

class TestWaxGlServer : public CWinApp
{
public:
	TestWaxGlServer();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern TestWaxGlServer theApp;