#include ".\CcBase.h"

CcBase::CcBase(void)
{
}

CcBase::~CcBase(void)
{
}


void CcBase::setDatenbankParameter(const EnDatenbankParameter &dbParameter)
{
	m_dbParameter = dbParameter;
}


const EnDatenbankParameter CcBase::getDatenbankParameter()const
{
	return m_dbParameter;
}