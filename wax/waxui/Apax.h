#pragma once

/// Allgemeiner Werkauftrag
class WAXUIDLL ApaxData : public WaxPart  
  
{
	DECLARE_SERIAL(ApaxData)
	DECLARE_MESSAGE_MAP()

public:
	/// Grafikdatei erzeugen

	/// Lädt einen Apax aus Altwax
	void serializeOldApax(BinaryArchive & ar);

	ApaxData();

	virtual ~ApaxData();

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	/// Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich 
	/// ist
	virtual CRuntimeClass* getGraphicView();

	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);

};

