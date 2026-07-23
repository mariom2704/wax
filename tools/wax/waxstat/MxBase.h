#pragma once
#include <QtCore>
#include "EnError.h"


/**
Basisklasse für alle weiteren Klassen.

Beinhaltet die Errorverwaltung
*/
class MxBase
{
public:
	MxBase(void);
	virtual ~MxBase(void);

	void appendError(const EnError &error);
	void appendError(const QList<EnError> &errorList);
	void appendErrorId(const QString &id,EnError::ErrorTyp typ = EnError::Critical);
	void appendErrorDescription(const QString &error,EnError::ErrorTyp typ = EnError::Critical);

	void clearErrorList();

	const QList<EnError> getErrorList()const;
	

private:
	QList<EnError> m_errorListe;
};
