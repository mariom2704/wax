#pragma once

#include "MergeBase.h"

#include "../PreislistenArtikel.h"

// Tabellen aus SpanxDb, neues Datenbankformat
#include <Langtexte.h>
#include <ArtikelNummer.h>
#include <SystemParameter.h>



namespace Merge
{
	/**
	Alle Einträge in der Tabelle old_Stueckliste werden umgesetzt in die neue Tabelle
	Stueckliste. Dabei wird die Artikelnummer im Materialstamm aus der Tabelle CodeArtikelnummer_Zuordnung
	ermittelt.
	*/
	class MergeLangtexte : public MergeBase
	{
	public:
		MergeLangtexte(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
			: MergeBase(spanxdb_alt, spanxdb_neu)
		{
			counter = 0;
			std::string stdb = SpanxDb::SystemParameter(db_neu).val("STAMMDB", "");
			stammdb = new SpanxDb::Database(stdb);
			stammdb->open();
		}

		virtual ~MergeLangtexte()
		{
			delete stammdb;
		}

		std::string run()
		{
			// Cache mit Artikelnummer-Code Zuordnung erstellen
			load_cache(); 
			MxBase::Locale::LocaleVec lv = MxBase::Locale().query_supported_locales(MxBase::Locale()); // Muss Deutsch an erster Stelle liefern
			
			for (MxBase::Locale::LocaleVec::iterator it = lv.begin(); it != lv.end(); it++)
			{
				MxBase::Locale l(it->second);
				update_langtexte(l);
			}

			if (info.size())
				info = boost::str(boost::format("Fehlende Artikel:\n%s\ninsgesamt %i Artikel\n") % info % counter);
		
			return info;
		}

	private:

		void update_langtexte(MxBase::Locale l)
		{
			AltTabellen::PreislistenArtikelLangtext part(db_alt, l);

			if (!part.start_fetch("code not like 'vortext%'"))
				throw part.error;

			counter = 0;
			while (part.fetch_next())
			{
				int artnr = search_cache(part.get_code());

				if (!artnr)
				{
					info += part.get_code() + "\n";
					counter++;
					continue;
				}

				// Je nach Artikelnummer liegen die Langtexte in der Spanx Neu DB oder der Stammdatenbank
				SpanxDb::Database* ltdb = SpanxDb::ArtikelNummer(artnr).from_stammdb(0) ? stammdb : &db_neu;

				SpanxDb::Langtexte lt(*ltdb, l);
				if (!lt.load(artnr))
					throw lt.error;

				bool update = lt.fetch_next();

				lt.set_langtext(part.get_langtext());
				lt.set_lvtext(part.get_lvtext());
				
				if (update)
				{
					if (!lt.update())
						throw lt.error;
				}
				else 
				{
					std::string msg = boost::str(boost::format("Inserted in [Langtexte]: %i\n") % artnr);
					info += msg;
					lt.set_artnr(artnr);
					if (!lt.insert())
					{
						std::string msg = boost::str(boost::format("Kein Eintrag in Materialstamm: %i\n") % artnr);
						info += msg;
					}
						
					//if (!lt.insert())
					//	throw lt.error;
				}
				
			}
			if (part.error.is_critical())
				throw part.error;
		}

		int counter;

		SpanxDb::Database* stammdb;
		
	};
}