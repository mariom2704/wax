#pragma once

#include "PreislistenArtikel.h"

namespace Action
{
	class CheckDoppelteArtnr
	{
	public:

		CheckDoppelteArtnr(SpanxDb::Database& spanxdb) : db(spanxdb)
		{
		}

		void run()
		{
			AltTabellen::PreislistenArtikel art(db, false);

			if (!art.start_fetch())
				throw art.error;
			
			while (art.fetch_next())
			{
				int a = art.get_col("ArtikelNummer")->to_int();
				std::map<int, int>::iterator it = artcounter.find(a);
				if (it != artcounter.end())
					it->second += 1;
				else if (a)
				{
					artcounter.insert(std::make_pair(a, 1));
				}
			}
		}

		typedef std::map<int, int> DoubleArtnrList;

		DoubleArtnrList get_list() const
		{
			DoubleArtnrList lst;
			for (DoubleArtnrList::const_iterator it = artcounter.begin(); it != artcounter.end(); it++)
			{
				if (it->second > 1)
					lst.insert(*it);
			}
			return lst;
		}

		std::string evaluate() const
		{
			std::string ret;
			for (DoubleArtnrList::const_iterator it = artcounter.begin(); it != artcounter.end(); it++)
			{
				if (it->second > 1)
				{
					ret += boost::str(boost::format("ArtNr: %i : %i mal \n") % it->first % it->second);
				}
			}
			return ret;
		}

	private:
		SpanxDb::Database& db;
		DoubleArtnrList artcounter;
	};















}