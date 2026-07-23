#pragma once

#include "Verteiler.h"

/**
Pumpenverteiler
*/
class ROHRKONSTDLL Pumpenverteiler : public Verteiler  
{
private:
	/// mit Reduzierstück ?
	bool reduziert;
	
	/// Abstand der Schweißmuffe (Anschlußlänge L4)
	int abstandSchwMuffe;
	
	/// Schweissmuffe ?
	bool schwMuffe;
	
	/// Winkel der Schweißmuffe
	int winkelSchwMuffe;
	
	/// Nennweite des Pumpendruckstutzen
	int dnPumpDruckSt;
	
	/// Winkel des Rohrnippels
	int winkelRohnippel;
	
	/// Nennweite des Probieranschlusses
	int dnProbAnschl;
	
	/// ID des Elementes
	int probAnschl;
	
	/// Länge des Probieranschlusses (Anschlusslänge L1)
	int laengeProbAnschl;
	
	/// Abstand des Probieranschlusses (Anschlusslänge L2)
	int abstandProbAnschl;
	
	/// ID des Elementes Sprinkleranlagenanschluß
	int sprAnlAnschl;
	
	/// Nennweite des  Sprinkleranlagenanschlusses
	int dnSprAnlAnschl;
	
	/// Gesamthöhe
	int gesamthoehe;

public:
	const int get_abstandProbAnschl() const;

	void set_abstandProbAnschl(int value);

	const int get_abstandSchwMuffe() const;

	void set_abstandSchwMuffe(int value);

	const int get_dnProbAnschl() const;

	void set_dnProbAnschl(int value);

	const int get_dnPumpDruckSt() const;

	void set_dnPumpDruckSt(int value);

	const int get_dnSprAnlAnschl() const;

	void set_dnSprAnlAnschl(int value);

	const int get_gesamthoehe() const;

	void set_gesamthoehe(int value);

	const int get_laengeProbAnschl() const;

	void set_laengeProbAnschl(int value);

	const int get_probAnschl() const;

	void set_probAnschl(int value);

	const bool get_reduziert() const;

	void set_reduziert(bool value);

	const bool get_schwMuffe() const;

	void set_schwMuffe(bool value);

	const int get_sprAnlAnschl() const;

	void set_sprAnlAnschl(int value);

	const int get_winkelRohnippel() const;

	void set_winkelRohnippel(int value);

	const int get_winkelSchwMuffe() const;

	void set_winkelSchwMuffe(int value);

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Rohrkonstruktion erzeugen
	virtual void createModell();

	/// Konstruktor
	Pumpenverteiler();

	/// Destruktor
	virtual ~Pumpenverteiler();

	virtual bool isPumpenverteiler() const { return true; }
protected:
	/// create stueckliste
	virtual void create_stueckliste();

	/// Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
	/// Stationsverteiler oder Entnahmeleitungen
	virtual void fill_elementArray();

	/// Speichern und Laden der Rohrleitung. Achtung:
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
	virtual CString checkRohrtrennstelle();
};

