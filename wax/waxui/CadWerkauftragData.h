#pragma once


class WAXUIDLL CadWerkauftragData : public WaxPart  
 {
	DECLARE_MESSAGE_MAP()
	DECLARE_SERIAL(CadWerkauftragData)

public:
	virtual ~CadWerkauftragData();

	CadWerkauftragData();

	//Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	//Serialisierung
	virtual void serialize(BinaryArchive& ar);

	//Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
	//einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
	//erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
	//Objekte in subobjects auf.
	virtual bool synchronize(WaxPart* with);
};

