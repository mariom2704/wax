#pragma once

#include <string>

/**
Generierung bzw. Dekodierung eines Lizensschlüssels
*/
class LizenzKey
{
public:
	LizenzKey(){};

	enum ProgramIdentifier
	{
		SPANX = 0,
		WAX = 1,
		MXCALC = 2,
	};

	enum ProgramOptions
	{
		DEFAULT = 0,
		/// MxCalc Löschgase
		NOVEC = 1,
		HFC   = 2,
		CO2   = 4,
		ARGON = 8,
		
	};

	/// Alle nötigen Informationen für die Schlüsselgenerierung setzen
	void init_key(const std::string& hdkennung, unsigned int begin, unsigned int expiration, enum ProgramIdentifier program, enum ProgramOptions testkey);

	/// Prüft einen Key auf Gültigkeit
	bool validate_key(const std::string& key, const std::string& hdkennung, enum ProgramIdentifier program);

	/// Prüft auf gültiges Anfang und Endedatum
	int validate_date(unsigned int date);
	
	/// Aus dem eingegeben Key die resultierenden Daten erzeugen
	void decode_key();

	/// Setzen der HD Kennung
	void set_hdkennung(const std::string& val) { hd_kennung = val; }

	/// Auslesen der HD Kennung
	std::string get_hdkennung() const { return hd_kennung; }

	/// Setzen des Lizenzschlüssels
	void set_key(const std::string& val) { key = val; }

	/// Auslesen des Lizenzschlüssels
	std::string get_key() const { return key; }

	/// Liefert true wenn der CRC richtig berechnet werden konnte
	bool get_crcok() const;

	/// Auslesen des Ablaufdatums
	unsigned int get_expiration() const { return expiration; }

	/// Auslesen des Anfangsdatums
	unsigned int get_begin() const { return begin; }

	/// Auslesen des Programmtags
	unsigned char get_program_identifier() const { return program_identifier; }

	/// Auslesen des Optionentags
	unsigned char get_option_identifier() const { return option_identifier; }

private:
	/// Aus den Attributen den resultierenden Key erstellen
	void create_key();

	/// Umwandlung eines Characters in einen Hexstring und anhängen an den Keystring
	void append_hexkey(unsigned char val);

	/// Liefert ein Zeichen als Teilstring des Keys an der Stelle offset
	int get_partkey(int& offset) const;

	/// Setzen des Optionstags
	void set_option_identifier(unsigned char val) { option_identifier = val; }

	/// Setzen des Programmtags
	void set_program_identifier(unsigned char val) { program_identifier = val; }

	/// Setzen des Ablaufdatums
	void set_expiration(unsigned int val) { expiration = val; }

	/// Setzen des Anfangsdatums
	void set_begin(unsigned int val) { begin = val; }

	/// Setzen des CRC
	void set_crc(unsigned int val) { crc = val; }

	/// Aus den vorhandenen Daten den CRC generieren
	unsigned int create_crc() const;

    /// Festplattenkennung
	std::string hd_kennung;

	/// Tage beginnend beim 30 Dezember 1899 zum Ablaufdatum
	unsigned int expiration;

	/// Tage beginnend beim 30 Dezember 1899 zum Datum der Erstellung des Keys
	unsigned int begin;

	/// Der berechnete CRC 
	unsigned int crc;

	/// Der aus dem Key gelesene CRC
	unsigned int key_crc;
	
	/// Programmtag
	unsigned char program_identifier;

	/// Optionentag
	unsigned char option_identifier;

	/// Der erstellte Schlüssel
	std::string key;
};


