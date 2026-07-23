#pragma once
#include "Rohrleitung.h"





/// Strangrohrs - Definition des Strangrohrs
class ROHRKONSTDLL Strangrohr : public Rohrleitung  
{
	

public:
	enum LIMITS 
	{  
		MIN_DNVL = 32,

		MAX_DNVL = 80,

		MIN_MUFFE_DN = 10,

		MAX_MUFFE_DN = 25,

		MIN_ABSTAND_ANFANG = 100,

		MIN_ABSTAND_ENDE = 100,

		MIN_ABSTAND_SONDERMUFFE_L60 = 130,

		MIN_RELATIVER_ABSTAND = 300,

		MAX_ANZAHL_STDELEMENTE = 10,

		MIN_ROHRLAENGE = 1400,
	
		MAX_ROHRLAENGE = 6000,
	
		MAX_GESAMTLAENGE = 12000,

		MIN_SONDERMUFFEN_LAENGE = 10,

		MAX_SONDERMUFFEN_LAENGE = 80
	};

	

private:

	
	int  BohrschelleBohrung;  // Merkt sich den BohrungsDurch bevor dieser geschloescht wird

	double ElementAnzahldnMuffeOrDBohrung;
	int ElementAnzahlid;
	/// DN der Standardelemente
	int muffenDN;
	bool muffenDNSET80;

	/// Standardelement (Muffe, Bohrung)
	int muffenID;

	/// Winkel der Standardelemente
	int muffenWinkel;

	/// Muffen gekehlt
	bool muffenGekehlt;

	/// Sondermuffenlänge von OKR - OK Muffe
	int sonderMuffenLaenge;

	/// Restabstand nach letztem Standardelement
	int muffenRestabstand;

	/// Relative Abstaende der Standardelemente
	int muffenRelAbst[MAX_ANZAHL_STDELEMENTE];

	/// die Laengen der Rohrstuecke
	int rohrLaengen[256];

	double durchmesser;

	int get_muffenLaenge();

	/// Setzt die Standardmuffen Daten
	void detect_stdmuffe(const EnContainerRohrElemente& from);

	/// Verschiebt alle Nicht-Standardmuffen aus from nach to (also i.a. vom elementArray ins avelementArray)
	void move_avelements(EnContainerRohrElemente& from, EnContainerRohrElemente& to);
	
	/// Rotiert alle Elemente in einem Container um winkel Grad
	void rotate_elements(EnContainerRohrElemente& elementarray, int winkel) const;

	/// Initialisierung des Arrays muffenRelAbst
	void init_muffenRelAbst(EnContainerRohrElemente& elementArr, EnContainerRohrElemente& avelementArr);

protected:
	/// Prüft, ob am Anfang eine Scheibe sitzt und die DN bzw. die Beschichtung ok 
	/// ist. Bei verzinkten Rohren immer false
	virtual void check_AnfangEnde_ScheibeOk() const;

	/// Berechnung des Aufschlags fuer Rohrverschnitt
	virtual void addRohrausschuss(CObList& list);

	virtual void fill_elementArray();

	/// Erzeugen der Afos fuer die Beschichtung
	virtual void createBeschAfos(Arbeitsplan& plan);

public:
	void changeMuffenToDependsOnPipe80();
	virtual void processAVElemente();
	int getRelLaengeStdMuffe(int nr)const;
	void changeElementArrayMuffenTöSchellen();
	
	int getElementAnzahlid(){ return  ElementAnzahlid;};
	double getElementAnzahldMuffeOrDBohrung() {return ElementAnzahldnMuffeOrDBohrung;};

	virtual void check_rohrQuali(int dnvl) const;

	/// Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
	virtual CString getTabname();

	/// Liefert ja nach Rohrleitungstyp das (AV)Element Array
	virtual EnContainerRohrElemente& getElementArray();

	virtual const int getMuffenRestabstand() const;

	virtual const int getMuffenRelAbst(int index) const;

	/// Liefert die maximale Anzahl an Standardelementen für den Rohrleitungstyp
	virtual const int getMaxAnzahlStdelemente() const;

	/// vk3 Liste erstellen
	virtual bool createVk3Liste();

	/// Initialisierung der Daten
	virtual void initDaten(int anzahl, int dnvl, int laenge, CString posNr, int rohrQuali, int rohrVerb);

	/// Gesamtgewicht der Rohrleitung berechnen
	virtual double getGesGewicht(int teile = 0) const;

	/// Berechnung des Aufschlags fuer Muffen
	void addMuffenausschuss(CObList& list);


	//gibt die Rohrlänge des Rohres zurück auf dem die Standardelemente liegen
	virtual int getStandardRohrlaenge();
	
	
	/// Liefert der Anzahl der Standardelemente auf dem Strang
	int getAnzahlMuffen();

	CString checkStdElemente();

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Standardkonstruktor
	Strangrohr();
	
	/// Destruktor
	virtual ~Strangrohr();

	void copy(Strangrohr& other);

	virtual CString copy(const Rohrleitung& other);

	
	/// Speichern und Laden des Strangrohrs
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);
	void createAutoArbeitMagdeburg(bool beschichtung);

	bool checkPlausiFertigungMagdeburg(CString& strError);

	virtual void create_stueckliste();
	/// Rohrkonstruktion erzeugen
	virtual void createModell();

	const int get_muffenRestabstand() const;

	void set_muffenRestabstand(int value);

	const bool get_muffenGekehlt() const;

	void set_muffenGekehlt(bool value);

	const int get_muffenWinkel() const;

	void set_muffenWinkel(int value);

	const int get_muffenID() const;

	void set_muffenID(int value);

	const int get_muffenRelAbst(int index) const;

	void set_muffenRelAbst(int index, int value);

	int get_muffenDN();

	void set_muffenDN(int value);

	void set_durchmesser(double value);

	const double get_durchmesser() const;

	int get_sonderMuffenLaenge() const;

	void set_sonderMuffenLaenge(int laenge);


	///  Liefert true, wenn im Sammelauftrag die Nacharbeit überprüft werden muss
	virtual bool avNacharbeitsPruefung() const;

	/// verschiebt die relativen Abstaende nach links, wenn zwischendrin einer 0 ist 
	void sortMuffen();

	/// bererchnet die nachfolgenden relativen Abstaende neu, wenn zwischendrin einer 0 gesetzt wird
	void setMuffenRelAbst(int index, int value);

	///  Liefert eine spezielle Kurzbezeichnung für das Anfangs bzw. Endeelement
	std::string get_abschluss_kurzbezeichnung(bool anfang) const;

	/// Muffenlaenge je nach Typ zurückgeben
	int getMuffenLaenge();

	virtual bool isStrangrohr() const { return true; }

};

