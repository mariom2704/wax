#include "stdafx.h"
#include "produktinfo.h"


produktinfo::produktinfo(DWORD db, const CString& the_code) : preislistenTable(db, "produktinfo", the_code),
code("code"),datei("datei")
{
	tbl.attach(&code);
	tbl.attach(&datei);
}

produktinfo::~produktinfo()
{
}

int produktinfo::load()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	bool succ = tbl.load(select, (CString)"", true);
	if (!succ || !tbl.get_count())
		AfxThrowNotSupportedException();
	return tbl.get_count();
}


void produktinfo::copy(const produktinfo &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*code.data = *other.code.data;
		*datei.data = *other.datei.data;
		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("produktinfo : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void produktinfo::delete_entry()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
