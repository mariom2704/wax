#pragma once

#include <mxcontrols/binaryarchive.h>
#include "mxdate.h"

/**
Statuselemente der Leitkarte betreffend der Freigabe
*/
class ROHRKONSTDLL Status
{
protected:
	/// Werkauftragsnummer
	long wanr;

public:
	enum STATUSVAR 
	{
		/// Beim Laden ist ein CRC Fehler aufgetreten. Weitere Info sind in statusstr gespeichert.
		CRCERROR = 0x01,
		/// Werkauftrag wurde storniert
		STORNIERT = 0x08,
	};


	enum STATUS
	{
		NichtGesetzt	= 0,
		Positioniert	= 1,	//Bauteil ist positioniert bzw. der Werkauftrag ist angelegt worden und noch nicht freigegeben
		Freigegeben		= 2,	//Werkauftrag wurde vom User freigegeben
		InBearbeitungAv = 3,	//Werkauftrag wurde im SegmentRohr eingelesen
		FreigegebenAv	= 4,	//Werkauftrag wurde im SegmentRohr freigegeben und nach SAP übertragen
        Preisanfrage    = 7,    //Preisanfrage an die AV
        Angebot         = 8,    //Angebot durch die AV
		Storniert		= 10,	//Werkauftrag bzw. Position wurde vom Segment Rohr zurückgesetzt
		PreisanfrageStorniert  = 11,  //Preisanfrage storniert 
		AngebotAbgelehnt	= 12, //Angebot abgelehnt
	};

	bool isFreigabedatumOld();

	inline const long get_wanr() const;

	inline void set_wanr(long value);

	/// Setzt die Daten zurück. Muss bei Kopieraktionen aufgerufen werden.
	void reset();

	Status();

	virtual ~Status();

	void serialize(MxFilesystem::BinaryArchive& ar);

	inline const MXDate& get_freigabedatum() const;

	inline void set_freigabedatum(const MXDate& value);

	inline const CString& get_sachbearbeiterSegment() const;

	inline void set_sachbearbeiterSegment(const CString& value);

	void set_status(Status::STATUS stat);

	Status::STATUS get_status()const;

	const MXDate& get_mbtermin() const;

	void set_mbtermin(const MXDate& value);

	inline const MXDate& get_bearbeitungsdatum() const;

	inline void set_bearbeitungsdatum(const MXDate& value);

	inline int get_freigabecounter() const;

	inline void set_freigabecounter(int value);

	inline const UINT& get_statusflag() const;

	inline void set_statusflag(const UINT& value);

	inline bool get_storniert() const;

	inline void set_storniert(bool value);

    void copy(const Status& other);

private:
	/// Sachbearbeiter Segment
	CString sachbearbeiterSegment;

	/// von der AV angepeiltes Lieferdatum
	MXDate mbtermin;

	/// Datum der Bearbeitung in OD
	MXDate bearbeitungsdatum;

	MXDate freigabedatum;

	/// Anzahl der Freigaben
	int freigabecounter;
	
	/// Status des Objektes, wird diverse Flags enthalten, z.B. CRC Fehler beim 
	/// Laden aus Datenbank.
	UINT statusflag;

	CString statusstr;

	Status::STATUS status;

public:
	/// Setzt das Statusflag CRCERROR und fügt eine Statusmeldung hinzu.
	void setCRCError(const CString& stat, const CString &inf = "");

	inline const CString& get_statusstr() const;

	/// Fügt dem statusstr eine Meldung hinzu. Die Meldung wird an die bisherigen 
	/// Meldungen angehängt.
	void addstatusstr(const CString & stat);

};



inline const MXDate& Status::get_freigabedatum() const
{
	return freigabedatum;
}

inline void Status::set_freigabedatum(const MXDate& value)
{
	freigabedatum = value;
}

inline const CString& Status::get_sachbearbeiterSegment() const
{
	return sachbearbeiterSegment;
}

inline void Status::set_sachbearbeiterSegment(const CString& value)
{
	sachbearbeiterSegment = value;
}

inline const MXDate& Status::get_mbtermin() const
{
	return mbtermin;
}



inline const MXDate& Status::get_bearbeitungsdatum() const
{
	return bearbeitungsdatum;
}

inline void Status::set_bearbeitungsdatum(const MXDate& value)
{
	bearbeitungsdatum = value;
}

inline int Status::get_freigabecounter() const
{
	return freigabecounter;
}

inline void Status::set_freigabecounter(int value)
{
	freigabecounter = value;
}


inline const UINT& Status::get_statusflag() const
{
	return statusflag;
}

inline void Status::set_statusflag(const UINT& value)
{
	statusflag |= value;
}

inline const long Status::get_wanr() const
{
	return wanr;
}

inline void Status::set_wanr(long value)
{
	wanr = value;
}
inline const CString& Status::get_statusstr() const
{
	return statusstr;
}

inline void Status::set_storniert(bool value)
{
	if (value)
		set_statusflag(STORNIERT);
	else
		statusflag &= ~STORNIERT;

}

inline bool Status::get_storniert() const
{
	return statusflag & STORNIERT ? true : false;
}


