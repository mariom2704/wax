#pragma once 

#include "Stueckliste.h"
#include <Stueckliste.h>

namespace Action
{
	/**
	Alle Einträge in der Tabelle old_Stueckliste werden umgesetzt in die neue Tabelle
	Stueckliste.
	Die Codes werden in Artikelnummern umgesetzt.
	*/
	class ConvertStueckliste
	{
	public:
		ConvertStueckliste(SpanxDb::Database& spanxdb, SpanxDb::Database* ref ) : db(spanxdb), refdb(ref)
		{
			
		}

	
		void run()
		{
			reset();

			AltTabellen::Stuecklisten ostk(db, true);

			if (!ostk.start_fetch("", "CodeArtikelnummer_Zuordnung.ArtNr"))
				throw ostk.error;

			int martnr = 0;
			int lfdnr = 0;
			while (ostk.fetch_next())
			{
				int oldnr = martnr;
				martnr = ostk.get_joined_col("CodeArtikelnummer_Zuordnung", "ArtNr")->to_int();
				if (oldnr == martnr)
					lfdnr++;
				else
					lfdnr = 0;
				int partnr = ostk.get_col("Artikelnummer")->to_int();
				double anz = ostk.get_col("Anzahl")->to_double();
				create_stk_entry(martnr, partnr, lfdnr, anz);
			}
		}

		void synchronize()
		{
			/**
			Übernimmt die Stücklisten aus der neuen DB, falls vorhanden
			*/
			if (!refdb)
				return;


			SpanxDb::Materialstamm mat(db);

			if (!mat.start_fetch())
				throw mat.error;

			SpanxDb::Stueckliste stk(*refdb);
			SpanxDb::Stueckliste thisstk(db);

			while (mat.fetch_next())
			{
				std::string search = boost::str(boost::format("Materialstamm_ArtNr = %i") % mat.get_artnr() );
				if (!stk.start_fetch(search) || !stk.fetch_next())
					continue;	

				db.exec_sql("DELETE * FROM Stueckliste WHERE " + search);

				do 
				{
					thisstk.copy_data(mat.get_artnr(), stk);
					thisstk.insert();
				} while (stk.fetch_next());

			}
		}

	private:
		void reset()
		{
			db.exec_sql("DELETE * FROM Stueckliste");
		}
			
		void create_stk_entry(int Materialstamm_ArtNr, int Preise_Materialstamm_ArtNr, int lfdNr, double Anzahl)
		{
			/**
			Erzeugt einen Eintrag in der Stücklistentabelle, neues Format
			*/

			if (Preise_Materialstamm_ArtNr <= 100)
				return;
			SpanxDb::Stueckliste stk(db);
			stk.get_col("Materialstamm_ArtNr")->from_int(Materialstamm_ArtNr);
			stk.get_col("Preise_Materialstamm_ArtNr")->from_int(Preise_Materialstamm_ArtNr);
			stk.get_col("lfdNr")->from_int(lfdNr);	
			stk.get_col("Anzahl")->from_double(Anzahl);
			if (!stk.insert())
				throw stk.error;
		}

		SpanxDb::Database& db;
		SpanxDb::Database* refdb;
	
	};
}

