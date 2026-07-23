#include ".\DbArbeitsfolgenSql.h"
#include <QtSql>
#include <QtGui>
#include "EnWaxdate.h"
#include "DbSqlBase.h"

DbArbeitsfolgenSql::DbArbeitsfolgenSql(void)
{
	//Standard Tabellenname zuweisen
	m_sqlDb.setTabellenname("arbeitsfolgen");
}

DbArbeitsfolgenSql::~DbArbeitsfolgenSql(void)
{
}

/**
Zuweisen der Datenbankparameter:
DBTyp, Datenbankname, User, Passwort
*/
void DbArbeitsfolgenSql::setDatenbankParameter(const EnDatenbankParameter &datenbankParameter)
{
	m_datenbankParameter = datenbankParameter;

	//Datenbankparameter zuweisen
 	m_sqlDb.setDatenbankParameter(m_datenbankParameter);

}


/**
Select Statement erzeugen.
*/
const QString DbArbeitsfolgenSql::getSelectSqlStatement()const
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



	//Sql Statement erzeugen
	sql = "SELECT r.dnvl,a.id,a.txt,Sum(a.anzahl) "
		  "FROM waxserver.werkauftrag w, "
		  "waxserver.arbeit a, "
		  "waxserver.rohrleitung r, "
		  "waxserver.status s "
		  "where w.waid=r.waid "
		  "and   w.waid=a.waid "
		  "and   w.waid=s.waid "
		  "and   w.classid='GewinderohrData' "
		  "and   s.storniert=0 ";
		  
		  
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

	
	//GROUP BY an SQL Statement anhängen
	sql += "group by r.dnvl,a.txt order by r.dnvl,a.txt";


	return sql;
}




/**
Arbeitsfolgen aus einer SQL fähigen Datenbank lesen

\param liste Liste mit Arbeitsfolgen sortiert nach DN-Wert und Bezeichnung
*/
bool DbArbeitsfolgenSql::readFromDb(QList<EnArbeitsfolge> &liste)
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
	EnArbeitsfolge arbeitsfolge;
	while (query.next()) {
		arbeitsfolge.setDn(query.value(0).toInt());
		arbeitsfolge.setNr(query.value(1).toInt());
		arbeitsfolge.setText(query.value(2).toString());
		arbeitsfolge.setAnzahl(query.value(3).toInt());
		
		liste.append(arbeitsfolge);
		//qDebug() << name << salary;
    }

	m_sqlDb.closeDb();

	return true;
}

//Werte in Db (Über)Schreiben
bool DbArbeitsfolgenSql::writeToDb(const QList<EnArbeitsfolge> &liste)
{	
	QString sqlTemp,
			sql,
			error;

	QSqlQuery query;



	//INSERT INTO Statement holen. (Spaltennamen müssen gesetzt sein);
	sql = m_sqlDb.getInsertIntoStatement();

	if (sql.isEmpty())
	{
		appendError(m_sqlDb.getErrorList());
		appendErrorId("DbArbeitsfolgenSql1"); //Das InsertInto SQl Statement konnte nicht erzeugt werden
		return false;
	}


	//Arbeitsfolgen in die Exceldatei schreiben
	EnArbeitsfolge arbeitsfolge;
	for (int i = 0; i < liste.size(); ++i) 
	{
		arbeitsfolge = liste.at(i);

		//Werte in Sql Statement einfügen
		sqlTemp = sql.arg(arbeitsfolge.getDn())
					 .arg(arbeitsfolge.getText())
			         .arg(arbeitsfolge.getAnzahl());

		//Sql Statement ausführen
		if (!m_sqlDb.executeSql(sqlTemp,query))
		{
			appendError(m_sqlDb.getErrorList());
			appendErrorId("DbArbeitsfolgenSql2"); //Das Sql Statement wurde nicht ausgeführt
			m_sqlDb.closeDb();
			return false;
		}
		
	}
	m_sqlDb.closeDb();

	return true;
}

//Werte in Db Updaten
bool DbArbeitsfolgenSql::updateInDb(const QList<EnArbeitsfolge> &liste)
{
	return false;
}



/**
Die Arbeitsfolgenliste (Dn,Af-Bez,Anz) in eine Matrix (Anz <-> DN1,DN2,...) schreiben
in der die Anzahl der Arbeitsfolgen zu den einzelnen DN Werten stehen
*/
bool DbArbeitsfolgenSql::writeToDbAsMatrix(const QList<EnArbeitsfolge> &liste)
{
	QString			sqlTemp,
					sql,
					error,
					afName,
					afName2;

	int				afNr = 0;

	QList<EnStringPair> spalten;

	QList<int>		anzahlProDn;

	QList<EnArbeitsfolge>	arbeitsfolgenListe;

	EnArbeitsfolge	arbeitsfolge;

	QSqlQuery query;




	//Spalten ermitteln
	spalten = m_sqlDb.getSpalten();

	//Liste Prüfen. 1.Spalte sind die Arbeitsfolgen, damrum muß die Liste min. 2
	if (spalten.size() < 2)
		return false;


	//Liste mit allen vorkommenden Arbeitsfolgen ermitteln
	bool found;
	for (int i = 0; i < liste.size(); ++i) 
	{
		arbeitsfolge = liste.at(i);
		
		found = false;
		//Prüfen ob Arbeitsfolge schon in der Liste steht
		for (int iAf=0; iAf < arbeitsfolgenListe.size(); ++iAf)
		{
			if (arbeitsfolge.getText() == arbeitsfolgenListe.at(iAf).getText())
			{
				found = true;
				break;
			}
		}

		//Wenn die Arbeitsfolge noch nicht in der Liste steht, diese anhängen
		if (!found)
		{
			arbeitsfolgenListe.append(arbeitsfolge);
		}
	}



	//INSERT INTO Statement holen
	sql = m_sqlDb.getInsertIntoStatement();

	if (sql.isEmpty())
	{
		appendError(m_sqlDb.getErrorList());
		appendErrorId("DbArbeitsfolgenSql1"); //Das InsertInto SQl Statement konnte nicht erzeugt werden
		return false;
	}


	int dnWert = 0;
	int anzahl = 0;

	//für jeden DN Wert 

	//zu jeder Arbeitsfolge ermitteln wie oft sie pro DN vorkommt
	for (int iAf=0; iAf < arbeitsfolgenListe.size(); ++iAf)
	{
		//Name der Arbeitsfolge
		afName = arbeitsfolgenListe.at(iAf).getText();
		afNr   = arbeitsfolgenListe.at(iAf).getNr();
				
		anzahl = 0;

		//Arbeitsfolgenliste durchlaufen und die Anzahl speichern wie oft die Arbeitsfolge (afName) vorkommt 		
		arbeitsfolge = liste.at(0);
		dnWert = arbeitsfolge.getDn();
		found = false;
		
		for (int iDn = 0; iDn < liste.size(); ++iDn) 
		{
			arbeitsfolge = liste.at(iDn);

			if (arbeitsfolge.getDn() != dnWert)
			{
				//in der Arbeitsfolgenliste beginnt ein neuer DN Wert, für den alten Dn Wert die Anzahl 
				//speichern und Werte zurücksetzen
				anzahlProDn.append(anzahl);

				//Werte zurücksetzen
				anzahl = 0;
				found = false;

				dnWert = arbeitsfolge.getDn();
			}


			afName2 = arbeitsfolge.getText();
			
			if (!found)
			{
				//Prüfen ob Arbeitsfolge aus Lister der Arbeitsfolge aus übergeordneter Schleife entspricht
				if (afName2 == afName)
				{
					//Anzahl der Arbeitsfolge speichern
					anzahl = arbeitsfolge.getAnzahl();
					found = true;
				}
			}
		}
		//Für den letzten DN Wert in der Liste die Anzahl speichern
		anzahlProDn.append(anzahl);
		
		
		//Für die ermittelten Mengen pro Dn den Datensatz schreiben

		//SqlStatement verfolständigen
		//Arbeitsfolgennr u. -name in Sql Statement einfügen
		sqlTemp = sql.arg(afNr).arg(afName);

		//Anzahl pro DN in Sql Statement einfügen
		for (iDn = 0; iDn < anzahlProDn.size(); iDn++)
		{
			sqlTemp = sqlTemp.arg(anzahlProDn.at(iDn));
		}

		//Liste mit Anzahl wieder zurücksetzen
		anzahlProDn.clear();

		
		//Arbeitsfolgen in die Exceldatei schreiben
		if (!m_sqlDb.executeSql(sqlTemp,query))
		{
			appendError(m_sqlDb.getErrorList());
			appendErrorId("DbArbeitsfolgenSql2");
			m_sqlDb.closeDb();
			return false;
		}
	}

	m_sqlDb.closeDb();

	return true;
}