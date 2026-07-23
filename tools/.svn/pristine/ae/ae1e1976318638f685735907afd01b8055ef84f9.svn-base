#include "lizenzkey.h"
#include <mxutil/crc32.h>

using namespace std;

void LizenzKey::init_key(const string& hdkennung, unsigned int begin, unsigned int expiration
						 , enum ProgramIdentifier program, enum ProgramOptions testkey)
{
	set_hdkennung(hdkennung);
	set_begin(begin);
	set_expiration(expiration);
	set_program_identifier((unsigned char)program);
	set_option_identifier((unsigned char)testkey);
	set_crc(create_crc());
	create_key();
}

bool LizenzKey::get_crcok() const
{
	return  crc == key_crc;
}

unsigned int LizenzKey::create_crc() const
{
	Crc32 crc;
	crc.addData((unsigned char*)hd_kennung.c_str(), (int)hd_kennung.length());
	crc.addData((int)begin);
	crc.addData((int)expiration);
	crc.addData(program_identifier);
	crc.addData(option_identifier);
	return crc.get_crc();
}

void LizenzKey::decode_key()
{
	/** 
	Erzeugt aus dem eingegebenen keystring die Daten
	*/
	string hdk = "";
	key_crc = 0;
	if (get_key().length() != 36)
	{
		// damit get_crcok auch wirklich false liefert
		crc = 1;
		return;
	}
	int key_offset = 0, dez = 0;
	unsigned int crcbit = 0;

	// HD Kennung extrahieren
	for (int i = 24; i > 16; i--)
	{
		dez = get_partkey(key_offset);
        hdk += (char)(dez & 0x7f); // Oberstes Bit gehört zum CRC;
		crcbit = dez & 0x80;
		crcbit <<= i;
		key_crc |= crcbit;
	}
	// und vergleichen
	if (hd_kennung != hdk)
	{
		crc = 1;
		return;
	}

	// Programmbyte
	dez = get_partkey(key_offset);
	set_program_identifier((unsigned char)(dez & 0x0f));
	crcbit = dez & 0xf0;
	key_crc |= crcbit << 16;


	// Programmbyte
	dez = get_partkey(key_offset);
	set_option_identifier((unsigned char)(dez & 0x0f));
	crcbit = dez & 0xf0;
	key_crc |= crcbit << 12;

	// Ablaufdatum
	unsigned int expire = 0;
	dez = get_partkey(key_offset);
	expire |= dez << 24;
	dez = get_partkey(key_offset);
	expire |= dez << 16;
	dez = get_partkey(key_offset);
	expire |= dez << 8;
	dez = get_partkey(key_offset);
	expire |= dez;
	crcbit = expire & 0xff000000;
	key_crc |= crcbit >> 16;
	set_expiration(expire & 0x00ffffff);

	// Anfangsdatum
	unsigned int begin = 0;
	dez = get_partkey(key_offset);
	begin |= dez << 24;
	dez = get_partkey(key_offset);
	expire |= dez << 16;
	dez = get_partkey(key_offset);
	begin |= dez << 8;
	dez = get_partkey(key_offset);
	begin |= dez;
	crcbit = begin & 0xff000000;
	key_crc |= crcbit >> 24;
	set_begin(begin & 0x00ffffff);
	set_crc(create_crc());
}

int LizenzKey::get_partkey(int& offset) const
{
	char hex[3];
	memset(hex, 0, 3);
	hex[0] = key[offset++];
	hex[1] = key[offset++];
	int dez;
	sscanf(hex, "%x", &dez);
	return dez;
}

void LizenzKey::create_key()
{
    /** 
	Erzeugt aus dem eingegebenen keystring die Daten
	*/
	string hd = get_hdkennung();

	key = "";

	// Die ersten 8 Byte enthalten jeweils als erstes Bit, das erste Byte des CRC
	unsigned int bitpattern = 0x80000000;

	for (int i = 0; i < 8; i++)
	{
		hd[i] |= crc & bitpattern ? 0x80  : 0x00;
		bitpattern >>= 1;
		append_hexkey((unsigned char)hd[i]);
	}
	unsigned char prog = program_identifier;
	unsigned int bits = crc & 0xf00000;

	// 16 Bits nach rechts, damit sie die oberen 4 Bit bilden
	prog |= (unsigned char)(bits >> 16);
	append_hexkey(prog);

	unsigned char test = option_identifier;
	bits = crc & 0xf0000;
    // 16 Bits nach rechts, damit sie die unteren 4 Bit bilden
	test |= (unsigned char)(bits >> 12);
	append_hexkey(test);

	// Bleiben noch 16 Bit übrig für die Datumsangaben
	bits = crc & 0xff00;
	unsigned int expire = expiration;
	expire |= bits << 16;
	append_hexkey((unsigned char)(expire >> 24));
	append_hexkey((unsigned char)((expire >> 16) & 0xff));
	append_hexkey((unsigned char)((expire >> 8) & 0xff));
	append_hexkey((unsigned char)(expire & 0xff));

	// Bleiben noch 8 Bit übrig für das zweite Datum
	bits = crc & 0xff;
	unsigned int beg = begin;
	beg |= bits << 24;
	append_hexkey((unsigned char)(beg >> 24));
	append_hexkey((unsigned char)((beg >> 16) & 0xff));
	append_hexkey((unsigned char)((beg >> 8) & 0xff));
	append_hexkey((unsigned char)(beg & 0xff));
}

void LizenzKey::append_hexkey(unsigned char val)
{
	char tmpkey[3];
	memset(tmpkey, 0, 3);
	sprintf(tmpkey,"%.2x", (unsigned char)val);
	key += tmpkey;
}

bool LizenzKey::validate_key(const string& key, const string& hdkennung, enum ProgramIdentifier program)
{
	/**
	Prüft den Key auf Gültigkeit. Es wird nur der Key decodiert und der CRC überprüft.
	*/
	set_hdkennung(hdkennung);
	set_key(key);
	set_program_identifier((unsigned char)program);
	decode_key();
	return get_crcok();
}

int LizenzKey::validate_date(unsigned int date)
{
	/**
	Prüft das übergebene Datum auf Gültigkeit innerhalb der Lizenzgültigkeit.
	Liefert die restliche Anzahl von Lizenztagen zurück.
	*/
	bool dateok =  date >= begin && date <= expiration;
	if (dateok)
		return expiration - date + 1;
	return 0;
}