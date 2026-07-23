// Copyright (C) 2001 by Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_crc_3E55FD760134_INCLUDED
#define _INC_crc_3E55FD760134_INCLUDED

//Errechung eines crc Wertes
class crc 
{
public:
	enum CONSTANTS 
	{
	
	//Dieser Wert ergibt sich aus diversen mathematischen Operationen. Wie er 
	//errechnet wird muss in CRC Doku nachgelesen werden.
		QUOTIENT = 0x04c11db7
	};


	crc();

	virtual ~crc();

	void addData(const CString& data);

	void addData(int data);

	void addData(double data);

	void addData(bool data);

	const unsigned int get_crc() const;

private:
	unsigned int mCrc;

	//Tabelle mit Konstanten, die für den CRC Algorithmus nötig sind. Erster Wert 
	//wird mit 0 initialisiert, um im Konstruktor die Initialisierung zu 
	//überprüfen und ggf. vorzunehmen.
	static unsigned int crctable[256];

	static void init_crctable();

	static unsigned int  reflect(unsigned int ref, char ch);

private:
	inline void update_crc(const unsigned char* buffer);

public:
	void addData(const unsigned char* data, unsigned int len);

	void addData(unsigned long data);

	void addData(const unsigned char data);

	//Setzt das crc Attribut auf den Ausgangswert.
	void reset();

};

inline void crc::update_crc(const unsigned char* buffer)
{
	mCrc = (mCrc >> 8) ^ crctable[(mCrc & 0xFF) ^ *buffer];

}


#endif /* _INC_crc_3E55FD760134_INCLUDED */
