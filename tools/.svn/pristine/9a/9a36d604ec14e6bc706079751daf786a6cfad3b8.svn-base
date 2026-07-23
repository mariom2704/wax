#include "stdafx.h"
#include "resource.h"
#include <windows.h>
#include <string>
using namespace std;


#define MAX_LOADSTRING 100

char cmdline[255];
#define WM_EXECUTE_TEST (WM_USER+1)

// Globale Variablen:
HINSTANCE hInst;					// aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];								// Text der Titelzeile
TCHAR szWindowClass[MAX_LOADSTRING];								// Text der Titelzeile

// Vorausdeklarationen von Funktionen, die in diesem Code-Modul enthalten sind:
ATOM				MyRegisterClass( HINSTANCE hInstance );
BOOL				InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// ZU ERLEDIGEN: Fügen Sie hier den Code ein.
	MSG msg;
	HACCEL hAccelTable;

	strcpy(cmdline, lpCmdLine);

	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Initialisierung der Anwendung durchführen:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEST);

	PostMessage(hWnd, WM_COMMAND, WM_EXECUTE_TEST, 0);
	// Hauptnachrichtenschleife:
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
		if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  AUFGABE: Registriert die Fensterklasse.
//
//  KOMMENTARE:
//
//    Diese Funktion und ihre Verwendung sind nur notwendig, wenn dieser Code
//    mit Win32-Systemen vor der 'RegisterClassEx'-Funktion kompatibel sein soll,
//    die zu Windows 95 hinzugefügt wurde. Es ist wichtig diese Funktion aufzurufen,
//    damit der Anwendung kleine Symbole mit den richtigen Proportionen zugewiesen
//    werden.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEST);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNKTION: InitInstance(HANDLE, int)
//
//   AUFGABE: Speichert die Instanzzugriffsnummer und erstellt das Hauptfenster
//
//   KOMMENTARE:
//
//        In dieser Funktion wird die Instanzzugriffsnummer in einer globalen Variable
//        gespeichert und das Hauptprogrammfenster erstellt und angezeigt.
//

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   

   hInst = hInstance; // Instanzzugriffsnummer in unserer globalen Variable speichern

   hWnd = CreateWindow(szWindowClass, szTitle, WS_DLGFRAME,
      0, 0, 300, 60, NULL, NULL, hInstance, NULL);

   if( !hWnd ) 
   {
      return FALSE;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

	

   return TRUE;
}

//
//  FUNKTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  AUFGABE:  Verarbeitet Nachrichten für das Hauptfenster.
//
//  WM_COMMAND	- Anwendungsmenü verarbeiten
//  WM_PAINT	- Hauptfenster darstellen
//  WM_DESTROY	- Beendigungsnachricht ausgeben und zurückkehren
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch( message ) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Menüauswahlen analysieren:
			switch( wmId ) 
			{
				
				case IDM_EXIT:
				   DestroyWindow( hWnd );
				   break;
				case WM_EXECUTE_TEST:
					{
						char app[255];
						char dest[1024];
						char source[255];
						DWORD result = ::GetPrivateProfileString("startx", "source", "mxsupport.dnu", source, 255, ".\\startx.ini");
						// Programmname z.B. mxsupport.mdb
						DWORD result1 = ::GetPrivateProfileString("startx", "dest", "mxsupport.mdb", app, 255, ".\\startx.ini");
						// Verzeichnis, aus dem das Programm gestartet werden soll, z.B. c:\temp
						DWORD result2 = ::GetPrivateProfileString("startx", "dir", "c:\\temp", dest, 255, ".\\startx.ini");
						string destfile;
						destfile = (string)dest + (string)"\\" + (string)app;
						BOOL result3 = ::CopyFile(source, destfile.c_str(), false);
						
						string cmdline;
						cmdline = (string)"cmd /c start " + destfile;

						STARTUPINFO start;
						memset(&start, 0, sizeof(STARTUPINFO));
						start.dwFlags = STARTF_USESHOWWINDOW;
						start.wShowWindow = SW_SHOWMINIMIZED;
						PROCESS_INFORMATION inf;

						LPSTR proc = const_cast<LPSTR>(cmdline.c_str());
						if (!CreateProcess(NULL, proc, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &start, &inf))
						{
							PostQuitMessage(1);
						}
						WaitForInputIdle( inf.hProcess, 5000); 
						PostQuitMessage(0);
					}
					break;
				default:
				   return DefWindowProc( hWnd, message, wParam, lParam );
			}
			break;
		case WM_PAINT:
		{
			char app[255];
			char dest[1024];
			// Programmname z.B. mxsupport.mdb
			DWORD result = ::GetPrivateProfileString("startx", "start", "mxsupport.mdb", app, 255, ".\\startx.ini");
			// Verzeichnis, aus dem das Programm gestartet werden soll, z.B. c:\temp
			DWORD result1 = ::GetPrivateProfileString("startx", "tmp", "c:\\temp", dest, 255, ".\\startx.ini");

			RECT rt;
			GetClientRect( hWnd, &rt );
			hdc = BeginPaint (hWnd, &ps);
			SetBkColor(hdc, RGB(167,167,167));
			sprintf(szHello, "Kopiere %s nach %s ...", app, dest); 
			ExtTextOut(hdc, 10, 10, ETO_OPAQUE, &rt, szHello, strlen(szHello), NULL);
			EndPaint( hWnd, &ps );
		}

		break;
		
			
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}


/*
int _tmain(int argc, _TCHAR* argv[])
{
	
	char app[255];
	char dest[255];
	memset(app, 0, 255);
	memset(dest, 0, 255);
	unsigned int result, result1;
	BOOL result3;
	
	// Programmname z.B. mxsupport.mdb
	result = ::GetPrivateProfileString("startx", "start", "mxsupport.mdb", app, 255, ".\\startx.ini");
	
	// Verzeichnis, aus dem das Programm gestartet werden soll, z.B. c:\temp
	result1 = ::GetPrivateProfileString("startx", "tmp", "c:\\temp", dest, 255, ".\\startx.ini");

	cout << "Programm wird kopiert\n";
	// Datei ins Tempverzeichnis kopieren und starten
	string destfile;
	destfile = (string)dest + (string)"\\" + (string)app;
	result3 = ::CopyFile(app, destfile.c_str(), false);
	
	string cmdline;
	cmdline = (string)"cmd /c start " + destfile;

	STARTUPINFO start;
	memset(&start, 0, sizeof(STARTUPINFO));
	start.dwFlags = STARTF_USESHOWWINDOW;
	start.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION inf;

	LPSTR proc = const_cast<LPSTR>(cmdline.c_str());
	cout << "Programm wird gestartet\n";
	if (!CreateProcess(NULL, proc, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &start, &inf))
	{
		return 0;
	}
	WaitForInputIdle( inf.hProcess, 5000); 
	return (int)inf.hProcess;
}

*/