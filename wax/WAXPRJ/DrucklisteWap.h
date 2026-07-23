#pragma once
#include "druckliste.h"
#include "..\rohrkonst\GpWapPreis.h"
#include "..\rohrkonst\zinkkosten.h"

namespace WaxDruck
{
	class WAXPRJDLL DrucklisteWap : public Druckliste
	{
	public:
		DrucklisteWap(HtmlDruck& file, WaxPart* pt, GpWapPreis::PREISART part);	
		
		DrucklisteWap(HtmlDruck& file, CObList* ptWerkauf, GpWapPreis::PREISART part);	
	
		int get_AnzahlRohreProPos();
		
		
		virtual ~DrucklisteWap(void);
		
		virtual bool create() {return false;}
		double createWap();
		bool createMaterialliste(bool showPreise);
		bool createArbeitsplan(bool showPreise);

		void addFertigungskosten(double &gesFert, bool showPreise, const string ueberschrift);
		void addMaterialkosten(double &gesMat, bool showPreis, const string ueberschrift);

	private:
		GpWapPreis::PREISART preisart;
		
		void addFremdleistungen(double &gesFremd);
		void addGesKosten(double gesMat, double gesFert, double gesFremd, double &selbstkosten);
		void addVk3(double vk3Ges, double selbstkosten);

		void addRohrQuali();
		void addBemerkung();

		void setGesamtmaterialliste(MaterialListe &matliste);
		void setGesamtArbeitsplan(Arbeitsplan &ap, Beschichtung &beschichtung, int &maxLaenge, double &gesGewicht, int &wanr);

		void getZinkkosten(ZinkKosten &zk);
	};
}