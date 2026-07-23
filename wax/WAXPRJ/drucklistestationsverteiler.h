#pragma once
#include "druckliste.h"


namespace WaxDruck
{
	class DrucklisteStationsverteiler : public Druckliste
	{

	public:
		DrucklisteStationsverteiler(HtmlDruck& file, WaxPart* pt);
		virtual ~DrucklisteStationsverteiler();

		virtual bool create();

	private:
		/// Bemerkungstext aus erster Position einfügen
		void print_header();

		bool print_list(const Stationsverteiler* stat);
	};
}
