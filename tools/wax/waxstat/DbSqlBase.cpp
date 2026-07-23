#include ".\dbsqlbase.h"
#include <QtGui>

DbSqlBase::DbSqlBase(void)
{
}

DbSqlBase::~DbSqlBase(void)
{
}




void DbSqlBase::setDatenbankParameter(EnDatenbankParameter param)
{
	m_datenbankParameter = param;
}


/**
Zuweisen der Tabellenspalten
*/
void DbSqlBase::setSpalten(const QList<EnStringPair> &spalten)
{
	m_spalten = spalten;
}


void DbSqlBase::setTabellenname(const QString &tabName)
{
	m_tabellenName = tabName;
}



const QString DbSqlBase::getTabellenname()const
{
	return m_tabellenName;
}

const QList<EnStringPair> DbSqlBase::getSpalten()const
{
	return m_spalten;
}

/**
Datenbank initialisieren.

Es müssen dazu über setDatenbankParameter() die Parameter vorher gesetzt werden.
*/
bool DbSqlBase::validDb()
{
	QString con,
		    error;


	if (!m_Database.isValid())
	{
		EnDatenbankParameter::EnumDbTyp dbTyp = m_datenbankParameter.getDbTyp();

		//Datenbank je nach Typ initialisieren
		switch (dbTyp)
		{
		case EnDatenbankParameter::MYSQL:

			m_Database = QSqlDatabase::addDatabase("QODBC");


			con = QString("DRIVER={MySQL ODBC 3.51 Driver};"
						  "SERVER=%1;"
						  "DATABASE=%2;"
						  "USER=%3;"
						  "PASSWORD=%4;"
						  "OPTION=265219;STMT=;")
						  .arg(m_datenbankParameter.getServer())
						  .arg(m_datenbankParameter.getDatabaseName())
						  .arg(m_datenbankParameter.getUser())
						  .arg(m_datenbankParameter.getPasswort());

			m_Database.setDatabaseName(con);

			break;

		case EnDatenbankParameter::EXCEL:

			m_Database = QSqlDatabase::addDatabase("QODBC");
			con = "DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=" + m_datenbankParameter.getDatabaseName() + ";DBQ=" + m_datenbankParameter.getDatabaseName();
			m_Database.setDatabaseName(con);
			break;

		case EnDatenbankParameter::ACCSESS:

			m_Database = QSqlDatabase::addDatabase("QODBC");
			con = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + m_datenbankParameter.getDatabaseName();
			m_Database.setDatabaseName(con);
						
			break;
		default:
			appendErrorId("DbSqlBase1"); //Unbekannter Datenbanktyp
			return false;
		}
	}

	return true;
}



bool DbSqlBase::openDb()
{
	QString error;

	//Die Datenbank initialisiert sein
	
	if (!validDb())
		return false;
	
		
	if (!m_Database.isOpen())
	{
		if (!m_Database.open())
		{
				if (m_Database.lastError().isValid())
					appendErrorDescription(m_Database.lastError().text());
				else
					appendErrorId("DbSqlBase2"); //Die Datenbank kann nicht geöffnet werden

				return false;
		}
	}

	return true;
}


///Datenbank schließen
bool DbSqlBase::closeDb()
{
	if (m_Database.isOpen())
	{
		m_Database.close();
	}

	return true;
}


/**
Sql Statement ausführen. Die Datenbank wird dabei automatisch geöffnet.

\return Es wird ein Zeiger auf das QSqlQuery Objekt zurückgegeben, wenn das Sql Statement nicht
        ausgeführt werden konnte, wird NULL zurückgegeben.
*/
bool DbSqlBase::executeSql(const QString &sql,QSqlQuery &sqlQuery)
{

	//Wenn die datenbank noch nicht geöffnet war dies öffnen
	if (!openDb())
		return false;


	QSqlQuery query(m_Database);

	//Sql Statement ausführen
	if (!query.exec(sql))
	{
		if (query.lastError().isValid())
		{
			QString st = query.lastError().text();
			appendErrorDescription(query.lastError().text());
		}
		else
			appendErrorId("DbSqlBase3"); //Das SQL Statement kann nicht ausgeführt werden:

		return false;
	}

	//Query zuweisen
	sqlQuery = query;

	return true;
}

/**
Löscht die in den Membervariablen eingestellte Tabelle. Diese wird mit setTabellenname() gesetzt
*/
 bool DbSqlBase::deleteTable()
 {
	QString sql;

	if (m_tabellenName.isEmpty())
	{
		appendErrorId("DbSqlBase4"); //Tabellenname nicht definiert
		 return false;
	}

	//Sql Statement
	sql = QString("DROP TABLE `%1`").arg(m_tabellenName);

	//Sql Statement ausrühren
	QSqlQuery query;
	if (!executeSql(sql,query))
	{
		if (query.lastError().isValid())
			appendErrorDescription(query.lastError().text());
		else
			appendErrorId("DbSqlBase3"); //Das SQL Statement kann nicht ausgeführt werden.

		return false;
	}
	return true;
 }



/**
Erstellt eine Tabelle in der Datenbank.
Der Tabellenname muß gesetzt sein. (Über setTabellenname())
*/
bool DbSqlBase::createTable()
{
	QString			stSql,
					stTabelle;
			
	EnStringPair	spalte;

	QList<EnStringPair> spalten;


	//Prüfen ob Tabellenname und Spalten gesetzt sind
	stTabelle = getTabellenname();
	if (stTabelle.isEmpty())
	{
		appendErrorId("DbSqlBase4"); //Tabellenname nicht definiert
		return false;
	}


	spalten = getSpalten();
	if (spalten.size() <= 0)
	{
		appendErrorId("DbSqlBase5"); //Keine Spalten definiert
		return false;
	}
		

	//Sql Statement erstellen
	stSql = QString("CREATE TABLE `%1` (").arg(stTabelle);

	//Tabellenfelder an Statement anhängen
	for (int i = 0; i < spalten.size(); ++i) 
	{
   		if (i>0)
			stSql += ",";

		spalte = spalten.at(i);

		stSql += QString("`%1` %2").arg(spalte.getString1()).arg(spalte.getString2());
    }

	stSql += ")";

	
	//Sql Statement ausrühren
	QSqlQuery query;
	if (!executeSql(stSql,query))
	{
		if (query.lastError().isValid())
			appendErrorDescription(query.lastError().text());
		else
			appendErrorId("DbSqlBase3"); //Das SQL Statement kann nicht ausgeführt werden.

		return false;
	}

	return true;
}

/**
Generiert den INSERT INTO String aus Tabellenname und Spaltennamen 
mit Platzhaltern (%1, %2,...) für die Values
*/
QString DbSqlBase::getInsertIntoStatement()
{
	QString			stSql,
					stTabelle;
			
	EnStringPair	spalte;

	QList<EnStringPair> spalten;



	//Prüfen ob Tabellenname und Spalten gesetzt sind
	stTabelle = getTabellenname();
	if (stTabelle.isEmpty())
	{
		appendErrorId("DbSqlBase4"); //Tabellenname nicht definiert
		return "";
	}


	spalten = getSpalten();
	if (spalten.size() <= 0)
	{
		appendErrorId("DbSqlBase5"); //Keine Tabellenspalten definiert		
		return "";
	}
		

	//Sql Statement erstellen
	stSql = QString("INSERT INTO `%1` (").arg(stTabelle);

	//Tabellenfelder an Statement anhängen
	for (int i = 0; i < spalten.size(); ++i) 
	{
   		if (i>0)
			stSql += ",";

		spalte = spalten.at(i);

		stSql += QString("`%1`").arg(spalte.getString1());
    }

	stSql += ") VALUES (";

	//Platzhalter an Statement hängen
	for (int i = 0; i < spalten.size(); ++i) 
	{
   		if (i>0)
			stSql += ",";

		stSql += QString("'%%1'").arg(i+1);
    }

	stSql += ")";


	return stSql;
}
