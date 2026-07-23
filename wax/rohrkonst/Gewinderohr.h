#pragma once

#include "cadwerkauftrag.h"

/**
Gewinderohr - Definition des Gewinderohrs
*/
class ROHRKONSTDLL Gewinderohr : public Rohrleitung  
{
// public
public:
	
	

	enum DICHTMITTEL 
	{
		OHNE = 0,

		LOCTITE = 1,

		HANF = 2,

		TEFLON = 3,
	
		SYNTHESOL = 4,

		NEOFERMIT = 5,

		PERMABOND = 6,

		BESTMK2241 = 8

	};

	enum uTyp
	{
		SCHAUM = 12
	};

	//Standardkonstruktor
	Gewinderohr();
	
	//Standardkonstruktor
	Gewinderohr(const EnContainerRohrElemente& eArray, int anfid,int endid);

	//Destruktor
	virtual ~Gewinderohr();

	//Speichern und Laden der Rohrleitung. Achtung:
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	//Rohrkonstruktion erzeugen
	virtual void createModell();

	bool setLaengeDnvl();

	//get typ
	virtual int get_typ() const;

	//set typ
	void set_typ(int value);

	//get dichtmittel
	const int get_dichtmittel() const;

	//set dichtmittel
	void set_dichtmittel(int value);

	const int get_zmassLinks() const;

	void set_zmassLinks(int value);
	

	void set_kaltbandgeschweisst(int val);

	virtual CString copy(const Rohrleitung& other);

	virtual bool hasRohr2Quali() const;

	virtual bool isBogen() const;

	const int get_zmassRefDN() const;

	void set_zmassRefDN(int value);

	bool isAnlagOK(int dn, int lange) const;

	const int get_zmassTyp() const;

	void set_zmassTyp(int value);

	//Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	//Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);
	
	///vk3liste erstellen
	virtual bool createVk3Liste();

	virtual void createPulverlaengenListe();

	virtual bool isGewinderohr() const { return true; }

	bool makeGewinderohr(EnCObListModell& elementList, MxErrorHandling::MxErrorHandler& error) const;

	// Setzt die Rohrqualität des Rohres für die Druckliste
	void setRohrquali();

	/// Bezeichnung für Grafikausgabe
	virtual std::string get_beschriftung(int dn, bool zoll, int rohrcount) const;

	/// Automatisches Setzen der Flags kaltbandgeschweisst
	void auto_set_kaltbandgeschweisst();

protected:
	///Erzeugen der Afos fuer die Beschichtung
	virtual void createBeschAfos(Arbeitsplan& plan);

	//Rohrqualitaet checken
	virtual void check_rohrQuali(int dnvl) const;

	//Berechnungs des Aufschlags fuer Rohrverschnitt
	virtual void addRohrausschuss(CObList& list);

	//create stueckliste
	virtual void create_stueckliste();

	//Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
	//Stationsverteiler oder Entnahmeleitungen
	virtual void fill_elementArray();

	virtual void check_elementArray();


private:

	void init();

	//Referenz-DN aus der zusammen mit dem zmassTyp das ZMass Links errechnet wird.
	int zmassRefDN;

	int zmassTyp;

	//Anlagetyp
	int typ;

	//Dichtmittel
	int dichtmittel;

	int zmassLinks;



	CString checkRohr();

	CString checkTypDichtmittel();

	int getRohrquali(int dn) const;

};

