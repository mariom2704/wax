#pragma once

/**
Initialisierung der MFC und der Userparameter und Stammdaten
*/
class UnitTestApp : public CWinApp
{
public:	
	virtual BOOL InitInstance( )
		{
		int nRetCode = 0;

		// MFC initialisieren und drucken. Bei Fehlschlag Fehlermeldung aufrufen.
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		{
			// TODO: Den Fehlercode an Ihre Anforderungen anpassen.
			_tprintf(_T("Schwerwiegender Fehler bei der MFC-Initialisierung\n"));
			return false;
		}
		
		SetRegistryKey("Wax+");

		HINSTANCE hRes = NULL;
		hRes= LoadLibrary("wax.exe");
		if(hRes)
			AfxSetResourceHandle(hRes);

		cout << "Lade Userdb..." << endl;
		DBConnection con;
		
		con.set_userdbentry("..\\..\\..\\wax\\datenbanken\\waxuser.mdb");
		WaxSys::load(con.get_userdb());

		cout << "Lade Stammdaten..." << endl;
		con.set_userdbentry("..\\..\\..\\wax\\datenbanken\\waxstamm.mdb");
		getStammdaten().loadData((LPCSTR)con.get_userdb(), NULL);
		return CWinApp::InitInstance();
	}

	virtual ~UnitTestApp() 
	{
		WaxSys::unload();
	}

};