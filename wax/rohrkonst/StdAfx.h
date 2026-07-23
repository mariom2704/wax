#pragma once

#define VC_EXTRALEAN		// Selten benutzte Teile der Windows-Header nicht einbinden

#include <afx.h>
#include <afxwin.h>         // Kern- und Standardkomponenten von MFC
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für allgemeine Steuerelemente von Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <mxutil\xassert.h>
#include <mxutil\ddx_extension.h>
#include <mxutil\strloader.h>

#include "..\WaxDef.h"

#define ROHRKONST_DLL "rohrkonst.dll"

using namespace std;
#include <mxcontrols\binaryarchive.h>
using namespace MxFilesystem;

#include <boost/boost.h>
using namespace boost;
using namespace boost::algorithm;


#include "rohrkonst_dll.h"
#include "..\waxprj\waxprj_dll.h"