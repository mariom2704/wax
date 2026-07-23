#pragma once

#include "MergeBase.h"
#include <Hinweistexte.h>

namespace Merge
{
	/**
	Alle Hinweistexte werden von der alten in die neue Datenbank übernommen. Es werden keine Daten gelöscht.
	*/
	class MergeHinweistexte : public MergeBase
	{
	public:
		MergeHinweistexte(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
			: MergeBase(spanxdb_alt, spanxdb_neu)
		{
		
		}

		std::string run()
		{
			MxBase::Locale::LocaleVec lv = MxBase::Locale().query_supported_locales(MxBase::Locale()); // Muss Deutsch an erster Stelle liefern
			
			for (MxBase::Locale::LocaleVec::iterator it = lv.begin(); it != lv.end(); it++)
			{
				MxBase::Locale l(it->second);

				update_hinweistexte(l);
			}
			info = boost::str(boost::format("Hinweistexte:\n%i update, %i insert\n") % update_counter % insert_counter);
			return info;
		}

	private:

		void update_hinweistexte(MxBase::Locale l)
		{
			SpanxDb::HinweisTexte hw_alt(db_alt, l);

			if (!hw_alt.start_fetch())
				throw hw_alt.error;

			while (hw_alt.fetch_next())
			{
				SpanxDb::HinweisTexte hw_neu(db_neu, l);

				bool update = hw_neu.load(hw_alt.get_nIndex());
				hw_neu.set_HinweisText(hw_alt.get_HinweisText());
				if (update)
				{
					update_counter++;
					if (!hw_neu.update())
						throw hw_neu.error;
				}
				else
				{
					insert_counter++;
					hw_neu.set_nIndex(hw_alt.get_nIndex());
					if (!hw_neu.insert())
						throw hw_neu.error;
				}
			}
		}
		
	};
}
