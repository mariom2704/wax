#pragma once
	
#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

class artikel : public preislistenTable
{
		friend class MkStammdbDlg;
public:
	
	artikel(DWORD db, const CString& the_code = "");
	virtual ~artikel();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const artikel &other, int set_counter);
protected:
	StringCol Liste;
	StringCol Gruppe;
	StringCol code;
	IntCol count;// int, 
	StringCol ArtikelNummer;//  varchar(50), 
	StringCol Montagezeit;//  varchar(5), 
	StringCol WM;//  varchar(8), 
	StringCol WL;//  varchar(8), 
	StringCol KurzText;//  varchar(255), 
	LongStringCol LangText;//  varchar(255), 
	LongStringCol LVText;//  varchar(255), 
	StringCol KurzTextEnglish;//  varchar(255), 
	LongStringCol LangTextEnglish;//  varchar(255), 
	LongStringCol LVTextEnglish;//  varchar(255),
	StringCol KurzTextFrancais;//  varchar(255), 
	LongStringCol LangTextFrancais;//  varchar(255), 
	LongStringCol LVTextFrancais;//  varchar(255), 
	StringCol KurzTextNederlands;//  varchar(255), 
	LongStringCol LangTextNederlands;//  varchar(255), 
	LongStringCol LVTextNederlands;//  varchar(255), 
	StringCol Hinweis;//  varchar(255), 
	StringCol posnr;//  varchar(255), 
	IntCol IstFremdleistung;//  INT, 
	IntCol IstSprinkler;//  INT, 
	IntCol IstGeloescht;//  INT, 
	IntCol IstGeaendert;//  INT, 
	IntCol IstNeu;//  INT, 
	StringCol GeaendertVon;//  varchar(20), 
	StringCol GeaendertAm;//  varchar(20), 
	DoubleCol gewicht;
	IntCol festpreis;
};
