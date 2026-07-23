#pragma once
	
#include <mxodbc\mxodbc_lib.h>
#include "components/DateColMfc.h"
#include "preislistentbl.h"

class liste : public preislistenTable
{
public:
	liste(DWORD db, const CString& the_code = "");
	virtual ~liste();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const liste &other, int set_counter);
protected:
	StringCol code;
	DateColMfc datum;
	StringCol Beschreibung;
	StringCol BeschreibungEnglish;
	StringCol email;
	StringCol BeschreibungFrancais;
	StringCol BeschreibungNederlands;
};
