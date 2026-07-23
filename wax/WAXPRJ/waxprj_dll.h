
#pragma once




#ifdef _WAXPRJDLL 
#define WAXPRJDLL __declspec( dllexport )
#include "resource.h"
#else
#define WAXPRJDLL __declspec( dllimport )
#define IDD_SAMMELAUFTRAG_LEITKARTE 0
#define IDD_GLIEDERUNG 0
#define IDD_PROJEKTLEITKARTE 0
#define IDD_WAXPARTFORM 0
#define IDD_STATUS 0
#endif




#include "..\waxstamm\waxstamm_dll.h"
#include "..\rohrkonst\rohrkonst_dll.h"
#include "..\WAXGL\WaxGl_dll.h"


// Basisdatenklassen

/////////////////////////////////////////////////////////////////////////////
#include ".\SAPXMLConnect\SoapProxyWrapper.h"


#include "WaxHtmlFile.h"
#include "WaxPart.h"
#include "WaxPartForm.h"

#include "PrjLeitkarteData.h"

#include "PrjLeitkarteForm.h"
#include "SammelAuftragForm.h"

#include "PrjGliederung.h"
#include "BauAbschnittForm.h"
#include "SammelAuftrag.h"

#include "PrjAnhangForm.h"


#include "SonderkostenEingabeForm.h"
#include "SonderkostenSammelEingabeForm.h"

#include "BeschichtungEingabeForm.h"

#include "ElementeEingabeForm.h"

#include "EditArbeitsfolge.h"
#include "EditProfile.h"

#include "MaterialEingabeForm.h"
#include "MaterialSammelEingabeForm.h"
#include "ArbeitEingabeForm.h"
#include "ArbeitSammelEingabeForm.h"

#include "Preise.h"
#include "Preisausgabe.h"

#include "Server.h"

#include "..\waxint\waxint_dll.h"

#define ERRORMSG1(x) { LPVOID lpMsgBuf; \
FormatMessage(  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, \
    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
    (LPTSTR) &lpMsgBuf, 0, NULL );\
::MessageBox( NULL,(char*)lpMsgBuf, x, MB_OK|MB_ICONINFORMATION ); \
LocalFree( lpMsgBuf ); }

