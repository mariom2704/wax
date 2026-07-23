#pragma once

#include <mxcontrols/basedialog.h>
#include <mxcontrols/listctrl.h>


/**
Ausgabe der Zinkkosten
*/
class ZinkKostenDlg : public BaseDialog  
{
	DECLARE_MESSAGE_MAP()

public:
	ZinkKostenDlg(CObList& sm);

	virtual ~ZinkKostenDlg();
		
private:
	/// Rekursiv die Aufträge in eine Liste packen
	void add_rohre(CObList& p);

	enum SPALTEN
	{
		BEZEICHNUNG = 0,
		MENGE = 1,
		PREIS = 2,
		ZUSCHLAG = 3,
		BETRAG = 4,
	};

	/// Interne Liste mit Rohrleitungen
	vector<const Rohrleitung*> rohre;

	ListCtrl list;

	/// Anzeige der Werte, füllen des Listcontrols
	void show_values();

protected:

	virtual void init_ui();

	virtual BOOL DestroyWindow();

	virtual void DoDataExchange(CDataExchange* pDX);
};

