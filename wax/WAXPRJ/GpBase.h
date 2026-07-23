#pragma once
#include "afx.h"
#include "..\rohrkonst\WaxBase.h"
#include "Fortschrittanzeige.h"

/**
Basisklasse für Geschäftsprozesse
*/
class WAXPRJDLL GpBase :
	public WaxBase
{
public:
	GpBase(void);
	virtual ~GpBase(void);

	///es kann eine Fortschrittsanzeige übergeben werden
	void set_Fortschritt(Fortschrittanzeige* pFortschritt);

	Fortschrittanzeige* get_Fortschritt();

private:
	Fortschrittanzeige* m_pFortschritt;

};
