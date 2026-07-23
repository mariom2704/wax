#include "stdafx.h"

#include "MergeMaterialstamm.h"
#include <MaxArtikelnrQuery.h>
#include <ArtikelNummer.h>
#include <Materialstamm.h>
#include <Stueckliste.h>
#include <SystemParameter.h>

using namespace Merge;
using namespace std;

MergeMaterialstamm::MergeMaterialstamm(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu)
	: MergeBase(spanxdb_alt, spanxdb_neu)
{
	load_cache();

	string stdb = SpanxDb::SystemParameter(db_neu).val("STAMMDB", "");
	stammdb = new SpanxDb::Database(stdb);
	stammdb->open();
}

MergeMaterialstamm::~MergeMaterialstamm()
{
	delete stammdb;
}

string MergeMaterialstamm::run()
{
	process_artikel();
	return boost::str(boost::format("%s\r\n%i neue Positionen\r\n") % info % insert_counter );
}

void MergeMaterialstamm::insert_artikel(AltTabellen::PreislistenArtikel& art)
{
	/**
	Neue Positionen erzeugen. Folgende Fälle sind zu betrachten:

	1. Neue Position hat Artikelnummer, Artikelnummer gibt es noch nicht im Materialstamm der Stammdb
		-> Eintrag in Materialstamm der Stammdb (SAP Artikel). 

	2. Neue Position hat Artikelnummer, Artikelnummer ist bereits in Stammdb hinterlegt. 
		-> Es wird eine neue Artikelnummer im Materialstamm der neuen Spanxdb erzeugt. Dazu wird ein 
		Eintrag in der Stückliste mit der bereits vorhandenen Artikelnummer angelegt.

	3. Neue Position hat keine Artikelnummer. 
		-> Es wird ein neuer Eintrag im Materialstamm der Spanxdb erzeugt. Der eingetragegene Preis wird
		als Eintrag in der Preistabelle hinterlegt

	Jede Artikelnummer, die nicht im SAP Bereich liegt wird ignoriert. Preise für SAP Artikelnummern
	werden vom Preisupdate eingepflegt.
	*/
	int artnr = 0;

	artnr = extract_artnr(art.get_ArtikelNummer());
	

	if (SpanxDb::ArtikelNummer(artnr).from_sap())
	{
		// 
		if (!exists(artnr))
		{
			create_sap_artnr(artnr); // Neuer Eintrag in Materialstamm in Stammdatenbank
		}
		else
		{
			// Komponente anlegen mit Eintrag in Stückliste
			int artnrsap = artnr;
			artnr = create_artnr(art.get_code()); // Neue Artikelnummer in Materialstamm mit SAP-Eintrag in Stückliste
			create_stk_entry(artnr, artnrsap);
		}
	}
	else
	{
		artnr = create_artnr(art.get_code()); // Neue Artikelnummer in Materialstamm, keine SAP Nummer
	}

	update_val(artnr, art);

	info += boost::str(boost::format("Neu: %s->%i\n") % art.get_code() % artnr);
	insert_counter++;
}

void MergeMaterialstamm::update_val(int artnr, AltTabellen::PreislistenArtikel& art)
{
	/**
	Werte im Materialstamm (Kurztext etc.) updaten. Artikelnummer wurde vorher erzeugt. 
	*/
	SpanxDb::Database& db = get_correct_db(artnr);

	SpanxDb::Materialstamm matstamm(db, MxBase::Locale());

	if (!matstamm.load(artnr))
		throw matstamm.error;

	if (!matstamm.fetch_next())
	{
		std::string err = boost::str(boost::format("Position nicht im Materialstamm gefunden: Artnr: %i, Code %s") % artnr % art.get_code());
		throw MxBase::Error(err, MxBase::Error::Critical);
	}

	matstamm.set_kurztext(art.get_KurzText());
	

	if (!matstamm.update())
		throw matstamm.error;

}

bool MergeMaterialstamm::exists(int artnr)
{
	SpanxDb::Database& db = get_correct_db(artnr);

	SpanxDb::MaterialstammKurz matstamm(db, MxBase::Locale());

	if (!matstamm.load(artnr))
		throw matstamm.error;

	return matstamm.fetch_next();
}

void MergeMaterialstamm::create_stk_entry(int artnr_komp, int artnr)
{
	SpanxDb::Stueckliste stk(db_neu);
	if (!stk.start_fetch())
		throw stk.error;

	stk.set_materialstamm_artnr(artnr_komp);
	stk.set_artikelnr(artnr);
	stk.set_nr(1);
	stk.set_anzahl(1);
	if (!stk.insert())
		throw stk.error;
}

int MergeMaterialstamm::extract_artnr(const std::string& artnr) const
{
	int nr = atoi(artnr.c_str());
	if (nr < 100000)
		return 0;
	return nr;
}

SpanxDb::Database& MergeMaterialstamm::get_correct_db(int artnr) const
{
	if (SpanxDb::ArtikelNummer(artnr).from_sap())
	{
		return *stammdb;
	}
	return db_neu;
}

int MergeMaterialstamm::create_artnr(const std::string& code)
{
	SpanxDb::MaxArtikelNrQuery maxnr(db_neu, true, false);

	int nr = maxnr.get_new_and_create();

	cache_artnr(nr, code);

	return nr;
}

void MergeMaterialstamm::cache_artnr(int artnr, const std::string& code)
{
	c2a.insert(make_pair<string, int>(code, artnr));
	// Eintrag in C2A Tabelle und Cache

	TmpTabellen::CodeArtikelnummer_Zuordnung c2atab(db_neu);
	if (!c2atab.start_fetch())
		throw c2atab.error;

	c2atab.set_Code(code);
	c2atab.set_ArtNr(artnr);

	if (!c2atab.insert())
		throw c2atab.error;
}

void MergeMaterialstamm::create_sap_artnr(int anr)
{
	std::string sql = boost::str(boost::format("INSERT INTO Materialstamm (ArtNr) VALUES (%i)") % anr);
	SpanxDb::Database& db = get_correct_db(anr);
	
	db.exec_sql(sql);
	if (db.get_errorhandler().get_count())
		throw *db.get_errorhandler().get_errors().begin();
	
	db.commit_trans();
}

void MergeMaterialstamm::process_artikel()
{
	AltTabellen::PreislistenArtikel art(db_alt, false, "PreislistenArtikel");

	if (!art.start_fetch("code not like 'vortext%'"))
		throw art.error;
	
	while (art.fetch_next())
	{
		C2A::iterator it = c2a.find(art.get_code());
		if (it != c2a.end())
			continue; // Code ist bereits umgesetzt

		insert_artikel(art);
	}
}
