#pragma once

#include "BaseAction.h"

#include <Materialstamm.h>
#include <Gruppen.h>

namespace Action
{

	class UpdateSparte : public BaseAction
	{
	public:
		UpdateSparte(SpanxDb::Database& dbalt, SpanxDb::Database& dbneu ) : BaseAction(dbalt, dbneu)
		{
			SpanxDb::Gruppen g(dbalt);

			if (!g.start_fetch())
				throw g.error;

			while (g.fetch_next())
			{
				grp2sparte.insert(std::make_pair<std::string, int>(g.get_code(), g.get_sparte()));
			}
		}

	protected:

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			int artnr = act_caz.artnr_from_code(pa.get_code());
			
			if (!artnr)
				return;

			SpanxDb::MaterialstammKurz ms(dbneu, MxBase::Locale());
			if (!ms.load(artnr))
				return;

			std::string gruppe = pa.get_Gruppe();
			std::map<std::string, int>::iterator it = grp2sparte.find(gruppe);
			
			int sparte = (it != grp2sparte.end()) ? it->second : 0;

			ms.set_sparte(sparte);
			if (!ms.update())
				throw ms.error;

		}

		std::map<std::string, int> grp2sparte;
	};
}



