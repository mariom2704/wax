#pragma once

#include "PreislistenArtikel.h"
#include "TmpTabellen.h"

namespace Action
{

	class CreateCodeArtikelnummerTable
	{
	public:

		enum Nummernbereich
		{
			MIN_OD = 20000,
			MAX_OD = 99999,
			MIN_NL = 0,
			MAX_NL = 9999,
		};

		CreateCodeArtikelnummerTable(SpanxDb::Database& spanxdb, bool od_master, SpanxDb::Database* ref) : db(spanxdb), is_od(od_master), refdb(ref)
		{
			artnr = is_od ? MIN_OD : MIN_NL;	
		}

		void reset()
		{
			db.exec_sql("DELETE * FROM CodeArtikelnummer_Zuordnung");
		}

		void prepare()
		{
			AltTabellen::PreislistenArtikel art(db, false);
			TmpTabellen::CodeArtikelnummer_Zuordnung caz(db);

			if (!art.start_fetch())
				throw art.error;
			
			while (art.fetch_next())
			{
				std::string c = art.get_code();
#ifdef _DEBUG
				art.dump();
#endif
				std::string a = art.get_ArtikelNummer();
				caz.get_col("ArtNr")->from_string(a);
				caz.set_Code(c);
				caz.set_Gruppe(art.get_Gruppe());
				if (!caz.insert())
					throw caz.error;
			}
		}

		void set_artnr()
		{
			TmpTabellen::CodeArtikelnummer_Zuordnung caz(db);

			if (!caz.start_fetch("ArtNr < 20000 OR ArtNr = NULL"))
				throw caz.error;

			while (caz.fetch_next())
			{
				caz.get_col("ArtNr")->from_int(artnr++);
				if (!caz.update())
					throw caz.error;
			}
		}

		void process_refdb()
		{
			/**
			Lädt jeden Eintrag in der CodeArtikelnummer Tabelle und prüft in der Referenzdb
			ob bereits eine Nummer vergeben wurde. Wenn ja, wird diese übernommen und der Artikelnummern
			zähler auf diese Nummer gesetzt, damit neue Positionen keine bereits vergebenen Nummern erhalten.
			*/
			if (!refdb)
				return;
			
			TmpTabellen::CodeArtikelnummer_Zuordnung caz(db);
			if (!caz.start_fetch("ArtNr < 100000 OR ArtNr = NULL"))
				throw caz.error;

			while (caz.fetch_next())
			{
				TmpTabellen::CodeArtikelnummer_Zuordnung refcaz(*refdb);
				std::string sql = "Code = '" + caz.get_col("Code")->to_string() + "'";
				if (!refcaz.start_fetch(sql))
					continue;

				if (!refcaz.fetch_next())
					continue;

				int refartnr = refcaz.get_col("ArtNr")->to_int();
				caz.get_col("ArtNr")->from_int(refartnr);
				if (refartnr > artnr)
					artnr = refartnr+1;

				if (!caz.update())
					throw caz.error;
			}

		}

		void run()
		{
			prepare();
			
			// Bei Artikeln, die in der Stücklistendb bereits eine Nr. haben, wird diese übernommen
			process_refdb();

			set_artnr();
		}

	private:
		SpanxDb::Database& db;
		SpanxDb::Database* refdb;
		bool is_od;
		int artnr;
	};
}