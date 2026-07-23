/// PropPageEmail.cpp : Cpp-Datei
/// Eigenschaftenseite PropPageEmail

#include "stdafx.h"
#include "mxparam_dll.h"
#include "resource.h"
#include "PropPageEmail.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

USE_PARAID

BEGIN_MESSAGE_MAP(PropPageEmail, WaxPropPage)
	ON_EN_CHANGE(IDC_PARA_EMPFAENGER,	onParaUpdate)
	ON_EN_CHANGE(IDC_PARA_DISPLAY,		onParaUpdate)
	ON_EN_CHANGE(IDC_PARA_BETREFF,		onParaUpdate)
END_MESSAGE_MAP()

/// Konstructor
PropPageEmail::PropPageEmail(MxParameter *para) : WaxPropPage(IDD_PARA_EMAIL, para)
{
	ADD_PARA_STR(EMPFAENGER);
	ADD_PARA_STR(DISPLAYNAME);
	ADD_PARA_STR(BETREFF);
}

/// Destructor
PropPageEmail::~PropPageEmail()
{
}




