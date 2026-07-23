#pragma once
#include "EnDatenbankParameter.h"
#include "MxBase.h"

/**
Basisklasse für die Control Klassen der Anwendungsschicht. Diese dienen als Schnittstelle
zur Präsentationsschicht
*/
class CcBase : public MxBase
{
public:
	CcBase(void);
	virtual ~CcBase(void);

	void setDatenbankParameter(const EnDatenbankParameter &dbParameter);

	const EnDatenbankParameter getDatenbankParameter()const;

private:
	EnDatenbankParameter m_dbParameter;

	

};
