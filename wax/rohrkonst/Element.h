#pragma once


#include "Material.h"
#include "EnCObListModell.h"




class ROHRKONSTDLL CStringConvert : public CString
{
  public:
	CStringConvert(int iInCome){(*this)(iInCome);};
	CStringConvert(double fValue){(*this)(fValue);};
   // CStringConvert(CString& strInCome){(*this) = strInCome;};
	CStringConvert(CString strInCome){(*this) = strInCome;};
    CStringConvert(){};
    CString operator()(int iValue){ (*this).Format( _T("%d"), iValue); return (*this);};
    CString operator()(double fValue){(*this).Format( _T("%.5f"), fValue); return (*this);};
	operator int(){return atoi((char*)this->GetBuffer());};
	operator double(){return atof((char*)this->GetBuffer());};
 
	operator CTime()
	{
		CString strTemp;
        int d = (CStringConvert)Left(2);    
		int y = (CStringConvert)Right(4);
        strTemp = Right(7);
        int m = (CStringConvert) strTemp.Left(2);
        return CTime (y, m, d, 23, 59, 59); 
	};

    CStringConvert& operator=(CString& strToCopy){ CString::operator = (strToCopy); return (*this);};
};

class Rohrleitung;
/// Definition zur Benutzung von ElementeIDs in anderen Klassen
#define ROHR Element::ROHR
#define MUFFE Element::MUFFE
#define NUTST Element::NUTST
#define KRED Element::KRED
#define ERED Element::ERED
#define BOG2S Element::BOG2S
#define ELEMENT_BOGEN Element::BOGEN
#define EINZ Element::EINZ
#define GWRST Element::GWRST
#define FFL Element::FFL
#define SCH Element::SCH
#define NUT Element::NUT
#define GLATT Element::GLATT
#define SNAHT Element::SNAHT
#define GEW Element::GEW
#define VFL10 Element::VFL10
#define AFL Element::AFL
#define SFL Element::SFL
#define BOD Element::BOD
#define VFL16 Element::VFL16
#define KB40 Element::KB40
#define KB50 Element::KB50
#define BOHR50 Element::BOHR50
#define KNICK Element::KNICK
#define STDMUFFE Element::STDMUFFE
#define STDSTUTZEN Element::STDSTUTZEN
#define ROHR_Q0 Element::ROHR_Q0
#define ROHR_Q1 Element::ROHR_Q1
#define ROHR_Q3 Element::ROHR_Q3
#define ROHR_Q5 Element::ROHR_Q5
#define ROHR_Q6 Element::ROHR_Q6
#define ROHR_Q7 Element::ROHR_Q7
#define ROHR_Q8 Element::ROHR_Q8
#define ROHR_Q9 Element::ROHR_Q9
#define ROHR_Q10 Element::ROHR_Q10
#define ROHR_Q11 Element::ROHR_Q11
#define SCHWEISS Element::SCHWEISS
#define SPANS Element::SPANS
#define BOHR24 Element::BOHR24
#define BOGVERZ Element::BOGVERZ
#define GRFITTING Element::GRFITTING
#define KRMUFFE Element::KRMUFFE
#define FRMUFFE Element::FRMUFFE
#define ROHRNIPPEL Element::ROHRNIPPEL
#define FORMSTUECKE Element::FORMSTUECKE
#define FITTINGE Element::FITTINGE
#define PROFIL Element::PROFIL
#define VERBSTUECK Element::VERBSTUECK
#define SONDERMUFFE Element::SONDERMUFFE
#define ROHR_Q12 Element::ROHR_Q12
#define ROHR_Q13 Element::ROHR_Q13
#define ROHR_Q14 Element::ROHR_Q14
#define ROHR_Q15 Element::ROHR_Q15
#define ROHR_Q11 Element::ROHR_Q11
#define ROHR_Q16 Element::ROHR_Q16
#define ROHR_Q18 Element::ROHR_Q18  // mapped RQ2448NLOS_MX1230200_DN65_VERZINKT 
#define ROHR_Q19 Element::ROHR_Q19
#define ROHR_Q20 Element::ROHR_Q20

#define NEW_ROBBI_MUFFE Element::NEW_ROBBI_MUFFE

#define NOARTNR Element::NOARTNR
#define FFL16  Element::FFL16
#define FFL10  Element::FFL10
#define VFL10NEU  Element::VFL10NEU
#define VFL16NEU  Element::VFL16NEU
#define SPRINKLERSCHELLE_VIROTEC Element::SPRINKLERSCHELLE_VIROTEC 
#define POTENIALAUSGLEICH  Element::POTENIALAUSGLEICH


#define WINKEL_LINKS_COII						Element::WINKEL_LINKS_COII
#define BLINDFLANSCH_COII						Element::BLINDFLANSCH_COII
#define GEGENFLANSCH_COII_SCHARZ_HAUPTROHR		Element::GEGENFLANSCH_COII_SCHARZ_HAUPTROHR

#define GEGENFLANSCH_COII_ABGANG				Element::GEGENFLANSCH_COII_ABGANG
#define GEGENFLANSCH_COII_SCHWARZ_ABGANG		Element::GEGENFLANSCH_COII_SCHWARZ_ABGANG
#define FLANSCH_COII					Element::FLANSCH_COII
#define BODEN_COII						Element::BODEN_COII
#define GEGENFLANSCH_GEWINDE_COII		Element::GEGENFLANSCH_GEWINDE_COII
#define GEGENFLANSCH_GEWINDE_MUFFE_COII	Element::GEGENFLANSCH_GEWINDE_MUFFE_COII
#define ROHRNIPPEL_COII					Element::ROHRNIPPEL_COII
#define MUFFE_COII						Element::MUFFE_COII
#define KABELKANAL_COII					Element::KABELKANAL_COII
#define ENDSCHALTER_ZWEI_KON			Element::ENDSCHALTER_ZWEI_KON	
#define ENDSCHALTER_VIER_KON			Element::ENDSCHALTER_VIER_KON	
#define WINKEL_RECHTS_COII				Element::WINKEL_RECHTS_COII
#define ROHR_Q17						Element::ROHR_Q17
#define GEWINDE_KAPPE_COII				Element::GEWINDE_KAPPE_COII
#define GEWINDE_KAPPE_TYP_D				Element::GEWINDE_KAPPE_TYP_D
#define FASE_COII						Element::FASE_COII

/// Klasse Element - Definition der Elemente
class ROHRKONSTDLL Element : public Material  
{
public:

/// private
private:

	int wanr;
	///  Setzt alle Memberattribute auf default
	void init_ele();

	/// Liste fuer AddOnElemente
	EnCObListModell uelementList;

	bool m_IgnoreRohrOrVk3Rohr;

	/// Position
	int pos;

	/// Nennweite
	int dn;

	/// Nennweite der Rohrleitung
	int anschlussDN;

	/// Laenge des Rohmaterials (bei Rohrstutzen/Standardstutzen/Standardmuffe inkl. 1/2 Rohrdurchmesser)  
	int laenge;

	int laengeZuschnitt;

	/// Winkel
	int winkel;

	/// Abstand (relativ)
	int abstRel;

	/// Abstand (absolut)
	int abstAbs;

	/// Verkettungselement
	int verkettung;

	/// Abstand Verkettungselement
	int verkettungAbst;

	/// Posnr. des Rohrs
	CString rohrPosNr;

	/// Nr. des Rohrstuecks
	int rohrNr;

	/// Laenge des Rohrstuecks
	int rohrLaenge;

	int ElementAnzahlid;

	double ElementAnzahldnMuffeOrDBohrung;

	/// Rohrstueck mit Stutzen
	bool rohrMitStut;

	/// Beschriftung des Rohrstuecks links
	CString beschriftungLinks;

	/// Beschriftung des Rohrstuecks rechts
	CString beschriftungRechts;

	/// Element wird ueber Anlage verarbeitet
	bool makeAnlage;

	/// letztes Element
	bool lastele;

	/// erstes Element
	bool firstele;

	/// Fitting: Nennweite 1
	int dn1;

	/// Fitting: Nennweite 2
	int dn2;

	/// Fitting: Nennweite 3
	int dn3;

	/// Fitting: Nennweite 2
	int dn4;

	/// Fitting: gegenueberliegende Nennweiten muesen getauscht werden 
	bool changeDN;

	/// Fitting: GruppenID
	int gruppenid;

	/// Fitting: ZMass
	int zmass;

	/// Fitting: ZMass2 
	int zmass2;

	/// Fitting: ZMass3 
	int zmass3;

	/// Fitting: nur fuer Element GRFitting und Gruppe T-Stueck erforderlich, Anschluss an DN2
	int kopfstueck;

	/// Fitting: Fitting ohne Rohrstueck
	bool nurFitting;

	/// Fitting: GF-Nummer
	CString gfzeichen;

	///  Korrekturwinkel für Abgänge
	int korrektur_winkel;

	/// Initialisierung des Elements mit den Stammdaten aus der Tabelle Bauelemente
	virtual void init(const Bauelemente::data &g);

	/// Initialisierung des Elements mit den Stammdaten aus der Tabelle Sondermaterial
	virtual void init(const Sondermaterial::data &s);

	virtual void init(const FormstueckeUndFittinge::data& f);

	/// Wirft eine Exception mit Debug Info
	void throwError(int line);

	/// Wenn in check_elementArray() ermittelt wurde, dass Nennweite getauscht werden muss,
	/// erfolgt hier die Umsetzung der Nennweite und vom ZMass
	void changeDNundZMass();

/// public
public:

	int get_korrektur_winkel() const
	{
		//TRACE("get Korrekturwinkel = %i\n", korrektur_winkel);
		return korrektur_winkel;
	}

	void set_korrektur_winkel(int winkel)
	{
		//TRACE("set Korrekturwinkel = %i\n", winkel);
		korrektur_winkel = winkel;
	}
	
	// Gibt an, fuer welche Rohrseite das Z-Mass geliefert werden soll
	enum RICHTUNG 
	{
		LINKS = 0, 
			
		RECHTS = 1
	};

	/// WAX - Elementbezeichnungen, muessen mit den Elementids in der Tabelle 
	/// Bauelemente uebereinstimmen
	enum IDS 
	{  
		LEER = 0, /// Nicht festgelegt
		ROHR = 1, /// Rohrstutzen
		NUTST = 2, /// Nutstutzen
		GWRST = 4, /// Gewinderohrstutzen/DN65
		MUFFE = 5, /// Muffe
		FRMUFFE = 6, /// Robbi-Muffe flach
		KRMUFFE = 7, /// Robbi-Muffe gekehlt
		FFL = 8, /// Flach-Flansch
		/// Vorschw.flansch PN16 bis DN150, PN10 ab DN 200
		//VFLAN = 10,  //alt jetzt 58
		/// Vorschw.flansch PN16 ab DN 200
		//VFL16 = 11, //alt jetzt 57
		AFL = 12, /// ANSI-Flansch
		SCHWEISS = 13, /// Schweissnaht im Werk
		SFL = 14, /// Slip-On-Flansch
		SCH = 15, /// Scheibe
		BOD = 16, /// Boden
		SPANS = 17, /// Spuelanschluss
		KB40 = 18, /// Spuelanschluss40
		KB50 = 19, /// Spuelanschluss50
		NUT = 20, /// Nut
		GEW = 21, /// Gewinde
		SNAHT = 22, /// Schweissnaht vor Ort
		GLATT = 23, /// Glatt
		BOHR24 = 24, /// Bohrung
		EINZ = 25, /// Einziehung
		KRED = 26, /// K-Reduzierung
		ERED = 27, /// E-Reduzierung
		BOGEN = 28, /// Bogen
		BOG2S = 29, /// Bogen2S
		ROHR_Q0 = 30, /// Rohr-Q0
		ROHR_Q1 = 31, /// Rohr-Q1
		ROHR_Q3 = 32, /// Rohr-Q3
		ROHR_Q5 = 34, /// Rohr-Q5
		BOHR50 = 35, /// Bohrung 50 mm
		BOGVERZ = 36, /// Bogen verzinkt
		ROHR_Q6 = 37, /// Rohr-Q6
		ROHR_Q7 = 38, /// Rohr-Q7
		STDMUFFE = 39, /// Standardverteiler-Muffe flach
		STDSTUTZEN = 40, /// Standardverteiler-Stutzen flach
		ROHRNIPPEL = 41, /// Rohrnippel
		FORMSTUECKE = 42, /// Formstuecke bei Materialbestellung
		FITTINGE = 43, /// Fittinge bei Materialbestellung
		GRFITTING = 44, /// Gewinderohr-Fitting
		PROFIL = 45, /// Profile
		SPRINKLERSCHELLE_TXG = 46, /// Sprinklerschellen TXG
		SPRINKLER = 47, /// Sprinkler
		HALTER = 48, /// Halter
		ROHR_Q8 = 49, /// Rohr-Q8
		SPRINKLERSCHELLE_VICTAULIC = 50, /// Sprinklerschellen Victaulic
		ROHR_Q9 = 51, /// Rohr-Q9
		ROHR_Q10 = 52, /// Rohr-Q10
		VERBSTUECK = 53, /// Verbindungsstuecke
		VFLANSCH_FEDER = 54, /// V-Flansch mit Feder
		VFLANSCH_NUT = 55, /// V-Flansch mit Nut
		ROHR_Q11 = 56, /// APSAD - Rohr
		VFL10 = 57, /// Vorschw.flansch PN10
		VFL16 = 58, /// Vorschw.flansch PN16
		SONDERMUFFE = 59, /// Sondermuffen mit variabler Länge von 10-80mm
		ROHR_Q12 = 60, /// Rohr-Q12
		ROHR_Q13 = 61, //entspricht DINEN 10217-1 Standardrohr verzinkt
		ROHR_Q14 = 62, //DIN EN 10255 Gewinderohr geschweißt (vormals DIN 2440) Kaltbandgeschweißt
		ROHR_Q15 = 63, //DIN EN 10255 Gewinderohr geschweißt verzinkt (vormals DIN 2440) Kaltbandgeschweißt
		KNICK = 64, //BIEGUNG
		ROHR_Q16 = 66, // CO II
		ROHR_Q18 = 67, // Chemie
		ROHR_Q19 = 68, // Chemie Rohr DN 65 rotgepulvert Sonderfall extraDick
		ROHR_Q20 = 69,  // Rohr-Q9
		NEW_ROBBI_MUFFE = 79,	
		NOARTNR = 100, //Artikel ohne Artikelnummer
		KNICK15 = 110, //Vk3 Element Biegung bis 15°
		KNICK45 = 112, //Vk3 Element Biegung bis 45°
		KNICK90 = 114, //Vk3 Element Biegung bis 90°
		MAX_MIN_ID = 80, /// Maximale Elementid, zur Prüfung
		BFL16 = 82,
		FFL16  = 83,
		FFL10  = 1393,
		VFL10NEU  = 1394,
		VFL16NEU  = 1395,
		SPRINKLERSCHELLE_VIROTEC = 138,
		POTENIALAUSGLEICH		 = 1407, 
	

		/*GAS*/
		
		
		
		FLANSCH_COII						= 5000,
		WINKEL_LINKS_COII					= 5001,
		BODEN_COII							= 5002,
		BLINDFLANSCH_COII					= 5003,
		GEGENFLANSCH_GEWINDE_COII			= 5004,
		GEGENFLANSCH_GEWINDE_MUFFE_COII		= 5005,
		ROHRNIPPEL_COII						= 5006,
		MUFFE_COII							= 5007,
		KABELKANAL_COII						= 5019,
		GEGENFLANSCH_COII_ABGANG			= 5022,
		GEGENFLANSCH_COII_SCHWARZ_ABGANG	= 5023,
		GEWINDE_KAPPE_COII					= 5026,
		ENDSCHALTER_ZWEI_KON				= 5027,
		ENDSCHALTER_VIER_KON				= 5028,
		WINKEL_RECHTS_COII					= 5029,
		ROHR_Q17							= 5030,
		GEWINDE_KAPPE_TYP_D					= 5032,
		GEGENFLANSCH_COII_SCHARZ_HAUPTROHR  = 5033,
		FASE_COII							= 5034,

		MAX_MAX_ID						= 5500, /// Maximale Elementid, zur Prüfung
		
		

	};

	enum GRUPPEN_ID
	{
		TSTUECK = 14,
		KREUZSTUECK = 6
	};

	/// Text fuer Ausnahme
	class Error 
	{
	private:
		std::string errorText;
	public:
		std::string get_errorText() const { return errorText; }
		Error(const char* eText) { errorText = eText; }
	};
	
	/// Standardkonstruktor
	Element();


	/// Kopykonstruktor
	Element(const Element& element);

	void operator = (const Element& other)
	{
		copy(other);
	}

	/// Konstruktor
	Element(int id, int laenge = 0, int dn = 0, int anschlussdn = 0, int abstRel = 0, int abstAbs = 0, int winkel = 0);

	Element(int eleid, const CString& elebez, int eleanz, int eledn, int eleanschlussdn, double festpreis, double sappreis);

	/// Destruktor
	virtual ~Element();

	/// Speichern und Laden der Elementdaten
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Liefer den Kurztext
	virtual const CString& get_kurztext() const;

	/// Kopieren der Elementattribute
	void copy(const Element& other);

	/// Kopieren der Unterelemente, wird in CadElement überschrieben
	virtual void copy_uelementlist(const EnCObListModell& ue);


	virtual WaxBase *deepCopy()const;		/// Gibt eine neues mit new erzeugtes Objekt zurück

	int getLaengeZuschnitt()const;

	/// Pruefung der DN
	CString check_dn();

	/// Prueft Elemente
	bool verify();

	void SetDoIgnoreRohrOrVk3Rohr(bool IgnoreRohrOrVk3Rohr )
	{ // Für Magdeburg Damit nicht die Länge der Rohre angezeigt wird
	  // sondern die Rohranzahl
		m_IgnoreRohrOrVk3Rohr = IgnoreRohrOrVk3Rohr;
	};

	bool Get_DoIgnoreRohrOrVk3Rohr()const
	{
		return m_IgnoreRohrOrVk3Rohr;
	};
	
	/// Laenge/Durchmesser des Elements aus den Stammdaten holen
	int get_Stammdaten(int &laenge, double &durchmesser, int id, int dn, int anschlussDN);

/// Initialisierung der fehlenden Daten
	void initDaten(int abstAbs, int abstRel, int laenge, int id, int winkel, double durchmesser = 0.0);

	/// Initialisierung der Stammdaten für alle Elemente, benutzt je nach Typ die 
	/// richtige Funktion. Die relevanten Daten (Typ, dn usw.) müssen vorher 
	/// explizit gesetzt werden. Benutzt wird die Methode in Serialize, damit 
	/// aktuelle Daten auf die Festplatte geschrieben werden.
	void initStammdaten(bool verzinkt);

	/// Initialisierung der Stammdaten fuer normale Elemente aus der 
	//Bauelementetabelle
	void initStammdaten(int dn, int anschlussDN, int id, int laenge = 0);

	/// Initialisierung der Stammdaten für GF-Fittinge aus der Formstuecke-Fittinge Tabelle
	void initStammdaten(int artnr, int id = 0);

	//Initialisierung der Stammdaten für GF-Fittinge aus der Formstücke-Fittinge Tabelle
	void initStammdaten(int id, int dn, int gruppenid, CString kurztext, CString gfzeichen = "");

	/// Initialisierung der Stammdaten für VK3Elemente
	int initStammdatenVK3Ele(int dn, int anschlussDN, int id, bool verzinkt, int Rohlaenge, int AnzahlElemente, int ElementAnzahld, double ElementAnzahldn, int ElementLaenge, int Anlage);

	/// Initialisierung der Stammdaten fuer Sondermaterial aus der Sondermaterialtabelle
	void Element::initStammdatenSoma(int dn, int anschlussDN, int id);

	/// Setzt die (Anschluss-)Nennweite
	void setAnschlussNennweite();

	/// Gewicht berechnen
	double getGewicht() const;
	
	/// Hier wird ermittelt welche Afos ein Element hat. Ist firstafo gleich true, 
	/// wird nur die erste ermittelte Afo zurueckgeliefert.
	void getAfos(int iProduktTyp, CArray<int,int> &afoNummern);

	// Prueft, ob Rohr fuer Anlage gedreht werden muss 
	int checkDrehen(int laenge, int anfang, int ende);

	bool isEleRohrQ1() const;
	bool isEleRohrQ11() const;

	const bool isEleVK3Rohr() const; 

	bool isElePulvern();

	bool isEleRohr() const;

	bool isEleRedEinz() const;

	bool isEleBogen() const;

	bool isEleRedBogEinz() const;

	bool isEleRed() const;

	bool isEleRedBog() const;

	bool isEleAbgang() const;

	bool isEleAbgEnde() const;

	bool isEleFlansch() const;

	bool isEleEnde() const;

	bool isTStueck()const;

	bool isKreuzstueck()const;

	bool isFitting() const;

	bool isKnick() const;

	/// get uelementList
	EnCObListModell& get_uelementList();

	const EnCObListModell& get_uelementList() const;

	/// get abstAbs
	const int get_abstAbs() const;

	/// get abstRel
	const int get_abstRel() const;

	/// get dn
	const int get_dn() const;

	/// get dnvl
	const int get_anschlussDN() const;

	/// get laenge
	virtual int get_laenge() const;

	/// get winkel
	virtual int get_winkel(bool ohne_korrektur = false) const;

	/// set abstAbs
	void set_abstAbs(int value);

	/// set abstRel
	void set_abstRel(int value);

	/// set dn
	void set_dn(int value);

	/// set dnvl
	void set_anschlussDN(int value);

	/// set laenge
	void set_laenge(int value);

	/// set winkel
	void set_winkel(int value);

	/// get pos
	const int get_pos() const;

	/// set pos
	void set_pos(int value);

	/// get verkettung
	const int get_verkettung() const;

	/// set verkettung
	void set_verkettung(int value);

	/// get verkettungAbst
	const int get_verkettungAbst() const;

	/// set verkettungAbst
	void set_verkettungAbst(int value);

	/// get rohrLaenge
	const int get_rohrLaenge() const;

	/// set rohrLaenge
	void set_rohrLaenge(int value);

	void set_ElementAnzahlid(int elementAnzahlid)
	{
		ElementAnzahlid = elementAnzahlid;
	}

	const int get_ElementAnzahlid()const
	{
		return ElementAnzahlid;
	}

	void set_ElementAnzahldn(double elementAnzahldn)
	{
		ElementAnzahldnMuffeOrDBohrung = elementAnzahldn;
	}

	const double get_ElementAnzahldnMuffeOrDBohrung()const
	{
		return ElementAnzahldnMuffeOrDBohrung;
	}
	
	/// get rohrNr
	const int get_rohrNr() const;

	/// set rohrNr
	void set_rohrNr(int value);

	/// get rohrMitStut
	const bool get_rohrMitStut() const;

	/// set rohrMitStut
	void set_rohrMitStut(bool value);

	/// get beschriftungLinks
	const CString& get_beschriftungLinks() const;

	/// set beschriftungLinks
	void set_beschriftungLinks(const CString& value);

	/// get beschriftungRechts
	const CString& get_beschriftungRechts() const;

	/// set beschriftungRechts
	void set_beschriftungRechts(const CString& value);

	/// get makeAnlage
	const bool get_makeAnlage() const;

	/// set makeAnlage
	void set_makeAnlage(bool value);

	/// Prueft Rohr auf Elemente,die ueber Anlage laufen
	bool hasRohrStdelemente();

	/// get rohrPosNr
	const CString& get_rohrPosNr() const;

	/// set rohrPosNr
	void set_rohrPosNr(const CString& value);

	/// set rohrPosNr
	void set_WaNr(int wanrLocal)
	{
		wanr = wanrLocal;
	};

	
	int get_WaNr()
	{
		return wanr;
	};

	/// get lastele
	const bool get_lastele() const;

	/// set_lastele
	void set_lastele(bool value);

	/// get firstele
	const bool get_firstele() const;

	/// set firstele
	void set_firstele(bool value);

	/// *****Fuer Fittinge*****

	/// ZMass-Typ des ZMasses am Rohranfang
	enum ZMASSTYP 
	{
		/// direkte Eingabe des ZMasses Links
		EINGABE = 0,

		/// ZMass wird durch Fitting vorgegeben
		FITTING = 1,

		/// ZMass wird durch Muffe auf Verteiler festgelegt
		MUFFE_AUF_VERTEILER = 2
	};
	
	/// Liefert True, wenn Fitting Aussengewinde hat
	bool hasEleAussengewinde() const;

	/// Liefert True, wenn Fitting Innengewinde hat
	bool hasEleInnengewinde() const;

	/// Liefert True, wenn Fitting Innen- und Aussengewinde hat
	bool hasEleInnenAussengewinde() const;

	/// Liefert die Nennweite fuer den naechsten Anschluss
	int getReferenceDN(bool oben = false);

	/// Liefert das ZMass fuer den Abzug je nach Rohrseite
	int getZmass(enum RICHTUNG seite) const;

	/// Liefert den Abstand zum vorherigen Fitting, wenn Fitting rechts ohne Rohrstueck
	double getAbstand();

	const int get_dn1() const;

	void set_dn1(int value);

	const int get_dn2() const;

	void set_dn2(int value);

	const int get_dn3() const;

	void set_dn3(int value);

	const int get_dn4() const;

	void set_dn4(int value);

	const bool get_changeDN() const;

	void set_changeDN(bool value);

	const int get_gruppenid() const;

	void set_gruppenid(int value);

	const int get_zmass() const;

	void set_zmass(int value);

	const int get_zmass2() const;

	void set_zmass2(int value);

	const int get_zmass3() const;

	void set_zmass3(int value);

	const int get_kopfstueck() const;

	//nur fuer Element GRFitting und Gruppe T-Stueck erforderlich, Anschluss an DN2
	void set_kopfstueck(int value);

	bool get_nurFitting() const;

	void set_nurFitting(bool value);

	const CString &  get_gfzeichen() const;

	void set_gfzeichen(const CString & value);

	void set_laengeZuschnitt(int val);

#ifdef _DEBUG
	//Ausgabe der Attribute
	virtual void Dump(CDumpContext& dc) const;
#endif

};


