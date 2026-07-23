// WaxGlServer.h : Hauptheaderdatei für die WaxGlServer-DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CWaxGlServerApp
// Siehe WaxGlServer.cpp für die Implementierung dieser Klasse
//


class CWaxGlServerApp : public CWinApp
{
public:
	CWaxGlServerApp();
	~CWaxGlServerApp();


// Überschreibungen
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

};
