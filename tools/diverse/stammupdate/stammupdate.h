#pragma once


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole


class StammUpdateApp : public CWinApp
{
public:
	StammUpdateApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

