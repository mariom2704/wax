#pragma once

#include "..\rohrkonst\Rohrleitung.h"

class Rohrleitung;
class CObList;

class WAXPRJDLL Ausgaben  
{
private:
	//ziel,
	CString ziel;

	int zielwert;

public:
		//Listenindex
	enum LISTEN 
	{
		VKABRECHNUNG = 0,

		ZEUGNISSTELLE = 1,

		SCHWEISSEREI = 2,

		ROBBI1 = 3,

		ROBBI2 = 4,

		GEWINDEROHR = 5,

		PULVERANLAGE = 6,

		GRUNDIERER = 7,

		BUENDLER1 = 8,

		BUENDLER2 = 9,

		VERSAND1 = 10,

		BELIEBIGERTEXT =11,

		AWFERTIGUNG =12,

		//Zähler für die Listen. Muss immer der höchste Wert sein
		LISTCOUNT =13
	};

	//walist, Liste der abzuarbeitenden Werkauftraege
	CObList walist;

	// set_sokolist, fuellt die Sokoliste des Werkauftragobjekts mit der 
	//uebergebenen Sokoliste des aktuellen Werkauftrages
	void set_sokolist(const CObList& value);

	// set_passlist, fuellt die Passlaengenliste des Werkauftragobjekts mit der 
	//uebergebenen Passlaengenliste des aktuellen Werkauftrages
	void set_passlist(const CObList& value);

	// set_pulverlist, fuellt die Pulverlaengenliste des Werkauftragobjekts mit 
	//der uebergebenen Pulverlaengenliste des aktuellen Werkauftrages
	void set_pulverlist(EnCObListVk3& value, bool sort);
	//void set_pulverlist(const CObList& value);

	// setVK3Gesamtpreise, berechnet alle VK3-Preise des Werkauftragobjekts
	void setVK3Gesamtpreis();

	// set_elelist, fuellt die VK3-Elementliste des Werkauftragobjekts mit der 
	//uebergebenen VK3-Elementliste des aktuellen Werkauftrages
	void set_elelist(EnCObListVk3& value,bool sort);

	 Rohrleitung& get_wa() ;

	const CString& get_ziel() const;

	void set_ziel(const CString& value);

	const int get_zielwert() const;

	void set_zielwert(const int value);

	//get htmlFiles
	const CStringArray& get_htmlFiles() const;

	// setGesamtpreise, berechnet alle Preise des Werkauftragobjekts
	void setGesamtpreise();

	//Konstruktor
	Ausgaben();

	//Destruktor
	virtual ~Ausgaben();

	//get werkauftrag
	virtual Werkauftrag* get_werkauftrag() ;

	//set werkauftrag
	void set_werkauftrag(Werkauftrag* value);

	//set walist
	void set_walist(CObList& value);

	// initAusgaben, Ausgaben initialisieren
	void virtual initAusgaben();
	
	//get ausgaben
	const CArray<int,int>& get_ausgaben();

	//get currentAusgabe
	const int get_currentAusgabe() const;
	
	//set currentAusgabe
	void set_currentAusgabe(int value);

	// get lastAusgabe
	int get_lastAusgabe() const;
	
	//set lastAusgabe
	void set_lastAusgabe(int value);
	
	// makeAusgabe, Gewuenschte Ausgabe ausfuehren
	virtual const CStringArray& makeAusgabe(int id = 0);

	double getApaxVK3Gesamtpreis();

	double getFormVK3Gesamtpreis();

protected:
	//gesamtzeit, Herstellungszeit der Rohrleitung ohne Pulvern
	double gesamtzeitOhnePulvern;

	int strangrohr;

	//Rohrleitung wegen Stueckliste
	Rohrleitung wa;

	//htmlFiles, Dateinamen der erzeugten HTML-Files
	 CStringArray htmlFiles;

	//Aktuell gültige Ausgaben
	CArray<int,int> ausgaben;

	//lastAusgabe, ID der zu erzeugenden Ausgabe
	static int lastAusgabe;
	
	//currentAusgabe, ID der zu erzeugenden Ausgabe
	static int currentAusgabe;

	// resetAttributes, setze die Klassenattribute auf 0
	virtual void resetAttributes();

	///Liste auf Preis 0,00€ pruefen
	void check_NullEuro();

	void isMatNull();

	// afoSonderbehandlung, hier ggbf. Afo 54 auf 17 aufgeschlagen & die 
	//Gesamtherstellzeit berechnet
	void afoSonderbehandlung();

	//Durchläuft die walist und errechnet die Gesamtgewichte und die maximale 
	//Länge
	virtual void setMaxLaengeUndGewicht(bool mat = true, bool stueck = true);

	//werkauftrag, der aktuelle Werkauftrag
	Werkauftrag* werkauftrag;

	// deleteListen, loescht die Listen im Objekt des Werkauftrages
	void deleteListen();

	//gesamtgewicht, gibt das Gewicht ALLER Teile an
	double gesamtgewicht;

	//gesamtgewichtEK, gibt das Gewicht aller EK-Teile an
	double gesamtgewichtEK;

	//gesamtgewichtLAGER, gibt das Gewicht aller LAGER-Teile an
	double gesamtgewichtLAGER;

	//gesamtgewichtOP, gibt das Gewicht aller Teile OHNE PREIS an
	double gesamtgewichtOP;
	
	//gesamtlaenge, Laenge der Rohrleitung
	int gesamtlaenge;

	//maxLaenge, gibt die grosste in eine Richtung vorkommende Laenge an
	int maxlaenge;

	//gesamtzeit, Herstellungszeit der Rohrleitung
	double gesamtzeit;

	// set_stklist, fuellt die Stueckliste des Werkauftragobjekts mit der 
	//uebergebenen Stueckkliste des aktuellen Werkauftrages
	void set_stklist(const CObList& value, bool nullpreis = false, bool einkauf = true);

	// set_matlist, fuellt die Materialliste des Werkauftragobjekts mit der 
	//uebergebenen Materialliste des aktuellen Werkauftrages
	void set_matlist(const CObList& value, int list = 11, bool nullpreis = false, bool einkauf = true);

	// setGesamtmaterialliste, fuellt die Materialiste des Werkauftragobjekts mit 
	//der Stueckliste des Werkauftragobjekts auf
	void setGesamtmaterialliste();
};

