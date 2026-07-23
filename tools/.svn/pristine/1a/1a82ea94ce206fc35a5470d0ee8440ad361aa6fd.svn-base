#include ".\dbarbeitsfolgensqlexcel.h"
#include <QtGui>

/*
Änderung: 21.03.2006, Jacobs
Spalten, die Integer sind werden auch als diese definiert, da sonst ein Apostroph in dem Feld
vor der Zahl dargestellt wird
*/


DbArbeitsfolgenSqlExcel::DbArbeitsfolgenSqlExcel(void)
{
	//Standard Tabellenname zuweisen
	m_sqlDb.setTabellenname("Arbeitsfolgen");

}

DbArbeitsfolgenSqlExcel::~DbArbeitsfolgenSqlExcel(void)
{
}


//Werte in Db (Über)Schreiben
bool DbArbeitsfolgenSqlExcel::writeToDb(const QList<EnArbeitsfolge> &liste)
{	
	QString sqlTemp,
			sql,
			error;


	//Exceldokument erstellen bzw. Arbeitsmappe(entspricht einer Tabelle) erstellen
	if (m_datenbankParameter.getDbTyp() == EnDatenbankParameter::EXCEL)
	{
		QList<EnStringPair> spalten;

		spalten.append(EnStringPair("DN-Wert","TEXT"));
		spalten.append(EnStringPair("Bezeichnung","TEXT"));
		spalten.append(EnStringPair("Anzahl","INT"));		//Änderung: 21.03.2006

		//Tabellenspalten defnieren
		m_sqlDb.setSpalten(spalten);

		if (!m_sqlDb.createTable())
		{
			appendError(m_sqlDb.getErrorList());
			return false;
		}
	}


	//Werte in die Exceldatei schreiben
	if (!DbArbeitsfolgenSql::writeToDb(liste))
	{
		appendErrorId("DbArbeitsfolgenSqlExcel1"); //Die Daten konnten nicht in die Exceldatei geschrieben werden!
		return false;
	}
	

	return true;
}


/**
Die Arbeitsfolgenliste (Dn,Af-Bez,Anz) in eine Matrix (Anz <-> DN1,DN2,...) schreiben
in der die Anzahl der Arbeitsfolgen zu den einzelnen DN Werten stehen
*/
bool DbArbeitsfolgenSqlExcel::writeToDbAsMatrix(const QList<EnArbeitsfolge> &liste)
{
	QString sqlTemp,
			sql,
			error;

	EnArbeitsfolge  arbeitsfolge;


	//Exceldokument erstellen bzw. Arbeitsmappe(entspricht einer Tabelle) erstellen
	if (m_datenbankParameter.getDbTyp() == EnDatenbankParameter::EXCEL)
	{
		QList<EnStringPair> spalten;

		//Feldspalten für die Matrix setzen 
		spalten.append(EnStringPair("AF-Nr","INT"));		//Änderung: 21.03.2006
		spalten.append(EnStringPair("Arbeitsfolge","TEXT"));
		
		//Aus der Arbeitsfolgenliste die vorkommenden DN-Werte ermitteln. Für jeden DN Wert
		//wird eine Spalte angelegt
		int dnWert = 0;
		for (int i = 0; i < liste.size(); ++i) 
		{
			arbeitsfolge = liste.at(i);
			
			//Wenn in der Liste ein neuer Dn Wert vorkommt, diesen als neue Spalte definieren
			//Funktioniert, weil die Arbeitsfolgenliste nach Dn Werten sortiert ist.
			if (arbeitsfolge.getDn() != dnWert)
			{
				dnWert = arbeitsfolge.getDn();

				//DN Wert als Text defninieren. Ergibt eine bessere Formatierung
				spalten.append(EnStringPair(QString("DN %1").arg(dnWert),"INT"));  //Änderung: 21.03.2006
			}
		}

		//Spalten neu definieren
		m_sqlDb.setSpalten(spalten);


		if (!m_sqlDb.createTable())
		{
			appendError(m_sqlDb.getErrorList());
			return false;
		}
	}


	//Werte in die Exceldatei schreiben
	if (!DbArbeitsfolgenSql::writeToDbAsMatrix(liste))
	{
		appendErrorId("DbArbeitsfolgenSqlExcel1"); //Die Daten konnten nicht in die Exceldatei geschrieben werden!
		return false;
	}
	

	return true;
}


