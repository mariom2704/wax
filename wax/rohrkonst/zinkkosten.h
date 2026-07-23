#pragma once

#include "..\waxstamm\verzinkung.h"

/**
Berechnung der Kosten für die Verzinkung bei OTD bzw. OTN
Die Kosten errechnen sich aus einem mengenabhängigen Grundbetrag plus Zuschlag.
Dazu kommen die Kosten für das Putzen. Diese Kosten errechnen sich aus 
der Anzahl  der Elemente auf dem Rohr inkl. Rohranfang und Ende mal der Zeit 
pro Element mal Stundensatz. Die Zeit pro Element ist abhängig vom 
Rohrdurchmesser.

Alle Attribute sind Preise in Euro, solange nicht entsprechend benamst
*/



class ROHRKONSTDLL ZinkKosten
{
public:
	ZinkKosten(const Verzinkung::data* z, const Rohrleitung& r);

	bool ChangeVirtualAssembledElements(const Element* element);
	void Assembleelement(Element* hele);
	/// Defaultkonstruktor für Aufsummierungen
	ZinkKosten();

	enum ELE
	{
		STDELEMENT = 0,
		AUSSENGEW = 1,
		NELE = 2,
	};

	typedef struct verzele
	{
		enum ELE kennung;
		int dn;
		int anzahl;
		double zeit;
		double preis;
	};


	std::vector<verzele> elelist;

	inline double get_gesamt() const;

	inline double get_grund() const;

	inline double get_zuschlag() const;

	double get_putzkosten(enum ELE ele) const;

	int get_element_anzahl(enum ELE ele) const;

	double get_putz_zeit(enum ELE ele) const;

	inline double get_gewicht() const;

	inline double get_vkzuschlag() const;

	/// Stundensatz aus den Stammdaten liefern bzw. berechnen
	double get_euro_pro_kg() const;

	/// Zinkkostenaufschlag aus den Stammdaten liefern bzw. berechnen
	double get_versicherungsaufschlag() const;

	/// Stundensatz fürs Putzen liefern bzw. berechnen
	double get_stundensatz() const;

	double get_zuschlagproz() const;



	/// Zusammenfassen zweier Rohrleitungen, es werden die einzelnen Kosten addiert
	void add(const ZinkKosten& other); 

	/// Liefert den Namen der Verzinkerei
	std::string get_name() const;

	/// Summiert die Zeit für einen Elementtyp und eine DN auf
	double sum_zeit(enum ELE ele, int dn = -1) const;

	/// Summiert die Anzahl für einen Elementtyp und eine DN auf
	int sum_anz(enum ELE ele, int dn = -1) const;

	/// Summiert die Putzkosten der Elemente auf
	double sum_putzkosten() const;

	double GesamtGrundkosten();
	double GesamtGewicht();
	double GesamtMGK();
	double GesamtVKZuschlag();
	double GesamtKosten();
	double GesamtZuschlag();
	void SetGesamtVerzinkerei(const Verzinkung::data* z);

private:

	/// Zähler, wie oft Zinkkosten addiert wurden. Wenn grösser Null, dann müssen die Stundensätze etc. aus den Ergebnissen
	/// rückgerechnet werden, da die addierten Zinkkosten auf anderen Stammdaten beruhen könnten.
	int added; 
	
	const Verzinkung::data* verzinkerei;

	/// Die Rohrleitung, für die die Kosten berechnet werden
	const Rohrleitung* rohr;

	/// Der Gesamtpreis ist die Summe aus Grundpreis (kg*Faktor) + Zuschlag + Putzen (Anzahl der Elemente*Faktor) + Versicherung;
	double gesamt;

	/// Grundpreis errechnet sich aus dem Gewicht * euro_pro_kg aus den Stammdaten
	double grund;


	/// Zuschlag errechnet sich aus Grundpreis * zuschlag (aus Stammdaten)
	double zuschlag;

	/// Gewicht der Rohrkonstruktion
	double gewicht;

	/// Zuschlag auf grund+putzen+zink
	double vkzuschlag;

	void calc();

	void calc_grund();

	void calc_zuschlag();

	void calc_vkzuschlag();	

	
	/// Durchläuft die Stückliste und errechnet die Gesamtanzahl der Elemente und 
	/// füllt die dn_id_lst für die Zeitberechnung des Putzens
	void scan_stueckliste();

	void scan_stueckliste(const EnCObListModell& liste);

	/// Liefert die Putzzeit in Abhängigkeit von der DN
	double get_zeit(enum ELE elekennung, int dn) const;

	/// Fügt ein Element in die interne Elementliste ein
	void add_ele(enum ELE kennung, int anzahl, int dn);
};


inline double ZinkKosten::get_gesamt() const
{
	return gesamt;	
}


inline double ZinkKosten::get_grund() const
{
	return grund;
}

inline double ZinkKosten::get_zuschlag() const
{
	return zuschlag;
}

inline double ZinkKosten::get_gewicht() const
{
	return gewicht;
}

inline double ZinkKosten::get_vkzuschlag() const
{
	return vkzuschlag;
}