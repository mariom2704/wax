#include "stdafx.h"
#include "artikel.h"


artikel::artikel(DWORD db, const CString& the_code) : preislistenTable(db, "PreislistenArtikel", the_code),
Liste("Liste"), Gruppe("Gruppe"), code("code"), count("count"),
ArtikelNummer("ArtikelNummer"),
Montagezeit("Montagezeit"),
WM("WM"),
WL("WL"),
KurzText("KurzText"),
LangText("LangText"),
LVText("LVText"),
KurzTextEnglish("KurzTextEnglish"),
LangTextEnglish("LangTextEnglish"),
LVTextEnglish("LVTextEnglish"),
KurzTextFrancais("KurzTextFrancais"),
LangTextFrancais("LangTextFrancais"),
LVTextFrancais("LVTextFrancais"),
KurzTextNederlands("KurzTextNederlands"),
LangTextNederlands("LangTextNederlands"),
LVTextNederlands("LVTextNederlands"),
Hinweis("Hinweis"),
posnr("posnr"),
IstFremdleistung("IstFremdleistung"),
IstSprinkler("IstSprinkler"),
IstGeloescht("IstGeloescht"),
IstGeaendert("IstGeaendert"),
IstNeu("IstNeu"),
GeaendertVon("GeaendertVon"),
GeaendertAm("GeaendertAm"),
gewicht("gewicht"),
festpreis("festpreis")
{
	tbl.attach(&Liste);
	tbl.attach(&Gruppe);
	tbl.attach(&code);
	tbl.attach(&count);
	tbl.attach(&ArtikelNummer);
	tbl.attach(&Montagezeit);
	tbl.attach(&WM);
	tbl.attach(&WL);
	tbl.attach(&KurzText);
	tbl.attach(&LangText);
	tbl.attach(&LVText);
	tbl.attach(&KurzTextEnglish);
	tbl.attach(&LangTextEnglish);
	tbl.attach(&LVTextEnglish);
	tbl.attach(&KurzTextFrancais);
	tbl.attach(&LangTextFrancais);
	tbl.attach(&LVTextFrancais);
	tbl.attach(&KurzTextNederlands);
	tbl.attach(&LangTextNederlands);
	tbl.attach(&LVTextNederlands);
	tbl.attach(&Hinweis);
	tbl.attach(&posnr);
	tbl.attach(&IstFremdleistung);
	tbl.attach(&IstSprinkler);
	tbl.attach(&IstGeloescht);
	tbl.attach(&IstGeaendert);
	tbl.attach(&IstNeu);
	tbl.attach(&GeaendertVon);
	tbl.attach(&GeaendertAm);
	tbl.attach(&gewicht);
	tbl.attach(&festpreis);
}

artikel::~artikel()
{
}

int artikel::load()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	bool succ = tbl.load(select, (CString)"", true);
	if (!succ || !tbl.get_count())
		AfxThrowNotSupportedException();
	return tbl.get_count();
}


void artikel::copy(const artikel &other, int set_counter)
{
	TRY
	{
		tbl.load();
		tbl.addNew();
		*Liste.data = *other.Liste.data;
		*Gruppe.data = *other.Gruppe.data;
		*code.data = *other.code.data;
		if (set_counter >= 0)
			count.data = set_counter;
		else
			count.data = other.count.data;
		*ArtikelNummer.data = *other.ArtikelNummer.data;
		*Montagezeit.data = *other.Montagezeit.data;
		*WM.data = *other.WM.data;
		*WL.data = *other.WL.data;
		*KurzText.data = *other.KurzText.data;
		*LangText.data = *other.LangText.data;
		*LVText.data = *other.LVText.data;
		*KurzTextEnglish.data = INITTEXT(KurzText, English);
		*LangTextEnglish.data = INITTEXT(LangText, English);
		*LVTextEnglish.data = INITTEXT(LVText, English);
		*KurzTextFrancais.data = INITTEXT(KurzText, Francais);
		*LangTextFrancais.data = INITTEXT(LangText, Francais);
		*LVTextFrancais.data = INITTEXT(LVText, Francais);
		*KurzTextNederlands.data = INITTEXT(KurzText, Nederlands);
		*LangTextNederlands.data = INITTEXT(LangText, Nederlands);
		*LVTextNederlands.data = INITTEXT(LVText, Nederlands);
		
		*Hinweis.data = *other.Hinweis.data;
		*posnr.data = *other.posnr.data;
		IstFremdleistung.data = other.IstFremdleistung.data;
		IstSprinkler.data = other.IstSprinkler.data;
		IstGeloescht.data = other.IstGeloescht.data;
		IstGeaendert.data = other.IstGeaendert.data;
		IstNeu.data = other.IstNeu.data;
		*GeaendertVon.data = *other.GeaendertVon.data;
		*GeaendertAm.data = *other.GeaendertAm.data;
		gewicht.data = other.gewicht.data;
		festpreis.data = other.festpreis.data;
		tbl.update();
		tbl.close();
	}
	CATCH(CDBException, e)
	{
		char err[1024];
		e->GetErrorMessage(err, 1024, NULL);
		CString msg;
		msg.Format("Artikel : %s\n%s", *code.data, err);
		AfxMessageBox(msg, MB_ICONERROR | MB_OK);
	}
	END_CATCH
}

void artikel::delete_entry()
{
	CString select;
	select.Format("code = '%s'", cur_code);
	tbl.load(select);
	tbl.deleteData();
	tbl.close();
}
