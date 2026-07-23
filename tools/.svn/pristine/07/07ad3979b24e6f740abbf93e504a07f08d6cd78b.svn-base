#pragma once 

#include "BaseAction.h"

#include "VerkaufsPreise.h"
#include <VerkaufsPreis.h>

namespace Action
{
	/**
	Alle Einträge in der Tabelle old_Stueckliste werden umgesetzt in die neue Tabelle
	Stueckliste.
	Die Codes werden in Artikelnummern umgesetzt.
	*/
	class CopyVerkaufsPreise : public BaseAction
	{
	public:
		CopyVerkaufsPreise(SpanxDb::Database& dbalt, SpanxDb::Database& dbneu ) : BaseAction(dbalt, dbneu)
		{
			
		}

	protected:

		virtual void process(AltTabellen::PreislistenArtikel& pa)
		{
			AltTabellen::VerkaufsPreise p(dbalt);
			if (!p.load(pa.get_code()))
				return;

			int artnr = act_caz.artnr_from_code(pa.get_code());

			if (!artnr)
				return;

			SpanxDb::VerkaufsPreis pneu(dbneu);
			
			if (pneu.load(artnr))
			{
				pneu.set_vk(p.get_preis());
				if (!pneu.update())
					throw pneu.error;
			}
			else
			{
				pneu.set_artikelnr(artnr);
				pneu.set_vk(p.get_preis());
				pneu.set_vk_service(p.get_preis());
				if (!pneu.insert())
					return; // Kein Datensatz in Materialstamm
			}
		}
	};
}

