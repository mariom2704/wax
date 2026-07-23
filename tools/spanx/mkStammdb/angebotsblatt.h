#pragma once
	
#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

/**
Tabelle Angebotsblatt
*/
class angebotsblatt : public preislistenTable
{
public:
	angebotsblatt(DWORD db, const CString& the_code = "");
	virtual ~angebotsblatt();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const angebotsblatt &other, int set_counter);
protected:
	StringCol code;
	StringCol blatt;
};

