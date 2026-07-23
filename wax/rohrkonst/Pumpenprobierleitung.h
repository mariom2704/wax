#pragma once

#include "Verteiler.h"

/**
Pumpenprobierleitung
Spezialfall von Verteiler
*/
class ROHRKONSTDLL Pumpenprobierleitung : public Verteiler  
{
public:
	/// Konstruktor
	Pumpenprobierleitung();

	/// Destruktor
	virtual ~Pumpenprobierleitung();

	/// Index in der Struktur der Minimalwerte in get_minval.
	/// Je nach Kombination von DN1 und DN2 kann der entsprechende Minimalwert 
	/// ermittelt werden.
	enum MINWERTE 
	{
		L1 = 0,
		L2 = 1,
		L3 = 2,
		L4 = 3,
		L5 = 4,
		MESSBLSCHIEBER = 5,
		D = 6,
		
		LGES = 10,
	};
	
	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Rohrkonstruktion erzeugen
	virtual void createModell();

	const int get_anschlussA() const;

	void set_anschlussA(int value);

	const int get_anschlussB() const;

	void set_anschlussB(int value);

	const int get_laenge2() const;

	void set_laenge2(int value);

	const int get_laenge3() const;

	void set_laenge3(int value);

	const int get_laenge1() const;

	void set_laenge1(int value);

	const int get_dn1() const;

	void set_dn1(int value);

	const int get_dn2() const;

	void set_dn2(int value);

	/// Prüft die DN1/DN2 Kombination und liefert den zugehörigen Minimalwert für 
	/// L4/L5/d oder Lges. valid ist ein Wert aus MINWERTE.
	/// Wird für dn1, dn2 Null übergeben, werden die Objektdaten benutzt.
	CString get_minval(int valid, int dn1 = 0, int dn2 = 0) const;

	/// Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
	virtual CString checkRohrtrennstelle();

	virtual bool isPumpenprobierleitung() const { return true; }

protected:
	/// Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
	/// Stationsverteiler oder Entnahmeleitungen
	virtual void fill_elementArray();

	/// Speichern und Laden der Rohrleitung. Achtung:
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

private:
	/// Element-ID des Anschlusses A
	int anschlussA;

	/// Element-ID des Anschlusses B
	int anschlussB;

	/// Länge L1
	int laenge1;

	/// Länge L2
	int laenge2;

	/// Länge L3
	int laenge3;

	int dn1;

	int dn2;

	/// Setzt das Bemerkungsfeld auf den Standardtext. Erweitert wird der Text um 
	/// den Hinweis auf die Bohrungsgrösse für die Muffen.
	void setBemerkung();

	/// Wird hier überschrieben, damit die Bohrung nicht angemeckert wird (Abstand 
	/// 55 mm lt. Plausis min. 60 mm)
	virtual void check_absAbst_rohrlaenge(const Element * ele) ;

};

