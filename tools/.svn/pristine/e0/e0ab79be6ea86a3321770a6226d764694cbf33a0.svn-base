#include "StdAfx.h"
#include <assert.h>
#include "crc32.h"

#include "licensegenerator.h"

LicenseGenerator::LicenseGenerator()
{
}

LicenseGenerator::~LicenseGenerator()
{
}

void LicenseGenerator::initkey(const string& hdkennung, unsigned int begin, unsigned int expiration, enum kennung program, enum kennung testkey)
{
	set_hdkennung(hdkennung);
	set_begin(begin);
	set_expiration(expiration);
	set_program((unsigned char)program);
	set_testkey((unsigned char)testkey);
	set_crc(create_crc());
	create_key();
}

bool LicenseGenerator::get_crcok() const
{
	return  mCrc == mKeyCrc;
}

unsigned int LicenseGenerator::create_crc() const
{
	crc crc32;
	crc32.addData((unsigned char*)mHdkennung.c_str(), (int)mHdkennung.length());
	crc32.addData((int)mBegin);
	crc32.addData((int)mExpiration);
	crc32.addData(mProgram);
	crc32.addData(mTestkey);
	return crc32.get_crc();
}

void LicenseGenerator::decode_key()
{
	/** 
	Erzeugt aus dem eingegebenen keystring die Daten
	*/
	mHdkennung = "";
	mKeyCrc = 0;
	if (get_key().length() != 36)
	{
		// damit get_crcok auch wirklich false liefert
		mCrc = 1;
		return;
	}
	int key_offset = 0, dez = 0;
	unsigned int crcbit = 0;

	// HD Kennung extrahieren
	for (int i = 24; i > 16; i--)
	{
		dez = get_partkey(key_offset);
        mHdkennung += (char)(dez & 0x7f); // Oberstes Bit gehört zum CRC;
		crcbit = dez & 0x80;
		crcbit <<= i;
		mKeyCrc |= crcbit;
	}
	// Programmbyte
	dez = get_partkey(key_offset);
	set_program(dez & 0x0f);
	crcbit = dez & 0xf0;
	mKeyCrc |= crcbit << 16;


	// Programmbyte
	dez = get_partkey(key_offset);
	set_testkey(dez & 0x0f);
	crcbit = dez & 0xf0;
	mKeyCrc |= crcbit << 12;

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
	mKeyCrc |= crcbit >> 16;
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
	mKeyCrc |= crcbit >> 24;
	set_begin(begin & 0x00ffffff);
	set_crc(create_crc());
}

int LicenseGenerator::get_partkey(int& offset) const
{
	char hex[3];
	memset(hex, 0, 3);
	hex[0] = mKey[offset++];
	hex[1] = mKey[offset++];
	int dez;
	sscanf(hex, "%x", &dez);
	return dez;
}

void LicenseGenerator::create_key()
{
    /** 
	Erzeugt aus dem eingegebenen keystring die Daten
	*/
	string hd = get_hdkennung();
	assert(hd.length() == 8);

	mKey = "";

	// Die ersten 8 Byte enthalten jeweils als erstes Bit, das erste Byte des CRC
	unsigned int bitpattern = 0x80000000;

	for (int i = 0; i < 8; i++)
	{
		hd[i] |= mCrc & bitpattern ? 0x80  : 0x00;
		bitpattern >>= 1;
		append_hexkey((unsigned char)hd[i]);
	}
	unsigned char prog = mProgram;
	unsigned int bits = mCrc & 0xf00000;

	// 16 Bits nach rechts, damit sie die oberen 4 Bit bilden
	prog |= (unsigned char)(bits >> 16);
	append_hexkey(prog);

	unsigned char test = mTestkey;
	bits = mCrc & 0xf0000;
    // 16 Bits nach rechts, damit sie die unteren 4 Bit bilden
	test |= (unsigned char)(bits >> 12);
	append_hexkey(test);

	// Bleiben noch 16 Bit übrig für die Datumsangaben
	bits = mCrc & 0xff00;
	unsigned int expire = mExpiration;
	expire |= bits << 16;
	append_hexkey((unsigned char)(expire >> 24));
	append_hexkey((unsigned char)((expire >> 16) & 0xff));
	append_hexkey((unsigned char)((expire >> 8) & 0xff));
	append_hexkey((unsigned char)(expire & 0xff));

	// Bleiben noch 8 Bit übrig für das zweite Datum
	bits = mCrc & 0xff;
	unsigned int begin = mBegin;
	begin |= bits << 24;
	append_hexkey((unsigned char)(begin >> 24));
	append_hexkey((unsigned char)((begin >> 16) & 0xff));
	append_hexkey((unsigned char)((begin >> 8) & 0xff));
	append_hexkey((unsigned char)(begin & 0xff));
}

void LicenseGenerator::append_hexkey(unsigned char val)
{
	char tmpkey[3];
	memset(tmpkey, 0, 3);
	sprintf(tmpkey,"%.2x", (unsigned char)val);
	mKey += tmpkey;
}