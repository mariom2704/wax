#pragma once


/// Datenklasse der Gewinderohre
class WAXUIDLL GewinderohrData : public WaxPart  
{
	DECLARE_SERIAL(GewinderohrData)
	DECLARE_MESSAGE_MAP()
	
public:
	/// Methode, die in abgeleiteten Klassen überschrieben 
	/// werden kann, um über einen Aufruf von synchronize(this) einen Abgleich von 
	/// Memberattributen zu erreichen. Defaultmässig ruft synchronize die 
	/// synchronize Methode aller Objekte in subobjects auf.
	virtual bool synchronize(WaxPart* with);

	/// Konstruktor
	GewinderohrData();

	/// Destruktor
	virtual ~GewinderohrData();


	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);
	
	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

};
