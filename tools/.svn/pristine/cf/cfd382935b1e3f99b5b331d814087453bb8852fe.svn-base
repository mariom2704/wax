// ConvertDb.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>

// Aktionen
#include "CreateCodeArtikelnummerTable.h"
#include "SplitPreislistenArtikelTable.h"
#include "ConvertArtikel2Component.h"
#include "CheckTables.h"
#include "ConvertStueckliste.h"
#include "CopyLangtexte.h"
#include "CopyPreise.h"
#include "CopyVerkaufsPreise.h"
#include "CopyProduktinfo.h"
#include "CopyVortexte.h"
#include "ReplaceCodesInSpl.h"

#include <SystemParameter.h>
#include <mxutil/tracer.h>

#include "boost/progress.hpp"




using namespace std;

int _tmain(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Usage: mergedbs [Spanxdb alt] [Spanxdb neu]" << endl;
		return 0;
	}
	boost::progress_timer t;

	SpanxDb::Database db(argv[1]);

	db.open();

	bool masterdb = SpanxDb::SystemParameter(db).val("IB_NL_KENNUNG", 0) ? false : true;
	string splpath = SpanxDb::SystemParameter(db).val("DIR_SPL", "");
	cout << (masterdb ? "Master DB erkannt" : "Niederlassungs DB erkannt") << endl;
	cout << "SPL Pfad: " << splpath << endl;

	try
	{
		cout << "Teste auf doppelte Artikelnummern" << endl;
		Action::CheckDoppelteArtnr check(db);
		check.run();
		cout << "Doppelte Artikelnummern werden umgesetzt in Komponenten" << endl;
		MxUtil::Tracer::trace2file("error.txt", check.evaluate());
		Action::ConvertArtikel2Component(db, check.get_list()).run();

		cout << "Erstelle Artikelnummer-Code Zuordnung und erzeuge neue Artikelnummern" << endl;
		Action::CreateCodeArtikelnummerTable catab(db, masterdb);
		catab.reset();
		catab.run();

		cout << "Erstelle Materialstamm" << endl;
		Action::SplitPreislistenArtikelTable(db).run();

		cout << "Kopiere Vortexte" << endl;
		Action::CopyVortexte(db).run();
		db.commit_trans();

		cout << "Setze Stuecklisten um..." << endl;
		Action::ConvertStueckliste(db).run();

		cout << "Kopiere Langtexte/LVTexte in neue Tabelle Langtexte..." << endl;
		Action::CopyLangtexte(db).run();

		cout << "Kopiere/Setze Preise um..." << endl;
		Action::CopyPreise(db).run();
		
		cout << "Kopiere/Setze Verkaufspreise um..." << endl;
		Action::CopyVerkaufsPreise(db).run();

		cout << "Fuelle Tabelle Produktinfo mit den Daten aus Angebotsblatt, Verkaufspreise, Produktinfo(alt)..." << endl;
		Action::CopyProduktinfo(db).run();

		cout << "Wandle SPL Dateien um..." << endl;
		Action::ReplaceCodesInSpl(db, splpath, ".").run();

		cout << "Umsetzung beendet" << endl;
	}
	catch(MxBase::Error e)
	{
		cout << "Ausführung abgebrochen. Fehler : " << e.get_description() << endl;
		MxUtil::Tracer::trace2file("error.txt", e.get_description());
	}

	return 0;
}

