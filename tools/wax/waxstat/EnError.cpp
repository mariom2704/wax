#include ".\enerror.h"

EnError::EnError(void)
{
}

EnError::~EnError(void)
{
}


void EnError::setErrorId(const QString &id,ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	m_id = id;
	m_typ = typ;
	m_bezeichnung.clear();
}
void EnError::setErrorDescription(const QString &error,ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	m_id.clear();
	m_typ = typ;
	m_bezeichnung = error;
}

const QString EnError::getErrorDescription()const
{
	return m_bezeichnung;
}
const QString EnError::getErrorId()const
{
	return m_id;
}

const EnError::ErrorTyp EnError::getErrorTyp()const
{
	return m_typ;
}
