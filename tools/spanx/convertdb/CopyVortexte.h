#pragma once

#include "PreislistenArtikel.h"
#include "TmpTabellen.h"

#include "BaseAction.h"

#include <Vortexte.h>


namespace Action
{
	class CopyVortexte : public BaseAction
	{
	public:
		CopyVortexte(SpanxDb::Database& dbalt, SpanxDb::Database& dbneu) : BaseAction(dbalt, dbneu)
		{
			sqlselect = "code like 'vortext_%'";
		}


	protected:

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			MxOdbc::Table::set_loc(MxBase::Locale());
			std::string c = pa.get_col("code")->to_string();
			std::string ldef = pa.get_col("LangText")->to_string();
			std::string kdef = pa.get_col("KurzText")->to_string();
			
			SpanxDb::Vortexte vt(dbneu);

			if (vt.load(c) && vt.fetch_next())
			{
				vt.set_langtext(ldef);
				if (!vt.update())
					throw vt.error;
			}
			else
			{
				vt.set_code(c);
				vt.set_langtext(ldef);
				vt.set_kurztext(kdef);
				if (!vt.insert())
					throw vt.error;
			}

			const int nloc = 3;
			MxBase::Locale locs[nloc] = 
			{
				MxBase::Locale(MxBase::Locale::ENGLISH),
				MxBase::Locale(MxBase::Locale::FRANCAIS),
				MxBase::Locale(MxBase::Locale::NEDERLANDS)
			};

			for (int i = 0; i < nloc; i++)
			{
				std::string lx = pa.get_col("LangText" + locs[i].get_locale_dbstr())->to_string();
				std::string kx = pa.get_col("KurzText" + locs[i].get_locale_dbstr())->to_string();
				SpanxDb::Vortexte vt(dbneu, locs[i]);
				if (!vt.load(c))
					throw vt.error;
 
				if (!vt.fetch_next())
				{
					vt.set_code(c);
					vt.set_langtext(lx);
					vt.set_kurztext(kx);
					if (!vt.insert())
						throw vt.error;
					continue;
				}

				vt.set_langtext(lx.length() ? lx : ldef);
				
				if (!vt.update())
					throw vt.error;
			}
		}
	};
}