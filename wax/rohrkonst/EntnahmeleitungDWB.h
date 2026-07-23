#pragma once

#include "Verteiler.h"

/**
EntnahmeleitungDWB
*/
class ROHRKONSTDLL EntnahmeleitungDWB : public Verteiler  
{
public:
	/// Minimal- und Maximallängen für die Plausiprüfung
	enum LAENGEN 
	{
		MINLAENGE_DN100_L1_TYPA =204,

		MINLAENGE_DN150_L1_TYPA =284,

		MINLAENGE_DN100_L1_TYPB =304,

		MINLAENGE_DN150_L1_TYPB = 458,

		MINLAENGE_DN100_L2_TYPB = 204,

		MINLAENGE_DN150_L2_TYPB = 284,
	
	/// Maximallänge für die Rohrleitung
		MAXLAENGE = 6000
	};
public:
	enum TYPE 
	{
	
	/// Typ A (früher Skizze A) besitzt nur einen Bogen und kann an diesem Bogen 
	/// eine Entleerung haben.
		TYP_A = 0,
	
	/// Typ B (früher Skizze B) besitzt zwei Bögen und kann an einer Seite eine 
	/// Entleerung haben.
		TYP_B = 1
	};


	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Konstruktor
	EntnahmeleitungDWB();
	
	/// Destruktor
	virtual ~EntnahmeleitungDWB();

	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Rohrkonstruktion erzeugen
	virtual void createModell();

	const int get_entleerung() const;

	void set_entleerung(int value);

	const bool get_entleerungRechts() const;

	void set_entleerungRechts(bool value);

	const int get_laenge2() const;


	const int get_laenge1() const;

	void set_laenge2(int value);

	void set_laenge1(int value);

	int get_entnahmeleitung_typ() const;

	void set_entnahmeleitung_typ(int value);

	virtual bool isEntnahmeleitungDWB() const { return true; }

protected:
	/// Speichern und Laden der Rohrleitung. Achtung:
	virtual void serialize(MxFilesystem::BinaryArchive& ar);
private:


	/// 0=Skizze A, 1=Skizze B
	int entnahmeleitung_typ;

	///  Länge 2
	int	laenge2;

	///  Länge 1
	int laenge1;
	
	/// Elementid der Entleerung
	int	entleerung;
	
	/// Seite der Entleerung (false=links, true=rechts). Nur gültig, wenn 
	/// entleerung true ist.
	bool	entleerungRechts;
protected:
	/// Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
	/// Stationsverteiler oder Entnahmeleitungen
	virtual void fill_elementArray();

	/// create stueckliste
	virtual void create_stueckliste();

};

