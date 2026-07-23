#include "stdafx.h"




#include "WaxBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

WaxBase::WaxBase(void)
{
}

WaxBase::~WaxBase(void)
{
}

/**
Alle Attribute von other kopieren
*/
bool WaxBase::copy(const WaxBase &other)
{
	ErrorHandler = other.getErrorHandler();

	return true;
}

/**
Gibt eine neues mit new erzeugtes Objekt zurück
*/
WaxBase *WaxBase::deepCopy()const
{
	WaxBase *pNew = new WaxBase();

	pNew->copy(*this);

	return pNew;
}



