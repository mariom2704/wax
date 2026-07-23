#ifndef __hinweistexte__
#define __hinweistexte__
	
#include <mxodbc\mxodbc_lib.h>
#include "preislistentbl.h"

class hinweistexte : public preislistenTable
{
public:
	hinweistexte(DWORD db, const CString& the_code = "");
	virtual ~hinweistexte();

	virtual int load();
	virtual void delete_entry();
	virtual void copy(const hinweistexte &other, int set_counter);
protected:
	StringCol nIndex;
	LongStringCol HinweisText;
	LongStringCol HinweisTextEnglish;
	LongStringCol HinweisTextFrancais;
	LongStringCol HinweisTextNederlands;
};



#endif