#pragma once

class WAXPRJDLL WpPreisanfrage : public WaxPart  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_SERIAL(WpPreisanfrage)
public:
	/// Titel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
	virtual CString getBezeichnung() const;

	/// Prüft die Eingaben der Leitkarten aller Unteraufträge auf Vollständigkeit
	virtual bool testLeitkarte(bool isPreisanfrage);

	///  Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
	virtual CRuntimeClass* getGraphicView() { return NULL; };


	/// Ist das Icon für nicht freigegeben zu modifizieren
	virtual bool is_freigegeben() const;

	//Gibt den Status zurück
	virtual Status::STATUS get_status() const;

	/// Konstruktor
	WpPreisanfrage();

	/// Destruktor
	virtual ~WpPreisanfrage();

	/// Objekt abspeichern
	virtual void serialize(BinaryArchive &ar);

	/// Kommentar/weitere Beschreibung
	CString kommentar;

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList &RuntimeModules);

	
	virtual bool subobj_allowed() const;

	///Summiert die Preise aller Unterordner
	double getGesamtpreis(WaxPart* ptHead) ;

	void copyGueltigBisToChilds(WaxPart *pt, MXDate gueltigBis);


	virtual bool isPreisanfrage()const;

protected:
	virtual void initPopUpMenu();

	const SammelAuftrag* getFirstSammelauftrag(const WaxPart* ptHead)const;
};
