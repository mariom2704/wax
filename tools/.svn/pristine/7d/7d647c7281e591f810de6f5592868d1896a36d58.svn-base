#include ".\dbmaterialsql.h"
#include "EnWaxDate.h"

/*
Änderung: 21.03.2006; Jacobs

getSelectSqlStatement(): 
Es wird nicht mehr die Tabelle stueckliste sondern element verwendet
Das SQl Statement ließt nur Material aus das kein Rohr ist. Länge wird auch nicht mehr ausgelesen

readFromDb(), writeToDb():
Es wird keine Länge mehr gelesen bzw. geschrieben

*/



DbMaterialSql::DbMaterialSql(void)
{
}

DbMaterialSql::~DbMaterialSql(void)
{
}


/**
Zuweisen der Datenbankparameter:
DBTyp, Datenbankname, User, Passwort
*/
void DbMaterialSql::setDatenbankParameter(const EnDatenbankParameter &datenbankParameter)
{
	m_datenbankParameter = datenbankParameter;

	//Datenbankparameter zuweisen
 	m_sqlDb.setDatenbankParameter(m_datenbankParameter);

}






/**
Select Statement erstellen
*/
const QString DbMaterialSql::getSelectSqlStatement()const
{
	QString sql,
    		stTemp;

	EnWaxDate startDate,
			  endeDate;

	long	  startWaxDate,
			  endWaxDate;


	//Zeitraum für die Auswertung holen
	startDate = getStartDatum();
	endeDate = getEndDatum();

	//in Wax Datum konvertieren
	startWaxDate = startDate.getWaxDate();
	endWaxDate = endeDate.getWaxDate();


	
	//Sql Statement zusammenbasteln
	sql =	"SELECT e.artikelnr, sap.materialbe, sum(w.anzahl) "
			"FROM waxserver.werkauftrag w, "
			"waxserver.status s, "
			"waxserver.element e "
			"LEFT JOIN waxserver.sapmaterialstamm sap ON e.artikelnr=sap.materialnu "
			"where w.waid=e.waid "
			"and   w.waid=s.waid "
			"and   s.storniert=0 "
			"and   w.classid='GewinderohrData' "
			"and   sap.warengrupp!='ROHRE' ";		//Änderung: 21.03.2006
	
	

	//Startdatum für Auswertung berücksichtigen
	if (startWaxDate > 0)
	{
		stTemp = QString("and   s.freigabedatum >= %1 ").arg(startWaxDate);
		sql += stTemp;
	}

	//Enddatum für Auswertung berücksichtigen
	if (endWaxDate > 0)
	{
		//Um einen Tag erhöhen, da es sich um ganze Tage handelt (Uhrzeit 0:00). Es werden sonst 
		//die Datensätze Enddatum nicht gefunden, weil Datensätze sind in Db mit Datum und Uhrzeit abgelegt
		endWaxDate += 10000;

		stTemp = QString("and   s.freigabedatum < %2 ").arg(endWaxDate);
		sql += stTemp;
	}
	
	//Nach der Artikelnummer gruppieren und sortieren
	sql += "group by e.artikelnr order by e.artikelnr ";


	return sql;
}


///Werte aus Db lesen
bool DbMaterialSql::readFromDb(QList<EnMaterial> &liste)
{
	QSqlQuery query;
  
	//Sql Statement ausführen
	if (!m_sqlDb.executeSql(getSelectSqlStatement(),query))
	{
		appendError(m_sqlDb.getErrorList());
		m_sqlDb.closeDb();
		return false;
	}


	//Datensätze durchlaufen und Werte in die Übergabeliste schreiben
	EnMaterial material;
	while (query.next()) {
		material.setArtikelnr(query.value(0).toLongLong());
		material.setBezeichnung(query.value(1).toString());
		material.setAnzahl(query.value(2).toInt());
		
		liste.append(material);
    }


	m_sqlDb.closeDb();

	return true;
}

///Werte in Db (Über)Schreiben
bool DbMaterialSql::writeToDb(const QList<EnMaterial> &liste)
{
	QString sqlTemp,
			sql,
			error;


	QSqlQuery query;

	//INSERT INTO Statement holen. (Spaltennamen müssen gesetzt sein);
	sql = m_sqlDb.getInsertIntoStatement();

	if (sql.isEmpty())
		return false;


	//Arbeitsfolgen in die Exceldatei schreiben
	EnMaterial material;
	for (int i = 0; i < liste.size(); ++i) 
	{
		material = liste.at(i);

		//Werte in Sql Statement einfügen
		sqlTemp = sql.arg(material.getArtikelNr())
					 .arg(material.getBezeichnung())
			         .arg(material.getAnzahl());


		//Sql Statement ausführen
		if (!m_sqlDb.executeSql(sqlTemp,query))
		{
			appendError(m_sqlDb.getErrorList());
			m_sqlDb.closeDb();
			return false;
		}
		
	}
	m_sqlDb.closeDb();

	return true;
}



//Werte in Db Updaten
bool DbMaterialSql::updateInDb(const QList<EnMaterial> &liste)
{
	return false;
}
