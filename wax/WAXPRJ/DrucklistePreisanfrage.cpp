#include "stdafx.h"


#include "DrucklistePreisanfrage.h"
#include "WpPreisanfrage.h"

using namespace WaxDruck;

DrucklistePreisanfrage::DrucklistePreisanfrage(HtmlDruck& file, WaxPart* pt) : Druckliste(file, pt)
{
}

DrucklistePreisanfrage::~DrucklistePreisanfrage(void)
{
}


bool DrucklistePreisanfrage::create()
{
	WpPreisanfrage *pa = dynamic_cast<WpPreisanfrage *>(waxpart);
	if (!pa)
		return false;

	WaxHtmlFile &w = html.get_file();
	CString txt, str;

	
	txt.Format("Diese Angebot ist gueltig bis: %s", pa->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_gueltigBis().Format("%d.%m.%y"));
	w.appendString(txt);
	w.appendString("Danach gelten unsere Preise vorbehaltlich der Preise unserer Vorlieferanten. Bei Preisänderungen unserer Vorlieferanten werden unsere Preise diesen angepasst.");

	w.appendString("<p><b>Preise</b></p>");

	//Tabelle Preise
	const int rows[] = {100, 100};
	
	w.startTable();
	w.startRow();
	w.appendCol("Position", rows[0], "center");
	w.appendCol("Preis [€]",rows[1], "right");
	w.endRow();

	//Preise der Sammelaufträge
	for (POSITION pos = pa->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(pa->get_subobjects().GetNext(pos));
		if (!sammel || sammel->get_deleted())
			continue;

		w.startRow();
		w.appendCol(sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_position(), rows[0], "center");
		w.appendCol(sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_preis(), rows[1], "right");	
		w.endRow();
	}

	


	w.startRow();
	w.appendCol(".", rows[0], "center");	
	w.appendCol(".", rows[1], "right");	
	w.endRow();


	//Gesamtpreis
	w.startRow();
	w.appendCol("<b>Gesamtpreis für Fertigung</b>", rows[0], "center");	
	w.appendCol(pa->getGesamtpreis(pa), rows[1], "right");	
	w.endRow();

	w.startRow();
	w.appendCol("Transport", rows[0], "center");	
	w.appendCol(".", rows[1], "right");	
	w.endRow();

	w.startRow();
	w.appendCol("Preis incl. Transport", rows[0], "center");	
	w.appendCol(".", rows[1], "right");	
	w.endRow();


	w.endTable();
	
	return true;
}