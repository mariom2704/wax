#pragma once

#include <mxcontrols/basedialog.h>

#define WM_PREISUPDATE WM_USER+123
#define UPDATE_GESAMTPREIS PostMessage(WM_COMMAND, WM_PREISUPDATE);

typedef std::map<long, double> CMapArtikelPreis;

class WAXUIDLL SpanxRohrDialog : public BaseDialog
{

	private:
	int m_Muffen_anzahl_Before;


protected:

	bool m_Cancel;

	std::vector<MxErrorHandling::MxError> m_errorListe;

	typedef struct _NAMESTRINGITEM
	{
		string	strName;
		int		iItem;
	}NAMESTRINGITEM;

	static bool sortVectorNameStringItem (NAMESTRINGITEM& first,NAMESTRINGITEM& second); 

	int m_iRegion;
	
	DECLARE_MESSAGE_MAP()
public:
	SpanxRohrDialog(CWnd *parent, UINT dlgid, const string& data);

	virtual ~SpanxRohrDialog();

	inline double get_preis() const;

	/// Eingabedaten als XML
	virtual string xml_data();

protected:

	virtual string format_langtext() = NULL;

	string format_vortext() const;

	/// Combobox mit Kupplungen initialisieren
	void initKupplungenVorauswahl();
	void initKupplungen(int dnselect);

	/// Combobox mit Haltern initialisieren
	void initHalterVorauswahl();
	void initHalter();

	
	virtual void initSchellenVorauswahl()= 0;

	virtual void  initSprinklerVorauswahl();

	/// Maximale Anzahl von Elementen pro Rohrstück
	int max_elements;

	/// Rohrtyp-Enum für Zugriff auf Vortexte
	virtual int get_rohrtyp() const = NULL;

	
	/// Rohrtyp-Enum für Zugriff auf Montagezeiten
	virtual int get_rohrtyp_montage() const = NULL;

	/// XML Tag für die Datensatzkennung
	virtual string get_xmltag() = NULL;

	virtual void initRohrAnfangEnde() = NULL;

	virtual void initElemente() = NULL;

	virtual void initNennweite() = NULL;

	virtual double get_montagezeit() const = NULL;

	virtual void OnOK();

	/// Daten aus Eingabefeldern lesen
	virtual void DoDataExchange(CDataExchange* pDX);

	/// Dialog initialisieren
	virtual void init_ui();

	bool verzinkt;

	/// Montagehöhe zur Ermittlung der Montagezeit
	int hoehe;

	/// Wenn gesetzt, wird im xml ein Vortext eingefügt
	bool vortext_einfuegen;

	DWORD getItemData(CComboBox *bx);

	/// Ausgaben in Zoll statt DN
	bool zoll;

	virtual void update_preis();

	int muffen_dn;

	int muffen_typ;

	int m_Muffen_anzahl;

	/// Optionale Elementlänge für Rohrstutzen usw.
	int ele_laenge;

	int rohr_anfang;

	int rohr_ende;

	int rohr_typ;

	int rohr_dn;

	int rohr_laenge;

	int rohr_anzahl;
	

	double preis;


	double m_Gesamtgewicht;


	int sprinkler_anzahl;
	string m_SprinklerArtikelNr;

	int schellen_anzahl;
	string m_SchellenArtikelNr;
	
	
	/// Menge der Halter pro x Meter
	double halter_anzahl;
	string m_HalterArtikelNr;
	
	int kupplung_anzahl;
	string m_KupplungArtikelNr;


	/// Referenz auf die Bauelementeliste in den Stammdaten
	const CObList& m_SpanxPreiseArtikelstamm;


	/// Referenz auf die Bauelementeliste in den Stammdaten
	const CObList& bauele;

	const CObList& fittinge;

	CComboBox* cbSprinklervorauswahl;
	/// Combobox mit den Sprinklern
	CComboBox *cb_sprinkler;

	CComboBox *cb_schellenvorauswahl;
	/// Combobox mit den Sprinklerschellen
	CComboBox *cb_schellen;



	CComboBox *cb_haltervorauswahl;
	/// Combobox mit den Haltern
	CComboBox *cb_halter;

	CComboBox *cb_kupplungvorauswahl;
	/// Combobox mit den Kupplungen
	CComboBox *cb_kupplung;

	/// Combobox mit der Muffendn
	CComboBox *cb_muffendn;

	/// Setzen des Einzelpreises bei Änderung der Auswahl in der Combobox
	virtual void set_schellenpreis();

	/// Aus den DN Angaben die richtige Schelle auswählen
	virtual void updateDN();


	void postUpdatePreisMsg();


	double get_preis(UINT idanz, UINT idpreis);

	/// Halterpreis setzen
	void set_halterpreis();

	//// Prallblechpreis setzen
	void set_Prallblechpreis();

	/// Preis der Kupplung setzen
	void set_kupplungpreis();

	/// Schreibt einen double Wert als Preis (mit Komma) in das angegebene Editfeld
	void setpreis(UINT dlgid, double preis);

	/// Pointer auf einen Werkauftrag zur Preisberechnung
	Werkauftrag *werkauftrag;
	
	/// Prüfung, ob als Element Muffe ausgewählt wurde. Dann muss die Sprinklerschelle auf
	/// Anzahl 0 gesetzt und grau geschaltet werden

	FloatEditCtrl anzHalterCtrl;

	/// xml Daten zur Initialisierung des Dialogs
	string xmldata;

	/// Attribute initialisieren über data xml-string
	void init_attr(const string& data);

	
	void selChangeSprinklerschellenVorauswahl();
	void selChangeHalterVorauswahl();
	void selChangeKupplungVorauswahl();


	void initAllMXItems(int GruppenID,  CComboBox* ItemComboBox);
	void SetCurlSelOfGruppe(int artikelnr, CComboBox* pComboxGroup);
	void SetCurlSelDependOnItemData(int artikelnr, CComboBox* pComboxArtikel);
	void update_Anzahl_Sprinkler();

	void update_Anzahl_Muffen_with_Keystroke_Test();
	void updateAnzahlSchellenWithKeystrokeTest();
	void updateAnzahlKupplungWithKeystrokeTest();
	void update_Anzahl_Rohre_with_Keystroke_Test();

	protected:
	virtual void OnCancel();

};


inline double SpanxRohrDialog::get_preis() const 
{
	return preis;
}

