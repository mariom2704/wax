// Copyright (C) 2001 by Minimax GmbH

#include "stdafx.h"
#include "crc32.h"



//Tabelle mit Konstanten, die für den CRC Algorithmus nötig sind. Erster Wert 
//wird mit 0 initialisiert, um im Konstruktor die Initialisierung zu überprüfen 
//und ggf. vorzunehmen.
unsigned int crc::crctable[256] = {0}  ;

crc::crc() : mCrc(0xffffffff)
{
	if (!crctable[0])
		init_crctable();
}

crc::~crc()
{
}

//Setzt das crc Attribut auf den Ausgangswert.
void crc::reset()
{
	mCrc = 0xffffffff;
}

void crc::init_crctable()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		crctable[i] = reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++)
			crctable[i] = (crctable[i] << 1) ^ (crctable[i] & (1 << 31) ? QUOTIENT : 0);
		crctable[i] = reflect(crctable[i], 32);
	}
}

unsigned int  crc::reflect(unsigned int ref, char ch)
{
	unsigned int value = 0;

	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for(int i = 1; i < (ch + 1); i++)
	{
		if(ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

const unsigned int crc::get_crc() const
{
	return ~mCrc;

}


void crc::addData(const CString& data)
{
	const unsigned char *buffer = (unsigned char*)(LPCTSTR)data;
	int len = data.GetLength();

	while (len--)
		update_crc(buffer++);
}

void crc::addData(int data)
{
	const unsigned char *buffer = (unsigned char*)&data;
	int len = sizeof(int);

	while (len--)
		update_crc(buffer++);
}

void crc::addData(double data)
{

	//CString val;
	//val.Format("%.2f", data);
	char val[255];
	sprintf(val, "%.4f", data);
	//AfxMessageBox(val, MB_OK);
	addData((CString)val);

	return;

/*
	// Leider gibt es bei Doublewerten in den letzten Nachkommastellen Probleme
	// So sollte der Code aussehen
	const unsigned char *buffer = (unsigned char*)&data;
	int len = sizeof(double);

	while (len--)
		update_crc(buffer++);
*/
}

void crc::addData(bool data)
{
	const unsigned char *buffer = (unsigned char*)&data;
	int len = sizeof(bool);

	while (len--)
		update_crc(buffer++);
}




void crc::addData(const unsigned char data)
{
	const unsigned char *buffer = (unsigned char*)&data;
	update_crc(buffer);
}



void crc::addData(unsigned long data)
{
	const unsigned char *buffer = (unsigned char*)&data;
	int len = sizeof(long);

	while (len--)
		update_crc(buffer++);
}



void crc::addData(const unsigned char* data, unsigned int len)
{
	while (len--)
		update_crc(data++);
}

