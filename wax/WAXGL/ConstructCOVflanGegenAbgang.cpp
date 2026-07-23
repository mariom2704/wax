#include "stdafx.h"


#include "ConstructCOVflanGegenAbgang.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructCOGegenflanVflan::~ConstructCOGegenflanVflan()
{

}

ConstructCOGegenflanVflan::ConstructCOGegenflanVflan()
{

}

void ConstructCOGegenflanVflan::createBeschriftung()
{
	
	int iDn = elemerk.get_dn();
	text.Format( "DN%d FLANSCH", iDn);	
	if(elemerk.get_id() == GEGENFLANSCH_COII_SCHWARZ_ABGANG)
	{
		strSchwarztext = "(schwarz)";
	}
		ConstructCOIIGegenKomponent::createBeschriftung();
}



void ConstructCOGegenflanVflan::createElement()
{
	calculateDimisions();
	createFlansch();
}

