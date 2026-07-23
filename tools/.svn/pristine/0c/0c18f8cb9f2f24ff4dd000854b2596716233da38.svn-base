#pragma once

#include "BaseAction.h"
#include "ProduktinfoJoin.h"
#include <Produktinfo.h>



namespace Action
{
	/**
	Aus den Alttabellen Produktinfo, angebotsblatt und dem Eintrag doc in Verkaufspreise 
	wird die neue Tabelle Produktinfo gefüllt.
	*/
	class UpdateInfo : public BaseAction
	{
	public:
		UpdateInfo(SpanxDb::Database& dbalt, SpanxDb::Database& dbneu ) : BaseAction(dbalt, dbneu)
		{
			
		}

	protected:

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{

			std::string code = pa.get_code();
			int artnr = act_caz.artnr_from_code(code);

			if (!artnr)
				return;

			AltTabellen::ProduktinfoJoin p(dbalt);
			if (!p.load(code))
				return;

			MxOdbc::BaseCol* datei = p.get_joined_col("produktinfo", "datei");
			MxOdbc::BaseCol* doc = p.get_joined_col("verkaufspreise", "doc");
#ifdef ANGEBOTSBLATT
			MxOdbc::BaseCol* blatt = p.get_joined_col("angebotsblatt", "blatt");
#endif

			if (doc->to_string().length() < 3 
#ifdef ANGEBOTSBLATT
				&& blatt->to_string().length()  < 3 
#endif 				
				&& datei->to_string().length()  < 3 )
					return;


			SpanxDb::Produktinfo pi(dbneu);
			
			if (pi.load(artnr))
			{
#ifdef ANGEBOTSBLATT
				pi.set_angebotsblatt("-");
#endif
				pi.set_leitfaden("-");
				pi.set_info("-");
#ifdef ANGEBOTSBLATT
				pi.set_angebotsblatt(blatt->to_string());
#endif
				pi.set_leitfaden(doc->to_string());
				pi.set_info(datei->to_string());

				pi.update();				
			}
			else
			{
				pi.set_artikelnummer(artnr);
#ifdef ANGEBOTSBLATT
				pi.set_angebotsblatt("-");
#endif
				pi.set_leitfaden("-");
				pi.set_info("-");


				pi.set_leitfaden(doc->to_string());
#ifdef ANGEBOTSBLATT
				pi.set_angebotsblatt(blatt->to_string());
#endif
				pi.set_info(datei->to_string());

				if (!pi.insert())
					return; // Fehler wird ignoriert, da er auftreten kann, wenn ein Artikel gel

			}
		}
	};
}