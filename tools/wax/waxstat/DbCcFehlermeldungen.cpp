#include ".\dbccfehlermeldungen.h"
#include "DbFehlermeldungenSql.h"

DbCcFehlermeldungen::DbCcFehlermeldungen(void)
{
	m_pDbFehlermeldungen = NULL;
}

DbCcFehlermeldungen::~DbCcFehlermeldungen(void)
{
	delete m_pDbFehlermeldungen;
}


/**
Control Class für die Fehlermeldungen. Anhand des übergebenen Datenbanktyp in 
dbParam wird das Datenbank Objekt zurückgegeben.

\param dbParam Enthält die Datenbankparameter
\return Datenbankobjekt zum Lesen von Fehlermeldungen aus der Datenbank
*/
DbFehlermeldungen *DbCcFehlermeldungen::getDbFehlermeldungen(EnDatenbankParameter dbParam)
{
	if (!m_pDbFehlermeldungen)
	{
		EnDatenbankParameter::EnumDbTyp typ = dbParam.getDbTyp();

		//Wenn Objekt noch nicht existiert dieses erzeugen
		switch (typ)
		{
		case EnDatenbankParameter::EXCEL:
		case EnDatenbankParameter::MYSQL:
		case EnDatenbankParameter::ACCSESS:
		default:
			m_pDbFehlermeldungen = new DbFehlermeldungenSql();	
			break;
		}

		if (!m_pDbFehlermeldungen)
			return NULL;
		
		//Datenbank initialisieren 
		m_pDbFehlermeldungen->setDatenbankParameter(dbParam);

	}

	return m_pDbFehlermeldungen;
}