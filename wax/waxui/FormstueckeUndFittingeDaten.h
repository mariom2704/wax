#pragma once


/// Datenklasse der Formstücke und Fittinge
class WAXUIDLL FormstueckeUndFittingeDaten : public WaxPart  
  
{
	DECLARE_SERIAL(FormstueckeUndFittingeDaten)
	DECLARE_MESSAGE_MAP()

public:
	virtual ~FormstueckeUndFittingeDaten();

	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);

	/// Konstruktor
	FormstueckeUndFittingeDaten();

	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList& RuntimeModules);

	virtual CRuntimeClass* getGraphicView();

};

