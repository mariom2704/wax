#pragma once

#include "PreislistenArtikel.h"
#include "TmpTabellen.h"
#include "Materialstamm.h"
#include <Gruppen.h>

#include <map>

namespace Action
{
	class SplitPreislistenArtikelTable
	{
	private:
		std::map<std::string, int> spartenzuordnung;

	public:
		SplitPreislistenArtikelTable(SpanxDb::Database& spanxdb) : db(spanxdb)
		{
			load_spartenzuordnung();
		}

		void run()
		{
			reset();

			AltTabellen::PreislistenArtikel pa(db, true);

			if (!pa.start_fetch())
				throw pa.error;

			while (pa.fetch_next())
			{
				MxOdbc::Table::set_loc(MxBase::Locale());
				std::string c = pa.get_col("code")->to_string();
				std::string a = pa.get_col("ArtikelNummer")->to_string();
				std::string l = pa.get_col("LangText")->to_string();
				int artnr = pa.get_joined_col("CodeArtikelnummer_Zuordnung", "ArtNr")->to_int();

				SpanxDb::Materialstamm mat(db);

				mat.get_col("ArtNr")->from_int(artnr);
				std::string deftxt = pa.get_col("KurzText")->to_string();
				mat.set_kurztext(deftxt);
				mat.get_col("Gewicht")->from_string(pa.get_col("gewicht")->to_string());
				mat.get_col("Montagezeit")->from_string(pa.get_col("Montagezeit")->to_string());
				mat.get_col("WM")->from_string(pa.get_col("WM")->to_string());
				mat.get_col("WL")->from_string(pa.get_col("WL")->to_string());
				mat.get_col("Hinweis")->from_string(pa.get_col("Hinweis")->to_string());
				mat.get_col("Posnr")->from_string(pa.get_col("posnr")->to_string());
				mat.get_col("Leistung")->from_int(pa.get_col("IstFremdleistung")->to_int());
				mat.get_col("IstSprinkler")->from_int(pa.get_col("IstSprinkler")->to_int());
				//mat.get_col("IstGeaendert")->from_int(pa.get_col("IstGeaendert")->to_int());
				//mat.get_col("IstNeu")->from_int(pa.get_col("IstNeu")->to_int());
				//mat.get_col("IstGeloescht")->from_int(pa.get_col("IstGeloescht")->to_int());
				mat.get_col("GeaendertVon")->from_string(pa.get_col("GeaendertVon")->to_string());
				mat.get_col("GeaendertAm")->from_string(pa.get_col("GeaendertAm")->to_string());

				std::map<std::string, int>::iterator it = spartenzuordnung.find(pa.get_joined_col("CodeArtikelnummer_Zuordnung", "Gruppe")->to_string());
				if (it != spartenzuordnung.end())
					mat.set_sparte(it->second);

				if (!mat.insert())
					throw mat.error;

				const int nloc = 3;
				MxBase::Locale locs[nloc] = 
				{
					MxBase::Locale(MxBase::Locale::ENGLISH),
					MxBase::Locale(MxBase::Locale::FRANCAIS),
					MxBase::Locale(MxBase::Locale::NEDERLANDS)
				};

				for (int i = 0; i < nloc; i++)
				{

					MxOdbc::Table::set_loc(locs[i]);
					SpanxDb::Materialstamm l(db);
					l.set_artnr(artnr);
					if (!pa.get_col("KurzText" +  locs[i].get_locale_dbstr())->is_null())
						l.set_kurztext(pa.get_col("KurzText" +  locs[i].get_locale_dbstr())->to_string());
					else
						l.set_kurztext(deftxt);
					if (!l.update())
						throw l.error;
				}
			}
			if (pa.error.is_critical())
				throw pa.error;
		}

		void copy_dummy_material(SpanxDb::Database* refdb)
		{
			if (!refdb)
				return;

			SpanxDb::Materialstamm mat(*refdb, MxBase::Locale());
			SpanxDb::Materialstamm mat2(db, MxBase::Locale());

			std::string sql = "ArtNr < 20000";
			if (!mat.start_fetch(sql))
				throw mat.error;

			while (mat.fetch_next())
			{
				mat2.copy_data(mat.get_artnr(), mat);
				if (!mat2.insert())
					throw mat2.error;
			}
		}

	private:
		void reset()
		{
			db.exec_sql("DELETE * FROM Materialstamm");
			MxOdbc::Table::set_loc(MxBase::Locale());
		}

		void load_spartenzuordnung() 
		{
			SpanxDb::Gruppen grp(db);

			if (!grp.start_fetch())
				throw grp.error;

			while (grp.fetch_next())
			{
				spartenzuordnung.insert(std::make_pair<std::string, int>(grp.get_code(), grp.get_sparte()) );
			}
		}

		
		SpanxDb::Database& db;
	};
}