#pragma once
#include <QtCore>

/**
Klasse zur Fehlerverwaltung

Über die set Methoden kann der Fehler entweder 
	- über eine Id bestimmt werden  (setErrorId(..))
	- oder es wird ein Fehlertext zugewiesen (setErrorDescription(..))
*/
class EnError
{
public:
	EnError(void);
	virtual ~EnError(void);

	///Fehlertypen
	enum ErrorTyp{
		Information,
		Warning,
		Critical
	};


	//Set Methoden
	void setErrorId(const QString &id,ErrorTyp typ = ErrorTyp::Critical);
	void setErrorDescription(const QString &error,ErrorTyp typ = ErrorTyp::Critical);

	const QString getErrorDescription()const;
	const QString getErrorId()const;
	const EnError::ErrorTyp getErrorTyp()const;

private:
	QString m_id;	///ID des Fehlers

	QString m_bezeichnung; ///Fehlerbezeichnung

	ErrorTyp m_typ; ///Typ des Fehlers
	
};
