#pragma once 

#include "Preise.h"
#include <Preise.h>

namespace Action
{
	/**
	Alle Einträge in der Tabelle old_Stueckliste werden umgesetzt in die neue Tabelle
	Stueckliste.
	Die Codes werden in Artikelnummern umgesetzt.
	*/
	class CopyPreise
	{
	public:
		CopyPreise(SpanxDb::Database& spanxdb) : db(spanxdb)
		{
			
		}

	
		void run()
		{
			reset();

			copy_with_artnr();

			copy_without_artnr();
		}

	private:
		void reset()
		{
			db.exec_sql("DELETE * FROM Preise");
		}
			
		void copy_with_artnr()
		{
			/**
			Kopiert alle Einträge, die bereits eine Artikelnummer haben in die neue Tabelle
			*/
			AltTabellen::Preise p(db, false);
			if (!p.start_fetch("artikelnr > 0"))
				throw p.error;

			while (p.fetch_next())
			{
				SpanxDb::PreiseTab pneu(db);
				pneu.get_col("Materialstamm_ArtNr")->from_int(p.get_col("artikelnr")->to_int());
				pneu.get_col("Region")->from_int(p.get_col("region")->to_int());
				pneu.get_col("Preis")->from_double(p.get_col("preis")->to_double());
				if (!pneu.insert())
					continue;
			}
			if (p.error.is_critical())
				throw p.error;

		}

		void copy_without_artnr()
		{
			/**
			Kopiert alle Einträge in die neue Tabelle, die einen Code haben.
			Die Artikelnummer wird über einen Inner Join auf die Tabelle CodeArtikelnummer_Zuordnung
			aufgelöst.
			*/
			AltTabellen::Preise p(db, true); // Mit Inner Join
			if (!p.start_fetch("artikelnr = 0"))
				throw p.error;

			while (p.fetch_next())
			{
				SpanxDb::PreiseTab pneu(db);
				pneu.get_col("Materialstamm_ArtNr")->from_int(p.get_joined_col("CodeArtikelnummer_Zuordnung", "ArtNr")->to_int());
				pneu.get_col("Region")->from_int(p.get_col("region")->to_int());
				pneu.get_col("Preis")->from_double(p.get_col("preis")->to_double());
				if (!pneu.insert())
					continue;
			}
			if (p.error.is_critical())
				throw p.error;
		}

		SpanxDb::Database& db;
	
	};
}

