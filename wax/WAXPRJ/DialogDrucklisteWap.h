#pragma once
#include "druckliste.h"
#include "..\rohrkonst\GpWapPreis.h"
#include "..\rohrkonst\zinkkosten.h"


#include "Dialogdruckliste.h"
#include "DialogHtmlDruck.h"

namespace WaxDruck
{
	class WAXPRJDLL DialogDrucklisteWap : public DialogDruckliste
	{
	public:
		DialogDrucklisteWap(DialogHtmlDruck& file, WaxPart* pt, GpWapPreis::PREISART part);	
		
		DialogDrucklisteWap(DialogHtmlDruck& file, CObList* ptWerkauf, GpWapPreis::PREISART part);	
	
		int get_AnzahlRohreProPos();
		
		
		virtual ~DialogDrucklisteWap(void);
		
		virtual bool create(Werkauftrag *) {return false;}
		double createWap();
		bool createMaterialliste(bool showPreise);
		bool createArbeitsplan(bool showPreise, BOOL last);

	private:
		GpWapPreis::PREISART preisart;

		void addUberschriftWaNrPosNr();
		void addMaterialkosten(double &gesMat, bool showPreis, const string ueberschrift);
		void addFertigungskosten(double &gesFert, bool showPreise, const string ueberschrift, BOOL last);
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