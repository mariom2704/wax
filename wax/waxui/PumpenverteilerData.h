#pragma once

//Sondermz Eingabe Pumpenverteiler
class WAXUIDLL PumpenverteilerData : public WaxPart  
{
	DECLARE_SERIAL(PumpenverteilerData)
	DECLARE_MESSAGE_MAP()

public:
	PumpenverteilerData();

	virtual ~PumpenverteilerData();

	//Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	//Serialisierung
	virtual void serialize(BinaryArchive& ar);
	
};

