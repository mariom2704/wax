#pragma once

#include <Database.h>

#include <string>

#include "../TmpTabellen.h"
#include <map>

namespace Merge
{
	class MergeBase
	{
	public:
		MergeBase(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
			: db_alt(spanxdb_alt), db_neu(spanxdb_neu)
		{
			update_counter = 0;
			insert_counter = 0;
		}

		virtual std::string run() = 0;

	protected:
		SpanxDb::Database& db_alt;
		SpanxDb::Database& db_neu;

		std::string info;

		int update_counter;
		int insert_counter;

		typedef std::map<std::string, int> C2A;
		C2A c2a;


		void load_cache()
		{
			TmpTabellen::CodeArtikelnummer_Zuordnung c2atab(db_neu);
			if (!c2atab.start_fetch())
				throw c2atab.error;

			while (c2atab.fetch_next())
				c2a.insert(std::make_pair<std::string, int>(c2atab.get_Code(), c2atab.get_ArtNr()));
		}

		int search_cache(const std::string& code)
		{
			C2A::iterator it = c2a.find(code);
			if (it == c2a.end())
				return 0;
			return it->second;
		}
	};
}