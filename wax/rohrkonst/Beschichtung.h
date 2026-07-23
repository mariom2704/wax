#pragma once

#include <mxcontrols/binaryarchive.h>

/**
Definition der Beschichtung einer Rohrkonstruktion. Gilt immer 
fuer den ganzen Werkauftrag.
*/
class ROHRKONSTDLL Beschichtung  
{

public:
	Beschichtung();

	Beschichtung(int typ, int ral, CString bezeichnung);

	virtual ~Beschichtung();

	enum TYP 
	{
		OHNE = 0,

		//SCHWARZ = 1,

		ROT_GEPULV = 2,

		WEISS_GEPULV = 3,

		WEISSALU_GEPULV = 4,

		LICHTGRAU_GEPULV = 5,

		ROTBRAUN_GEPULV = 6,

		GRUEN_GEPULV = 7,

		GEPULVERT = 8,

		VERZINKT = 9,

		GRUND = 10,

		DOPPELT_GRUND = 11,

		MAXTYP = 12,
	};

	enum VERZINKUNG
	{
		DEFAULT_VERZINKEREI = 255,
	};

	/// Kopiert die Beschichtungsattribute
	void copy(const Beschichtung& other);

	/// Prüft auf gepulvert
	bool isGepulvert() const;

	/// Prüft den Typ auf Grundierung (typ = 9/10)
	bool isGrundiert() const;

	/// Prüft den Typ auf verzinkt (typ = 8)
	bool isVerzinkt() const;
	
	/// Speichern und Laden einer Beschichtung.
	void serialize(MxFilesystem::BinaryArchive& ar);

	CString get_bezeichnung() const;

	void set_bezeichnung(const CString& value);

	const int get_ral() const;

	void set_ral(int value);

	const int get_typ() const;

	void set_typ(int value);

	void set_verzinkerei(int val);

	int get_verzinkerei() const;

	//Prüft die Ral Nummern auf Sonderfarben. Standardfarben sind rot 3000 und weiss 9010.
	bool isSonderfarbe() const;

private:
	CString bezeichnung;

	//RAL-Nr.
	int ral;

	int typ;

	int verzinkerei;
};

