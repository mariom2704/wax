// Copyright (C) 2001 Minimax GmbH

#include "stdafx.h"


#include "..\mxparam\mxparam_dll.h"
#include "..\waxprj\waxprj_dll.h"
#include "..\rohrkonst\rohrkonst_dll.h"
#include "..\waxui\waxui_dll.h"
#include "resource.h"
#include <afxcoll.h>
#include <afxadv.h>

#include "PreiseTesten.h"


using Rohrleitung::RQ2440_2458GSCH;

const int ODBC_SQLTEXT   = 0;
const int ODBC_SQLINT    = 1;
const int ODBC_SQLDOUBLE = 2;

PreiseTesten::~PreiseTesten()
{
	
}

PreiseTesten::PreiseTesten()
{
	setfile();
	initwa();
	writeToClipboard(); // Kann zum Debuggen dann in Projekt eingefügt werden
	writeToExcel();
}

void PreiseTesten::writeToExcel()
{
	if (!isLoaded())
		return;
	

	char *sColumns[WA_MAX_VERT+1];
	char *values[WA_MAX_VERT+1];
	sColumns[WA_MAX_VERT] = NULL;
		
	int types[WA_MAX_VERT];
	types[WA_MAX_VERT] = ODBC_SQLDOUBLE;
	for (int i = 0; i < WA_MAX_VERT; i++)
	{
		CString y;
		y.Format("%d", (int)daten[i].x);
		sColumns[i] = (char*)malloc(y.GetLength()+1);
		strcpy(sColumns[i], y);
		y.Format("%f", daten[i].y);
		values[i] = (char*)malloc(y.GetLength()+1);
		strcpy(values[i], y);
		types[i] = ODBC_SQLDOUBLE;
	}

	
	char *table = "Tabelle1";
	char fname[1024];
	strcpy(fname, file);
	
	ExcelTableSetVar(fname, table);
	ExcelTableSetColumnNames(sColumns, WA_MAX_VERT);
	
	ExcelTableSetColumnType(WA_MAX_VERT, types);
	ExcelTableCreateTable();
	ExcelTableOpenTable(fname, table);

	for (int j = 0; j < 4; j++)
	{
		for (i = 0; i < WA_MAX_VERT; i++)
		{
			free(values[i]);
			CString y;
			y.Format("%f", daten[i].y[j]);
			values[i] = (char*)malloc(y.GetLength()+1);
			strcpy(values[i], y);
		}

		ExcelTableInsertInto(values);
	}	
	ExcelTableCloseTable(fname, table);

	ShellExecute(NULL, "open", file, NULL, NULL, SW_SHOWNORMAL);

	for (i = 0; i < WA_MAX_VERT; i++)
	{
		free(sColumns[i]);
	}
}

void PreiseTesten::initwa()
{
	Rohrleitung *rohr = (Rohrleitung*)verteilerwa.get_werkauftrag();

	for (int i  = 0; i < 50; i++)
	{
		int laenge = i*300 + 100;
		rohr->initDaten(1, 100, laenge, "1",  RQ2440_2458GSCH, 22);
			
		rohr->set_rohrAnfang(20);
		rohr->set_rohrEnde(20);
		if (laenge > 12000)
		{
			CString deflaeng;
			int restlen = laenge;
			while (restlen)
			{
				deflaeng += "6000 ";
				restlen -= 6000;
				if (restlen <= 6000)
				{
					CString rest;
					rest.Format("%i ", restlen);
					deflaeng += rest;
					restlen = 0;
				}
			}
			((Verteiler*)verteilerwa.get_werkauftrag())->set_rohrDefLaengen(deflaeng);
		}
		rohr->set_changed(true);
		daten[i].x = laenge;
		daten[i].y[0] = rohr->get_preis().get_gesamtpreis();
		daten[i].y[1] = rohr->get_preis().get_preisarbeit().get_afoWap();
		daten[i].y[2] = rohr->get_preis().get_sonderkosten();
		daten[i].y[3] = rohr->get_preis().get_preismaterial().get_materialWap();
	}
}

void PreiseTesten::writeToClipboard()
{
	WaxPart *obj = &verteilerwa;
	UINT format = ::RegisterClipboardFormat("WAXPART");
	XASSERT(format);
	WaxPart *pt = dynamic_cast<WaxPart*>(obj);

	CSharedFile *f = new CSharedFile(GMEM_DDESHARE | GMEM_MOVEABLE);
	BinaryArchive ar( f, BinaryArchive::store);
	ar.WriteClass(pt->GetRuntimeClass());
	pt->serialize(ar);
	ar.Close();	
		
	::OpenClipboard(NULL);
	::EmptyClipboard();
	::SetClipboardData(format, f->Detach());
	::CloseClipboard();
	delete f;
}


//Fügt in der Debug Version den Menüpunkt "Preise testen"  hinzu
void PreiseTesten::alterMenu(CWnd * wnd)
{
	CMenu *men = wnd->GetParentFrame()->GetMenu();
	CMenu *sub = men->GetSubMenu(0);

	men->DeleteMenu(ID_VK3TEST, MF_BYCOMMAND);
#ifdef _DEBUG
	XASSERT(men->InsertMenu(ID_APP_EXIT, MF_STRING | MF_BYCOMMAND, ID_VK3TEST, "&Preise testen"));
	wnd->GetParentFrame()->DrawMenuBar();
#endif	

}



void PreiseTesten::setfile()
{
	file = "c:\\temp\\vk3test.xls";
}

