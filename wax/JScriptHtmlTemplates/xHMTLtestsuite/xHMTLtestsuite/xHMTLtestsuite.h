
// xHMTLtestsuite.h: Hauptheaderdatei für die xHMTLtestsuite-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole


// CxHMTLtestsuiteApp:
// Siehe xHMTLtestsuite.cpp für die Implementierung dieser Klasse
//

class CxHMTLtestsuiteApp : public CWinAppEx
{
public:
	CxHMTLtestsuiteApp();


// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CxHMTLtestsuiteApp theApp;
