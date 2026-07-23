#include "stdafx.h"


#include "GpAnlage.h"

GpAnlage::GpAnlage(MxCObList *auftraege)
{
	this->auftraege = auftraege;
}

GpAnlage::~GpAnlage(void)
{
}

MxCObList *GpAnlage::get_auftraege()
{
	return auftraege;
}

