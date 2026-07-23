#include "stdafx.h"
#include "verkaufspreise.h"


verkaufspreise::verkaufspreise(DWORD db, const CString& the_code) : preislistenTable(db, "verkaufspreise", the_code),
code("code"),preis("preis"), doc("doc")
{
	tbl.attach(&code);
	tbl.attach(&preis);
	tbl.attach(&doc);
}

verkaufspreise::~verkaufspreise()
{
}

int verkaufspreise::load()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	bool succ = tbl.load(select, (CString)"", true);
	if (!succ || !tbl.get_count())
		AfxThrowNotSupportedException();
	return tbl.get_count();
}


void verkaufspreise::copy(const verkaufspreise &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*code.data = *other.code.data;
		preis.data = other.preis.data;
		*doc.data = *other.doc.data;
		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("verkaufspreise : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void verkaufspreise::delete_entry()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
