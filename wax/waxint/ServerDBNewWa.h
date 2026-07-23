#pragma once

#include "ServerDBOpen.h"

/**
Spezielle Ableitung der ServerDBOpen Klasse zum Laden aller neuen Werkaufträge 
innerhalb eines Zeitraumes. Als Erweiterung wird für jede Art von WA ein 
Bearbeiter festgelegt. Diese Klasse soll die frühere 'Konvertierung' ersetzen.
*/
class WAXINT ServerDBNewWa : public ServerDBOpen  
{
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheckMitKC();
private:
	int nl;

	CComboBox cb_nl;

	//Datenklasse für die Zuweisung von Dialogid, Registrykey und Variable
	class items 
	{
	public:
		items(const CString& classname, int dlgid,  CString* var  );

	//Zeiger auf die Variable
		CString* var;

	//Dialogid
		int dlgid;

	//Klassenname des WaxPart
		CString classname;

	};



	//Name des Sachbearbeiters für die alle anderen Wa Arten
	CString sachbearbeiter_sonstiges;

	//Name des Sachbearbeiters für die Handelslängenaufträge.
	CString sachbearbeiter_handelslaengen;

	//Liste mit items-Objekten
	CPtrList itemlist;

	//Name des Sachbearbeiters für die Verteileraufträge. 
	CString sachbearbeiter_verteiler;

	//Name des Sachbearbeiters für die Stationsverteiler. 
	CString sachbearbeiter_stationsverteiler;

	//Name des Sachbearbeiters für die Strangrohraufträge. 
	CString sachbearbeiter_strangrohr;

	//Name des Sachbearbeiters für die Gewinderorhaufträge. 
	CString sachbearbeiter_gewinderohr;
public:
	//Import starten
	virtual void OnOK();

	virtual BOOL ResetWaNr(WaxPart* pt);

	virtual ~ServerDBNewWa();

	ServerDBNewWa(Status::STATUS status);

	virtual BOOL DestroyWindow();

	//Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	//{{AFX_VIRTUAL(ServerDBNewWa)
	public:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//Speichert das Projekt
	virtual bool serialize_project(CObList* project);

	/// Update des Status
	virtual void updateStatus(WaxPart* pt);

	//Erzeugen des SQL Select Strings aus den Angaben in der Maske
	virtual const CString createSQLSelect() const;

	//Erzeugen des SQL Sort Strings aus den Angaben in der Maske
	virtual const CString createSQLSort(CString tablename) const;

	// Initialisierung von neu geladenen Objekten
	virtual void initObject(WaxPart *pt);

private:
	void initSachbearbeiter(WaxPart* pt);

	
};


