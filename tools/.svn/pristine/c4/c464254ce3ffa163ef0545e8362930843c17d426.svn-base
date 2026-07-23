#pragma once

#include <Database.h>
#include <mxodbc2/Table.h>

namespace AltTabellen
{
	
	class Preise : public MxOdbc::Table
	{
	public:
		Preise(SpanxDb::Database& db, bool withjoin_codeartikelzuordung = false) : MxOdbc::Table("Preise", db)
		{
			init_base();

			if (withjoin_codeartikelzuordung)
				init_join();

		}

		GETSET_INT(region);
		GETSET_DOUBLE(preis);

	private:

		void init_join()
		{
			add_join<std::string>("CodeArtikelnummer_Zuordnung", "code", true);
			add_join<int>("CodeArtikelnummer_Zuordnung", "ArtNr");

			add_joined_keys("INNER JOIN CodeArtikelnummer_Zuordnung ON CodeArtikelnummer_Zuordnung.Code = old_Preise.code") ;
		}

		void init_base()
		{
			add_column<std::string>("code");
			add_column<int>("artikelnr");
			add_column<int>("region");
			add_column<double>("preis");
		}
	};


}