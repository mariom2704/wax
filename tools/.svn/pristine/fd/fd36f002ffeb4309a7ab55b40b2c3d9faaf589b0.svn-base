#pragma once

#include "MergeBase.h"
#include "../PreislistenArtikel.h"
#include <Vortexte.h>
#include <MxBase/Locale.h>

namespace Merge
{
	/**
	Alle Vortexte der Tabelle PreislistenArtikel (alte DB) werden in die Tabelle Vortexte (neue DB)
	kopiert, bereits vorhandene werden überschrieben, neue werden erstellt. Es werden keine Daten gelöscht.
	*/
	class MergeVortexte : public MergeBase
	{
	public:
		MergeVortexte(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
			: MergeBase(spanxdb_alt, spanxdb_neu)
		{
		
		}

		std::string run()
		{
			MxBase::Locale::LocaleVec lv = MxBase::Locale().query_supported_locales(MxBase::Locale()); // Muss Deutsch an erster Stelle liefern
			
			for (MxBase::Locale::LocaleVec::iterator it = lv.begin(); it != lv.end(); it++)
			{
				MxBase::Locale l(it->second);

				update_vortexte(l);
			}
			return info;
		}

		private:
			void update_vortexte(MxBase::Locale l)
			{
				std::string deftext; // Deutsch als Default, falls andere Sprachen nicht belegt sind
				AltTabellen::PreislistenArtikelLangtext art(db_alt, l);	
				
				if (!art.start_fetch("code like 'vortext%'"))
					throw art.error;

				while (art.fetch_next())
				{
					SpanxDb::Vortexte vt(db_neu, l);
					
					if (!vt.load(art.get_code()))
						throw vt.error;

					if (vt.fetch_next())
					{
						if (!deftext.length()) // Erste Sprache als Default, sollte z.Zt. Deutsch sein
							deftext = art.get_langtext();
						vt.set_langtext(art.get_langtext().size() ? art.get_langtext() : deftext);
						if (!vt.update())
							throw vt.error;
					}
					else
					{
						vt.set_code(art.get_code());
						vt.set_langtext(art.get_langtext());
						if (!vt.insert())
							throw vt.error;
					}
				}
			}
		

		
	};
}
