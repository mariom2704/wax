#include "stdafx.h"
#include "stueckliste.h"


stueckliste::stueckliste(DWORD db, const CString& the_code) : preislistenTable(db, "Stuecklisten", the_code),
code("Code"), anzahl("Anzahl"), artikelnr("artikelnummer"),
bezeichnung("Bezeichnung")
{
	tbl.attach(&code);
	tbl.attach(&anzahl);
	tbl.attach(&artikelnr);
	tbl.attach(&bezeichnung);
}

stueckliste::~stueckliste()
{
}

int stueckliste::load()
{
	CString select;
	select.Format("Code like '%s'", cur_code);
	tbl.load(select, (CString)"", true);
	return tbl.get_count();
}


void stueckliste::copy(const stueckliste &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*code.data = *other.code.data;
		*anzahl.data = *other.anzahl.data;
		*artikelnr.data = *other.artikelnr.data;
		*bezeichnung.data = *other.bezeichnung.data;
		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("stueckliste : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void stueckliste::delete_entry()
{
	CString select;
	select.Format("Code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}

string stueckliste::get_code() const
{
	return (LPCSTR)*code.data;
}