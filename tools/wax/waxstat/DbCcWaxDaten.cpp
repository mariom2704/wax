#include ".\dbccwaxdaten.h"
#include "DbArbeitsfolgenSql.h"
#include "DbArbeitsfolgenSqlExcel.h"
#include "DbMaterialSql.h"
#include "DbMaterialSqlExcel.h"


DbCcWaxDaten::DbCcWaxDaten(void)
{
	m_pDbArbeitsfolgen = NULL;
	m_pDbMaterial = NULL;
}

DbCcWaxDaten::~DbCcWaxDaten(void)
{
	if (m_pDbArbeitsfolgen)
		delete m_pDbArbeitsfolgen; 

	if (!m_pDbMaterial)
		delete m_pDbMaterial;
}

/**
Control Class für die Arbeitsfolgen. Anhand des übergebenen Datenbanktyp in 
dbParam wird das Datenbank Objekt zurückgegeben.

\param dbParam Enthält die Datenbankparameter
\return Datenbankobjekt zum Lesen und Schreiben von Arbeitsfolgen in der Datenbank
*/
DbArbeitsfolgen *DbCcWaxDaten::getDbArbeitsfolgen(EnDatenbankParameter dbParam)
{
	if (!m_pDbArbeitsfolgen)
	{
		EnDatenbankParameter::EnumDbTyp typ = dbParam.getDbTyp();

		//Wenn Objekt noch nicht existiert dieses erzeugen
		switch (typ)
		{
		case EnDatenbankParameter::EXCEL:
			m_pDbArbeitsfolgen = new DbArbeitsfolgenSqlExcel();	
			break;

		case EnDatenbankParameter::MYSQL:
		case EnDatenbankParameter::ACCSESS:
		default:
			m_pDbArbeitsfolgen = new DbArbeitsfolgenSql();	
			break;
		}

		if (!m_pDbArbeitsfolgen)
			return NULL;
		
		//Datenbank initialisieren 
		m_pDbArbeitsfolgen->setDatenbankParameter(dbParam);

	}

	return m_pDbArbeitsfolgen;
}


/**
Control Class für die Material. Anhand des übergebenen Datenbanktyp in 
dbParam wird das Datenbank Objekt zurückgegeben.

\param dbParam Enthält die Datenbankparameter
\return Datenbankobjekt zum Lesen und Schreiben von Material in der Datenbank
*/
DbMaterial *DbCcWaxDaten::getDbMaterial(const EnDatenbankParameter &dbParam)
{
	if (!m_pDbMaterial)
	{
		EnDatenbankParameter::EnumDbTyp typ = dbParam.getDbTyp();

		//Wenn Objekt noch nicht existiert dieses erzeugen
		switch (typ)
		{
		case EnDatenbankParameter::EXCEL:
			m_pDbMaterial = new DbMaterialSqlExcel();	
			break;

		case EnDatenbankParameter::MYSQL:
		case EnDatenbankParameter::ACCSESS:
		default:
			m_pDbMaterial = new DbMaterialSql();	
			break;
		}
		
		//Datenbank initialisieren 
		m_pDbMaterial->setDatenbankParameter(dbParam);

		if (!m_pDbMaterial)
			return NULL;
	}



	return m_pDbMaterial;
}