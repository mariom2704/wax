#pragma once

#include "stl.h"

/**
Generierung bzw. Dekodierung eines Lizensschlüssels
*/
class LicenseGenerator
{
public:
	LicenseGenerator();
	virtual ~LicenseGenerator();

	enum kennung
	{
		SPANX = 0,
		WAX = 1,
		NORMALKEY = 0, 
		TESTKEY = 1,
	};

	/// Alle nötigen Informationen für die Schlüsselgenerierung setzen
	void initkey(const string& hdkennung, unsigned int begin, unsigned int expiration, enum kennung program, enum kennung testkey);
	
	/// Aus dem eingegeben Key die resultierenden Daten erzeugen
	void decode_key();

	/// Setzen der HD Kennung
	inline void set_hdkennung(const string& val);

	/// Auslesen der HD Kennung
	inline string get_hdkennung() const;

	/// Setzen des Lizenzschlüssels
	inline void set_key(const string& val);

	/// Auslesen des Lizenzschlüssels
	inline string get_key() const;

	/// Liefert true wenn der CRC richtig berechnet werden konnte
	bool get_crcok() const;

	/// Auslesen des Ablaufdatums
	inline unsigned int get_expiration() const;

	/// Auslesen des Anfangsdatums
	inline unsigned int get_begin() const;

	/// Auslesen des Programmtags
	inline unsigned char get_program() const;

	/// Auslesen des "30-Tage-Schlüssel" Tags
	inline unsigned char get_testkey() const;
private:
	/// Aus den Attributen den resultierenden Key erstellen
	void create_key();

	/// Umwandlung eines Characters in einen Hexstring und anhängen an den Keystring
	void append_hexkey(unsigned char val);

	/// Liefert ein Zeichen als Teilstring des Keys an der Stelle offset
	int get_partkey(int& offset) const;

	/// Setzen des "30-Tage-Schlüssel"
	inline void set_testkey(unsigned char val);

	/// Setzen des Programmtags
	inline void set_program(unsigned char val);

	/// Setzen des Ablaufdatums
	inline void set_expiration(unsigned int val);

	/// Setzen des Anfangsdatums
	inline void set_begin(unsigned int val);

	/// Setzen des CRC
	inline void set_crc(unsigned int val);

	/// Aus den vorhandenen Daten den CRC generieren
	unsigned int create_crc() const;

    /// Festplattenkennung
	string mHdkennung;

	/// Tage beginnend beim 30 Dezember 1899 zum Ablaufdatum
	unsigned int mExpiration;

	/// Tage beginnend beim 30 Dezember 1899 zum Datum der Erstellung des Keys
	unsigned int mBegin;

	/// Der berechnete CRC 
	unsigned int mCrc;

	/// Der aus dem Key gelesene CRC
	unsigned int mKeyCrc;
	
	/// Programmschlüssel
	unsigned char mProgram;

	/// Testlizenz
	unsigned char mTestkey;

	/// Der erstellte Schlüssel
	string mKey;
};




inline void LicenseGenerator::set_hdkennung(const string& val)
{
	mHdkennung = val;
}

inline string LicenseGenerator::get_hdkennung() const
{
	return mHdkennung;
}

inline void LicenseGenerator::set_key(const string& val)
{
	mKey = val;
}

inline string LicenseGenerator::get_key() const
{
	return mKey;
}


inline void LicenseGenerator::set_expiration(unsigned int val)
{
	mExpiration = val;
}

inline unsigned int LicenseGenerator::get_expiration() const
{
	return mExpiration;
}

inline void LicenseGenerator::set_begin(unsigned int val)
{
	mBegin = val;
}

inline unsigned int LicenseGenerator::get_begin() const
{
	return mBegin;
}

inline void LicenseGenerator::set_program(unsigned char val)
{
	mProgram = val;
}

inline unsigned char LicenseGenerator::get_program() const
{
	return mProgram;
}

inline void LicenseGenerator::set_testkey(unsigned char val)
{
	mTestkey = val;
}

inline unsigned char LicenseGenerator::get_testkey() const
{
	return mTestkey;
}

inline void LicenseGenerator::set_crc(unsigned int val)
{
	mCrc = val;
}