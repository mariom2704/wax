#pragma once

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automatisierungsklassen
#include <afxhtml.h>
#include <afxole.h>         // MFC OLE-Klassen
#include <afxodlgs.h>       // MFC OLE-Dialogfeldklassen
#include <afxdisp.h>        // MFC OLE-Automatisierungsklassen
#include <afxtempl.h>
#include <afxadv.h>
#include <afxpriv.h>
#include <atlbase.h>
#include <atlcom.h>
#include <dispex.h>

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC-Datenbankklassen
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO-Datenbankklassen
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC-Unterstützung für allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterstützung für gängige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <string>
using namespace std;

#include "wax.h"


#include "..\WaxDef.h"
#include <mxutil\xassert.h>
#include <mxcontrols\medit.h>
#include <mxcontrols\binaryarchive.h>
using namespace MxFilesystem;
#include <mxcontrols/msgbox.h>
using namespace MxGui;

#include "..\waxint\waxint_dll.h"
#include <afxdisp.h>
#include <afxdhtml.h>
#include <afxdisp.h>


 

