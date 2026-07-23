#include "stdafx.h"


		#include "Preisanfrage.h"



Preisanfrage::Preisanfrage(void)
{
	reset();
}

Preisanfrage::~Preisanfrage(void)
{
}


void Preisanfrage::reset()
{
	preisanfrageNr = 0;
	preisanfragePos = 0;
	preis = 0;
	info = "";

	int frist = 14;
	COleDateTimeSpan diff(frist, 0, 0, 0);	
	gueltigBis = gueltigBis.get_current_date();
	gueltigBis += diff;
}


//Serialisierung
void Preisanfrage::serialize(BinaryArchive &ar) 
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	if (ar.is_storing())
	{
		ar << preisanfrageNr;
		ar << preisanfragePos;
		ar << preis;
		ar << (COleDateTime&)gueltigBis;
		ar << info;
	}
	else 
	{
		ar >> preisanfrageNr;
		ar >> preisanfragePos;
		ar >> preis;
		ar >> (COleDateTime&)gueltigBis;

		if (version >= 51)
			ar >> info;
	}

}

void Preisanfrage::copy(const Preisanfrage& pa)
{
	preisanfrageNr = pa.preisanfrageNr;
	preisanfragePos = pa.preisanfragePos;
	preis = pa.preis;
	gueltigBis = pa.gueltigBis;
	info = pa.info;
}
