#pragma once




class ROHRKONSTDLL GpWapPreis
{
public:
	GpWapPreis(void);
	virtual ~GpWapPreis(void);

	enum PREISART
	{
		Vollkosten,
		Grenzkosten
	};

	//Material Herstelkostenzuschlag
	double getHkMaterialkostenSatz();

	//Material Grenzkostenzuschlag
	double getGkMaterialkostenSatz();

	//Vollkosten Zuschlag für Verwaltungskosten
	double getVkVerwaltungskostenSatz();

	//Grenzkosten Zuschlag für Verwaltungskosten
	double getGkVerwaltungskostenSatz();


	bool calcMaterialkosten(GpWapPreis::PREISART part, const MaterialListe &matliste, double& gesFestPreis, double& gesSapPreis, double& gesPulverpreis, double& zuschlagProzent, double& zuschlag);
	
	bool calcFertigungskosten(vector<Werkauftrag*>&  auftraege, GpWapPreis::PREISART part, const Beschichtung &beschichtung, int maxlaenge, double gesGewicht, int wanr,
							  Arbeitsplan &ap, double& gesPreis, double& gesPreisMitzuschlag, double& gesZeit);
	
	bool calcSelbstkosten(GpWapPreis::PREISART part, double gesMatKosten, double gesFertkosten, double gesFremd, double &zuschlag, double &zuschlagProzent, double &herstellkosten, double &selbstkosten);
};
