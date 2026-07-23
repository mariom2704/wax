#pragma once 

#include "ProduktinfoJoin.h"
#include <Produktinfo.h>

namespace Action
{
	/**
	Alle Einträge in der Tabelle old_Stueckliste werden umgesetzt in die neue Tabelle
	Stueckliste.
	Die Codes werden in Artikelnummern umgesetzt.
	*/
	class CopyProduktinfo
	{
	public:
		CopyProduktinfo(SpanxDb::Database& spanxdb) : db(spanxdb)
		{
			
		}

	
		void run()
		{
			reset();

			copy();
		}

	private:
		void reset()
		{
			db.exec_sql("DELETE * FROM Produktinfo");
		}
			
		void copy()
		{
			AltTabellen::ProduktinfoJoin p(db);
			if (!p.start_fetch())
				throw p.error;

			while (p.fetch_next())
			{
				MxOdbc::BaseCol* datei = p.get_joined_col("old_Produktinfo", "datei");
				MxOdbc::BaseCol* blatt = p.get_joined_col("old_Angebotsblatt", "blatt");
				MxOdbc::BaseCol* doc = p.get_joined_col("old_Verkaufspreise", "doc");

				
				if (datei->is_null()
					&& blatt->is_null()
					&& doc->is_null())
					continue;
				

				SpanxDb::Produktinfo pneu(db);
				pneu.get_col("Materialstamm_ArtNr")->from_int(p.get_col("ArtNr")->to_int());
				pneu.get_col("Info")->from_string(datei->is_null() ? "-" : datei->to_string());
				pneu.get_col("Angebotsblatt")->from_string(blatt->is_null() ? "-" : blatt->to_string());
				pneu.get_col("Kalkulationsleitfaden")->from_string(doc->is_null() ? "-" : doc->to_string());
				if (!pneu.insert())
					throw pneu.error;
			}
			if (p.error.is_critical())
				throw p.error;

		}

	
		SpanxDb::Database& db;
	
	};
}

