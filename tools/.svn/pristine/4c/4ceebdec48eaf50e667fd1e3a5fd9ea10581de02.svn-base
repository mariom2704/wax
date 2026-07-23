#pragma once


#include "BaseAction.h"

#include <Materialstamm.h>
#include <HinweisTexte.h>

namespace Action
{

	class UpdateHinweistexte : public BaseAction
	{
	public:
		UpdateHinweistexte(SpanxDb::Database& dbalt, SpanxDb::Database& dbneu ) : BaseAction(dbalt, dbneu)
		{			
		}

	protected:

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			int artnr = act_caz.artnr_from_code(pa.get_code());

			if (!artnr)
				return;

			
			// Step 1: Hinweistexte einpflegen
			if (pa.get_Hinweis() != "-" && pa.get_Hinweis() != "default")
			{
				MxBase::Locale::LocaleVec lv = MxBase::Locale().query_supported_locales();
				
				std::string defhwtxt;

				for (MxBase::Locale::LocaleVec::iterator it = lv.begin(); it != lv.end(); it++)
				{
					SpanxDb::HinweisTexte hwalt(dbalt, it->second);
					
					if (!hwalt.load(pa.get_Hinweis()))
					{
						if (hwalt.error.is_critical())
							throw hwalt.error;

						return; // Hinweistext zwar in Tabelle PreislistenArtikel gesetzt, gibt es aber gar nicht. Keine Änderung am Materialstamm!
					}
					if (!defhwtxt.length())
					{
						defhwtxt = hwalt.get_HinweisText();
						if (!defhwtxt.length())
							continue; // Lassen wir aus
					}
					
					SpanxDb::HinweisTexte hwneu(dbneu, it->second);
					if (!hwneu.load(pa.get_Hinweis()))
					{
						if (hwneu.error.is_critical())
							throw hwneu.error;
						// Hinweistext neu erzeugen
						hwneu.set_nIndex(hwalt.get_nIndex());
						
						if (hwalt.get_HinweisText().length() > 3) // Evlt. steht nur - o.ä. drin
							hwneu.set_HinweisText(hwalt.get_HinweisText());
						else
							hwneu.set_HinweisText(defhwtxt);

						if (!hwneu.insert())
							throw hwneu.error;
					}
					else
					{
						if (hwalt.get_HinweisText().length() > 3) // Evlt. steht nur - o.ä. drin
							hwneu.set_HinweisText(hwalt.get_HinweisText());
						else
							hwneu.set_HinweisText(defhwtxt);
						if (!hwneu.update())
							throw hwneu.error;
					}

				}

			}




			// Step 2: Feld Hinweis setzen, muss nach Step1 passieren, da Hinweistext bereits in Tabelle vorhanden sein muss

			SpanxDb::Materialstamm matstamm(dbneu);
			if (!matstamm.load(artnr))
			{
				if (matstamm.error.is_critical())
					throw matstamm.error;
				return;
			}

			matstamm.set_hinweis(pa.get_Hinweis());
			if (!matstamm.update())
				throw matstamm.error;





		}
	};
}