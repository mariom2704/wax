#pragma once

class WAXUIDLL HandelslaengeData : public WaxPart  
 {
	DECLARE_SERIAL(HandelslaengeData)
	DECLARE_MESSAGE_MAP()
public:
	virtual ~HandelslaengeData();

	HandelslaengeData();

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);

	/// Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
	/// einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
	/// erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
	/// Objekte in subobjects auf.
	virtual bool synchronize(WaxPart* with);
};
