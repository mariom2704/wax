#pragma once
#include "afx.h"
#include "stdafx.h"
#include "MxError.h"


/**
Basisklasse für Entityklassen der Anwendungsschicht
*/
class  MxBase :
	public CObject
{
public:
	MxBase(void);
	virtual ~MxBase(void);


	bool copy(const MxBase &other); ///kopiert übergebenes Objekt

	//virtuelle Funktionen:
	virtual MxBase *deepCopy()const;		///Gibt eine neues mit new erzeugtes Objekt zurück


	//Errorbehandlung
	void appendError(const MxError &error);
	void appendError(const CObArray &errorList);
	void appendErrorId(const CString &id,MxError::ErrorTyp typ = MxError::Critical);
	void appendErrorDescription(const CString &error,MxError::ErrorTyp typ = MxError::Critical);

	void clearErrorList();

	const CObArray& getErrorListe()const;
	

private:
	CObArray m_errorListe;
};
