#include "stdafx.h"


#include "ExcelODBC.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ExcelODBC::ExcelODBC()
{
	ExcelTableSetVar			= NULL;
	ExcelTableSetColumnQuantity = NULL;
	ExcelTableSetColumnNames	= NULL; 
	ExcelTableCreateTable		= NULL; 
	ExcelTableOpenTable			= NULL; 
	ExcelTableCloseTable		= NULL; 
	ExcelTableInsertInto		= NULL; 
	ExcelTableSetColumnType		= NULL;
	ExcelTableUpdate			= NULL;
	setExcelTableOffice2010     = NULL;
	loadDLL();
}

ExcelODBC::~ExcelODBC()
{
	
}



bool ExcelODBC::loadDLL()
{
	/*
	Lädt die ExcelTable.dll 
	*/
	if (!isLoaded())
	{

#ifdef _DEBUG
		hinstance = LoadLibrary("ExcelTable.dll");
#else
		hinstance = LoadLibrary("ExcelTable.dll");
#endif

		if (!hinstance) 
		{
			CString str; str = getStammdaten().getText("IDS_DLL_EXCEL").c_str();
			AfxMessageBox(str, MB_OK | MB_ICONERROR);
			return false;
		}

		ExcelTableSetVar			= (pfExcelTableSetVar)GetProcAddress(hinstance, "ExcelTableSetVar");
		ExcelTableSetColumnQuantity	= (pfExcelTableSetColumnQuantity)GetProcAddress(hinstance, "ExcelTableSetColumnQuantity");	
		ExcelTableSetColumnNames	= (pfExcelTableSetColumnNames)GetProcAddress(hinstance, "ExcelTableSetColumnNames");	
		ExcelTableCreateTable		= (pfExcelTableCreateTable)GetProcAddress(hinstance, "ExcelTableCreateTable");
		ExcelTableOpenTable			= (pfExcelTableOpenTable)GetProcAddress(hinstance, "ExcelTableOpenTable");
		ExcelTableCloseTable		= (pfExcelTableCloseTable)GetProcAddress(hinstance, "ExcelTableCloseTable");
		ExcelTableInsertInto		= (pfExcelTableInsertInto)GetProcAddress(hinstance, "ExcelTableInsertInto");
		ExcelTableSetColumnType		= (pfExcelTableSetColumnType)GetProcAddress(hinstance, "ExcelTableSetColumnType");
		ExcelTableUpdate			= (pfExcelTableUpdate)GetProcAddress(hinstance, "ExcelTableUpdate");
		setExcelTableOffice2010			= (pfSetExcelTableOffice2010) GetProcAddress(hinstance, "set_ExcelTableOffice2010");;
		
	}		
	return isLoaded();
}



bool ExcelODBC::isLoaded()
{
	return ExcelTableSetColumnType
		&& ExcelTableSetColumnQuantity
		&& ExcelTableSetVar 
		&& ExcelTableSetColumnNames 
		&& ExcelTableCreateTable 
		&& ExcelTableOpenTable 
		&& ExcelTableCloseTable
		&& ExcelTableInsertInto
		&& ExcelTableUpdate;
}

