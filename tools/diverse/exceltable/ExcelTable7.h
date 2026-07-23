#pragma once

//Klasse zum Erzeugen und Bearbeiten von Excel-Tabellen.

#ifdef __cplusplus

#define DLL_EXPORT __declspec(dllexport) 

#include <string>

/**
Excelverwaltung
 Diese Klasse implementiert die vollständige Erzeugung und Bearbeitung von Exceltabellen
*/
class CExcelTable 
{

private:
	/// Counter für fehlgeschlagene SQL Anweisungen
	int fehlerzahl;

	/// Der Name des verwendeten ODBC-Treibers.
	//         The name of the ODBC driver.
	CString driverName;

	/// Der Name und Pfad einer Exceldatei. 
	//         The name of the file.
	CString fileName;

	/// Der Name einer Exceltabelle.
	//         The name of the excel sheet.
	CString tableName;

	/// Die Namen der Spalten einer Tabelle.
	//         The names of the columns.
	char *columnName[255];

	int columnType[255];

	/// Die Anzahl der existierenden Tabellen innerhalb einer Exceldatei.
	//         The quantity of the existing tables in the file.
	int tableQuantity;

	/// Die Anzahl der Spalten in einer Tabelle.
	//         The quantity of the columns in the current table.
	int columnQuantity;

	bool office2010_mode;

public:
	/// Typendeklarationen für TEXT, DOUBLE und INT
	enum SQLTYPES 
	{
	
		TEXT = 0,

		INT = 1,

		DOUBLE = 2
	};


	CExcelTable();

	~CExcelTable();

	/// True, wenn ein Office 2010 Treiber gefunden wurde. Die Dateiendung ist dann .xlsb, wenn nicht angegegeben
	bool office2010() const;

	void set_office2010(bool val);

	std::string get_ext() const;

	CString getDriver();

	CString get_columnType(int index) const;

	void set_columnType(int value, int type[]);

	int SetVar(char * File, char * Table);

	int set_columnQuantity(int value);

	/// SetColumnNames-Funktion
	int SetColumnNames(char *Names[], int Quantity);

	/// CreateTable-Funktion
	int CreateTable();

	/// OpenTable-Funktion
	int OpenTable(char * File, char * Table);

	/// CloseTable-Funktion
	int CloseTable(char * File, char * Table);

	/// AddColumn-Funktion
	int AddColumn(int Number);

	/// InsertInto-Funktion
	int InsertInto(char * Values[]);

	/// Update-Funktion
	int Update(char * SetColumn, char * SetValue, int SetType, char * WhereColumn, char * WhereValue, int WhereType);

	/// GetValue-Funktion
	CString GetValue(char * Column);

};

#endif 
