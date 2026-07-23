#pragma once


class WAXUIDLL EntnahmeleitungData : public WaxPart  
  
{
	DECLARE_SERIAL(EntnahmeleitungData)
	DECLARE_MESSAGE_MAP()

public:
	EntnahmeleitungData();

	virtual ~EntnahmeleitungData();

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);
};
