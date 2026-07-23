#ifndef _STATUSAKTUALISIERUNG_
#define _STATUSAKTUALISIERUNG_


/**
Aktualisierung des Status einer oder mehrerer Werkaufträge
Liest die übergebene Liste mit Objekten aus und sorgt für ein Update der 
Statusfelder durch Auslesen der Statustabelle der Serverdatenbank.

Übergeben wird die komplette Liste mit Objekten in der Datei und bzw. oder das gerade 
ausgewählte Objekt. Im Dialog kann dann u.U. ausgewählt werden, ob nur das ausgewählte
Objekt oder alle Objekte aktualisiert werden sollen.
*/
class StatusAktualisierung : public CDialog  
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor, setzt die Objektliste und das zu aktualisierende Objekt
	StatusAktualisierung(CObArray & objects, WaxPart *selObject);

	/// Ermittelt die Anzahl der Werkaufträge, deren Status upgedatet wird.
	int get_update_count();

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

private:
	/// Startet die Aktualisierung
	void on_aktualisierung();

	/// Fortschrittsanzeiger
	CProgressCtrl progress;

	/// Aktualisierung für einen Wa inklusive Unteraufträge durchführen
	int aktualisierung(WaxPart* pt, bool testOnly = false);

	/// Selektiertes Objekt, das aktualisiert werden soll. Null wenn alle Objekte 
	/// aktualisiert werden sollen.
	WaxPart* selObj;

	/// Rootobjekte der Projektdatei aus WaxDoc
	CObArray& objectList;
};

#endif 
