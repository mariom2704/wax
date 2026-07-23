#include "stdafx.h"
#include "gruppe.h"


gruppe::gruppe(DWORD db, const CString& the_code) : preislistenTable(db, "PreislistenGruppen", the_code),
Liste("Liste"), code("Code"),Beschreibung("Beschreibung"), sparte("sparte"),
BeschreibungEnglish("BeschreibungEnglish"),
BeschreibungFrancais("BeschreibungFrancais"),
BeschreibungNederlands("BeschreibungNederlands")
{
	tbl.attach(&Liste);
	tbl.attach(&code);
	tbl.attach(&sparte);
	tbl.attach(&Beschreibung);
	tbl.attach(&BeschreibungEnglish);
	tbl.attach(&BeschreibungFrancais);
	tbl.attach(&BeschreibungNederlands);
}

gruppe::~gruppe()
{
}

int gruppe::load()
{
	CString select;
	select.Format("Code = '%s'", cur_code);
	tbl.load(select, (CString)"", true);
	return tbl.get_count();
}


void gruppe::copy(const gruppe &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*Liste.data = *other.Liste.data;
		*code.data = *other.code.data;
		sparte.data = other.sparte.data;
		*Beschreibung.data = *other.Beschreibung.data;
		*BeschreibungEnglish.data = INITTEXT(Beschreibung, English);
		*BeschreibungFrancais.data = INITTEXT(Beschreibung, Francais);
		*BeschreibungNederlands.data = INITTEXT(Beschreibung, Nederlands);

		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("gruppe : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void gruppe::delete_entry()
{
	CString select;
	select.Format("Code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
