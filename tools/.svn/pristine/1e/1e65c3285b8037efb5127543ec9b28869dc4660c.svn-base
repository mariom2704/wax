
#include "stdafx.h"
#include <iostream>

// Aktionen
#include "MergeVortexte.h"
#include "MergeHinweistexte.h"
#include "MergeMaterialstamm.h"
#include "MergeLangtexte.h"
#include "MergePreislisten.h"
#include "MergeGruppen.h"
#include "CreateSpl.h"


#include <SystemParameter.h>
#include <mxutil/tracer.h>

#include "boost/progress.hpp"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 3)
	{
		cout << "Usage: mergedbs [Spanxdb alt] [Spanxdb neu]" << endl;
		return 0;
	}
	boost::progress_timer t;

	SpanxDb::Database db_alt(argv[1]);
	db_alt.open();

	SpanxDb::Database db_neu(argv[2]);
	db_neu.open();

	bool masterdb = SpanxDb::SystemParameter(db_alt).val("IB_NL_KENNUNG", 0) ? false : true;

	string splpath = SpanxDb::SystemParameter(db_alt).val("DIR_SPL", "");
	string splpath_neu = SpanxDb::SystemParameter(db_neu).val("DIR_SPL", "");

	cout << (masterdb ? "Master DB erkannt" : "Niederlassungs DB erkannt") << endl;
	cout << "SPL Pfad Alt: " << splpath << endl;
	cout << "SPL Pfad Neu: " << splpath_neu << endl;

	string msg;

	try
	{
		// Update der Einträge in der Gruppen Tabelle, neue einfügen, alte updaten
		//cout << "Tabelle Gruppen kopieren..." << endl;
		//msg += Merge::MergeGruppen(db_alt, db_neu).run();
		//db_neu.commit_trans();

		// Update der Einträge in der Preislisten Tabelle, neue einfügen, alte updaten
		//cout << "Tabelle Preislisten kopieren..." << endl;
		//msg += Merge::MergePreislisten(db_alt, db_neu).run();
		//db_neu.commit_trans();

		// Update der Vortexte, neue einfügen, alte updaten
		//cout << "Vortexte kopieren..." << endl;
		//msg += Merge::MergeVortexte(db_alt, db_neu).run();
		//db_neu.commit_trans();

		// Hinweistexte updaten, neue einfügen
		cout << "Hinweistexte kopieren..." << endl;
		msg += Merge::MergeHinweistexte(db_alt, db_neu).run();
		db_neu.commit_trans();

		// Auslesen der PreislistenArtikel Tabelle und feststellen aller neuen Positionen
		// Neue Positionen bekommen Artikelnummer und Eintrag in Materialstamm und Preise Tabelle
		//cout << "Neue Artikel erzeugen..." << endl;
		//msg += Merge::MergeMaterialstamm(db_alt, db_neu).run();
		//db_neu.commit_trans();
		
		// Langtexte updaten, neue einfügen
		cout << "Langtexte kopieren..." << endl;
		msg += Merge::MergeLangtexte(db_alt, db_neu).run();
		db_neu.commit_trans();
		
		// Preislisten erzeugen, Neue SPL Dateien erstellen anhand der Tabelle CodeArtikelnummer_Zuordnung
		//cout << "SPL Dateien erzeugen..." << endl;
		//msg += Merge::CreateSpl(db_alt, db_neu).run();
		//cout << "Merge beendet" << endl;
	}
	catch(MxBase::Error e)
	{
		cout << "Ausführung abgebrochen. Fehler : " << e.get_description() << endl;
		MxUtil::Tracer::trace2file("error.txt", e.get_description());
	}
	if (msg.size())
		cout << msg << endl;

	Sleep(10000);
	return 0;
}

