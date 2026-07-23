#pragma once

#include "HtmlDruck.h"
#include "JScriptDruck.h"

#include "..\rohrkonst\zinkkosten.h"
#include "drucklistezuschnitt.h"

namespace WaxDruck
{

	/**
	DrucklistenVerwaltung erzeugt für einen Werkauftrag die möglichen Drucklisten
	Für Formstücke und Fittinge ist dies z.B. nur eine Materialliste und ein 
	Arbeitsplan. Bei Verteilern sind, je nach Eingabe mehrere Listen möglich: z.B. 
	Elementlisten, Zuschnittslisten usw.
	*/
	/*
	Änderung 30.03.06, Jac: Maschine 35060 gegen 32070 getauscht
	*/
	class WAXPRJDLL DrucklistenVerwaltung
	{
	/// public
	public:
		CJScriptPrinting JScriptVisualisationAndPrinting;

		enum DRUCKLISTEN 
		{
			///  Druckvorschau wird abgebrochen
			DRUCKVORSCHAU_ABBRECHEN = 1,
		
			///  Nur die Graphik
			GRAPHIKDRUCK = 2,

			///  Alle Elemente nach Rohrstuecken geordnet mit den dazugehoerigen 
			///  Unterelementen direkt ausgegeben
			ROHRLEITUNGSPLAN = 3,

			///  Liste mit den Abgaengen
			ELEMENTPLAN = 4,

			///  Liste mit Arbeitsfolgen
			ARBEITSPLAN = 5,

			///  Liste mit Zusatzmaterialien
			MATERIALLISTE_GESAMT = 11,

			///  Abrechnung WAP kurz
			WAP_GRENZKOSTEN = 14,

			///  Abrechnung Vollkosten WAP
			WAP_VOLLKOSTEN = 15,

			///  Abrechnung nach VK-Tabelle lang
			VK3_ABRECHNUNG_LANG = 18,

			///  Abrechnung nach VK-Tabelle kurz
			VK3_ABRECHNUNG_KURZ = 19,

			///   Gewinderohrliste
			GEWINDEROHRLISTE = 20,
			
			///  Alle Gewinderohr nach Laenge sortiert
			GEWINDEROHRE_LAENGE = 23,

			GEWINDEROHRE_BIS_10_STUECK = 24,

			GEWINDEROHRE_AB_10_STUECK = 25,

			GEWINDEROHRE_20_STUECK_LAENGE_50_500 = 26,


			///  Zusammenfassung von Saegen und Nuten fuer 32020
			SAEGENUTANLAGELISTE = 27,

			///  Verteilerzuschnittsliste
			ZUSCHNITTSLISTE_32020 = 31,

			///  Verteilerzuschnittsliste
			ZUSCHNITTSLISTE_32060 = 32,

			///  Verteilerzuschnittsliste  
			ZUSCHNITTSLISTE_32070 = 33,

			///  Stutzenzuschnittsliste Handschweißung
			ZUSCHNITTSLISTE_38030_HANDSW = 34,  

			///  Stutzenzuschnittsliste Anlage
			ZUSCHNITTSLISTE_38030_ANLAGE = 35,  

			///  Alle Rohre nach Laenge sortiert
			ROHRE_LAENGE = 36,

			///  Liste fuer Strangrohre
			STRANGROHRLISTE = 38,

			///  Spezialdruckliste für Stationsverteiler
			STATIONSVERTEILER = 39,

			///  Spezialdruckliste fuer Pumpenprobierleitung
			PUMPENPROBIERLEITUNG = 40,

			///  Spezialdruckliste fuer Pumpenverteiler
			PUMPENVERTEILER = 41,

			///  Spezialdruckliste fuer EntnahmeleitungDWB
			ENTNAHMELEITUNGDWB = 42,

			///  Profilliste für L und Flachprofile
			PROFILLISTE_38010 = 43,

			///  Liste mit U-Profilen
			PROFILLISTE_38040 = 44,

			///  Leere Zuschnittsliste
			ZUSCHNITT_LEER = 45,

			GEWINDEROHRGRAPHIK = 46,

			VERZINKUNG_PREISE = 47,

			PREISANFRAGE = 48,
			
			///Zuschnittlist EK
			ZUSCHNITTSLISTE_EK = 49, 

			///  Spezialdruckliste für COMX1230
			COMXVERTEILER_MONTAGE_LISTE = 50,

			SEPARATOR = 0xffffffff
		};

		DrucklistenVerwaltung(WaxPart* pt, bool bWidthJScript= false);

		/// Destruktor
		virtual ~DrucklistenVerwaltung();

		/// Erzeugt die gewünschte Ausgabedatei
		const CString makeAusgabe(int id = 0);
	
		/// Erzeugt die Liste mit den möglichen Drucklisten
		void init_menu();

		const vector<int>& get_menuitems() const;

		/// Liefert den Namen (fuer Menueeintrag) fuer eine Ausgabe.
		std::string get_name(int ausgabe_id, bool menu) const;

		// get lastAusgabe
		int get_lastAusgabe() const;

		//set currentAusgabe
		void set_currentAusgabe(int value);

		//get currentAusgabe
		const int get_currentAusgabe() const;

		
		//set lastAusgabe
		void set_lastAusgabe(int value);
		
		void set_ziel(const CString& z);
		

		/// Liefert den aktuell zu bearbeitenden Werkauftrag
		Werkauftrag* cur_werkauftrag();

		bool check2Rohrquali(const CObList& walist) const;
	

	private:
	
		

		bool m_bWidthJScript;

		void init(WaxPart* pt);
	
		void initListen();

		/// Erzeugt ein Menü mit Material-Drucklisten, falls nur Materialbestellung. 
		bool init_material_menu();

		void create_drucklist(WaxPart* curwaxpart);

		Werkauftrag* werkauftrag;

		void set_werkauftrag(Werkauftrag* w);

		//htmlFiles, Dateinamen der erzeugten HTML-Files
		CString htmlFiles;

		vector<int> menuitems;

		//currentAusgabe, ID der zu erzeugenden Ausgabe
		static int currentAusgabe;

		static int lastAusgabe;

		void set_walist(CObList& value);

		CObList walist;

		Rohrleitung* pDrucklisteRohrwaCollector;

		WaxPart* waxpart;

		CString listname;

		CUIntArray rohrnennweiten;
		
		CUIntArray nennweiten;

		CUIntArray anz;

		CUIntArray anzahlStutzen;
		
		CUIntArray anzahlMuffen;

		CStringArray preise;

		bool dn2zoll;

		int anzahlRohre;

		int anzahlRohrstuecke;

		int anzahlElemente;

		/// Grafikdatei
		CStringArray dwfFiles;

		/// Gesamtliste Elemente
		CObList elelist;
		
		/// Prüft auf gültigen Wert von currentAusgabe. Bei Gültigkeit wird 
		/// true zurückgeliefert
		bool check_currentAusgabe() const;

		/// Schaltet auf die erste mögliche Druckliste um bzw. bricht ab 
		bool set_default_druckliste();

		/// Generierung der ausgewaehlten Liste
		void writeList();
		
		/// An diese Ausgabe wird der Rest der DrucklistenVerwaltung angehängt.
		void write_header();
		
		/// Liefert eine Defaultdatei zurück, die angezeigt wird, wenn die gewünschte 
		/// DrucklistenVerwaltung nicht verfügbar ist, bzw. wenn keine DrucklistenVerwaltung verfügbar ist.
		void createDefaultDruckliste();

		// Fuegt die Graphik in eine DrucklistenVerwaltung ein
		bool insertGrafic();

		/// Erzeugt den Graphikdruck
		void createGraphikdruck();

				
		/// DrucklistenVerwaltung fuer die VK - Abrechnung
		void createAbrechnung();

		/// DrucklistenVerwaltung fuer den Rohrleitungsplan, die Elemente der Rohrkonstruktion 
		/// werden hier direkt ausgegeben
		void createRohrleitungsplan();

		/// DrucklistenVerwaltung fuer die Elementliste, die Elemente der Rohrkonstruktion 
		/// werden hier direkt ausgegeben
		void createElementplan();

		/// DrucklistenVerwaltung fuer die Saegenutanlage
		void createSaegenutanlageliste();

		///  Hilfsmethode zur Ausgabe von Unterelementen
		void Saegenutanlageliste_insert_uelement(WaxHtmlFile &w, const Element *uelement, const int eleAbst, const int eleWinkel);

		/// Zuschnittslisten
		void createRohrnuten_Zuschnittsliste(CObList& liste, int anlage);

		/// Erfast die Daten fuer den Fertigungsablaufplan
		void createFertigungsdata();

		/// DrucklistenVerwaltung fuer den Fertigungsablaufplan
		void createFertigungsablaufplan();

		/// Erfast die Daten fuer die Gewinderohrliste
		void createGewinderohrdata();
		
	
		/// DrucklistenVerwaltung fuer die Textgraphik bei Gewinderohr
		void createGewinderohrgraphik();

		/// DrucklistenVerwaltung für die Pumpenprobierleitung. Die Grafik wird als Bitmap 
		/// eingebunden, wie Sondermz. Grafik ist VDS abgenommen. Für Infos: Hr. 
		/// Richter
		void createPumpenprobierleitung();

		/// DrucklistenVerwaltung für den Pumpenverteiler. Die Grafik wird als Bitmap 
		/// eingebunden, wie Sondermz. Grafik ist VDS abgenommen. Für Infos: Hr. 
		/// Richter
		void createPumpenverteiler();

		/// DrucklistenVerwaltung für die EntnahmeleitungDWB. Die Grafik wird als Bitmap 
		/// eingebunden, wie Sondermz. Grafik ist VDS abgenommen. Für Infos: Hr. 
		/// Richter
		void createEntnahmeleitungDWB();

		CString startname;

		void createProfilliste(int liste);

		HtmlDruck html;
		

		/// Erfast die Daten fuer die VK3 - Abrechnung
		void createVK3Abrechnungdata();

		/// DrucklistenVerwaltung fuer die VK3 - Abrechnung
		void createVK3Abrechnung();

		

		// set_stklist, fuellt die Stueckliste des Werkauftragobjekts mit der 
		//uebergebenen Stueckkliste des aktuellen Werkauftrages
		void set_stklist(const CObList& value, bool nullpreis = false, bool einkauf = true);

		// set_matlist, fuellt die Materialliste des Werkauftragobjekts mit der 
		//uebergebenen Materialliste des aktuellen Werkauftrages
		void set_matlist(const CObList& value, int list = 11, bool nullpreis = false, bool einkauf = true);

		// setGesamtmaterialliste, fuellt die Materialiste des Werkauftragobjekts mit 
		//der Stueckliste des Werkauftragobjekts auf
		void setGesamtmaterialliste();

		// set_sokolist, fuellt die Sokoliste des Werkauftragobjekts mit der 
		//uebergebenen Sokoliste des aktuellen Werkauftrages
		void set_sokolist(const CObList& value);

		// set_passlist, fuellt die Passlaengenliste des Werkauftragobjekts mit der 
		//uebergebenen Passlaengenliste des aktuellen Werkauftrages
		void set_passlist(const CObList& value);

		// set_pulverlist, fuellt die Pulverlaengenliste des Werkauftragobjekts mit 
		//der uebergebenen Pulverlaengenliste des aktuellen Werkauftrages
		void set_pulverlist(EnCObListVk3& value, bool sort);
	
		// setVK3Gesamtpreise, berechnet alle VK3-Preise des Werkauftragobjekts
		void setVK3Gesamtpreis();

		// set_elelist, fuellt die VK3-Elementliste des Werkauftragobjekts mit der 
		//uebergebenen VK3-Elementliste des aktuellen Werkauftrages
		void set_elelist(EnCObListVk3& value,bool sort);

		///Liste auf Preis 0,00€ pruefen
		void check_NullEuro();

		/// Zurücksetzen der Attribute
		void reset();

			//gesamtlaenge, Laenge der Rohrleitung
		int gesamtlaenge;

		//maxLaenge, gibt die grosste in eine Richtung vorkommende Laenge an
		int maxlaenge;

		//gesamtzeit, Herstellungszeit der Rohrleitung
		double gesamtzeit;

		//gesamtgewicht, gibt das Gewicht ALLER Teile an
		double gesamtgewicht;


		// setGesamtpreise, berechnet alle Preise des Werkauftragobjekts
		void setGesamtpreise();

		void setMaxLaengeUndGewicht();

		CString ziel;

		void set_dn2zoll(int menuitem);

		

		vector<Werkauftrag*> get_walist();
	};
}


