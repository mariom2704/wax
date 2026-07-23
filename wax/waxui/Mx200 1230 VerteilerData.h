#pragma once


class WAXUIDLL CMx2001230VerteilerData : public WaxPart  
{
	DECLARE_SERIAL(CMx2001230VerteilerData)
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	CMx2001230VerteilerData();

	/// Konstruktor
	
	/// Destruktor
	virtual ~CMx2001230VerteilerData();

	/// Klassen zur Bearbeitung liefern
	virtual void getForms(CObList& RuntimeModules);

	/// Speichern der Daten
	virtual void serialize(BinaryArchive& ar);

	/// Liefert den Stationsverteiler
    virtual CMx2001230Verteiler* get_CommonCOMXVerteiler();
protected:
	virtual bool synchronize(WaxPart *with);
};


/*
class WAXUIDLL MXIIVerteilerData : public CMx2001230VerteilerData  
{

		//DECLARE_SERIAL(MXIIVerteilerData)
		//DECLARE_MESSAGE_MAP()
	
	public:
		MXIIVerteilerData():CMx2001230VerteilerData(true){};
};
*/