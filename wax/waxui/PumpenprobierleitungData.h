#pragma once

/// Sondermz Eingabe von Pumpenprobierleitungen
class WAXUIDLL PumpenprobierleitungData : public WaxPart  
  
{
	DECLARE_SERIAL(PumpenprobierleitungData)
	DECLARE_MESSAGE_MAP()

public:
	PumpenprobierleitungData();

	virtual ~PumpenprobierleitungData();

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);
};
