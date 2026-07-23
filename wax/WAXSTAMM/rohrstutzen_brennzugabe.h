#pragma once

#include "StammDat.h"

/**
Dateninhalte der Tabelle rohrstutzen_brennzugabe
*/
class DLLINTERFACE RohrstutzenBrennzugabe : public StammDat  
{
public:
	RohrstutzenBrennzugabe();

	virtual void load(const string& db);
	
	
	class data : public CObject  
	  
	{
	public:
		data();

	private:
		friend class RohrstutzenBrennzugabe;

		int VerteilerDN;

		int StutzenDN;

		int Brennzugabe;

	};
};
