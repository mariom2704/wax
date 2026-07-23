#pragma once

#include "resource.h"		// Hauptsymbole


class CMkStammdbApp : public CWinApp
{
public:
	CMkStammdbApp();

	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};
