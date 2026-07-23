#pragma once

#include "WaxBase.h"
#include "Werkauftrag.h"
#include "EnContainerRohrElemente.h"
#include "EnCObListVk3.h"
#include <string>
#include <vector>

/**
Rohrleitung - Definition der Rohrkonstruktion
*/
class ROHRKONSTDLL Rohrleitung : public Werkauftrag  
{
private:
	
	void calhoehe(double winkel, double lh, double &h, double &l);

	double pi;
	
	/// Dieser Fertigungshinweis wird manuell eingegeben
	CString fertigungshinweis;

	/// Die verschiedenen Radien fuers Klinken 
	CArray<int,int> radien;

	/// Ruesten fuers Klinken
	bool ruesten[4];

	/// mit AV Elementen, die per Zeichnung nachgeliefert werden, weil nicht eingebbar
	bool avnachbearbeitung_per_zeichnung;

	/// Restabstand
	int restabstand;

	/// Typ
	int typ;

	/// Lange der Rohrkonstruktion in mm
	int laenge;

	/// Anzahl der Einzelrohre
	int nRohre;
	
	/// Rohrqualitaet
	int rohrQuali;
	
	/// Rohrverbindungen
	int rohrVerb; 

	/// max. Rohrlaenge
	int schnittlaenge;

	/// Neues Element in Elementliste packen
	void elementList_setElement(EnCObListModell& list, Element *ele,  int dn, int andn, int id, int abs, int rel, int laenge, int winkel, int rlaenge, bool correctID = false, bool initStamm = true);

	/// Neues Element in Elementliste packen
	void elementList_setElement(EnCObListModell& list, Element *elem, int rlaenge, bool correctID = false);

	/// Erstes Rohr in Elementliste setzen
	bool elementList_setRohr(EnCObListModell& elementList, int& rohrDN, int& rohrid, int& laengeVomRohrstueck, Element* ele);

	/// Abgaenge in Elementliste setzen
	void elementList_setAbgaenge(EnCObListModell& elementList, int& laengeVomRohrstueck, int& index, int& i, Element* elementRA, Element* ele);

	/// Ende vom Rohrstueck in Elementliste setzen
	void elementList_setEndeRohrstueck(EnCObListModell& elementList, int& rohrDN, int& rohrid, int& laengeVomRohrstueck, int& i, Element* elementRA, Element* ele, int summe1);

	/// Anfang vom Rohrstueck in Elementliste setzen
	void elementList_setAnfangRohrstueck(EnCObListModell& elementList, int& rohrDN, int& index, int& i, Element* elementVA, Element* ele, int redabst);

	/// Rohrende in Elementliste setzen
	void elementList_setRohrende(EnCObListModell& elementList, int& rohrDN, int& laengeVomRohrstueck, int& i);

	/// Rohrquali auf neuen Standard aendern
	virtual void switchRohrquali();

	virtual bool isRohr() const { return true; }

/// protected
protected:

	
	/// Nennweite
	int dnvl;

	
	//Kennzeichen fuer Trockenanlagen
	bool trockenanlage;
	bool m_bUeberlaenge;
	bool m_IsApaxBauteil;


	/// Alle AV-Elemente (Handschweißungen) der Rohrkonstruktion (für Strangrohre, Stationsverteiler)
	EnContainerRohrElemente avelementArray;

	/// Rohrelemente aus Dialog außer bei Strangrohren, Stationsverteiler. Über fill_elementArray werden avelementArray und stdelementArray in diesem Array vereinigt
	EnContainerRohrElemente elementArray;

	/// Elemente aus Dialog
	EnContainerRohrElemente elementArraySap;

private:
	
	/// Elemente aus Dialog
	EnContainerRohrElemente stdelementArray;
	
	/// Alle Elemente der Rohrkonstruktion fuer SAP
	EnCObListModell elementListSap;

protected:

	/// Alle Elemente der Rohrkonstruktion, public fuer VK3 sonst protected
	EnCObListModell elementList;

	/// Liste mit Elemente fuers Pluvern nach VK3
	EnCObListVk3 pulverlaengenListe;

	/// Liste mit Elementen fuer Passlaenge nach VK3
	EnCObListVk3 passlaengenListe;

	/// Rohrlaengen
	int rohrLaengen[256];

	/// Anzahl der Rohrnennweiten fuer die Ermittlung der Kranzeit
	CUIntArray anzahlNennweiten;

	/// Kappenfarbe
	int rohrKFarbe;

	/// ID des Rohrstueckes
	int rohrID[256];

	/// Element ID vom Rohranfang
	int rohrAnfang;

	/// Element ID vom Rohranfang
	int rohrEnde;

	/// Laenge vom Rohranfangselement
	int rohrAnfaLaenge;

	/// Laenge vom Rohrendeelement
	int rohrEndeLaenge;

	/// Laenge vom Rohrverbindungselement
	int rohrVerbLaenge;
	
	/// Druckstufe (bisher nur fuer Stationsverteiler)
	int rohrDruckstufe;

	/// Erzeugen der Afos fuers Anliefern
	virtual void createAnliefAfos(Arbeitsplan& plan);

	/// Berechnungs des Aufschlags fuer Rohrverschnitt
	virtual void addRohrausschuss(CObList& list);

	/// Alle Elemente des Elementarrays werden geprueft
	virtual void check_elementArray();

	/// Prüft Abstand Rohranfang/-ende zu Außenkante Abgang 
	std::string check_abstandRohranfangEnde(int minAnfang1,int minEnde1,
												int minAnfang2,int dnAnfang2,
												int minEnde2,int dnEnde2,
												bool bAnlage,  bool proRohr, bool bVonBauteilAussenkante);

	//Prüft Mindestabstand zwischen Stutzen und Muffen
	virtual std::string check_abstandZwischenStutzen(int minAbst,bool bAnlage,bool bVonBauteilAussenkante);


	/// Prueft, ob am Anfang eine Scheibe sitzt und die DN bzw. die Beschichtung ok ist
	virtual void check_AnfangEnde_ScheibeOk() const;

	/// Rohrqualitaet checken
	virtual void check_rohrQuali(int dnvl) const;
	
	/// Absoluten Abstand zur Rohrlaenge pruefen
	virtual void check_absAbst_rohrlaenge(const Element* ele);

	/// Prüfung des ersten und letzten Bogens auf Mindestabstände vom Rohrende
	void check_bogen_abstand_anfang_ende() const;

	void check_biegung()const;

	void checkHoeheBeschichten();

	/// Fuellen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
	/// Stationsverteiler oder Entnahmeleitungen
	virtual void fill_elementArray();

	/// Speichern und Laden der Rohrleitung. 
	/// Achtung:UpToDate halten!
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Liefert true zurück, wenn die Materialliste und die Arbeitsliste leer ist. 
	/// Wird in Rohrleitung überschrieben, damit auch die Elementliste überprüft werden kann.
	virtual bool isEmpty() const;

	/// Generieren der Stueckliste
	virtual void create_stueckliste();

	/// Liefert als default avelementArray, bei Verteiler allerdings elementArray
	virtual const EnContainerRohrElemente& getelemente() const;
	
	/// Errechnet die Pulverzeit fuer den Werkauftrag
	virtual double getPulverzeit();

	/// Errechnet die Putzzeit fuer den Werkauftrag
	virtual double getPutzzeit();

	/// Errechnet die Grundierzeit fuer den Werkauftrag
	virtual double getGrundierungszeit();

	/// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen
	virtual void rohreBeschriften(CObList& elementList);

	/// Rohrqualitaet bei Rohrstutzen setzen
	void virtual setQualiRohrstutzen(CObList& templist);

	/// Prueft die Positionsnummer auf nicht erlaubte Zeichen
	void check_posnr() const;

	/// Rohr DN checken
	void check_dnvl() const;
	
	/// Rohranfang, Rohrende und Rohrverbindung checken
	virtual void check_rohrAnfangEndeVerb() const;
	
	/// Temporaere Stueckliste generieren
	void create_templist(CObList& templist, CObList& elementList);
	
	/// Templiste in Stueckliste zusammenfassen
	void putTemplistInStueckliste(CObList& templist, CObList& stklist);

	/// Fuegt die AV Elemente je nach absolutem Abstand in das Elementarray ein.
	virtual void processAVElemente();

	
	/// Laenge von Anfang/Ende/Verbindung setzen
	void setAnfaEndeVerbLaenge();
	 
	/// Automatische Aufteilung der Rohrlaenge
	CString setAutoRohrlaengen();

	
	
	/// Erstellen der Elementliste der Rohrleitung fuer n Rohre mit Elementen auf dem Rohr
	bool makeRohre(EnCObListModell& elementList);

	/// Loescht die Objekte und die Pointer aus der Stueckliste
	void delete_stueckliste();
	
	/// Gesamtoberflaeche der Rohrleitung berechnen
	double getGesOberfl();

	/// Rohr mit Stutzen
	void setRohrMitStut();

	/// Berechnung der Laenge pro Nr.
	void calc_laengeProNr();

	/// set rohrLaengen
	void set_rohrLaengen(int index, int value);
		
	
	/// get stutProNr
	const CArray<bool,bool>& get_stutProNr() const;

	/// get laengeProNr
	const CArray<int,int>& get_laengeProNr() const;

	/// get dnProNr
	const CArray<int,int>& get_dnProNr() const;

	CString checkRohrlaenge(int value, Element* ele);

	/// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
	virtual CString checkRohrtrennstelle();


	/// Array rohrlaengen und rohrID auf Null setzen
	void reset_rohrlaengen_und_ids();

public:
	

	int getLaengeZuschnitt()const;

	virtual int getStandardRohrlaenge();

	/// Setzen der Rohrlaengen
	virtual CString setRohrlaengen(); 

	///Limitwerte fuer Eigenschaften
	enum LIMITS 
	{  
		///Mindestabstand zum Rohranfang bei Handschweißung
		A_MIN_ABSTAND_ANFANG = 40,

		///Mindestabstand zum Rohrende bei Handschweißung
		A_MIN_ABSTAND_ENDE = 40,

		///Mindestabstand zwischen Muffen und Stutzen
		A_MIN_ZWISCHEN_STUTZEN = 50
	};


	/// WAX - RohrqualiIDs, muessen mit den Rohrqualids in der Stammdatentabelle uebereinstimmen
	enum IDS 
	{  
		/// Nicht festgelegt
		LEER,
		/// DIN 2440/2458 standard geschweißt normalwandig, eleID = ROHR_Q1
		RQ2440_2458GSCH = 1,
		/// DIN 2440 nahtlos, eleID = ROHR_Q3
		RQ2440NLOS = 2,
		/// DIN 2448 nahtlos, normalwandig, eleID = ROHR_Q5
		RQ2448NLOS = 4,
		/// DIN 2440 verzinkt, eleID = ROHR_Q6
		RQ2440VZIN = 7,
		/// DIN 2458 verzinkt, eleID = ROHR_Q7
		RQ2458VZIN = 8,
		/// DIN 2458 standard, geschweisst, dickwandig, eleID = ROHR_Q8
		RQ2458DICK = 19,
		/// DIN 2458 standard geschweißt normalwandig, eleID = ROHR_Q9
		RQ2458GSCH = 21,
		/// DIN 2440 mittelschwer, geschweißt, eleID = ROHR_Q10
		RQ2440GSCH = 22,
		/// DIN EN10217-1 APSAD-Rohr, eleID = ROHR_Q11
		RQEN10217 = 26,
		///CO2
		RQEN10217_CO2 = 30,
		///DIN EN 10217-1 Standard, verzinkt
		RQEN10217VERZ = 31,
		//DIN EN 10255 Gewinderohr geschweißt (vormals DIN 2440) Kaltbandgeschweißt
		RQEN10255_KALTBAND = 32,
		//DIN EN 10255 Gewinderohr geschweißt verzinkt (vormals DIN 2440) Kaltbandgeschweißt
		RQEN10255_KALTBAND_VERZ = 33,

		

		RQ2448NLOS_CO2_DICK = 36,
		/// DIN 2458 standard geschweißt dickwandig(HD Rohr), eleID = ROHR_16  => Element ID = ROHR_Q16
		
		RQ2448NLOS_MX1230200 = 37,

		RQ2448NLOS_MX1230200_DN65_GEPULVERT_EXTRA_DICK = 38,

		RQ2458GSCH_DICK = 39,

		DEFAULT_ROHRQUALI = RQ2458GSCH
	};

	/// KappenfarbenIDs, muessen mit den Kappenfarbenids in der Stammdatentabelle uebereinstimmen
	enum KFS 
	{  
		/// Nicht festgelegt
		KEINE,	
		//7rot
		ROT,
		/// blau
		BLAU,	
		/// gelb
		GELB,	
		/// gruen
		GRUEN,	
		/// weiss
		WEISS,	
		/// schwarz
		SCHWARZ,
		/// gold
		GOLD,
		WEISSALU=8,
		PINK=9,
		BRAUN=10,
		ORANGE=11,
		LILA=12

	};	

	enum PRODUKTTYP 
	{  
		LEERTYP = 0,
		WASSER = 1,
		GAS = 2,
		CHEMIE = 3
	};

	

	/// Stutzen pro Nr., public fuer VK3 sonst private
	CArray<bool,bool> stutProNr;

	/// Laenge pro Nr., public fuer VK3 sonst private
	CArray<int,int> laengeProNr;

	/// DN pro Nr., public fuer VK3 sonst private
	CArray<int,int> dnProNr;

	/// Standardkonstruktor
	Rohrleitung();

	/// Destruktor
	virtual ~Rohrleitung();
	
	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Rohrkonstruktion erzeugen
	virtual void createModell();

	void elementeingabe_invertieren();

	/// Beschriftung für Grafikausgabe
	virtual std::string get_beschriftung(int dn, bool zoll, int rohrcount) const;

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Prüft die Rohrleitung und liefert evtl. einen Hinweis bzw. eine (unkritische) Warnung
	virtual CString checkHinweis() const;

	/// Initialisierung der Daten
	virtual void initDaten(int anzahl, int dnvl, int laenge, CString posNr, int rohrQuali, int rohrVerb);

	/// Gesamtgewicht der Rohrleitung berechnen
	virtual double getGesGewicht(int teile = 0) const;

	/// Liefert die Anzahl der Rohrleitungen
	virtual int getAnzDefLaengen();

	virtual const int getMuffenRestabstand() const;

	virtual const CString getBemassungstext() const;

	virtual const int getMuffenRelAbst(int index) const;

	/// Liefert die maximale Anzahl an Standardelementen für den Rohrleitungstyp
	virtual const int getMaxAnzahlStdelemente() const;

	/// Prueft ob Verteiler ueber Verteileranlage laeuft
	bool checkAnlage() const;

	/// get schnittlaenge
	int get_schnittlaenge();

	/// set schnittlaenge
	void set_schnittlaenge(int value);

	bool setSchnittlaenge();



	/// Kopierfunktion fuer alle Attribute
	/// Achtung: UpToDate halten!
	virtual CString copy(const Rohrleitung& other);

	/// Rohrstutzen auf Abschlusselement pruefen 
	CString check_rohrstutzen();

	/// Prüfen ob anstatt Feuerverzinkt auch Werksverzinkt möglich ist
	CString check_werksVerzinkt() const;

	/// Berechnet die richtige Rohrstutzenlaenge
	int berechnungRohrstutzen(int eleDn, int anschlussDN, int eleLaenge, int flanschLaenge,bool anlage);

	/// Ermittelt die Kranzeit fuer die Afos 16/17 (Vorrichten und Schweissen)
	double getKranzeit(bool mitVerlaengerung = false);

	/// Kranzeit berechnen
	double calcKranzeit(CUIntArray& anznennw);
	
	/// Ermittelt die Anzahl der Rohre nach Nennweiten fuer Kranzeit
	virtual void setAnzahlNennweiten(bool mitVerlaengerung = false);

	void set_anzahlNennweiten(int dn, int laenge, double gewicht, bool mitVerlaengerung = false);

	/// Je nach Bedingung wird die richtige ID geliefert
	virtual int correctElemID(int id, int dn) const;

	/// Liefert den String der Kappenfarbe zurueck
	static CString getKappenfarbe(int farbe);
	

	/// Emittelt die VK3Elemente fuer die Passlaengen
	void createPasslaengenListe();

	/// Emittelt die VK3Elemente fuers Pulvern
	void createPulverlaengenListe();

	///vk3liste erstellen
	virtual bool createVk3Liste();

	/// Gibt an ob Rohrkonstruktion zwei Rohrqualis hat
	/// (2440 & 2458 std. bei Reduzierung moeglich)
	virtual bool hasRohr2Quali() const;

	/// Prüft die Elementliste auf Nicht-Standard-Winkel
	bool hasNonStandardWinkel() const;

	/// Gibt an ob ein Bogen in Rohrkonstruktion
	virtual bool isBogen() const;

	

	/// Ist ein Bogen in der Konstruktion?
	const virtual bool isRedBogEinz();

	/// Gibt an ob eine Reduzierung in Rohrkonstruktion
	const virtual bool isRed() const;

	/// Ob Das Rohr einen Knick hat
	const Element* getKnick()const;

	/// Prueft ob Abgaenge da sind, die geschweisst werden muessen
	const bool isSchweissen() const;

	/// Gibt an ob Rohrkonstruktion eine glatte Laenge ohne Abgaenge ist
	const bool isRohrGlatt() const;

	/// Liefert den ersten Abgang der Rohrleitung
	const Element* Rohrleitung::getLastAbgang() const;

	/// Liefert den letzten Abgang der Rohrleitung
	const Element* Rohrleitung::getFirstAbgang() const;

	/// Gibt an ob Rohrstutzen in Rohrkonstruktion vorhanden
	const int isRohrstutzen(int drehfaktor = 0) const;

	/// Legt neue Drehrichtung fest
	int setNewDrehfaktor(int drehfaktor, int winkel) const;

	/// Liefert die Anzahl der Rohtstuecke in der Rohrleitung
	int getAnzahlRohrstuecke();

	int getMaxXLaenge();

	int getMaxYLaenge();

	int getMaxAbgLaenge() const;

	/// Liefert die groesste Rohrnennweite in der Rohrkonstruktion
	int getMaxRohrDN() const;

	/// Liefert die kleinste Rohrnennweite in der Rohrkonstruktion
	int getMinRohrDN() const;
	
	/// Liefert die Nennweite des letzten Rohrstuecks
	int getLastRohrDN() const;

	/// Fuegt einen neuen Radius ein
	void add_radien(int radius);

	/// Ruestzeit fuers Klinken
	void addRuestzeiten();

	/// Generiert den Standardfertigungstext
	//void setFertigungshinweis();

	/// Sortiert die VK3Elemente
	void sortVk3Listen(CObList &liste);

	/// get radien
	const CArray<int, int>& get_radien() const;

	/// Typ: Wasser oder Gas
	virtual int get_typ() const;
	
	void set_trockenanlage(bool value) { trockenanlage = value; };
	bool get_trockenanlage() const { return trockenanlage; };

	void set_Ueberlaenge(bool value) { m_bUeberlaenge = value; };
	bool get_Ueberlaenge() const { return m_bUeberlaenge; };

	void set_IsApaxBauteil(bool value) { 
		m_IsApaxBauteil = value; 
	};
	bool get_IsApaxBauteil(){
		return m_IsApaxBauteil;
	}; 



//Rückgabe der Elementarrays

	/// get stdelementArray
	EnContainerRohrElemente& get_stdelementArray();

	/// get elementArray
	EnContainerRohrElemente& get_elementArray();

	/// get stdelementArray
	const EnContainerRohrElemente& get_stdelementArray() const;

	/// get elementArray
	const EnContainerRohrElemente& get_elementArray() const;


	/// get avelementArray
	EnContainerRohrElemente& get_avelementArray();

	///  get avelementArray
	const EnContainerRohrElemente& get_avelementArray() const;

	/// Liefert ja nach Rohrleitungstyp das (AV)Element Array
	virtual EnContainerRohrElemente& getElementArray();


	/// Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
	virtual CString getTabname();


	/// get elementListSap
	EnCObListModell& get_elementListSap();

	/// get elementList
	const EnCObListModell& get_elementList() const;

	EnCObListModell& get_elementList();

	
	

	/// get pulverlaengenListe
	EnCObListVk3& get_pulverlaengenListe();

	const EnCObListVk3& get_pulverlaengenListe() const;

	/// get passlaengenListe
	EnCObListVk3& get_passlaengenListe();
	
	const EnCObListVk3& get_passlaengenListe() const;

	/// get dnvl
	const int get_dnvl() const;

	/// set dnvl
	void set_dnvl(int value);

	/// get rohrLaengen
	const int get_rohrLaengen(int index) const;

	/// get laenge
	const int get_laenge() const;

	/// set laenge
	void set_laenge(int value);

	/// get rohrQuali
	int get_rohrQuali() const;

	/// set rohrQuali
	void set_rohrQuali(int value);

	/// get rohrKFarbe
	const int get_rohrKFarbe() const;

	/// set rohrKFarbe
	void set_rohrKFarbe(int value);

	/// get rohrDruckstufe
	const int get_rohrDruckstufe() const;

	/// set rohrDruckstufe
	void set_rohrDruckstufe(int value);

	/// get rohrAnfang
	const int get_rohrAnfang() const;

	/// set rohrAnfang
	void set_rohrAnfang(int value);

	/// get rohrEnde
	const int get_rohrEnde() const;

	/// set rohrEnde
	void set_rohrEnde(int value);

	/// get nRohre
	const int get_nRohre() const;
 
	/// set nRohre
	void set_nRohre(int value);

	/// get rohrVerb
	const int get_rohrVerb() const;

	/// set rohrVerb
	void set_rohrVerb(int value);

	/// get restabstand
	const int get_restabstand() const;

	/// set restabstand
	void set_restabstand(int value = 0);

	/// get fertigungshinweis
	const CString& get_fertigungshinweis() const;

	std::vector<std::string> Rohrleitung::getFertigungshinweis() const;

	/// set fertigungshinweis
	void set_fertigungshinweis(const CString& value);

	/// get rohrVerbLaenge
	const int get_rohrVerbLaenge() const;

	/// get rohrEndeLaenge
	const int get_rohrEndeLaenge() const;

	/// get rohrAnfaLaenge
	const int get_rohrAnfaLaenge() const;

	bool get_avnachbearbeitung() const;

	void set_avnachbearbeitung_per_zeichnung(bool value);

	bool get_avnachbearbeitung_per_zeichnung() const;

	/// get stdfertigungshinweis
	CString get_stdfertigungshinweis() const;

	/// set stdfertigungshinweis
	//void set_stdfertigungshinweis(const CString& value);

	/// get ruesten
	const bool get_ruesten(int index) const;

	/// set ruesten
	void set_ruesten(int index, bool value);

	/// get anzahlNennweiten
	int get_anzahlNennweiten(int index);

	///Anhand der RohrQuali ID dden kurztext bestimmen
	CString getRohrQualikurztext();

	static CString getRohrQualikurztext(int rohrQualiId,int dnvl);
	
	/// Liefert die Rohrqualität zu einer Elementid
	static Rohrleitung::IDS elementid2rohrquali(const Element::IDS eleids);
	
	int getDin();

	/// get stuecklisteSap  
	EnStueckliste& get_stuecklisteSap();

	/// Allen Elementen einen Korrekturwinkel zuweisen
	void apply_korrektur_winkel(int winkel)
	{
		for (int x = 0; x < get_elementArray().GetSize(); x++)
		{
			Element* e = get_elementArray().get_element_at(x);
			if (!e)
				continue;
			e->set_korrektur_winkel(360-winkel);
		}
	}

protected:
	/// Stueckliste fuer SAP, Materilabestellung
	EnStueckliste stuecklisteSap;
	 
	/// Loeschen der Stueckliste fuer SAP
	void delete_stuecklisteSap();

	/// Erzeugt Elementliste und Stueckliste fuer Sap
	void makeRohrleitungSap();

	/// Elemente von Standardelemente in Verteilerelemente aendern
	bool konvert_elementArraySap();

	/// Generieren der Stueckliste fuer SAP
	void create_stuecklisteSap();

	/// Liefert ein Anfangselement
	const Element get_anfang_element() const;

	/// Liefert ein Abschlusselement
	const bool get_abschluss_element(Element &ele) const;

	/// Mindestrohrlänge bzw. Mindestabstand
	int get_min_rohrlaenge(const Element& elem) const;

	bool calcZuschnittBeiBiegung();

	double calcBiegenZuschlag()const;
		

#ifdef _DEBUG
	//Ausgabe der Attribute
	virtual void Dump(CDumpContext& dc) const;
#endif
};


