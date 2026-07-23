#pragma once

#include "StammDat.h"

/**
Dateninhalte der Tabelle rohrstutzen_brennzugabe
*/
class WAXSTAMMDLL RohrstutzenBrennzugabe : public StammDat  
{
public:
	RohrstutzenBrennzugabe();

	virtual void load(const std::string& db);
		
	class data : public CObject  
	{
	public:
		data();

		int get_verteilerdn() const { return VerteilerDN; }
		int get_stutzendn() const { return StutzenDN; }
		int get_brennzugabe() const { return Brennzugabe; }

	private:
		friend class RohrstutzenBrennzugabe;

		int VerteilerDN;

		int StutzenDN;

		int Brennzugabe;
	};
};
