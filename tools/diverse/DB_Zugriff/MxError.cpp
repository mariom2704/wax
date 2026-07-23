#include "stdafx.h"
#include ".\MxError.h"

MxError::MxError(void)
{
}

MxError::~MxError(void)
{
}

/**
Alle Attribute von other kopieren
*/
bool MxError::copy(const MxError &other)
{
	//Hier attribute kopieren
	m_id  = other.m_id;
	m_typ = other.m_typ;
	m_bezeichnung = other.m_bezeichnung;

	return true;
}

/**
Gibt eine neues mit new erzeugtes Objekt zurück
*/
MxError *MxError::deepCopy()const
{
	MxError *pNew = new MxError();

	pNew->copy(*this);

	return pNew;
}

void MxError::setErrorId(const CString &id,ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	m_id = id;
	m_typ = typ;
	m_bezeichnung.Empty();
}
void MxError::setErrorDescription(const CString &error,ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	m_id.Empty();
	m_typ = typ;
	m_bezeichnung = error;
}

const CString MxError::getErrorDescription()const
{
	return m_bezeichnung;
}
const CString MxError::getErrorId()const
{
	return m_id;
}

const MxError::ErrorTyp MxError::getErrorTyp()const
{
	return m_typ;
}
