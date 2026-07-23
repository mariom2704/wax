#pragma once
#include "druckliste.h"


namespace WaxDruck
{
	class DrucklisteCOIIMx : public Druckliste
	{

	public:
		DrucklisteCOIIMx(HtmlDruck& file, WaxPart* pt);
		virtual ~DrucklisteCOIIMx();

		virtual bool create();

	private:
		/// Bemerkungstext aus erster Position einfügen
		void print_header();

		bool print_list(COIIVerteiler* stat);
		void populateNennweiten(WaxHtmlFile& w, COIIVerteiler* pCOIIMxLocal );
	};
}
