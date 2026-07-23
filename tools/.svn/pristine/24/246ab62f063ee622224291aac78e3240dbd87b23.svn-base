#pragma once


#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

class stueckliste : public preislistenTable
{
public:
	stueckliste(DWORD db, const CString& the_code = "");
	virtual ~stueckliste();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const stueckliste &other, int set_counter);

	string get_code() const;
protected:
	StringCol code;
	StringCol anzahl;
	StringCol artikelnr;
	StringCol bezeichnung;
};

