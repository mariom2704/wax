#include "stdafx.h"



#include "gpbase.h"

GpBase::GpBase(void)
{
	m_pFortschritt = NULL;
}

GpBase::~GpBase(void)
{
}


void GpBase::set_Fortschritt(Fortschrittanzeige* pFortschritt)
{
	m_pFortschritt = pFortschritt;
}


Fortschrittanzeige* GpBase::get_Fortschritt()
{
	return m_pFortschritt;
}
