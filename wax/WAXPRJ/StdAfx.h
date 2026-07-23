// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
#pragma once


#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#define WIN32_LEAN_AND_MEAN
#define AFX_NO_OLE_SUPPORT
#define AFX_NO_DAO_SUPPORT
//#define AFX_NO_AFXCMN_SUPPORT
#define AFX_NO_DB_SUPPORT

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen


#ifndef AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE-Klassen
#include <afxodlgs.h>       // MFC OLE-Dialogfeldklassen
#include <afxdisp.h>        // MFC Automatisierungsklassen
#endif // _AFX_NO_OLE_SUPPORT

#include <afxhtml.h>

#ifndef AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC-Datenbankklassen
#endif // _AFX_NO_DB_SUPPORT

#ifndef AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO-Datenbankklassen
#endif // _AFX_NO_DAO_SUPPORT


#ifndef AFX_NO_AFXCMN_SUPPORT
#include <afxdtctl.h>		
#include <afxcmn.h>			
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcoll.h>
#include <afxtempl.h>


#include <mxcontrols\binaryarchive.h>
#include <mxcontrols\medit.h>

#include <mxutil\xassert.h>
#include <mxodbc\mxodbc.h>


#include "..\WaxDef.h"

#include <string>
#include <boost/boost.h> 
#include <boost/boost_filesystem.h>


using namespace MxFilesystem;
using namespace std;
using namespace boost;
using namespace boost::iostreams;
using namespace boost::algorithm;



#include "waxprj_dll.h"