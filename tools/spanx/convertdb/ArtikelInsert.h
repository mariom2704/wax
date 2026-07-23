#pragma once

#include "CodeArtikelNummerZuordnung.h"
#include <Materialstamm.h>
#include <Stueckliste.h>
#include <Preise.h>
#include <MaxArtikelnrQuery.h>
#include <ArtikelNummer.h>
#include "TmpTabellen.h"

namespace Action
{
	class ArtikelInsert
	{
	public:

		ArtikelInsert(SpanxDb::Database& dbalt, SpanxDb::Database& dbneu) : dba(dbalt), dbn(dbneu)
		{
		}

		void run(Action::CodeArtikelNummerZuordnung& codezuordnung)
		{
			/**
			Neue Positionen einfügen: 
			Ohne Artikelnummer
				-> Neue Artikelnummer zuordnen
				-> weiter wie mit Artikelnummer

			Mit Artikelnummer
				-> SAP Artikelnummer gibt es noch nicht im Materialstamm, dann neuen Eintrag im Materialstamm
				mit dieser Artikelnummer
				-> SAP Artikelnummer gibt es bereits, dann neue Artikelnummer zuordnen und mit der 
				SAP Artikelnummer einen Eintrag in der Stückliste erzeugen
				-> Keine SAP Artikelnummer, Eintrag in Preistabelle vornehmen

			*/
			SpanxDb::MaxArtikelNrQuery qartnr(dbn, true, false);

			for (std::map<std::string, int>::iterator it = codezuordnung.newartlst.begin(); it != codezuordnung.newartlst.end(); it++)
			{
				if (!SpanxDb::ArtikelNummer(it->second).from_sap())
				{
					it->second = qartnr.get_new_and_create();
					insert_in_CodeArtikelNummerZuordnung(it->second, it->first);
				}				
			}


			SpanxDb::Materialstamm matstamm(dbn);
			SpanxDb::Stueckliste stk(dbn);
			SpanxDb::PreiseTab preise(dbn);

			stk.start_fetch();

			for (std::map<std::string, int>::iterator it = codezuordnung.newartlst.begin(); it != codezuordnung.newartlst.end(); it++)
			{
				if (!SpanxDb::ArtikelNummer(it->second).from_sap())
					continue; // Alle nicht SAP Nummern müssen bereits erzeugt sein

				bool existing = matstamm.load(it->second);

				if (!existing)
				{
					matstamm.set_artnr(it->second);
					matstamm.set_kurztext("-");
					matstamm.set_hinweis("-");
					matstamm.set_posnr("-");
					matstamm.set_geaendert_am("-");
					matstamm.set_geaendert_von("-");
					if (!matstamm.insert())
						throw matstamm.error;
				}
				else
				{
					// SAP Nummer gibt es bereits
					int sapnr = it->second;

					// Neue Artikelnummer erzeugen und dann einen Eintrag mit der SAP Nummer in der Stückliste
					it->second = qartnr.get_new_and_create();
					insert_in_CodeArtikelNummerZuordnung(it->second, it->first);
					
					stk.set_materialstamm_artnr(it->second);
					stk.set_artikelnr(sapnr);
					stk.set_nr(0);
					stk.set_anzahl(1);
					if (!stk.insert())
						throw stk.error;
				}
			}

		}

	private:

		void insert_in_CodeArtikelNummerZuordnung(int artnr, const std::string& code)
		{
			TmpTabellen::CodeArtikelnummer_Zuordnung caz(dbn);
			if (!caz.start_fetch())
				throw caz.error;

			caz.set_Code(code);
			caz.set_ArtNr(artnr);
			caz.set_Gruppe("-");
			
			if (!caz.insert())
				throw caz.error;
		}
	
		SpanxDb::Database& dba;
		SpanxDb::Database& dbn;
	};
}