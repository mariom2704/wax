#pragma once

#include "BaseAction.h"
#include <Langtexte.h>


namespace Action
{
	/**

	*/
	class CopyLangtexte : public BaseAction
	{
	public:
		CopyLangtexte(SpanxDb::Database& altdb, SpanxDb::Database& neudb) : BaseAction(altdb, neudb)
		{
			//sqlselect = "code not like 'vortext_%'";
		}

	protected:

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			int artnr = act_caz.artnr_from_code(pa.get_code());

			SpanxDb::Langtexte l(dbneu, MxBase::Locale(MxBase::Locale::DEUTSCH));
			
			if (l.load(artnr))
			{
				l.set_langtext(pa.get_LangText());
				l.set_lvtext(pa.get_LVText().length() ? pa.get_LVText() : pa.get_LangText());
				if (!l.update())
					throw l.error;
			}
			else
			{
				l.set_artnr(artnr);
				l.set_langtext(pa.get_LangText());
				l.set_lvtext(pa.get_LVText().length() ? pa.get_LVText() : pa.get_LangText());
				if (!l.insert())
					return;
			}
			
			const int nloc = 3;
			MxBase::Locale locs[nloc] = 
			{
				MxBase::Locale(MxBase::Locale::ENGLISH),
				MxBase::Locale(MxBase::Locale::FRANCAIS),
				MxBase::Locale(MxBase::Locale::NEDERLANDS)
			};

			std::string baselangtext = pa.get_LangText();

			for (int i = 0; i < nloc; i++)
			{
				SpanxDb::Langtexte l(dbneu, locs[i]);
				
				if (!l.load(artnr))
					throw l.error;

				std::string ltn;
				
				ltn = pa.get_col("LangText"+locs[i].get_locale_dbstr())->to_string();

				if (!ltn.length())
					ltn = baselangtext;

				l.set_langtext(ltn);

				std::string lvtn;
				lvtn = pa.get_col("LVText"+locs[i].get_locale_dbstr())->to_string();

				l.set_lvtext(lvtn.length() ? lvtn : ltn);
				if (!l.update())
					throw l.error;
			}
			dbneu.commit_trans();
		}
	};
}