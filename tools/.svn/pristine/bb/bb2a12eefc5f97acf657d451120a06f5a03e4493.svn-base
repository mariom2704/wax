#pragma once
#include "enbase.h"
#include <QtCore>




/**
Entitie Klasse für die Datenbankparameter und Anmeldeparameter für die Datenbank
*/
class EnDatenbankParameter :
	public EnBase
{
public:
	EnDatenbankParameter(void);
	virtual ~EnDatenbankParameter(void);


	///Typ der Datenbank
	enum EnumDbTyp {
		MYSQL,
		ACCSESS,
		EXCEL
	};


	//set Methoden
	void setServer(const QString &server);
	void setDatabaseName(const QString &dbName);
	void setPasswort(const QString &pwd);
	void setUser(const QString &uid);
	void setDbtyp(EnumDbTyp typ);

	//get Methoden
	const QString getServer()const;
	const QString getDatabaseName()const;
	const QString getPasswort()const;
	const QString getUser()const;
	const EnDatenbankParameter::EnumDbTyp getDbTyp()const;


	//mit Standardwerten für MySql Zugriff initialisieren
	bool initStandardMySqlParameter();


private:
	QString m_databaseServer,	///Servername
			m_databaseName,		///Datenbankname
		    m_passwort,			///Passwort
			m_user;				///User

	EnumDbTyp m_dbTyp;			///Datenbanktyp
};
