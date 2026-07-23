#pragma once

#include "Rohrleitung.h"

/**
Stationsverteilers - Definition des Stationsverteilers
*/
class ROHRKONSTDLL Stationsverteiler : 
public Rohrleitung  
{
public:
	enum LIMITS 
	{
		MAX_LAENGE_STDELEMENTE = 500,

		MAX_ANZAHL_STDELEMENTE = 8,

		MAXCOUNT = 6,

		MAX_GESAMTLAENGE = 12000
	};

protected:
	/// Das Array mit Elementen fuellen
	virtual void fill_elementArray();

private:
	int extraAbgObenAbsAbst;

	int extraAbgObenDN;

	int extraAbgObenID;

	int extraAbgObenLaenge;

	/// Berechnet aus dem Elementarray die relativen Abstände der Muffen.
	void set_muffenRelAbst();

	/// Relativer Abstand der Muffen, wird für die Grafik benötigt. Anzahl ist 
	/// MAX_ANZAHL_STDELEMENTE w.g. Extraabgang.
	int muffenRelAbst[MAX_ANZAHL_STDELEMENTE];

	/// Nennweite der Stutzen
	int	elementDN[MAXCOUNT];

	/// relative Abstände der Stutzen
	int	elementRelAbst[MAXCOUNT];
	
	/// absolute Abstände der Stutzen
	int	elementAbsAbst[MAXCOUNT];

	/// ID der Stutzen
	int elementID;

	/// Länge der Stutzen
	int	elementLaenge;

	/// ID des Elementes am Anfang des Verteilers
	int	elementlEnde;

	/// ID des Elementes am Ende des Verteilers
	int	elementrEnde;

	/// Abstand des Abganges
	int	extraAbgAbsAbst;

	/// Nennweite des Abganges
	int	extraAbgDN;
		
	/// ID des Abganges
	int	extraAbgID;

	/// Länge des Abganges
	int extraAbgLaenge;

	/// Elementlaenge von Rohranfang
	int elementlEndeLaenge;

	/// Elementlaenge von Rohrende
	int elementrEndeLaenge;
	
	/// Laenge Rohrstuecke
	int rohrLaengen[256];

	/// Laenge von Anfang/Ende/Verbindung setzen
	void setAnfaEndeVerbLaenge();

public:
	/// /Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
	virtual CString getTabname();

	/// /Liefert ja nach Rohrleitungstyp das (AV)Element Array
	virtual EnContainerRohrElemente& getElementArray();

	virtual const int getMuffenRelAbst(int index) const;

	/// /Liefert die maximale Anzahl an Standardelementen für den Rohrleitungstyp
	virtual const int getMaxAnzahlStdelemente() const;

	virtual const int getMuffenRestabstand() const;

	const int get_extraAbgObenAbsAbst() const;

	void set_extraAbgObenAbsAbst(int value);

	const int get_extraAbgObenDN() const;

	void set_extraAbgObenDN(int value);

	const int get_extraAbgObenID() const;

	void set_extraAbgObenID(int value);

	const int get_extraAbgObenLaenge() const;

	void set_extraAbgObenLaenge(int value);

	const int get_muffenRelAbst(int index) const;

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	CString checkStdElemente();

	/// / Je nach Bedingung wird die richtige ID geliefert
	virtual int correctElemID(int id, int dn) const;


	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Standardkonstruktor
	Stationsverteiler();

	/// Destruktor
	virtual ~Stationsverteiler();

	/// Speichern und Laden ders Stationsverteilers.
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Elementdaten setzen
	void setElementdaten(int elementDN[6], int elementRelAbst[6], int elementID, int elementLaenge,	int elementlEnde, int elementrEnde, 
		int extraAbgAbsAbst, int extraAbgDN, int extraAbgID, int extraAbgLaenge, int extraAbgObenAbsAbst, int extraAbgObenDN, int extraAbgObenID, int extraAbgObenLaenge);

	/// Erzeugung der Rohrkonstruktion
	void createModell();
	
	const int get_extraAbgAbsAbst() const;

	void set_extraAbgAbsAbst(int value);

	const int get_extraAbgDN() const;

	void set_extraAbgDN(int value);

	const int get_extraAbgID() const;

	void set_extraAbgID(int value);

	const int get_extraAbgLaenge() const;

	void set_extraAbgLaenge(int value);

	const int get_elementDN(int index) const;

	void set_elementDN(int index, int value);

	const int get_elementRelAbst(int index) const;

	void set_elementRelAbst(int index, int value);

	const int get_elementID() const;

	void set_elementID(int value);
	
	const int get_elementLaenge() const;

	void set_elementLaenge(int value);

	const int get_elementlEnde() const;

	void set_elementlEnde(int value);

	const int get_elementrEnde() const;

	void set_elementrEnde(int value);

	virtual bool isStationsverteiler() const { return true; }

};


