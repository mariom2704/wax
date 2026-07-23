#pragma once

#include <mxcontrols/binaryarchive.h>

/**
Arbeitsfolge - Definition der Arbeitsfolge
*/
class ROHRKONSTDLL Arbeitsfolge : public CObject  
{

public:


		///für welchen Rohrtyp der Ap erzeugt werden soll (siehe Hinweis in Klassenbeschreibung)
	enum ÂRBEITSFOLGENTYP
	{
		
		eSTRANGROHR_STRASSE	= 51
			
	};
	// Arbeitsfolge nach ID erstellen bzw. Standardkonstruktor
	Arbeitsfolge(int id = 0); 

	//Copyconstruktor
	Arbeitsfolge(const Arbeitsfolge& orig);

	//Destruktor
	virtual ~Arbeitsfolge();

	
	//Copyfunktion
	void copy(const Arbeitsfolge& other);

	//Addiert zwei Arbeitsfolgen
	bool merge(const Arbeitsfolge& other);


	//Daten zur Zeitberechnung einer Arbeitsfolge
	class apaxdat 
	{
	public:
	
		apaxdat();
		void serialize(MxFilesystem::BinaryArchive& ar);
		//Liefert true zurück, wenn wenigstens ein Wert gesetzt wurde
		bool isEmpty() const;
		int anzahl;
		int dn;
		int laenge;
		double zeit;
		int eleid;
		int dnvl;
	};

	void appendText(const CString& id);

	//die Bezeichnung des Arbeitsplatzes über die Stammdaten holen
	CString getArbplBez(CString arbpl);
	
	//die Kostenstelle über die Stammdaten holen
	long getKostst(CString arbpl);

	//den Tarif über die Stammdaten holen
	double getTarif(long kostst);

	//Speichern und Laden der Daten
	//Achtung: Wird von Rose überschrieben
	void serialize(MxFilesystem::BinaryArchive& ar);

	//get preis
	const double get_preis() const;

	//set preis
	void set_preis(double value);

	//get anz
	const double get_anz() const;

	//set anz
	void set_anz(double value);
	
	//get id
	const int get_id() const;

	//set id
	void set_id(int value);
		
	//get kostst
	const int get_kostst() const;

	//set kostst
	void set_kostst(int value);
	
	//get arbpl
	const CString get_arbpl() const;

	//set arbpl
	void set_arbpl(CString value);

	//get nr
	const int get_nr() const;

	//set nr
	void set_nr(int value);

	//get ppstd
	const double get_ppstd() const;

	//set ppstd
	void set_ppstd(double value);

	//get txt
	const CString& get_txt() const;

	const CString& get_originalTxt() const;

	//set txt
	void set_txt(const CString& value);

	void set_originalTxt(const CString& value);
	
	//get zeit
	const double get_zeit() const;
	
	//set zeit
	void set_zeit(double value);

	const double get_zuschlag() const;

	void set_zuschlag(double val);

	//get av
	const int get_av() const;

	//set av
	void set_av(int value);

	//get gemeinkosten
	const int get_gemeinkosten() const;

	//set gemeinkosten
	void set_gemeinkosten(int value);

	void init(int id);

	//DN Wert auf dem die Afo basiert
	void set_dn(int value);

	//DN Wert auf dem die Afo Basiert
	int  get_dn();

	const bool get_sap() const;

	void set_sap(bool value);

	const bool get_isTextGeandert() const;

	void set_isTextGeandert(bool value);

	const double get_ruestzeit() const;

	void set_ruestzeit(double value);

	
	Arbeitsfolge::apaxdat& get_xdat() ;

	
	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;	///< Ausgabe der Attribute
	#endif


	const int get_typ() const;
	
	void set_typ(int value);

private:
	bool sap;

	//Rüstzeit wird nur einmal pro Arbeitsfolge addiert
	double ruestzeit;

	//Extra Eingaben für die Arbeitsfolge bei Apax
	apaxdat xdat;

	//Anzahl der Arbeitsfolgen, falls eine Berechnungsmatrix vorliegt, 
	//wird der Wert daraus errechnet
	double anz;

	//ID afxxx, Primary Key in Datenbank
	int id;
		
	//Kostenstelle
	int kostst;

	// Arbeitsplatz, ist u.U. abhängig von DN/Länge
	CString arbpl;

	//lfd. Nr. im Arbeitsplan, u.U. fest eingestellt 
	//durch Eintrag in Tabelle Arbeitsfolgen
	int nr;

	//Preis pro Arbeitsstunde
	double ppstd;

	//Beschreibung der Arbeit
	CString txt;

	//Original Beschreibungstext
	CString originalTxt;

	//Zeit pro Arbeitsfolge
	double zeit;

	//DN Wert auf die die Afo basiert
	int dn;

	//Preis der Afo
	double preis;

	//Zuschalg zum Preis
	double zuschlag;

	
	//Preisberechnung
	

	bool isTextgeaendert;

	int typ;
public:
	void calc_preis();

};

