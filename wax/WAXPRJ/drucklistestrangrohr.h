#pragma once

#include "druckliste.h"


namespace WaxDruck
{
	class DrucklisteStrangrohr : public Druckliste
	{
	public:
		DrucklisteStrangrohr(HtmlDruck& file, WaxPart* pt);
		
		virtual bool create();
	private:
		
		std::string bemerkung;
		int gesamtlaenge;
		int anzahlRohre;
		int anzahlElemente;
		int rohrQuali; 
		int dnvl;
		int elementID;
		int elementDN;
		int sondermuffenlaenge;
		double durchmesser;
		bool trockenanlage;
		bool gekehlt;

		void create_header();

		bool create_strangrohrliste(const Strangrohr* const strang);

	};
}
