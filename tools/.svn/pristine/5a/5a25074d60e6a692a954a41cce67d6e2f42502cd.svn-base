#pragma once	

#include <mxodbc\mxodbc_lib.h>

#define INITTEXT(feld, sprache) other.##feld##sprache.data->GetLength() ? *other.##feld##sprache.data : *other.##feld.data



class preislistenTable
{
	friend class MkStammdbDlg;
public:
	preislistenTable(DWORD db, const CString& tblname, const CString& the_code);

	CString cur_code;
	virtual int load() = NULL;
	virtual void delete_entry() = NULL;

	std::string tblname() const
	{
		return (LPCSTR)tbl.get_tableName();
	}

protected:
	MXTable tbl;
	
};

