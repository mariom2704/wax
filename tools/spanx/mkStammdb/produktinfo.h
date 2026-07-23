#pragma once
	
#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

/**
Tabelle Angebotsblatt
*/
class produktinfo : public preislistenTable
{
public:
	produktinfo(DWORD db, const CString& the_code = "");
	virtual ~produktinfo();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const produktinfo& other, int set_counter);
protected:
	StringCol code;
	StringCol datei;
};

