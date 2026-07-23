#pragma once

#include "WaxPart.h"
#include "..\WAXPRJ\Maschinenlisten.h"

/// Sammelklasse für mehrere Werkaufträge gleicher Art
/// Mit dieser Klasse lassen sich mehrere gleichartige Werkaufträge unter 
/// einer Positionsnummer zusammenfassen.
/// Alle enthaltenen Werkaufträge lassen sich nur zusammen freigeben.

/// Bei der Druckausgabe wird die Leitkarte nur einmal ausgegeben, die Grafiken 
/// werden untereinander darangehängt.
class CObList;

/// Sammelauftrag
/// Diese Klasse bildet einen Container um gleichartige Werkaufträge. Die 
/// Leitkarte für alle Werkaufträge ist gleich, ebenso die WA Nummer. Ein 
/// Sammelauftrag entspricht einem Werkauftrag im Altwax
class WAXPRJDLL SammelAuftrag : public WaxPart  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_SERIAL(SammelAuftrag)

private:

	double gesamtgewicht;
	int maxlaenge;
	WaxDruck::Maschinenlisten		*m_pMaschinenlisten;
	
public:	

	/// Kranzeit fuer den gesamten Auftrag ermitteln und setzen
	void setKranzeit();

	/// Doppelte Ruestzeiten muessen nicht mehr entfernt werden, da beim Zusammenfassen gleicher
	/// Afos nur die Zeit und Anzahl addiert werden, nicht aber die Ruestzeit.
	/// Bezieht sich also nur auf die Afo Ruesten fuers Klinken. Dazu wird fuer den gesamten
	/// Sammelauftrag ermittelt wieoft umgeruestet werden muss.
	/// Es muss bei jedem neu einzustellenden Radius umgeruestet werden,
	/// nicht nur bei jeder neuen Nennweite.
	void remove_doppelteRuestzeiten();

	bool checkAVNachbearbeitung();

	WaxPart* get_testobject();

public:
	/// Standardkonstruktor
	SammelAuftrag();

	bool SammelCheckPlausiFertigungMagdeburg();

	/// Destruktor
	virtual ~SammelAuftrag();

	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);

	/// Grafikdatei erzeugen
	virtual void createGraphic();

	/// Werkauftrag freigeben
	//virtual void onFreigeben();
	virtual bool freigeben(bool sicherheitsabfrage, Fortschrittanzeige *fortschritt, DWORD dbhandle);

	/// Ist das Icon für nicht freigegeben zu modifizieren
	virtual bool is_freigegeben() const;

	virtual Status::STATUS get_status() const;

	/// Liefert eine Referenz auf das Kontextmenü zurück
	virtual CMenu* get_menu();

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);
	
	/// Prüft, ob das Objekt gezeigt werden muss. Das ist der Fall, wenn nicht freigegeben wurde.
	virtual bool initVisible();

	/// itel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
	virtual CString getBezeichnung() const;

	/// Führt die Synchronisation für alle Unterobjekte durch
	virtual bool doSync(bool checkFreigabe = true);

	virtual bool iconFreigegeben();

	virtual bool subobj_allowed() const;

	///  Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
	virtual CRuntimeClass* getGraphicView() { return NULL; };

	virtual void createAutoArbeit();


	virtual void createAutoSoko(bool deleteFixedSoko);

	virtual bool sollUpdateStatus();

	bool setAfosAndMoveToFirstPos();

	bool copyAfosToSammelauftrag(bool bWithClean);


	/// Durchlaeuft die Sammelliste und errechnet das Gesamtgewicht und die maximale Laenge
	void setMaxLaengeUndGewicht();

	bool checkMindermenge();

	/// Setzt Flag fuer Mindermenge
	void set_mindermenge();

	void onCreateAutoarbeit();
	

	inline double get_gesamtgewicht();
	inline int get_maxlaenge();

protected:
	/// Menü initialisieren
	virtual void initPopUpMenu();

};

inline double SammelAuftrag::get_gesamtgewicht()
{
	return gesamtgewicht;
}

inline int SammelAuftrag::get_maxlaenge()
{
	return maxlaenge;
}

