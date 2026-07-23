#pragma once

class Werkauftrag;

#include "VK3Berechnung.h"
#include <afxcoll.h>

//Preis - Definition der Preise
class ROHRKONSTDLL Preis 
{
public:
	//Standardkonstruktor
	Preis(bool vk3);

	//Destruktor
	virtual ~Preis();

	//Speichern und Laden der Preisdaten
	virtual void serialize(MxFilesystem::BinaryArchive & ar);

	void copy(const Preis& other);
	
	static double getValueGerundet(double value);

	//Loeschen der Liste mit Sonderkosten
	void delete_sokolist(bool deleteFixedSoko);

	//Automatische Erzeugung einiger Sonderkosten
	void calcAutoSoko(bool deleteFixedSoko);

	//Berechnung
	void calc_sonderkosten();

	//Werkauftrag setzen
	void set_werkauftrag(Werkauftrag* value);

	//Liste mit Sonderkostenobjekten
	 CObList &  get_sokolist();

	const CObList &  get_sokolist() const;

	 //get sonderkosten
	double get_sonderkosten();

	//set sonderkosten
	void set_sonderkosten(double value);
	
	//alle Preise berechnen
	void calc_all();


	void set_sokonr(int value);


	//Lädt und speichert die Liste mit Sonderkosten
	void serialize_soko(MxFilesystem::BinaryArchive& ar);

	//Erzeugt bei Änderungen die Rohrliste neu, erstellt die Afos und 
	//Sonderkosten
	void updatePreis();

	
	enum
	{
		DEFAULT_VERZINKEREI = 255,
	};

	//Sonderkosten
	class ROHRKONSTDLL Sonderkosten : public CObject  
	{
	// private
	private:
		//Lfd. Nr.
		int nr;

		//Text
		CString text;

		//Preis
		double preis;

		bool isfix;

	// public
	public:
		//Standardkonstruktor
		Sonderkosten();

		/// Copykonstruktor
		Sonderkosten(const Sonderkosten &other);

		//Destruktor
		virtual ~Sonderkosten();

		//Speichern und Laden der Sonderkostendaten
		virtual void serialize(MxFilesystem::BinaryArchive & ar);

		//Kopieren der Sokoattribute
		void copy(const Sonderkosten& other);

		//get preis
		double get_preis() const;

		//set preis
		void set_preis(double value);

		//get text
		const CString& get_text() const;

		//set text
		void set_text(const CString& value);
	
		//get nr
		const int get_nr() const;

		//set nr
		void set_nr(int value);

		//Ob Preis fxiert ist und nicht überschrieben wird
		bool get_isfix() const;
		void set_isfix(bool value);

		// Friend
		friend class Preis;
	};


	VK3Berechnung& get_vk3berechnung() ;

	const VK3Berechnung& get_vk3berechnung() const
	{
		return vk3berechnung;
	}

	double get_gesamtpreisVK3() ;

	void set_gesamtpreisVK3(double value);

	//VK3-Berechnung
	void calc_gesamtpreisVK3();

	inline int get_nrVerzinkerei();

	inline void set_nrVerzinkerei(int value);

	bool get_vk3flag() const;

	void set_vk3flag(bool vk3berechnung);

private:


	/// Bis zu nächsten Formatänderung...
	void serialize_bonus(MxFilesystem::BinaryArchive& ar);

	double gesamtpreisVK3;

	/// Schaltet die Preisberechnung auf VK3 um
	bool vk3flag;

	/// Nummer der Sonderkosten
	int sokonr;

	/// Verzinkerei
	int nrVerzinkerei;

	//Pointer auf Werkauftrag
	Werkauftrag* werkauftrag;

	//Liste der Sonderkosten
	CObList sokolist;


	VK3Berechnung vk3berechnung;

	//Summe der Sonderkosten
	double sonderkosten;

	
	//Soko fuers Verzinken
	void calcSokoVerzinken();
	
	
	//Soko fuer Rohrstutzen pulvern
	void calcSokoRohrstutzen();
	
	//Soko fuer Sonderfarbe
	void calcSokoSonderfarbe();

};

inline int Preis::get_nrVerzinkerei()
{
	return nrVerzinkerei;
}

inline void Preis::set_nrVerzinkerei(int value)
{
	nrVerzinkerei = value;
}
