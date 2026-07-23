// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PREISETESTEN_3DE23BE300E5_INCLUDED
#define _INC_PREISETESTEN_3DE23BE300E5_INCLUDED

#include "..\rohrkonst\rohrkonst_dll.h"
#include "..\waxui\waxui_dll.h"
#include "..\waxint\excelodbc.h"

class PreiseTesten : 
public ExcelODBC  
{
public:
	enum MAXDEFS 
	{	
		WA_MAX_VERT = 50
	};


	//Fügt in der Debug Version den Menüpunkt "Preise testen"  hinzu
	static void alterMenu(CWnd * wnd);

	virtual ~PreiseTesten();

	PreiseTesten();


private:
	class wertereihe 
	{
	public:
		double x;

		double y [5] ;

	};


	wertereihe daten [WA_MAX_VERT+ 1]  ;

	void initwa();

	void writeToClipboard();

	VerteilerData verteilerwa;

private:
	void writeToExcel();
	CString file;

	void setfile();

};

#endif /* _INC_PREISETESTEN_3DE23BE300E5_INCLUDED */
