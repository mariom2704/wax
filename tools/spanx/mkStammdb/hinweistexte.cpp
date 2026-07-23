#include "stdafx.h"
#include "hinweistexte.h"


hinweistexte::hinweistexte(DWORD db, const CString& the_code) : preislistenTable(db, "HinweisTexte", the_code),
nIndex("nIndex"), HinweisText("HinweisText"),
HinweisTextEnglish("HinweisTextEnglish"),
HinweisTextFrancais("HinweisTextFrancais"),
HinweisTextNederlands("HinweisTextNederlands")
{
	tbl.attach(&nIndex);
	tbl.attach(&HinweisText);
	tbl.attach(&HinweisTextEnglish);
	tbl.attach(&HinweisTextFrancais);
	tbl.attach(&HinweisTextNederlands);
}

hinweistexte::~hinweistexte()
{
}

int hinweistexte::load()
{
	CString select;
	select.Format("nIndex = '%s'", cur_code);
	tbl.load(select, (CString)"", true);
	return tbl.get_count();
}


void hinweistexte::copy(const hinweistexte &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*nIndex.data = *other.nIndex.data;
		*HinweisText.data = *other.HinweisText.data;
		*HinweisTextEnglish.data = INITTEXT(HinweisText, English);
		*HinweisTextFrancais.data = INITTEXT(HinweisText, Francais);
		*HinweisTextNederlands.data = INITTEXT(HinweisText, Nederlands);
		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("hinweistexte : %s\n%s", *nIndex.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void hinweistexte::delete_entry()
{
	CString select;
	select.Format("nIndex = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
