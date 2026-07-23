#pragma once
	
#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

/**
Tabelle verkaufspreise
*/
class verkaufspreise : public preislistenTable
{
public:
	verkaufspreise(DWORD db, const CString& the_code = "");
	virtual ~verkaufspreise();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const verkaufspreise &other, int set_counter);
protected:
	StringCol code;
	DoubleCol preis;
	StringCol doc;
};

