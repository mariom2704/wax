#pragma once

#include "WaxBase.h"
#include "..\waxstamm\bauelemente.h"
#include "..\waxstamm\profile.h"
#include "..\waxstamm\FormstueckeUndFittinge.h"
#include "..\waxstamm\Sondermaterial.h"

#include <mxcontrols/binaryarchive.h>

/**
Material ist die Basisklasse für Elemente und enthält die Basisattribute für 
beliebiges Material, z.B. Formstücke und Fittinge
*/
class ROHRKONSTDLL Material : public WaxBase  
{
protected:
	mutable CString kurztext;

	///  Bezeichnung, wird über die Typid aus den Stammdaten ermittelt oder kann 
	///  direkt gesetzt werden
	CString bezeichnung;

public:
	/// Um alle Winkel abspeichern zu können, braucht man leider mehr als 8 Bit, 
	/// deshalb hier 9 Bit
	/// Winkel rechts ist in in den unteren 9 Bit gespeichert
	/// Winkel links ist in den oberen 9 Bit gespeichert
	/// Dazwischen ist die Ansicht (Bit 10 bis 14) und die Seitenansicht (Bit 15 
	/// und 16) abgespeichert
	static int setMass2(int winkelLinks, int winkelRechts, int ansicht, int seitenansicht); 

	/// Entschlüsselt mass2 und liefert die Winkel und Ansichten
	static void getMass2(int mass2, int& winkelLinks, int& winkelRechts, int& ansicht, int &seitenansicht); 

	/// Liefert das Profilende
	int getProfilende();

	/// Liefert den Profiltyp
	CString getProfiltyp();

	/// Hier wird ermittelt welche Afo ein Profil hat. 
	void getAfos(CUIntArray &afoNr);

	/// Initialisierung des Materials mit den Stammdaten aus der Tabelle Sondermaterial
	virtual void init(const Sondermaterial::data &s);

	/// Initialisierung des Materials mit den Stammdaten aus der Tabelle FormstueckeUndFittinge
	virtual void init(const FormstueckeUndFittinge::data &f);

	/// Initialisierung des Materials mit den Stammdaten aus der Tabelle Profile
	virtual void init(const Profile::data &p);
		
	/// Initialisierung des Materials mit den Stammdaten aus der Tabelle Bauelemente
	virtual void init(const Bauelemente::data &g);

	inline const bool get_constpreis() const;

	inline void set_constpreis(bool value);

	virtual double getGewicht(bool anz = false) const;

	/// Konstruktor
	Material();

	/// Destruktor
	virtual ~Material();
	
	/// Speichern und Laden von Material
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Kopiert die Attribute eines anderen Materialobjektes. 
	/// Wird in Element gebraucht.
	void copy(const Material& other);

	virtual WaxBase *deepCopy()const;		///Gibt eine neues mit new erzeugtes Objekt zurück

	void initStammdaten();

	void setKurztext();

	const int get_anzahl() const;

	inline void set_anzahl(int value);

	const long get_artikelnr_ext() const;

	const long get_artikelnr() const;

	void set_artikelnr(long value);

	const virtual CString& get_kurztext();

	void set_kurztext(const CString& value);

	/// Liest bezeichnung aus, wenn bezeichnung leer ist, wird es anhand der id aus 
	/// den Stammdaten geladen.
	const virtual CString& get_bezeichnung();
	/// Konst Methode, die Bezeichnung nicht initialisiert
	const CString& getBezeichnung() const;
	
	inline void set_bezeichnung(const CString& value);

	/// Liefert das Gewicht in Kg pro Einheit
	inline const double get_gewicht() const;

	/// Setzen des Gewichtes in Kg pro Einheit
	inline void set_gewicht(double value);


	inline void set_idCounterVk3Preis(int value);

	inline const int get_idCounterVk3Preis()const;

	inline const int get_id() const;

	inline void set_id(int value);

	inline const int get_mass1() const;

	inline const int get_mass2() const;

	/// Preis pro Einheit (Basispreis) auslesen.
	inline const double get_festPreis() const;

	inline void set_festPreis(double value);


	inline const double get_sapPreis() const;

	inline void set_sapPreis(double value);



	inline const double get_pulverpreis() const;

	inline void set_pulverpreis(double value);

	inline void set_mass1(int value);

	inline void set_mass2(int value);

	
	inline const double get_oberfl() const;

	inline void set_oberfl(double value);

	inline const bool get_einkauf() const;

	inline void set_einkauf(bool value);

	inline const bool get_halbzeug() const;

	inline void set_halbzeug(bool value);

	///  Erweiterte Attribute für das Material (Spezialflags)
	enum EX_ATTRIB
	{
		USTAHL = 0x01000000,
		FSTAHL = 0x02000000,
		LSTAHL = 0x04000000,
	};

	///  Setzen der erweiterten Attribute für Profile, Flags werden in Artikelnummer gespeichert
	void set_profilkennung(enum EX_ATTRIB exattrib);

	///  In der Datenbank sind aus Lesbarkeitsgründen die Zeichen F, U oder L gespeichert
	void set_profilkennung(const CString& db_kennung);

	///  Auslesen der Profilkennung in den erweiterten Attributen. Null, wenn es kein Profil ist
	int get_profilkennung() const;

	/// Setzen des Durchmessers
	inline void set_durchmesser(double value);

	/// Liefert den Durchmesser
	inline const double get_durchmesser() const;

	/// Setzen der Posnr. 
	inline void set_posnr(const CString& value);

	/// Liefert die Posnr.
	inline const CString& get_posnr() const;

	inline void set_urid(int value);

	inline const int get_urid() const;


#ifdef _DEBUG
	//Ausgabe der Attribute
	virtual void Dump(CDumpContext& dc) const;
#endif

private:

	

	// Laufender Zähler für den Vk3Preis
	int idcounterVk3Preis;

	/// Element-ID
	int id;

	/// Anzahl
	int anzahl;

	/// Preis (Euro)
	double festPreis;

	///Sap Preis GLD
	double sapPreis;

	/// Preis fürs Pulvern einiger F&F(Euro)
	double pulverpreis;

	/// Preis bleibt const., sonst wird bei Serialize Element der VK3Elemente z.B. bei
	/// Muffe der Preis aus den Stammdaten geholt und überschreibt den VK3Preis.
	bool constpreis;
	
	double durchmesser;	

	/// Gewicht (kg)
	double gewicht;

	/// Artikelnummer
	long artikelnr;

	/// Mass 1, kann je nach Typ unterschiedliche Bedeutung haben
	int mass1;

	/// Mass 2 , kann je nach Typ unterschiedliche Bedeutung haben
	int mass2;

	/// Oberfläche
	double oberfl;

	///  Material ist Einkaufsteil
	bool einkauf;

	///  Material ist Halbzeug
	bool halbzeug;

	///  Posnr., wird fuer Profile verwendet
	CString posnr;

	///  urspruengliche ID bevor der Artikel bei fuer Verteileranlage umgewandelt wurde
	int urid;
};


///  Fuer Freigabe, damit die Profilkennung mit abgespeichert wird 
inline const long Material::get_artikelnr_ext() const
{
	/**
	Die oberen 8 Bits (erweiterte Attribute) werden nicht ausgeblendet
	*/
	return artikelnr;
}

inline const long Material::get_artikelnr() const
{
	/**
	Die oberen 8 Bits (erweiterte Attribute) werden ausgeblendet
	*/
	return artikelnr & 0x00FFFFFF;
}

inline const int Material::get_anzahl() const
{
	return anzahl;
}

inline void Material::set_anzahl(int value)
{
	anzahl = value;
}

/// Setzen des Gewichtes in Kg pro Einheit
inline void Material::set_gewicht(double value)
{
	gewicht = value;
}

inline void Material::set_oberfl(double value)
{
	oberfl = value;
}

inline const int Material::get_id() const
{
	return id;
}

inline void Material::set_id(int value)
{
	id = value;
}

inline void Material::set_idCounterVk3Preis(int value)
{
	idcounterVk3Preis = value;
}

inline const int Material::get_idCounterVk3Preis()const
{
	return idcounterVk3Preis;
}

inline const int Material::get_mass1() const
{
	return mass1;
}

inline void Material::set_mass1(int value)
{
	mass1 = value;
}

inline const int Material::get_mass2() const
{
	return mass2;
}

inline void Material::set_mass2(int value)
{
	mass2 = value;
}

inline void Material::set_festPreis(double value)
{
	festPreis = value;
}


inline void Material::set_sapPreis(double value)
{
	sapPreis = value;
}

inline void Material::set_pulverpreis(double value)
{
	pulverpreis = value;
}

/// Liefert das Gewicht in Kg pro Einheit
inline const double Material::get_gewicht() const
{
	return gewicht;
}

inline const double Material::get_oberfl() const
{
	return oberfl;
}

/// Preis pro Einheit (Basispreis) auslesen.
inline const double Material::get_festPreis() const
{
	return festPreis;
}



/// Sap Preis GLD
inline const double Material::get_sapPreis() const
{
	return sapPreis;
}

inline const double Material::get_pulverpreis() const
{
	return pulverpreis;
}


inline const bool Material::get_halbzeug() const
{
	return halbzeug;
}

inline void Material::set_halbzeug(bool value)
{
	halbzeug = value;
}

inline const bool Material::get_einkauf() const
{
	return einkauf;
}

inline void Material::set_einkauf(bool value)
{
	einkauf = value;
}


/// Setzen des Durchmessers
inline void Material::set_durchmesser(double value)
{
	durchmesser = value;
}

/// Liefert den Durchmesser
inline const double Material::get_durchmesser() const
{
	return durchmesser;
}

/// Setzen der Posnr. 
inline void Material::set_posnr(const CString& value)
{
	posnr = value;
}

/// Liefert die Posnr.
inline const CString& Material::get_posnr() const
{
	return posnr;
}

inline void Material::set_urid(int value)
{
	urid = value;
}

inline const int Material::get_urid() const
{
	return urid;
}

inline const bool Material::get_constpreis() const
{
	return constpreis;
}

inline void Material::set_constpreis(bool value)
{
	constpreis = value;
}

inline void Material::set_bezeichnung(const CString& value)
{
	bezeichnung = value;
}


