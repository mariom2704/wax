#pragma once


class WAXUIDLL COIIVerteilerData : public WaxPart  
{
	DECLARE_SERIAL(COIIVerteilerData)
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	COIIVerteilerData();

	/// Konstruktor
	COIIVerteilerData(bool rohrCOIIDruckstufe);
	/// Destruktor
	virtual ~COIIVerteilerData();

	/// Klassen zur Bearbeitung liefern
	virtual void getForms(CObList& RuntimeModules);

	/// Speichern der Daten
	virtual void serialize(BinaryArchive& ar);

	/// Liefert den Stationsverteiler
    virtual COIIVerteiler * get_CommonCOMXVerteiler();
protected:
	virtual bool synchronize(WaxPart *with);
};