#pragma once

#include "TmpTabellen.h"
#include "PreislistenArtikel.h"
#include <Materialstamm.h>


namespace Action
{
	class CodeArtikelNummerZuordnung
	{
	public:

		CodeArtikelNummerZuordnung(SpanxDb::Database& db) : stammdb(db)
		{
			load();
		}

		void load()
		{
			codelst.clear();
			grplst.clear();

			TmpTabellen::CodeArtikelnummer_Zuordnung cod(stammdb);

			if (!cod.start_fetch())
				throw cod.error;

			while (cod.fetch_next())
			{
				codelst.insert( std::make_pair<std::string, int>(cod.get_Code(), cod.get_ArtNr()) );
				grplst.insert( std::make_pair<int, std::string>(cod.get_ArtNr(), cod.get_Gruppe()) );
			}
		}

		int artnr_from_code(const std::string& code)
		{
			std::map<std::string, int>::iterator it = codelst.find(code);
			if (it != codelst.end())
				return it->second;

			it = newartlst.find(code);
			if (it != newartlst.end())
				return it->second;

			return 0;
		}

		void detect_new_art(SpanxDb::Database& dbalt)
		{
			AltTabellen::PreislistenArtikel spl(dbalt, false);

			if (!spl.start_fetch("code not like 'vortext%'"))
				throw spl.error;

			while (spl.fetch_next())
			{
				std::string code = spl.get_code();
				int artnr = atoi(spl.get_ArtikelNummer().c_str());
				if (codelst.find(code) == codelst.end())
				{
					newartlst.insert(std::make_pair<std::string, int>(code, artnr));
				}
			}
		}

		int restore_missing()
		{
			int missing = 0;
			std::map<std::string, int>::iterator it;

			SpanxDb::Materialstamm matstamm(stammdb);
			
			for (it = codelst.begin(); it != codelst.end(); it++)
			{
				if (matstamm.load(it->second))
					continue;
				matstamm.set_artnr(it->second);
				if (!matstamm.insert())
					throw matstamm.error;
				missing++;
			}
			return missing;
		}

		std::map<std::string, int> codelst;

		std::map<int, std::string> grplst;

		std::map<std::string, int> newartlst;

	private:
		SpanxDb::Database& stammdb;


	};
}