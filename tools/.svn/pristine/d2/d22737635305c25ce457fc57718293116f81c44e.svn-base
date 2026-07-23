#pragma once


#include "PreislistenArtikel.h"
#include "TmpTabellen.h"

namespace Action
{
	class BaseAction
	{
	public:
		BaseAction(SpanxDb::Database& spanxdbalt, SpanxDb::Database& spanxdbneu) : dbalt(spanxdbalt), dbneu(spanxdbneu), caz(spanxdbneu), act_caz(dbneu)
		{
			act_caz.load();
		}

		void run()
		{
			AltTabellen::PreislistenArtikel pa(dbalt, false); // Kein Join mit Umsetzungstabelle Code->Artikelnummer

			if (!pa.start_fetch(sqlselect))
				throw pa.error;

			while (pa.fetch_next())
			{
				process(pa);
			}
			MxOdbc::Table::set_loc(MxBase::Locale());
			if (pa.error.is_critical())
				throw pa.error;
		}

	protected:

		std::string sqlselect;

		virtual void process(AltTabellen::PreislistenArtikel& pa) = NULL;

		SpanxDb::Database& dbalt;
		SpanxDb::Database& dbneu;

		TmpTabellen::CodeArtikelnummer_Zuordnung caz;

		CodeArtikelNummerZuordnung act_caz;
	};
}