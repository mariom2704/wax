#include "stdafx.h"
#include "angebotsblatt.h"


angebotsblatt::angebotsblatt(DWORD db, const CString& the_code) : preislistenTable(db, "angebotsblatt", the_code),
code("code"),blatt("blatt")
{
	tbl.attach(&code);
	tbl.attach(&blatt);
}

angebotsblatt::~angebotsblatt()
{
}

int angebotsblatt::load()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	bool succ = tbl.load(select, (CString)"", true);
	if (!succ || !tbl.get_count())
		AfxThrowNotSupportedException();
	return tbl.get_count();
}


void angebotsblatt::copy(const angebotsblatt &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*code.data = *other.code.data;
		*blatt.data = *other.blatt.data;
		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("angebotsblatt : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void angebotsblatt::delete_entry()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
