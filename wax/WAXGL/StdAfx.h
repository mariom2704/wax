#pragma once


#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#define WIN32_LEAN_AND_MEAN
//#define AFX_NO_OLE_SUPPORT
#define AFX_NO_DAO_SUPPORT
#define AFX_NO_AFXCMN_SUPPORT
#define AFX_NO_DB_SUPPORT

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen

#ifndef AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE-Klassen
#include <afxodlgs.h>       // MFC OLE-Dialogfeldklassen
#include <afxdisp.h>        // MFC Automatisierungsklassen
#endif // _AFX_NO_OLE_SUPPORT


#ifndef AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC-Datenbankklassen
#endif // _AFX_NO_DB_SUPPORT

#ifndef AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO-Datenbankklassen
#endif // _AFX_NO_DAO_SUPPORT


#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente


#include <mxutil/xassert.h>




#define WAXGL_DLL "waxgl.dll"


#include <string>
#include <boost/boost.h>
#include <boost/boost_filesystem.h>
using namespace boost;
using namespace std;



#include "WaxGl_dll.h"
