#include "stdafx.h"
#include "odbcinst.h" 
#include <string.h>
#include <math.h>
#include "ExcelTable.h"
#include "ExcelTable7.h"

#include <vector>

// Objects
CDatabase database;
CRecordset recset ( &database );

// Constructor
CExcelTable::CExcelTable() : fehlerzahl(0), office2010_mode(true)
{
	driverName = getDriver();
	memset( columnName, 0, sizeof(columnName) );
	
	tableQuantity = 0;
	columnQuantity = 0;
	
}

// Destructor
CExcelTable::~CExcelTable()
{
	if (fehlerzahl)
	{
		CString msg;
		msg.Format("Beim Export sind %i Fehler aufgetreten.\n\nDie Daten sind fehlerhaft!!!", fehlerzahl);
		AfxMessageBox(msg);
	}
}

bool CExcelTable::office2010() const
{
	return office2010_mode;
}

void CExcelTable::set_office2010(bool val)
{
	office2010_mode = val;
	driverName = getDriver();
}


CString CExcelTable::getDriver()
{ 
	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
 
	// Search for the driver...
	// Kann auf mehrere Versionen erweitert werden bzw. auf automatisches Finden mit Suchprio
	std::vector<std::string> searchstr;

	// Hier die Suchreihenfolge festlegen
	if (office2010_mode)
	{
		searchstr.push_back("*.xlsx");
		searchstr.push_back("(*.xls)");
	}
	else // Erst alte Officeversionen benutzen
	{
		searchstr.push_back("(*.xls)");
		searchstr.push_back("*.xlsx");
	}

  
	// Get the names of the installed drivers ("odbcinst.h" has to be included )
	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
	{
		goto NODRIVER;
	}
		
	for (std::vector<std::string>::iterator it = searchstr.begin(); it != searchstr.end(); it++)
	{
		do {
			if( strstr( pszBuf, it->c_str() ) != 0 ) {// Found !
				TRACE("ExcelTreiber: %s\n", pszBuf);
				return CString(pszBuf);
			}
			pszBuf = strchr(pszBuf, '\0') + 1;
		}
		while(pszBuf[1] != '\0');
	}

	NODRIVER:
		AfxMessageBox("Leider kein Export möglich."
			"\n\nGründe können sein:\n"
			" a) Der ODBC-Treiber für Excel ist nicht installiert\n"
			" b) ODBC fehlerhaft.", MB_ICONERROR | MB_OK);
	return("");
} 


std::string CExcelTable::get_ext() const
{
	return office2010_mode ? ".xlsb" : "xls";

}

//Setzt den Pfad und Namen der zu 
//      erzeugenden Exceldatei und den Namen der Tabelle. 
//      Muss vor CreateTable() aufgerufen werden.
int CExcelTable::SetVar(char * File, char * Table)
{  
	fileName = File;
	tableName = Table;
	return 1;
}  

 CString CExcelTable::get_columnType(int index) const
{
	 switch (columnType[index])
	 {
		case CExcelTable::TEXT: return "TEXT";
		case CExcelTable::INT: return "INT";
		case CExcelTable::DOUBLE: return "DOUBLE";
		default: 
			ASSERT(FALSE);
	 }
	 return "TEXT";
}

void CExcelTable::set_columnType(int value, int type[])
{
	if (value > 255) value = 255;
	for (int i=0; i<value; i++) 
	{
		ASSERT(type[i] == CExcelTable::TEXT || type[i] == CExcelTable::INT || type[i] == CExcelTable::DOUBLE);
		if (type[i] != CExcelTable::TEXT && type[i] != CExcelTable::INT && type[i] != CExcelTable::DOUBLE)
		{
			fehlerzahl++;
			type[i] = CExcelTable::TEXT;
		}
		columnType[i] = type[i];
	}
}

int CExcelTable::set_columnQuantity(int value)
{
	if (value > 255) 
		value = 255;
	columnQuantity = value;
	return 1;
}

// Setzt die Spaltennamen der zu erzeugenden Tabelle. 
//      Muss vor CreateTable() aufgerufen werden.
int CExcelTable::SetColumnNames(char *Names[], int Quantity)
{ 
	if (Quantity > 255) 
		Quantity = 255;
	//for (i=columnQuantity; i<Quantity; i++) {
	for (int i=0; i<Quantity; i++) 
	{
		columnName[i] = Names[i];
	}
	columnQuantity = Quantity;
	return 1;
}  

// CreateTable-Funktion
// Erzeugt Datei und Tabelle mit den Spalten, 
//      die zuvor über die Set-Funktionen gesetzt wurden.
int CExcelTable::CreateTable()
{ 
	CString sSql, str;
	int ret = 1, i;

	TRY {
		// Build the creation string for access without DSN
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", driverName, fileName, fileName);
		// Create the database (i.e. Excel sheet)
		if ( database.OpenEx(sSql,CDatabase::noOdbcDialog) ) {
			// Create table structure
			sSql.Format("CREATE TABLE %s (", tableName);
			for (i = 0; i < (columnQuantity-1); i++) {	
				str.Format("%s %s,", columnName[i], get_columnType(i));
				sSql = sSql + str;
			}
			str.Format("%s %s)", columnName[columnQuantity-1], get_columnType(columnQuantity-1)); 
			sSql = sSql + str;
			
			database.ExecuteSQL(sSql);
		}      
	} CATCH_ALL(e) 
	{
		CString msg;
		msg.Format("Konnte Datei '%s' nicht erzeugen.", fileName);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
		TRACE1("Fehler bei Create Table: %s", sSql.Left(200));
		ret = 0;
	}
	END_CATCH_ALL;
	if (database.IsOpen())
		database.Close();
	return(ret);
} // 

/// OpenTable-Funktion
/// Oeffnet die angegebene Tabelle.
int CExcelTable::OpenTable(char * File, char * Table)
{
	CString sSql;
    int ret = 1;

	tableName = Table;

	TRY 
	{
		// Build the creation string for access without DSN
   		sSql.Format("DRIVER={%s};DSN=%s;FIRSTROWHASNAMES=1;READONLY=FALSE;DBQ=%s", driverName, File, File);
		// Open the database (i.e. Excel sheet)
		if (database.OpenEx(sSql)) 
		{
			return 1;
		}      
	} 
	CATCH_ALL(e) 
	{
		fehlerzahl++;
		e->ReportError();
		TRACE1("Fehler bei OpenTable: %s", sSql.Left(200));
		ret = 0;
	}
	END_CATCH_ALL;
	return ret;
}  

/// CloseTable-Funktion 
/// Schliesst die angegebene Tabelle.
int CExcelTable::CloseTable(char * File, char * Table)
{  
	CString sSql;
	int ret = 1;
    
	TRY {
		// Build the creation string for access without DSN
   		sSql.Format("DRIVER={%s};DSN'';FIRSTROWHASNAMES=1;READONLY=FALSE;DBQ=%s", driverName, File);
		// Close database
		database.Close();									
	} CATCH_ALL(e) {
		fehlerzahl++;
		e->ReportError();
		TRACE1("Fehler bei CloseTable: %s", sSql.Left(200));
		ret = 0;
	}
	END_CATCH_ALL;
	return(ret);
}  


/// AddColumn-Funktion
/// Fuegt ein neue Spalte zur geoeffneten Tabelle hinzu. 
int CExcelTable::AddColumn(int Number)
{ // 
	// TODO: Add your specialized code here.
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sSql;
	int ret = 1;

	TRY {
		sSql.Format("ALTER TABLE %s ADD %s TEXT", tableName, columnName[Number]);
		database.ExecuteSQL(sSql);
	} CATCH_ALL(e) {
		fehlerzahl++;
		TRACE1("Fehler bei AddColumn: %s", sSql.Left(200));
		ret = 0;
	}
	END_CATCH_ALL;
	return(ret);
} 


/// InsertInto-Funktion
/// Fuegt einen Datensatz an die Tabelle an.
int CExcelTable::InsertInto(char * Values[])
{ 
	CString sSql, str, txt;
	int ret = 1, i; 
		
	TRY 
	{
		// create string
		sSql.Format("INSERT INTO %s (", tableName);

		for (i = 0; i < columnQuantity; i++) 
		{	
			if (strlen(Values[i]))
			{// Nur Felder einfügen, die auch Daten haben
				str.Format("%s,", columnName[i]);
				sSql += str;
			}
		}
		sSql.SetAt(sSql.GetLength()-1, ')'); // Letztes Komma durch abschliessende Klammer ersetzen

		// Werte einfügen
		str = " VALUES (";
		sSql += str; 
		for (i = 0; i < columnQuantity; i++) 
		{	
			if (!strlen(Values[i]))
				continue;// Wenn kein Wert, dann keine Ausgabe

			// Werte werden hier nur auf "'" überprüft! Versteht der Exceltreiber den Wert nicht 
			// richtig, können Daten fehlerhaft sein.
			txt.Format("%s", Values[i]);
			txt.Replace("'", "´");

			if (columnType[i] == CExcelTable::TEXT) 
			{
				str.Format("'%s',", txt);
			} 
			else 
			{ 
				str.Format("%s,", txt);
			}
			sSql += str;
		}
				
		sSql.SetAt(sSql.GetLength()-1, ')'); // Letztes Komma durch abschliessende Klammer ersetzen
		TRACE("SQL: %s\n", sSql.Left(250));
		database.ExecuteSQL(sSql);
		
	} CATCH_ALL(e) {
		fehlerzahl++;
		TRACE1("Fehler bei InsertInto: %s", sSql.Left(200));
		ret = 0;
	}
	END_CATCH_ALL;
	return(ret);
} // 

/// Update-Funktion
/// Aktualisiert das Datenfeld der Spalte "SetColumn" in der Zeile, 
//  wo die Spalte "WhereColumn" den Wert "WhereValue" hat, mit dem 
//  Wert von "SetValue".
int CExcelTable::Update(char * SetColumn, char * SetValue, int SetType, char * WhereColumn, char * WhereValue, int WhereType)
{   
	CString sSql;
	int ret = 1;

	TRY {
		if (SetType == CExcelTable::TEXT && WhereType == CExcelTable::TEXT)
			sSql.Format("UPDATE %s SET %s = '%s' WHERE %s = '%s'", tableName, SetColumn, SetValue, WhereColumn, WhereValue);
		else if (SetType == CExcelTable::TEXT && WhereType != CExcelTable::TEXT)
			sSql.Format("UPDATE %s SET %s = '%s' WHERE %s = %s", tableName, SetColumn, SetValue, WhereColumn, WhereValue);
		else if (SetType != CExcelTable::TEXT && WhereType == CExcelTable::TEXT)
			sSql.Format("UPDATE %s SET %s = %s WHERE %s = '%s'", tableName, SetColumn, SetValue, WhereColumn, WhereValue);
		else if (SetType != CExcelTable::TEXT && WhereType != CExcelTable::TEXT)
			sSql.Format("UPDATE %s SET %s = %s WHERE %s = %s", tableName, SetColumn, SetValue, WhereColumn, WhereValue);
		database.ExecuteSQL(sSql);
	} CATCH_ALL(e) {
		fehlerzahl++;
		TRACE1("Fehler bei Update: %s", sSql.Left(200));
		ret = 0;
	}
	END_CATCH_ALL;
	return(ret);
} // 

/// GetValue-Funktion
/// Gibt den Wert des Datenfeldes der angegebenen Spalte und der
//      Zeile auf die gezeigt wird.
CString CExcelTable::GetValue(char * Column)
{     
	CString sItem, sSql;
	
	sSql.Format("SELECT * FROM %s ORDER BY %s", tableName, columnName[0]);
    // Execute that query (implicitly by opening the recordset)
	recset.Open(CRecordset::dynaset, sSql, CRecordset::none);
	recset.Requery();
    recset.GetFieldValue("Nummer", sItem);
	if (recset.IsOpen())
		recset.Close();
	return(sItem);
} 






