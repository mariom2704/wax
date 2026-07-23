#ifndef __gruppe__
#define __gruppe__
	
#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

class gruppe : public preislistenTable
{
public:
	gruppe(DWORD db, const CString& the_code = "");
	virtual ~gruppe();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const gruppe &other, int set_counter);
protected:
	StringCol code;
	StringCol Liste;
	IntCol sparte;
	StringCol Beschreibung;
	StringCol BeschreibungEnglish; 
	StringCol BeschreibungFrancais;
	StringCol BeschreibungNederlands;
};



#endif