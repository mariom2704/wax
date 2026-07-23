#include "stdafx.h"

#include "drucklisteverzinkung.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;

DrucklisteVerzinkung::DrucklisteVerzinkung(HtmlDruck& file, WaxPart* pt) : Druckliste(file, pt)
{
	init();
}


DrucklisteVerzinkung::~DrucklisteVerzinkung()
{
}

void DrucklisteVerzinkung::init()
{
	if (!waxpart)
		return;
	GesGewicht = 0;
	for (Auftragsliste::iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{
		const Rohrleitung *rohr = dynamic_cast<const Rohrleitung*>(*it);
		if (!rohr)
			continue;
		if(dynamic_cast<const COIIVerteiler*>(rohr))
		{
			GesGewicht += rohr->getGesGewicht();
		}
		ZinkKosten z(getStammdaten().get_verzinkung().query(rohr->getBeschichtung().get_verzinkerei()), *rohr);
		verzinkung.add(z); 

	}
}

bool DrucklisteVerzinkung::create()
{
	/**
	Druckliste mit Elementanzahl und Putzzeit
	*/
	WaxHtmlFile &w = html.get_file();

	html.insertLaengeGewichtBarcode(maxlaenge, GesGewicht, "", 0, "IDS_ROHRGEWICHT");

	w.null2whitespace();

	// Tabellenspalten, Breiten in %
	const int rows[] = { 20, 20, 20, 20, 20};
	const int rowcount = 5;

	w.startTable(0,0);

	// Überschrift Standardelement / Aussengewinde
	w.startRow();
	w.appendCol(" ", rows[0], "RIGHT", true);
	w.appendCol(w.bold_string("Standardelement"), rows[1] + rows[2], "CENTER", true);
	w.appendCol(w.bold_string("Aussengewinde"), rows[3] + rows[4], "CENTER", true);
	w.endRow();
	w.endTable();

	w.startTable();

	int row = -1;
#define NEXTROW rows[++row % rowcount]

	// Überschrift, zweite Zeile
	w.startRow();
	w.appendCol(w.bold_string_ids(dn2zoll ? "IDS_NENNW_ZOLL" : "IDS_NENNW_DN"), NEXTROW, "LEFT", true);
	w.appendCol(w.bold_string_ids("IDS_ANZ_STK2"), NEXTROW, "CENTER", true);
	w.appendCol(w.bold_string_ids("IDS_ZEIT_MIN"), NEXTROW, "CENTER", true);
	w.appendCol(w.bold_string_ids("IDS_ANZ_STK2"), NEXTROW, "CENTER", true);
	w.appendCol(w.bold_string_ids("IDS_ZEIT_MIN"), NEXTROW, "CENTER", true);
	w.endRow();

	const int dn[] = {15, 20, 25, 32, 40, 50, 65, 80, 100, 125, 150, 200, 250, 300, 0 };


	for (int i = 0; dn[i]; i++)
	{
		w.startRow();
		w.appendCol(dn2zoll ? atoi(WaxStamm::dn2zoll(dn[i])) : dn[i], NEXTROW, "RIGHT", true);
		w.appendCol(verzinkung.sum_anz(ZinkKosten::STDELEMENT, dn[i]), NEXTROW, "RIGHT", true);
		w.appendCol(verzinkung.sum_zeit(ZinkKosten::STDELEMENT, dn[i]), NEXTROW, "RIGHT", true);
		w.appendCol(verzinkung.sum_anz(ZinkKosten::AUSSENGEW, dn[i]), NEXTROW, "RIGHT", true);
		w.appendCol(verzinkung.sum_zeit(ZinkKosten::AUSSENGEW, dn[i]), NEXTROW, "RIGHT", true);
        w.endRow();
	}

	/// Aufsummierung
	w.startRow();
	w.appendCol("Summe", NEXTROW, "RIGHT", true);
	w.appendCol(verzinkung.sum_anz(ZinkKosten::STDELEMENT), NEXTROW, "RIGHT", true);
	w.appendCol(verzinkung.sum_zeit(ZinkKosten::STDELEMENT), NEXTROW, "RIGHT", true);
	w.appendCol(verzinkung.sum_anz(ZinkKosten::AUSSENGEW), NEXTROW, "RIGHT", true);
	w.appendCol(verzinkung.sum_zeit(ZinkKosten::AUSSENGEW), NEXTROW, "RIGHT", true);
    w.endRow();

	w.endTable();

	// Zweite kleine Tabelle mit Gesamtwerten Zeit und Kosten
	w.startTable();

	w.startRow();
	w.appendCol(w.bold_string(""), rows[0], "CENTER", true);
	w.appendCol(w.bold_string("Zeit [min]"), rows[1] + rows[2], "CENTER", true);
	w.appendCol(w.bold_string("Betrag [€]"), rows[3] + rows[4], "CENTER", true);
	w.endRow();

	w.startRow();
	w.appendCol(w.bold_string("Gesamt Nacharbeit"), rows[0], "CENTER", true);
	w.appendCol(verzinkung.sum_zeit(ZinkKosten::AUSSENGEW) + verzinkung.sum_zeit(ZinkKosten::STDELEMENT), rows[1] + rows[2], "CENTER", true);
	w.appendCol(verzinkung.sum_putzkosten(), rows[3] + rows[4], "CENTER", true);
	w.endRow();

	w.endTable();
	return true;
}

