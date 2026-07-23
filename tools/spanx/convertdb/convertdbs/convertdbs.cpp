// ConvertDb.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>

// Aktionen
#include "../CodeArtikelNummerZuordnung.h"
#include "../ArtikelInsert.h"
#include "../CreateCodeArtikelnummerTable.h"
#include "../CopyLangtexte.h"

#include "../SplitPreislistenArtikelTable.h"
#include "../ConvertArtikel2Component.h"
#include "../CheckTables.h"
#include "../ConvertStueckliste.h"
#include "../UpdateInfo.h"
#include "../UpdateSparte.h"
#include "../UpdateHinweistexte.h"
#include "../UpdateMaterialstamm.h"
#include "../InitPreise.h"

#include "../CopyPreise.h"
#include "../CopyVerkaufsPreise.h"
#include "../CopyProduktinfo.h"
#include "../CopyVortexte.h"
#include "../ReplaceCodesInSpl.h"

#include <SystemParameter.h>
#include <mxutil/tracer.h>

#include "boost/progress.hpp"


using namespace std;

std::map<std::string, int> code_artnr_lst;

int main(int argc, char* argv[])
{
	if (argc != 3 && argc != 2)
	{
		cout << "convertdb [Spanxalt] [Spanxneu] " << endl;
		return 0;
	}
	boost::progress_timer t;

	SpanxDb::Database dbalt(argv[1]);
	SpanxDb::Database dbneu(argv[2]);
	
	try
	{
		if (!dbalt.open())
			throw dbalt.get_errorhandler();

		if (!dbneu.open())
			throw dbneu.get_errorhandler();
	
		string splpath = SpanxDb::SystemParameter(dbalt).val("DIR_SPL", "");
		cout << "SPL Pfad: " << splpath << endl;

		cout << "Lade CodeArtikelnummer Zuordnung...";
		// Lädt die CodeArtikelnummer Zuordnung und erstellt zwei Listen: Code->Artikelnummer und Code->Gruppe
		Action::CodeArtikelNummerZuordnung canz(dbneu);
		
		cout << endl << "Neue Artikel werden gesucht.." ;

		// Prüft die neue Datenbank, ob neue Artikel dazugekommen sind. Diese werden in eine Liste eingetragen.
		canz.detect_new_art(dbalt);
		cout << " gefunden: " << canz.newartlst.size() << endl;

		// Einträge, die nicht existieren werden nachgepflegt
		cout << endl << "Fehlende Eintraege im Materialstamm werden restauriert.." ;
		int ret = canz.restore_missing();
		cout << " gefunden: " << ret << endl;

		cout << "Neue Artikel werden eingefuegt";
		// Neue Artikel aus der alten DB einfügen. Entweder mit neuer Artikelnummer oder mit Eintrag in Stückliste 
		Action::ArtikelInsert(dbalt, dbneu).run(canz);

		cout << endl << "Langtexte werden upgedated..." ;
		Action::CopyLangtexte(dbalt, dbneu).run();
		
		cout << endl << "Vortexte werden upgedated..." ;
		Action::CopyVortexte(dbalt, dbneu).run();

		cout << endl << "Kopiere Verkaufspreise ..." ;
		Action::CopyVerkaufsPreise(dbalt, dbneu).run();

		cout << endl << "Erstelle Produktinfo ..." ;
		Action::UpdateInfo(dbalt, dbneu).run();

		cout << endl << "Setze Sparten ..." ;
		Action::UpdateSparte(dbalt, dbneu).run();

		cout << endl << "Hinweistexte werden upgedated ..." ;
		Action::UpdateHinweistexte(dbalt, dbneu).run();

		cout << endl<< "Wandle SPL Dateien um..." << endl;
		Action::ReplaceCodesInSpl(dbneu, splpath, splpath).run();

		cout << endl << "Initialisiere Preistabelle..." << endl;
		Action::InitPreise(dbalt, dbneu).run();

		cout << endl<< "Update Materialstamm..." << endl;
		// Update aller anderen Werte (WM, Posnr, Gewicht usw) aus alter Tabelle in neue Tabelle Materialstamm
		Action::UpdateMaterialstamm(dbalt, dbneu).run();

		cout  << endl << "Umsetzung beendet" << endl;
	}
	catch(MxBase::Error e)
	{
		dbneu.rollback_trans();
		cout << "Ausführung abgebrochen. Fehler : " << e.get_description() << endl;
		MxUtil::Tracer::trace2file("error.txt", e.get_description());
	}
	catch(MxBase::ErrorHandler e)
	{
		cout << "Ausführung abgebrochen. Fehler : " << e.get_errors().begin()->get_description() << endl;
		MxUtil::Tracer::trace2file("error.txt", e.get_errors().begin()->get_description());
	}

	Sleep(3000);
	return 0;
}

