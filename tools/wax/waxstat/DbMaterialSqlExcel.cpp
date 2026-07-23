#include ".\dbmaterialsqlexcel.h"


/*
Änderung: 21.03.2006, Jacobs
Spalten, die Integer sind werden auch als diese definiert, da sonst ein Apostroph in dem Feld
vor der Zahl dargestellt wird
*/



DbMaterialSqlExcel::DbMaterialSqlExcel(void)
{
	//Standard Tabellenname zuweisen
	m_sqlDb.setTabellenname("Material");
}

DbMaterialSqlExcel::~DbMaterialSqlExcel(void)
{
}


/**
Werte in in der Exceldatei (Über)Schreiben
*/
bool DbMaterialSqlExcel::writeToDb(const QList<EnMaterial> &liste)
{	
	QString sqlTemp,
			sql,
			error;


	//Exceldokument erstellen bzw. Arbeitsmappe(entspricht einer Tabelle) erstellen
	if (m_datenbankParameter.getDbTyp() == EnDatenbankParameter::EXCEL)
	{
		QList<EnStringPair> spalten;

		//Alle Spalten als TEXT definieren. Ergibt beim Schreiben in die Tabelle eine bessere Formatierung
		spalten.append(EnStringPair("ArtikelNr","INT"));	//Änderung: 21.03.2006
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
	if (!DbMaterialSql::writeToDb(liste))
	{
		appendErrorId("DbMaterialSqlExcel1"); //Die Daten konnten nicht in die Exceltabelle geschrieben werden
		return false;
	}
	

	return true;
}


