#pragma once

#include "Rohrleitung.h"

class Element;

/**
Verteiler - Definition des Verteilers
*/
class ROHRKONSTDLL Verteiler : public Rohrleitung  
{
	/// Limitwerte fuer Eigenschaften
	enum LIMITS 
	{  
		MIN_DNVL = 25,

		MAX_DNVL = 300,

		MAX_ROHRLAENGE_DUENN = 9000,

		MAX_ROHRLAENGE = 6000,

		MAX_GESAMTLAENGE = 12000,
	};

public:
	/// Limitwerte fuer Eigenschaften
	enum ANLAGE_LIMITS 
	{  
		/// Minimale Rohrnennweite
		A_MIN_DNVL = 65,

		/// Maximale Rohrnennweite
		A_MAX_DNVL = 150,

		/// Minimale Muffennennweite
		A_MIN_MUFFE_DN = 25,

		/// Maximale Muffennennweite
		A_MAX_MUFFE_DN = 50,

		/// Minimale Stutzennennweite
		A_MIN_STUTZEN_DN = 32,

		/// Maximale Stutzennennweite
		A_MAX_STUTZEN_DN = 125,

		/// Minimal Stutzenlaenge von Rohroberkannte bis Elementende
		A_MIN_STUTZEN_LAENGE = 50,

		/// Maximale Stutzenlaenge von Rohrmitte bis Elementende
		A_MAX_STUTZEN_LAENGE = 200,

		/// Minimale Anzahl pro Rohr 
		A_MIN_ANZAHL_STDELEMENTE_PROROHR = 1,

		/// Maximale Anzahl pro Rohr
		A_MAX_ANZAHL_STDELEMENTE_PROROHR = 5,

		/// Mindestabstand zum Rohranfang
		A_MIN_ABSTAND_ANFANG = 400,

		/// Mindestabstand zum Rohrende bis Stutzennennweite DN100
		A_MIN_ABSTAND_ENDE = 100,

		/// Mindestabstand zum Rohrende bei stutzennennweite DN125
		A_MIN_ABSTAND_ENDE_DN125 = 125,

		/// Minimale Rohrlaenge 
		A_MIN_ROHRLAENGE = 1500,

		/// Maximale Rohrlaenge
		A_MAX_ROHRLAENGE = 6000,

		/// Mindesabstand zwischen Stutzen und Muffen
		A_MIN_ZWISCHEN_STUTZEN = 500,

		/// Maximal 6 Abgänge über die Anlage
		MAX_ANZAHL_ABGAENGE = 6

	};

public:
	/// Standardkonstruktor
	Verteiler();

	void handleSprinklerSchelle();
	
	/// Destruktor
	virtual ~Verteiler();

	virtual void check_rohrQuali(int dnvl) const;

	/// Erzeugen der Afos fuer die Beschichtung
	virtual void createBeschAfos(Arbeitsplan& plan);

	/// Liefert die Anzahl der Rohrleitungen
	virtual int getAnzDefLaengen();

	/// Ermittelt die Anzahl der Rohre nach Nennweiten fuer Kranzeit
	virtual void setAnzahlNennweiten(bool mitVerlaengerung = false);

	/// Speichern und Laden der Rohrleitung.
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Liefert die maximale Anzahl an Standardelementen für den Rohrleitungstyp
	virtual const int getMaxAnzahlStdelemente() const;

	/// getMuffenRelAbst
	virtual const int getMuffenRelAbst(int index) const;

	virtual const int getMuffenRestabstand() const;

	/// Kopierfunktion fuer den Verteiler
	void copy(Verteiler& other);

	/// Kopierfunktion fuer den Verteiler
	virtual void copy(Rohrleitung& other);

	/// Liefert false zurück, wenn im String falsche Eingaben gemacht wurden. 
	/// Erlaubt sind nur Zahlen, Leerzeichen und Punkte.
	bool setrohrDefLaengen();
	
	/// Das Rohrlaengenfeld checken und in defLaenge[] packen
	bool check_rohrDefLaengen(CString &error, int rohrQuali, int rohrDN);

	/// Rohrkonstruktion erzeugen
	void createModell();

	/// const int get_defLaengen(int index) const;

	const CString& get_rohrDefLaengen() const;

	void set_rohrDefLaengen(const CString& value);

	/// Standard checken
	std::string checkStdVl();

	/// Rohre checken
	CString checkRohre();

	/// Standardelemente checken
	CString checkStdElemente();

	/// Abstaende der Standardelemente checken
	std::string checkAbstandStdElemente();

	/// Rohrlänge zurückgeben. Wenn Def. Längen definiert sind, wird längstes Teilstück zurückgegeben
	const int getMaxRohrlaenge() const;

	virtual bool isVerteiler() const { return true; }

protected:
	/// Rohrquali auf neuen Standard aendern
	virtual void switchRohrquali();

	/// Berechnung der Rohrstuecke nach definierten Rohrlaengen ohne Elemente
	CString calDefRohrlaengenOhneEle();

	/// Berechnung der Rohrstuecke nach definierten Rohrlaengen
	CString calDefRohrlaengen();

	///  Rohrqualitaet bei Rohrstutzen setzen
	virtual void  setQualiRohrstutzen(CObList& templist);

	
	/// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen
	virtual void rohreBeschriften(CObList& elementList);

	/// Errechnet die Pulverzeit für den Werkauftrag
	virtual double getPulverzeit();

	/// Alle Elemente des Elementarrays werden geprueft
	virtual void check_elementArray();

	/// Liefert als default avelementArray, bei Verteiler allerdings elementArray
	virtual const EnContainerRohrElemente& getelemente() const;

	void setmaxLaenge();

	/// Laenge von Anfang/Ende/Verbindung setzen
	void setAnfaEndeVerbLaenge();


private:
	/// vom Benutzer def. Rohrlaengen als einzelner String
	CString rohrDefLaengen;

	/// defLaengen als Int-Array.  Diese Werte werden aus dem String rohrDefLaengen 
	/// berechnet.
	CArray<int,int> m_defLaengen; /// Änderung 04.04.2005 
	/// int defLaengen[256];

	/// Setzen der Rohrlaengen
	virtual CString setRohrlaengen();

	/// Definierte Rohrunterteilung
	CString setDefRohrlaengen();

	/// Liefert Zwischensumme der bereits abgearbeiteten Laengen
	int getSummeDeflaengen(int index);

	/// Afos fuer Sap 
	bool sap;
};

