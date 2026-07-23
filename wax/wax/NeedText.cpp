/////////////////////////////////////////////////////////////////////////////
// NeedText.cpp : implementation of the CSuperTipView class
//
// Written by Jason Strayer
// of Microsoft Product Support Services, Developer Support
// Copyright (c) 1998 Microsoft Corporation. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxpriv.h"
#include "wax.h"


// From AFXIMPL.H
#ifndef _countof
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

// OnNeedText
//
// This function has been designed so that this source code can be used in
// both UNICODE and non-UNICODE projects.

BOOL OnNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult, CStatusBar *statusbar)
{
	UNREFERENCED_PARAMETER(id);

	// OnNeedText should only be called for TTN_NEEDTEXT notifications!
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText; // used to hold final tool tip text before UNICODE conversion

	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT)(WORD)::GetDlgCtrlID((HWND)nID));
	}

// Okay, this function changes its behavior based on a BOOL in the app
// object. This BOOL, NoDynamicTips, is initially set to TRUE, which causes
// this function to load tip text from the string resource table. If you
// set AfxGetApp()->NoDynamicTips to FALSE, then the second part of this
// function will be called, where you can add code to decide what text
// to display at runtime!
	CWaxApp* pApp = (CWaxApp*)AfxGetApp();
	ASSERT(pApp != NULL);

	if (pApp->NoDynamicTips)
	{
		if (nID != 0) // will be zero on a separator
		{
			AfxLoadString(nID, szFullText);
				// this is the command id, not the button index
			AfxExtractSubString(strTipText, szFullText, 1, '\n');
		}
	}
	else // Dynamic Tips!
	{	
		CString strID;
		strID.Format("ID_%d", nID);
		std::string txt = getStammdaten().getText(strID.GetString());

		//Prüfen ob für die Statuszeile ein Text hinterlegt ist
		std::string txtStatuszeile;
		int index = txt.find_first_of('\n');
		if (index >= 0)
		{
			txtStatuszeile = txt.substr(0, index);
			strTipText = txt.substr(index + 1, txt.length() - index + 1).c_str();
		}
		else
			strTipText = txt.c_str();

		if (statusbar != NULL)
		{
			statusbar->SendMessage(SB_SETTEXT,0,(LPARAM)txtStatuszeile.c_str());
		}
	}

// handle conditionally for both UNICODE and non-UNICODE apps
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;    // message was handled
}

