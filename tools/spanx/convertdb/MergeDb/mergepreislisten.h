#pragma once

#include "MergeBase.h"
#include <Listen.h>
#include <MxBase/Locale.h>

namespace Merge
{
	/**
	Alle Vortexte der Tabelle PreislistenArtikel (alte DB) werden in die Tabelle Vortexte (neue DB)
	kopiert, bereits vorhandene werden überschrieben, neue werden erstellt. Es werden keine Daten gelöscht.
	*/
	class MergePreislisten : public MergeBase
	{
	public:
		MergePreislisten(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
			: MergeBase(spanxdb_alt, spanxdb_neu)
		{
		
		}

		std::string run()
		{
			MxBase::Locale::LocaleVec lv = MxBase::Locale().query_supported_locales(MxBase::Locale()); // Muss Deutsch an erster Stelle liefern
			
			for (MxBase::Locale::LocaleVec::iterator it = lv.begin(); it != lv.end(); it++)
			{
				MxBase::Locale l(it->second);

				update_preisliste(l);
			}
			return info;
		}

		private:
			void update_preisliste(MxBase::Locale l)
			{
				SpanxDb::Listen spl(db_alt, l);
				
				if (!spl.start_fetch())
					throw spl.error;

				while (spl.fetch_next())
				{
					SpanxDb::Listen spl_neu(db_neu, l);
					
					if (!spl_neu.start_fetch("Code = '" + spl.get_Code() + "'"))
						throw spl_neu.error;

					if (spl_neu.fetch_next())
					{
						spl_neu = spl;
						
						if (!spl_neu.update())
							throw spl_neu.error;
					}
					else
					{
						spl_neu = spl;

						if (!spl_neu.insert())
							throw spl_neu.error;
						info += "Neue Preisliste: " + spl.get_Code() + "\r\n";
					}
				}
			}
		

		
	};
}
