// stdafx.h : Include-Datei für Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die häufig benutzt, aber
//      in unregelmäßigen Abständen geändert werden.
//

#if !defined(AFX_STDAFX_H__C77186B0_7CE3_11D4_9753_0050DA35C3DB__INCLUDED_)
#define AFX_STDAFX_H__C77186B0_7CE3_11D4_9753_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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


#ifndef AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC-Datenbankklassen
#endif // _AFX_NO_DB_SUPPORT

#ifndef AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO-Datenbankklassen
#endif // _AFX_NO_DAO_SUPPORT


#ifndef AFX_NO_AFXCMN_SUPPORT
#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <mxodbc\mxodbc.h>
#include <mxutil/xassert.h>
#include "..\WaxDef.h"
#include "resource.h"

#include "waxstamm_dll.h"

using namespace std;

#endif
