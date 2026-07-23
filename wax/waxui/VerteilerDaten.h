#pragma once


///  Datenklasse der Verteiler
class WAXUIDLL VerteilerData : public WaxPart  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_SERIAL(VerteilerData)
public:
	///  Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
	///  einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
	///  erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
	///  Objekte in subobjects auf.
	virtual bool synchronize(WaxPart* with);

	///  Konstruktor
	VerteilerData();

	///  Destruktor
	virtual ~VerteilerData();


	///  Serialisierung
	virtual void serialize(BinaryArchive& ar);


	///  Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);
};
