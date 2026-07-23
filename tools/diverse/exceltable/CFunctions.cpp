////////////////////////////////////////////////////////////////////////
// CFunctions.cpp - Implementation file
//
//
// Company	 : Minimax GmbH
// Address	 : Industriestrasse 10-12 
// 			   23843 Bad Oldesloe
//	           Germany
// Telephone : +49 4531 803 0		
// Internet  : www.minimax.de
//
// Author    : Henning Kluetz		
// E-Mail    : kluetzh@minimax.de	
//
// Date	     : May.29.2001 	
//

// Include files
#include "stdafx.h"
#include "CFunctions.h"
#include "ExcelTable7.h"

// CExcelTable Object
CExcelTable ExcelTable;


int ExcelTableOffice2010()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ExcelTable.office2010() ? 1 : 0;
}


void set_ExcelTableOffice2010(bool val)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ExcelTable.set_office2010(val);
}

const char* ExcelExt()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	static char ext[255];
	memset(ext, 0, 255);
	std::string ext_t = ExcelTable.get_ext();
	int cnt = ext_t.length() > 255 ? 255 : (int)ext_t.length();
	strncpy_s(ext, ext_t.c_str(), cnt);
	return ext;
}


/// Ruft die Memberfunktion "SetVar" der Klasse "CExcelTable" auf.
int ExcelTableSetVar(char * File, char * Table)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ExcelTable.SetVar(File, Table);
	return(1);
}


/// Ruft die Memberfunktion "SetColumnNames" der Klasse "CExcelTable" auf.
int ExcelTableSetColumnNames(char *Names[],		
							  int Quantity)		
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ExcelTable.SetColumnNames(Names, Quantity);
	return(1);
}

/// Ruft die Memberfunktion "set_columnQuantity" der Klasse "CExcelTable" auf.
int ExcelTableSetColumnQuantity(int value)		
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ExcelTable.set_columnQuantity(value);
	return(1);
}

/// Ruft die Memberfunktion "SetColumnNames" der Klasse "CExcelTable" auf.
int ExcelTableSetColumnType(int quantity,		
							  int type[])		
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ExcelTable.set_columnType(quantity, type);
	return(1);
}


/// Ruft die Memberfunktion "CreateTable" der Klasse "CExcelTable" auf.
int ExcelTableCreateTable()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int ret;
	
	ret = ExcelTable.CreateTable();
	return(ret);
}

/// Ruft die Memberfunktion "OpenTable" der Klasse "CExcelTable" auf.
int ExcelTableOpenTable(
						char * File,
						char * Table)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int ret;	
	
	ret = ExcelTable.OpenTable(File, Table);
	return(ret);
}

/// Ruft die Memberfunktion "CloseTable" der Klasse "CExcelTable" auf.
int ExcelTableCloseTable(
						 char * File,
						 char * Table)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int ret;
	
	ret = ExcelTable.CloseTable(File, Table);
	return(ret);
}


/// Ruft die Memberfunktion "AddColumn" der Klasse "CExcelTable" auf.
int ExcelTableAddColumn(
						int Number)					
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int ret;

	ret = ExcelTable.AddColumn(Number);
	return(ret);
}

/// Ruft die Memberfunktion "InsertInto" der Klasse "CExcelTable" auf.
int ExcelTableInsertInto(char * Values[])
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int ret;

	ret = ExcelTable.InsertInto(Values);
	return(ret);
}


/// Ruft die Memberfunktion "Update" der Klasse "CExcelTable" auf.
int ExcelTableUpdate(char * SetColumn, char * SetValue, int SetType, char * WhereColumn, char * WhereValue, int WhereType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int ret;
	  
	ret = ExcelTable.Update(SetColumn, SetValue, SetType, WhereColumn, WhereValue, WhereType);
	return(ret);
}
