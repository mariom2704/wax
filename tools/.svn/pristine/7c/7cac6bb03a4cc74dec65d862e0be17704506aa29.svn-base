#pragma once

#include "BaseAction.h"
#include <Materialstamm.h>

#include <map>


namespace Action
{
	/**

	*/
	class UpdateMaterialstamm : public BaseAction
	{
	public:
		UpdateMaterialstamm(SpanxDb::Database& altdb, SpanxDb::Database& neudb) : BaseAction(altdb, neudb)
		{
			sqlselect = "code not like 'vortext_%'";
		}

	private:

		void update_matstamm(int artnr, AltTabellen::PreislistenArtikel& pa)
		{
			SpanxDb::MaterialstammEx matstamm(dbneu);			
			if (!matstamm.load(artnr))
			{
				if (matstamm.error.is_critical())
					throw matstamm.error;
				return;
			}

			matstamm.set_geaendert_von(pa.get_GeaendertVon());
			matstamm.set_geaendert_am(pa.get_GeaendertAm());
			matstamm.set_kurztext(pa.get_KurzText());
			matstamm.set_KurzTextEnglish(pa.get_KurzTextEnglish());
			matstamm.set_KurzTextFrancais(pa.get_KurzTextFrancais());
			matstamm.set_KurzTextNederlands(pa.get_KurzTextNederlands());
			matstamm.set_montagezeit(pa.get_Montagezeit());
			matstamm.set_wm(pa.get_WM());
			matstamm.set_wl(pa.get_WL());
			matstamm.set_wm_service(matstamm.get_wm());
			matstamm.set_wl_service(matstamm.get_wl());
			matstamm.set_me(0); // Stück als Default
			matstamm.set_posnr(pa.get_posnr());
			matstamm.set_leistung(pa.get_IstFremdleistung());
			matstamm.set_sprinkler(pa.get_IstSprinkler() ? true : false);
			matstamm.set_gewicht(pa.get_gewicht());
			if (pa.get_Hinweis() == "Default" || pa.get_Hinweis() == "default")
				matstamm.set_hinweis("");
			else
				matstamm.set_hinweis(pa.get_Hinweis());

			if (!matstamm.update())
				throw matstamm.error;
		}

		void update_stk(int artnr, AltTabellen::PreislistenArtikel& pa)
		{
			/**
			Überprüft die Stücklistenauflösung für Artikel, die bereits eine SAP Artikelnummer
			hatten. 
			Prüft, ob ein Eintrag für eine SAP Artikelnummer vorliegt. Falls nicht wird er erzeugt.
			*/
			int oldartnr = atoi(pa.get_ArtikelNummer().c_str());

			if (!SpanxDb::ArtikelNummer(oldartnr).from_sap())
				return; // Keine SAP Artikelnummer, können wir übergehen

			SpanxDb::Stueckliste stk(dbneu);
			
			if (!stk.load(artnr))
				throw stk.error;

			bool found = false;
			while (stk.fetch_next())
			{
				if (stk.get_artikelnr() == oldartnr)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				stk.set_materialstamm_artnr(artnr);
				stk.set_anzahl(1);
				stk.set_artikelnr(oldartnr);
				if (!stk.insert())
					throw stk.error;
			}
		}

		
	protected:

		

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			int artnr = act_caz.artnr_from_code(pa.get_code());

			if (!artnr)
				return;

			/// Daten in Tabelle Materialstamm updaten
			update_matstamm(artnr, pa);

			/// ggf. Daten in Stückliste erzeugen
			update_stk(artnr, pa);

		}
	};
}