#pragma once
#include <QtSql>
#include "MxBase.h"
#include "EnDatenbankParameter.h"
#include "EnStringPair.h"

/**
Basisklasse für SQL Datenbanken. Über diese Klasse wird der Zugriff auf die 
Datenbank realisiert.
Über setDatenbankParameter(..) müssen zuerst die Datenbankparameter gesetzt werden.
Danach können Datenbank Zugriffsfunktionen ausgeführt werden. Die Datenbank wird dabei
automatisch geöffnet.
Über closeDb() muß die Datenbank wieder geschlossen werden.
*/
class DbSqlBase : public MxBase
{
public:
	DbSqlBase(void);
	virtual ~DbSqlBase(void);
	
	
	//set Methoden
	void setDatenbankParameter(EnDatenbankParameter param);
	void setSqlSelectStatement(const QString &sql);
	void setSpalten(const QList<EnStringPair> &felder);		///Spaltennamen der Tabelle definiern
	void setTabellenname(const QString &tabName);			///Tabellenname bzw. Dateiname setzen


	//get Methoden
	const QString getTabellenname()const;			///Tabellenname bzw. Dateiname
	const QList<EnStringPair> getSpalten()const;	///Spaltendefinitionen holen

	///Sql Statement ausführen (die Datenbank wird automatisch geöffnet)
	bool executeSql(const QString &sql,QSqlQuery &sqlQuery);


	/**
	Generiert den INSERT INTO String aus Tabellenname und Spaltennamen 
	mit Platzhaltern (%1, %2,...) für die Values.
	Die Spalten müssen über setSpalten(..) gesetzt sein.
	*/
	QString getInsertIntoStatement();


	///Tabelle löschen (die Datenbank wird automatisch geöffnet)
	virtual bool deleteTable();

	///Tabelle erstellen (die Datenbank wird automatisch geöffnet)
	virtual bool createTable();
	
	///Datenbank öffnen
	virtual bool openDb();

	///Datenbank schließen
	virtual bool closeDb();

protected:
	EnDatenbankParameter m_datenbankParameter;


	///Datenbank initialisieren. DB Parameter werden zugewiesen
	bool validDb();

private:
	QSqlDatabase	m_Database;

	QList<EnStringPair> m_spalten; //Tabellen spalten

	QString			m_tabellenName; //Name der Tabelle

	QString			m_selectStatement,
					m_insertIntoStatement,
					m_updateStatement;
					

};
