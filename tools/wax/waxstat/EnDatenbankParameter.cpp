#include ".\endatenbankparameter.h"

EnDatenbankParameter::EnDatenbankParameter(void)
{
}

EnDatenbankParameter::~EnDatenbankParameter(void)
{
}


/**
Datenbankwerte mit den Standardparametern setzen. Diese könnten z.B. auch aus 
einer Datei gelesen werden.
*/
bool EnDatenbankParameter::initStandardMySqlParameter()
{
	QString server,
		    iniDatei = "WaxStat.ini";

	

	//Servernamen aus der WaxStat.ini lesen (in der 1. Zeile steht der Server)
	//(dies sollte am besten in eine XML Datei verlagert werden)
	QFile file(iniDatei);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		appendErrorId("EnDatenbankParameter1"); //Die WaxStat.ini konnte nicht geöffnet werden
		return false;
	}

    QTextStream in(&file);
    if  (!in.atEnd()) 
	{
		//in der 1. Zeile steht der Server
        server = in.readLine();
	}

	
	if (server.isEmpty())
	{
		appendErrorId("EnDatenbankParameter2"); //Der Servername konnte aus der WaxStat.ini nicht gelesen werden
		return false;
	}
	


	//Parameter für MySql Db setzen
	setServer(server);
	setDatabaseName("waxserver");  
	setUser("wax+");

	QString stTemp;
	stTemp.append("r").append("o").append("h").append("r").append("1");
	setPasswort(stTemp);
	
	setDbtyp(EnDatenbankParameter::MYSQL);


	return true;
}




void EnDatenbankParameter::setServer(const QString &server)
{
	m_databaseServer = server;
}


/**Name der Datenbank 
	   - Bei MySql Odbc	= Datenquellenname 
	   - Bei Accsess		= Pfad + Dateiname der mdb-Datei
	   - Bei Excel		= Pfad + Dateiname der xls-Datei
*/
void EnDatenbankParameter::setDatabaseName(const QString &dbName)
{
	m_databaseName = dbName;
}
void EnDatenbankParameter::setPasswort(const QString &pwd)
{
	m_passwort = pwd;
}
void EnDatenbankParameter::setUser(const QString &uid)
{
	m_user = uid;
}

void EnDatenbankParameter::setDbtyp(EnumDbTyp typ)
{
	m_dbTyp = typ;
}


//get Methoden
const QString EnDatenbankParameter::getDatabaseName()const
{
	return m_databaseName;
}
	
const QString EnDatenbankParameter::getPasswort()const
{
	return m_passwort;
}
	
const QString EnDatenbankParameter::getUser()const
{
	return m_user;
}


const EnDatenbankParameter::EnumDbTyp EnDatenbankParameter::getDbTyp()const
{
	return m_dbTyp;
}

const QString EnDatenbankParameter::getServer()const
{
	return m_databaseServer;
}


