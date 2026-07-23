#include "stdafx.h"


#include "Beschichtung.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Standardkonstruktor
Beschichtung::Beschichtung() : typ(ROT_GEPULV), ral(-1)
{
	bezeichnung = get_bezeichnung()	;
	verzinkerei = DEFAULT_VERZINKEREI;
}

//Konstruktor
Beschichtung::Beschichtung(int typ, int ral, CString bezeichnung)
{
	this->typ			= typ;
	this->bezeichnung	= bezeichnung;
	this->ral			= ral;
	verzinkerei = DEFAULT_VERZINKEREI;
}

//Destruktor
Beschichtung::~Beschichtung()
{
}

//Speichern und Laden einer Beschichtung.
void Beschichtung::serialize(BinaryArchive& ar)
{
#define VERZINKEREI_BITSHIFT 16
	/**
	verzinkerei wird vorerst mit als Bitmuster in Typ gespeichert. 
	Ein Abspeichern ist evtl. gar nicht nötig, da die Verzinkerei eh 
	nur im Segment gesetzt wird. In der NL ist verzinkerei default 0, 
	könnte also später leicht wieder entfernt werden ohne das Dateiformat zu ändern.
	*/
	if (ar.is_storing())
	{
		int storetyp = typ | (verzinkerei << VERZINKEREI_BITSHIFT);
		ar << get_bezeichnung() << ral << storetyp;
	}
	else
	{
		int typmask = 0xf; // typ ist in den unteren 8 Bit gespeichert
		ASSERT(typmask > MAXTYP); // Bei Erweiterungen von Typ muss Typ auch in die typmask passen
		ar >> bezeichnung >> ral >> typ;
		verzinkerei = typ >> VERZINKEREI_BITSHIFT;
		typ &= typmask;
	}
}

//get bezeichnung
CString Beschichtung::get_bezeichnung() const
{
	if (bezeichnung.GetLength() > 3)
		return bezeichnung;
	else 
	{
		for (POSITION pos = getStammdaten().get_beschichtungen().GetHeadPosition(); pos;)
		{
			Beschichtungen::data *besch = (Beschichtungen::data*)getStammdaten().get_beschichtungen().GetNext(pos);
			if (besch->get_index() == typ)
				return besch->get_bezeichnung();
		}
	}
	return "";
}

//set bezeichnung
void Beschichtung::set_bezeichnung(const CString& value)
{
	bezeichnung = value;
}

//get ral
const int Beschichtung::get_ral() const
{
	return ral;
}

//set ral
void Beschichtung::set_ral(int value)
{
	ral = value;
}

//get typ
const int Beschichtung::get_typ() const
{
	return typ;
}

//set typ
void Beschichtung::set_typ(int value)
{
	typ = value;
}

bool Beschichtung::isSonderfarbe() const
{
	/**
	Prüft die Ral Nummern auf Sonderfarben. Standardfarben sind rot 3000 und 
	weiss 9010.
	*/
	return typ != ROT_GEPULV && typ != WEISS_GEPULV;
}

// Prüft auf gepulvert)
bool Beschichtung::isGepulvert() const
{
	return typ == ROT_GEPULV || typ == WEISS_GEPULV || ral > 0;
}

//Prüft den Typ auf verzinkt
bool Beschichtung::isVerzinkt() const
{
	return get_typ() == VERZINKT;
}

//Prüft den Typ auf Grundierung
bool Beschichtung::isGrundiert() const
{
	return get_typ() == GRUND || get_typ() == DOPPELT_GRUND;
}

//Kopiert die Beschichtungsattribute
void Beschichtung::copy(const Beschichtung& other)
{
	set_typ(other.get_typ());
	set_bezeichnung(other.get_bezeichnung());
	set_ral(other.get_ral());
	set_verzinkerei(other.get_verzinkerei());
}

void Beschichtung::set_verzinkerei(int val)
{
	verzinkerei = val;
}

int Beschichtung::get_verzinkerei() const
{
	return verzinkerei;
}
