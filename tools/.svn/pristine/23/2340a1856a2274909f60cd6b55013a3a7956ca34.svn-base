#pragma once

#include <Database.h>
#include <mxodbc2/Table.h>


namespace AltTabellen
{
	class Stuecklisten : public MxOdbc::Table
	{
	public:
		Stuecklisten(SpanxDb::Database& db, bool withjoin_codeartikelzuordung = false) : MxOdbc::Table("old_Stuecklisten", db)
		{
			add_column<std::string>("Code");
			add_column<std::string>("Anzahl");
			add_column<std::string>("Artikelnummer");
			if (withjoin_codeartikelzuordung)
				init_join();
		}

	private:
		void init_join()
		{
			add_join<std::string>("CodeArtikelnummer_Zuordnung", "Code", true);
			add_join<int>("CodeArtikelnummer_Zuordnung", "ArtNr");

			add_joined_keys("INNER JOIN CodeArtikelnummer_Zuordnung ON CodeArtikelnummer_Zuordnung.Code = old_Stuecklisten.code") ;
		}
	
	};
}