#pragma once


//Projekt Leitkartendaten
class WAXPRJDLL PrjLeitkarteData : public WaxPart     
{
	DECLARE_SERIAL(PrjLeitkarteData)
	DECLARE_MESSAGE_MAP()

public:
	/// Titel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
	virtual CString getBezeichnung() const;

	/// Konstruktor
	PrjLeitkarteData(WaxPart *pParent = NULL);

	/// Destruktor
	virtual ~PrjLeitkarteData();

	/// Dialogklasse zurückliefern
	virtual void getForms(CObList &RuntimeModules);

	/// Speichern der Daten
	virtual void serialize(BinaryArchive& ar);

	/// Ist das Icon für nicht freigegeben zu modifizieren
	virtual bool is_freigegeben() const;
	
	/// Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
	virtual CRuntimeClass* getGraphicView() { return NULL; };


};

