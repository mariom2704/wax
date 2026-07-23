#include "stdafx.h"
//

	#include "gpconvertoldversion.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

GpConvertOldVersion::GpConvertOldVersion(void)
{}



GpConvertOldVersion::~GpConvertOldVersion(void)
{}



///alte FlaschId 10 und 11 in 57 und 58 umwandeln
int GpConvertOldVersion::getNewFlanschId(int oldId,int dn)
{

	if (oldId != 10 && oldId != 11)
		return oldId;

	if (dn <= 0)
	{
		XASSERT(dn);
		return VFL16;
	}

	int newId = oldId;

	//10 = VFLAN   VFL16
	if (oldId == 10) 
	{
		if (dn >= 32 && dn <= 150)
			newId = VFL16;
		else 
			newId = VFL10;
	}
	else if (oldId == 11)
	{
		if (dn >= 200 && dn <= 300)
			newId = VFL16;
		else 
			newId = VFL10;
	}

	return newId;
}