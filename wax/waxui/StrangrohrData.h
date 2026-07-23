#pragma once

/// Strangrohrdaten 
/// Diese Klasse enthält die Daten für die Eingabe der Strangrohrdaten
class WAXUIDLL StrangrohrData : public WaxPart  
{
	DECLARE_SERIAL(StrangrohrData)
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	StrangrohrData();

	/// Destruktor
	virtual ~StrangrohrData();

	/// Klassen zur Bearbeitung liefern
	virtual void getForms(CObList& RuntimeModules);

	/// Speichern der Daten
	virtual void serialize(BinaryArchive& ar);
	
protected:
	virtual bool synchronize(WaxPart *with);
};
