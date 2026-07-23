#pragma once

/**
Arbeitsfolgen für die Erstellung von Arbeitsplänen
Früher AP.TBL, jetzt Tabelle Arbeitsfolgen
*/
class WAXSTAMMDLL Arbeitsfolgen : 
public StammDat  
{

public:
	//Konstruktor
	Arbeitsfolgen();

	virtual void load(const std::string& db);

	//Datenklasse für Arbeitsfolgen
//Definiert die Felder der Tabelle
	class WAXSTAMMDLL data : 
public CObject  
	{

	public:
		const CString get_arbeitsplatz() const;



		data();

		bool get_gewinderohr() const;

		bool get_gewinderohrhd() const;

		bool get_allgemein() const;

		int get_posnr() const;

		bool get_argon() const;

		bool get_co2() const;

		bool get_co2hdverteiler_dn65() const;

		bool get_co2hdverteiler_dn80() const;

		bool get_co2tikko() const;

		int get_code() const;

		bool get_halter() const;

		const CString& get_txt() const;

		bool get_wasser() const;

	private:
	//Arbeitsplatz der auszuführenden Arbeit (auch Maschinengruppe genannt)
		CString arbeitsplatz;


		friend class Arbeitsfolgen;


		bool gewinderohr;

		bool gewinderohrhd;

	//afxxx Nummer
		int code;

	//Beschreibungstext
		CString txt;

	//vordefinierte Nummer im Arbeitsplan
		int posnr;

	//zulässig für Anlagentyp Wasser
		bool wasser;

	//zulässig für Anlagentyp CO2 Tikko
		bool co2tikko;

	//zulässig für Anlagentyp CO2 Hochdruck
		bool co2;

	//zulässig für Anlagentyp Argon
		bool argon;

	//zulässig für Anlagentyp Halter
		bool halter;

	//zulässig für alle
		bool allgemein;

	//zulässig für Anlagentyp CO2 HD Verteiler (DN65)
		bool co2hdverteiler_dn65;

	//zulässig für Anlagentyp CO2 HD Verteiler (DN80)
		bool co2hdverteiler_dn80;
	};
};
