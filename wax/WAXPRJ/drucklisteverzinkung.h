#pragma once

#include "druckliste.h"
#include "../rohrkonst/zinkkosten.h"

namespace WaxDruck
{
	class DrucklisteVerzinkung : public Druckliste
	{

	public:
		DrucklisteVerzinkung(HtmlDruck& file, WaxPart* pt);
		virtual ~DrucklisteVerzinkung();

		virtual bool create();


	private:
		ZinkKosten verzinkung;
		int GesGewicht;

		void init();
	};
}
