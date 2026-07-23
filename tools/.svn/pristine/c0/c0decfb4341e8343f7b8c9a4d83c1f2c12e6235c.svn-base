#pragma once 


#include "PreislistenArtikel.h"
#include "Stueckliste.h"
#include "CheckTables.h"



namespace Action
{
	/**
	Alle Positionen in der Tabelle PreislistenArtikel, die die selbe Artikelnummer
	haben, müssen zu Komponenten umgesetzt werden. Die Artikelnummer wird auf 0 gesetzt
	und mit dem Code und der Artikelnummer wird ein Eintrag in der Stuecklisten Tabelle
	erstellt. 

	Die Artikelnummer wird aus der Preisetabelle entfernt, da sonst die Referenzielle Integrität
	nicht stimmt. Die Preise müssen über ein Preisupdate wieder eingespielt werden.
	*/
	class ConvertArtikel2Component
	{
	public:
		ConvertArtikel2Component(SpanxDb::Database& spanxdb, const CheckDoppelteArtnr::DoubleArtnrList& list) : artlist(list), db(spanxdb)
		{
		}

		void run()
		{
			/**
			Geht die Liste durch und erzeugt für jede Position deren Artikelnummer in der Liste
			vorkommt einen Eintrag in der Stuecklistentabelle. Danach wird die Artikelnummer zurück-
			gesetzt
			*/
			AltTabellen::PreislistenArtikel art(db, false);

			for (CheckDoppelteArtnr::DoubleArtnrList::iterator it = artlist.begin(); it != artlist.end(); it++)
			{
				std::string sqlwhere = boost::str(boost::format("ArtikelNummer = '%i'") % it->first);
				if (!art.start_fetch(sqlwhere))
					throw art.error;
				while (art.fetch_next())
				{
					create_stk_entry(art.get_col("code")->to_string(), it->first);
				}
			}
			reset_artnr();
		}

	private:
		void create_stk_entry(const std::string& code, int artnr)
		{
			/**
			Erzeugt einen Eintrag in der Stücklistentabelle (altes Format, wird später umgesetzt)
			*/
			std::string sqlwhere = "code = '" + code + "'";
			AltTabellen::Stuecklisten stk(db);
			stk.get_col("Code")->from_string(code);
			stk.get_col("Anzahl")->from_string("1");
			stk.get_col("Artikelnummer")->from_int(artnr);			
			if (!stk.insert())
				throw stk.error;
		}

		void reset_artnr()
		{
			/**
			Entfernt bei allen Positionen aus der Liste die Artikelnummer und löscht die
			Einträge in der Preistabelle
			*/
			for (CheckDoppelteArtnr::DoubleArtnrList::iterator it = artlist.begin(); it != artlist.end(); it++)
			{
				std::string sql = boost::str(boost::format("UPDATE old_PreislistenArtikel SET ArtikelNummer = 'NIL' WHERE ArtikelNummer = '%s'") % it->first);
				db.exec_sql(sql);
				if (db.get_errorhandler().is_critical())
					throw db.get_errorhandler().get_errors().begin();

				sql = boost::str(boost::format("DELETE FROM old_Preise WHERE artikelnr = %s") % it->first);
				db.exec_sql(sql);
				if (db.get_errorhandler().is_critical())
					throw db.get_errorhandler().get_errors().begin();
			}
		}

		SpanxDb::Database& db;
		
		/// Liste mit doppelten Artikelnummern
		CheckDoppelteArtnr::DoubleArtnrList artlist;
	};
}

