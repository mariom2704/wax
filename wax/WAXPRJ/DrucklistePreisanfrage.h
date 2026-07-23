#pragma once

#include "druckliste.h"

namespace WaxDruck
{
	class DrucklistePreisanfrage  : public Druckliste
	{
	public:
		DrucklistePreisanfrage(HtmlDruck& file, WaxPart* pt);
		virtual ~DrucklistePreisanfrage(void);

		virtual bool create();
	};
}