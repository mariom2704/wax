#ifndef _INC_EXCELODBC_3DDB9FB402CA_INCLUDED
#define _INC_EXCELODBC_3DDB9FB402CA_INCLUDED

//Basisklasse zum Laden der Excelschnittstelle ExcelTable
class WAXINT ExcelODBC 
{
private:
	bool loadDLL();

	//Zugriff auf die ExcelTable.dll
	HINSTANCE hinstance;

public:
	enum TYPES 
	{
		EXCELTEXT = 0,

		EXCELINT = 1,

		EXCELDOUBLE = 2
	};

public:
	ExcelODBC();

	virtual ~ExcelODBC();

	bool isLoaded();

	typedef int (*pfExcelTableUpdate) (char * SetColumn, char * SetValue, int SetType, char * WhereColumn, char * WhereValue, int WhereType);			// ExcelTableSetVar

	typedef int (*pfExcelTableSetColumnType) (int quantity, int type[]);	// ExcelTableSetColumnType	

	typedef int (*pfExcelTableInsertInto) (char *Values[]);					// ExcelTableInsertInto

	typedef int (*pfExcelTableCloseTable) (char * File, char * Table);		// ExcelTableCloseTable

	typedef int (*pfExcelTableOpenTable) (char * File, char * Table);		// ExcelTableOpenTable

	typedef int (*pfExcelTableCreateTable)(void);							// ExcelTableCreateTable

	typedef int (*pfExcelTableSetColumnNames) (char *Names[], int Number);	// ExcelTableSetColumnNames

	typedef int (*pfExcelTableSetVar) (char * File, char * Table);			// ExcelTableSetVar

	typedef int (*pfExcelTableSetColumnQuantity) (int value);				// ExcelTableSetColumnQuatity

	typedef void (*pfSetExcelTableOffice2010) (bool value);				// pfSet_ExcelTableOffice2010
		

	
	pfExcelTableSetVar ExcelTableSetVar;

	pfExcelTableSetColumnQuantity ExcelTableSetColumnQuantity;

	pfExcelTableSetColumnNames ExcelTableSetColumnNames;

	pfExcelTableCreateTable ExcelTableCreateTable;

	pfExcelTableOpenTable ExcelTableOpenTable;

	pfExcelTableCloseTable ExcelTableCloseTable;

	pfExcelTableInsertInto ExcelTableInsertInto;

	pfExcelTableSetColumnType ExcelTableSetColumnType;

	pfExcelTableUpdate ExcelTableUpdate;
	
	pfSetExcelTableOffice2010 setExcelTableOffice2010;	

};

#endif /* _INC_EXCELODBC_3DDB9FB402CA_INCLUDED */
