#pragma once

#include "MergeBase.h"
#include <Gruppen.h>
#include <MxBase/Locale.h>

namespace Merge
{
	/**
	Alle Vortexte der Tabelle PreislistenArtikel (alte DB) werden in die Tabelle Vortexte (neue DB)
	kopiert, bereits vorhandene werden überschrieben, neue werden erstellt. Es werden keine Daten gelöscht.
	*/
	class MergeGruppen : public MergeBase
	{
	public:
		MergeGruppen(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
			: MergeBase(spanxdb_alt, spanxdb_neu)
		{
		
		}

		std::string run()
		{
			MxBase::Locale::LocaleVec lv = MxBase::Locale().query_supported_locales(MxBase::Locale()); // Muss Deutsch an erster Stelle liefern
			
			for (MxBase::Locale::LocaleVec::iterator it = lv.begin(); it != lv.end(); it++)
			{
				MxBase::Locale l(it->second);

				update_gruppen(l);
			}
			return info;
		}

		private:
			void update_gruppen(MxBase::Locale l)
			{
				SpanxDb::Gruppen grpl(db_alt, l);
				
				if (!grpl.start_fetch())
					throw grpl.error;

				while (grpl.fetch_next())
				{
					SpanxDb::Gruppen grpl_neu(db_neu, l);
					
					if (!grpl_neu.start_fetch("Code = '" + grpl.get_code() + "'"))
						throw grpl_neu.error;

					if (grpl_neu.fetch_next())
					{
						grpl_neu = grpl;
						
						if (!grpl_neu.update())
							throw grpl_neu.error;
					}
					else
					{
						grpl_neu = grpl;
						if (!grpl_neu.insert())
							throw grpl_neu.error;
						info += "Neue Gruppe: " + grpl.get_code() + "\r\n";
					}
				}
			}
		

		
	};
}
