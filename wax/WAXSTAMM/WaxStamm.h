#pragma once

#include "QueryBrennzugabe.h"
#include "QuerySondermaterial.h"
#include "QueryBauelemente.h"
#include "QueryFormstueckeFittinge.h"

#include <vector>

/**
Diese Klasse lädt die Daten aus den Stammdatentabellen. Die Stammdaten werden 
in Cache Objekten gespeichert, die in einer CObList als doppelt verkettete 
Liste gespeichert werden. Die Listen sind über Zugangsmethoden abrufbar, z.B. 
get_niederlassungen(). Dadurch wird bei maximaler Performance ein  Zugriff auf die 
Stammdaten sichergestellt. Komplizierte Abfragen sind so allerdings nicht 
möglich und müssen bei Bedarf extra aus der Datenbank gelesen werden. Dies 
geschieht durch Angabe einer Select Anweisung beim Erzeugen
einer der Klassen für die Stammdaten.
*/
class WAXSTAMMDLL WaxStamm  
{
public:
	WaxStamm();

	~WaxStamm();

	//Alle Stammdaten laden
	void loadData(const std::string& database, CProgressCtrl* prog = NULL);

	/// Fügt alle Niederlassungen in eine Combobox ein
	void initNL_ComboBox(CWnd * cb) const;	

	/// Umwandlung einer DN Angabe in einen String. Umwandlung kann als DN Angabe 
	/// oder in Zoll erfolgen.
	static CString dn2str(bool conv2zoll, int dn, int typ = 0);

	/// Umrechnungsmethode von DN zu Zoll Angaben
	static int zoll2dn(const CString & zoll);

	/// Umrechnungsmethode von DN zu Zoll Angaben
	static CString dn2zoll(int dn, int typ = 0);

	/// Public-Wrapper für den Aufruf ZMass::get_val
	int get_zmassval(FormstueckeUndFittinge::GRUPPENID  id, int dn, int dnvl);

	/// Liefert zu einer Elementid den zugehörigen Bezeichnungsstring
	CString getBaugruppenBezeichnung(int id);

	//Liefert zu einer NL Kennung die NL Bezeichnung
	CString getNLBezeichnung(int nl);

	//Einheit zur Artikelnummer ermitteln
	CString getMaterialeinheit(int artikelnr);

	CString getSapArtikelbezeichnung(int artikelnr);

	const std::string& getText(const std::string& id, bool nullIfNotFound = false);
	std::string getTextWithColon(const std::string& id);

	//Wandstärke
	double getWandstaerke(int elemId, int dn);

	const Verzinkung& get_verzinkung() const;

	const VK3Preise& get_vk3preise() const;

	const CObList& get_dnzuaussendurchmesser() const;

	const CObList& get_abzugradius() const;

	const CObList& get_arbeitsfolgen() const;

	const CObList& get_ral() const;

	const CObList& get_profile() const;

	const CObList& get_bauelementgruppen() const;

	const CObList&  get_niederlassungen() const;

	const CObList& WaxStamm::get_Brandschutztechniken() const;

	const CObList& get_beschichtungen() const;

	const CObList& get_formstueckefittinge() const;

	const CObList& get_bauelementeafos() const;

	const CObList& get_afozeiten() const;

	const CObList& get_preisfaktoren() const;

	const CObList& get_pulvernzeiten() const;

	const CObList& get_dummymaterialnummern() const;

	const CObList& get_kostenstellen() const;

	const CObList& get_arbeitsplaetze() const;

	const CObList& get_biegung() const;

	const CObList& get_aussendurchmesserwandstaerke() const;

	const CObList& get_sokos() const;

	const CObList& get_zmass() const;

	const CObList& get_sondermaterial() const;

	const CObList& get_krantaetigkeit() const;

	const CObList& get_montagezeiten() const;

	const CObList& get_rohrtexte() const;

	const CObList& get_profilearbeitsfolgen() const;

	const CObList& get_profilesaegezeiten() const;

	const CObList& get_zulassung() const;

	const CObList& get_putzenzeiten() const;
	
	const CObList& get_sapartikelstamm() const;

	const CObList& get_SpanxPreiseArtikelstamm() const;

	const CObList& get_bauelemente() const;

	const Bauelemente& getBauelemente() const { return bauelemente; };

	const BauelementGruppen& getBauelementGruppen() const { return bauelementgruppen; };

	const CObList& get_texte() const;

	WaxStammdaten::QueryBrennzugabe queryBrennzugabe;

	WaxStammdaten::QuerySondermaterial querySondermaterial;

	WaxStammdaten::QueryBauelemente queryBauelemente;

	WaxStammdaten::QueryFormstueckFittinge queryFormstueckeFittinge;

private:
	ProfileArbeitsfolgen profilearbeitsfolgen;

	zulassung zulassung;

	ProfileSaegezeiten profilesaegezeiten;

	//Liste mit gecachten Daten aus Tabelle Krantaetigkeit
	Krantaetigkeit krantaetigkeit;

	//Liste mit gecachten Daten aus Tabelle Sondermaterial
	Sondermaterial sondermaterial;

	Montagezeiten montagezeiten;

	RohrTexte rohrtexte;

	VK3Preise vk3preise;

	Profile profile;

	Soko sokos;

	ZMass zmass;

	//Liste mit den Stammdatentabellen
	std::vector<StammDat*> tbllst;


	Preisfaktoren preisfaktoren;

	BauelementeAfos bauelementeafos;

	AfoZeiten afozeiten;

	DNzuAussendurchmesser dnzuaussendurchmesser;

	AbzugRadius abzugradius;

	//Liste mit den Arbeitsfolgen
	Arbeitsfolgen arbeitsfolgen;

	Ral RAL;

	BauelementGruppen bauelementgruppen;

	//Liste mit gecachten Daten aus Tabelle Niederlassungen
	Niederlassungen niederlassungen;

	Brandschutztechniken brandschutztechniken;

	CSpanxPreiseArtikelstamm m_SpanxPreiseArtikelstamm;

	//Liste mit gecachten Daten aus Tabelle Bauelemente
	Bauelemente bauelemente;

	FormstueckeUndFittinge formstueckefittinge;

	//Liste mit gecachten Daten aus Tabelle Beschichtungen
	Beschichtungen beschichtungen;

	PulvernZeiten pulvernzeiten;

	DummyMaterialnummern dummymaterialnummern;

	Arbeitsplaetze arbeitsplaetze;

	Kostenstellen kostenstellen;

	AussendurchmesserWandstaerke aussendurchmesserwandstaerke;

	Verzinkung verzinkung;

	PutzenZeiten putzenzeiten;

	SapArtikelstamm sapartikelstamm;

	RohrstutzenBrennzugabe rohrstutzen_brennzugabe;

	Biegung biegung;

	Texte texte;
};
