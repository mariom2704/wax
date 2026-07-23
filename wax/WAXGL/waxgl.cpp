#include "stdafx.h"


#include <afxdllx.h>
#include "waxgl.h"
#include "..\wax\resource.h"
#include <locale.h>




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE WaxglDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WAXGL.DLL Initializing!");
		TRACE(" hInstance = 0X%x\n", hInstance);
		
		// One-Time-Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(WaxglDLL, hInstance))
			return 0;

		new CDynLinkLibrary(WaxglDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("WAXGL.DLL Terminating!\n");
		// Bibliothek vor dem Aufruf der Destruktoren schlieﬂen
		AfxTermExtensionModule(WaxglDLL);
	}
	return 1;   // OK
}


//Konstruktor
WaxGl::WaxGl() :  bottom(0), top(0), right(0), left(0)
{
	setlocale( LC_ALL, LANGUAGE_DEUTSCH);
}


//Destruktor
WaxGl::~WaxGl()
{}
