#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE SVGGraficDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Entfernen Sie dies, wenn Sie lpReserved verwenden.
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("SVG Grafic.DLL wird initialisiert!\n");
		
		// Einmalige Initialisierung der Erweiterungs-DLL
		if (!AfxInitExtensionModule(SVGGraficDLL, hInstance))
			return 0;

		new CDynLinkLibrary(SVGGraficDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SVG Grafic.DLL wird abgebrochen!\n");

		// Bibliothek vor dem Aufruf der Destruktoren schlieﬂen.
		AfxTermExtensionModule(SVGGraficDLL);
	}
	return 1;   // OK
}

