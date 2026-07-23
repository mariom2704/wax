#pragma once

/**
Projektgliederung
PrjGliederung dient nur der allgemeinen Untergliederung von Projekten in 
logische Einheiten. Diese Klasse kann, aufgrund der beschränkten 
Funktionalität, als Schablone für 
neue Klassen benutzt werden.
*/
class WAXPRJDLL PrjGliederung : public WaxPart  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_SERIAL(PrjGliederung)
public:
	/// Titel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
	virtual CString getBezeichnung() const;

	/// Prüft die Eingaben der Leitkarten aller Unteraufträge auf Vollständigkeit
	virtual bool testLeitkarte(bool isPreisanfrage);

	///  Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
	virtual CRuntimeClass* getGraphicView() { return NULL; };

	/// Freigeben eines ganzen Bauabschnitts
	virtual bool freigeben(bool sicherheitsabfrage, Fortschrittanzeige *fortschritt, DWORD dbhandle);

	/// Ist das Icon für nicht freigegeben zu modifizieren
	virtual bool is_freigegeben() const;

	/// Konstruktor
	PrjGliederung();

	/// Destruktor
	virtual ~PrjGliederung();

	/// Objekt abspeichern
	virtual void serialize(BinaryArchive &ar);

	/// Kommentar/weitere Beschreibung
	CString kommentar;

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList &RuntimeModules);

	/// default Titel aus Registry setzen oder lesen
	static void getsetTitel(bool save, CString &title);
	
	virtual bool subobj_allowed() const;

protected:
	virtual void initPopUpMenu();
};


