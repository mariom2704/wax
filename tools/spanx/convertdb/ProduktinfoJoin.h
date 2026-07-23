#pragma once

#include <Database.h>
#include <mxodbc2/Table.h>

namespace AltTabellen
{
	
	class ProduktinfoJoin : public MxOdbc::Table
	{
	public:
		ProduktinfoJoin(SpanxDb::Database& db) : MxOdbc::Table("PreislistenArtikel", db)
		{
			init_base();
			init_join();
		}

		bool load(const std::string& code)
		{
			if (!start_fetch("PreislistenArtikel.code ='" + code + "'"))
				return false;
			if (!fetch_next())
				return false;
			return true;
		}

		
	private:

		void init_base()
		{
			add_column<std::string>("code", true);
		}

		void init_join()
		{
			add_join<std::string>("angebotsblatt", "blatt");
			add_join<std::string>("produktinfo", "datei");
			add_join<std::string>("verkaufspreise", "doc");


			add_joined_keys("LEFT OUTER JOIN angebotsblatt ON PreislistenArtikel.code = angebotsblatt.code") ;
			add_joined_keys("LEFT OUTER JOIN produktinfo ON PreislistenArtikel.code = produktinfo.code") ;
			add_joined_keys("LEFT OUTER JOIN verkaufspreise ON PreislistenArtikel.code = verkaufspreise.code") ;
		}
	};
}