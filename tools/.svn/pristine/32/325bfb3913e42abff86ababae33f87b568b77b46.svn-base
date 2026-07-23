#include ".\mxbase.h"

MxBase::MxBase(void)
{
}

MxBase::~MxBase(void)
{
}


void MxBase::appendError(const EnError &error)
{
	m_errorListe.append(error);
}


void MxBase::appendError(const QList<EnError> &errorList)
{
	m_errorListe += errorList;
}


void MxBase::appendErrorId(const QString &id,EnError::ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	EnError error;

	error.setErrorId(id,typ);
	appendError(error);
}



void MxBase::appendErrorDescription(const QString &error,EnError::ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	EnError fehler;

	fehler.setErrorDescription(error,typ);
	appendError(fehler);
}





void MxBase::clearErrorList()
{
	m_errorListe.clear();
}

const QList<EnError> MxBase::getErrorList()const
{
	return m_errorListe;
}
	
