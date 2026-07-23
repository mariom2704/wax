#pragma once

#include "BaseAction.h"
#include <Stueckliste.h>
#include <Preise.h>
#include "Preise.h"
#include <map>


namespace Action
{
	/**

	*/
	class InitPreise : public BaseAction
	{
	public:
		InitPreise(SpanxDb::Database& altdb, SpanxDb::Database& neudb) : BaseAction(altdb, neudb)
		{
			sqlselect = "code not like 'vortext_%'";
		}

	private:

		void update_preise(int artnr, AltTabellen::PreislistenArtikel& pa)
		{
			// Preise aus alter Tabelle laden
			AltTabellen::Preise altpreis(dbalt);

			std::map<int, double> preise;

			if (!altpreis.start_fetch("code = '" + pa.get_code() + "'"))
				throw altpreis.error;
			
			while (altpreis.fetch_next())
				preise.insert(std::make_pair<int, double>(altpreis.get_region(), altpreis.get_preis()));

			int aartnr = atoi(pa.get_ArtikelNummer().c_str());
			if (aartnr > 0)
			{
				if (!altpreis.start_fetch("artikelnr = " + pa.get_ArtikelNummer()))
					throw altpreis.error;
				
				while (altpreis.fetch_next())
					preise.insert(std::make_pair<int, double>(altpreis.get_region(), altpreis.get_preis()));
			}

			// Preise in neuer Datenbank übernehmen
			SpanxDb::PreiseTab preistab(dbneu);

			if (!preistab.load(artnr))
				throw preistab.error;

			while (preistab.fetch_next())
			{
				std::map<int, double>::iterator it = preise.find(preistab.get_region());
				if (it == preise.end())
					continue;
				preistab.set_preis(it->second);
				preise.erase(it);
			}

			for (std::map<int, double>::iterator it = preise.begin(); it != preise.end(); it++)
			{
				preistab.start_fetch();
				preistab.set_artikelnr(artnr);
				preistab.set_preis(it->second);
				preistab.set_region(it->first);
				if (!preistab.insert())
					throw preistab.error;
			}
		}

		bool has_stk(int artnr, AltTabellen::PreislistenArtikel& pa)
		{
			int oldartnr = atoi(pa.get_ArtikelNummer().c_str());

			SpanxDb::Stueckliste stk(dbneu);
			
			if (!stk.load(artnr))
				throw stk.error;

			return stk.fetch_next();
		}

		
	protected:

		

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			std::string code = pa.get_code();
			int artnr = act_caz.artnr_from_code(code);

			if (!artnr)
				return;

			if (has_stk(artnr, pa)) 
				return;

			/// Daten in Tabelle Preise updaten
			update_preise(artnr, pa);

		}
	};
}