////////////////////////////////////////////////////////////////////////
// ExcelTable.cpp : Legt die Initialisierungsroutinen für die DLL fest.
//
//
// Company	 : Minimax GmbH
// Address	 : Industriestrasse 10-12 
// 			   23843 Bad Oldesloe
//	           Germany
// Telephone : +49 4531 803 0		
// Internet  : www.minimax.de
//
// Author    : Henning Kluetz		
// E-Mail    : kluetzh@minimax.de	
//
// Date	     : May.29.2001 	
//

#include "stdafx.h"
#include "ExcelTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Beachten Sie!
//
//		Wird diese DLL dynamisch an die MFC-DLLs gebunden,
//		muss bei allen von dieser DLL exportierten Funktionen,
//		die MFC-Aufrufe durchführen, das Makro AFX_MANAGE_STATE
//		direkt am Beginn der Funktion eingefügt sein.
//
//		Beispiel:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//		// Hier normaler Funktionsrumpf
//		}
//
//		Es ist sehr wichtig, dass dieses Makro in jeder Funktion
//		vor allen MFC-Aufrufen erscheint. Dies bedeutet, dass es
//		als erste Anweisung innerhalb der Funktion ausgeführt werden
//		muss, sogar vor jeglichen Deklarationen von Objektvariablen,
//		da ihre Konstruktoren Aufrufe in die MFC-DLL generieren
//		könnten.
//
//		Siehe MFC Technical Notes 33 und 58 für weitere
//		Details.
//

/////////////////////////////////////////////////////////////////////////////
// CExcelTableApp

BEGIN_MESSAGE_MAP(CExcelTableApp, CWinApp)
	//{{AFX_MSG_MAP(CExcelTableApp)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelTableApp Konstruktion

CExcelTableApp::CExcelTableApp()
{

}

CExcelTableApp theApp;
