#pragma once

/**
Anzeige einer Eingabebox zur Auswahl einer Datenbank.Diese 
Datenbank ist die Benutzerdatenbank. Sie kann auch in der 
Systemsteuerung unter dem Namen WaxSys eingetragen werden. 
In den Systemparametern von WaxSys sind Verweise auf die 
Stammdatenbank und die Serverdatenbank enthalten. Somit 
muss die Benutzerdatenbank als erstes geladen werden. Beim 
ersten Start von Wax+ muss dieser Dialog automatisch 
aufgerufen werden, damit der Benutzer die Datenbank 
auswählen kann.
*/
class DBAuswahl : public CDialog  
{
	DECLARE_MESSAGE_MAP()
private:

	bool m_bFinished;
	/// Wählt eine Access Datenbank nach Pfadangabe aus
	bool selectDB(CString& pfad);

	/// Schalter, ob beim Laden ein Auswahlmenü für die Datenbanken erscheinen soll.
	BOOL prompt;

	/// Bezeichnung der Datenbank, z.B. Serverdatenbank
	CString name;

	/// Bezeichnung der Datenbank, z.B. Lokale Datenbank
	CString name2;

	/// Pfad zur Wax+ Benutzerdatenbank, bzw. Eintrag 
	/// in der Systemsteuerung/ODBC
	CString pfad2;

	/// Sucht aus den installierten Treibern den Treiberstring für Access heraus
	static CString getDriver() ;

	/// Pfad zur Wax+ Benutzerdatenbank, bzw. Eintrag 
	/// in der Systemsteuerung/ODBC
	CString pfad;

	void setText(int id, const string &dbid);

public:
	bool get_prompt() const;

	bool ShouldApplicationfinishUserdbNotFound()
	{
		return m_bFinished;
	}
	/// Erzeugt aus einem Datenbankpfad einen Connectstring
	static CString getConnect(CString connect) ;
	const CString& get_pfad() const;
	const CString& get_pfad2() const;

	void set_pfad(const CString& value);

	virtual ~DBAuswahl();

	DBAuswahl();


	/// Tooltips ausgeben
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	/// Subclassing Methode der MFC, wird für einen 
	/// Aufruf von EnableToolTips() überschrieben.
	virtual void PreSubclassWindow();

	/// Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	/// Datenaustausch mit Dialogelementen
	virtual void DoDataExchange(CDataExchange* pDX);

	/// Datenbankpfad abspeichern
	virtual void OnOK();
	
protected:

	void OnDbsel();
	/// Indicates the user clicked a button
	void OnClickedDbsel2();
};
