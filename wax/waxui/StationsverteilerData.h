#pragma once

/// Stationsverteilerdaten
/// Diese Klasse enthält die Daten zum Standard MZ
class WAXUIDLL StationsverteilerData : public WaxPart  
{
	DECLARE_SERIAL(StationsverteilerData)
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	StationsverteilerData();

	/// Destruktor
	virtual ~StationsverteilerData();

	/// Klassen zur Bearbeitung liefern
	virtual void getForms(CObList& RuntimeModules);

	/// Speichern der Daten
	virtual void serialize(BinaryArchive& ar);

	/// Liefert den Stationsverteiler
    Stationsverteiler *  get_stationsverteiler();
protected:
	virtual bool synchronize(WaxPart *with);
};
