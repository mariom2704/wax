#include "stdafx.h"
#include "liste.h"


liste::liste(DWORD db, const CString& the_code) : preislistenTable(db, "Preislisten", the_code),
code("Code"), datum("Datum"), Beschreibung("Beschreibung"),
BeschreibungEnglish("BeschreibungEnglish"),
BeschreibungFrancais("BeschreibungFrancais"),
BeschreibungNederlands("BeschreibungNederlands"),
email("email")
{
	tbl.attach(&code);
	tbl.attach(&datum);
	datum.set_dateOnly(false);
	tbl.attach(&Beschreibung);
	tbl.attach(&BeschreibungEnglish);
	tbl.attach(&BeschreibungFrancais);
	tbl.attach(&BeschreibungNederlands);
	tbl.attach(&email);
}

liste::~liste()
{
}

int liste::load()
{
	CString select;
	select.Format("Code = '%s'", cur_code);
	tbl.load(select, (CString)"", true);
	return tbl.get_count();
}


void liste::copy(const liste &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*code.data = *other.code.data;
		datum.setdate(other.datum.getdate());
		*Beschreibung.data = *other.Beschreibung.data;
		*BeschreibungEnglish.data = INITTEXT(Beschreibung, English);
		*BeschreibungFrancais.data = INITTEXT(Beschreibung, Francais);
		*BeschreibungNederlands.data = INITTEXT(Beschreibung, Nederlands);
		*email.data = *other.email.data;
		tbl.update();
		tbl.close();


	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("liste : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void liste::delete_entry()
{
	CString select;
	select.Format("Code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
