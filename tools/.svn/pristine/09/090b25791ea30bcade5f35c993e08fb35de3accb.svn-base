#include "stdafx.h"
#include "MxBase.h"

MxBase::MxBase(void)
{
}

MxBase::~MxBase(void)
{
	clearErrorList();
}

/**
Alle Attribute von other kopieren
*/
bool MxBase::copy(const MxBase &other)
{
	//Hier attribute kopieren
	clearErrorList();
	appendError(other.m_errorListe);

	return true;
}

/**
Gibt eine neues mit new erzeugtes Objekt zurück
*/
MxBase *MxBase::deepCopy()const
{
	MxBase *pNew = new MxBase();

	pNew->copy(*this);

	return pNew;
}




void MxBase::appendError(const MxError &error)
{
	m_errorListe.Add(error.deepCopy());
}


void MxBase::appendError(const CObArray &errorList)
{
	//Objekte mit deepCopy kopieren
	for (int i=0; i<errorList.GetSize(); i++)
	{
		const MxError* pError = (const MxError*)errorList.GetAt(i);
		m_errorListe.Add(pError->deepCopy());
	}
}


void MxBase::appendErrorId(const CString &id,MxError::ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	MxError error;

	error.setErrorId(id,typ);
	appendError(error);
}



void MxBase::appendErrorDescription(const CString &error,MxError::ErrorTyp typ /*= ErrorTyp::Critical*/)
{
	MxError fehler;

	fehler.setErrorDescription(error,typ);
	appendError(fehler);
}




/**
Speicher der Errorliste wieder freigeben
*/
void MxBase::clearErrorList()
{
	for (int i=0; i<m_errorListe.GetSize(); i++)
	{
		delete m_errorListe.GetAt(i);
	}

	m_errorListe.RemoveAll();
}

const CObArray& MxBase::getErrorListe()const
{
	return m_errorListe;
}
	