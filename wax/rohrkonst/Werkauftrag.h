#pragma once
#include "Arbeitsplan.h"
#include "Leitkarte.h"
#include "Preis.h"
#include "Status.h"
#include "MaterialListe.h"
#include "Beschichtung.h"
#include "WaxBase.h"
#include "EnStueckliste.h"
#include "mxdate.h"
#include "EnCObListVk3.h"

#include <mxcontrols/binaryarchive.h>

/**
Werkauftrag - Definition eines Werkauftrages. Ein Werkauftrag besteht aus einem 
Arbeitsplan und einer Materialliste. Jeder Werkauftrag bekommt bei der Freigabe 
eine Nummer zurück (waid). Dies ist nicht die Werkauftragsnummer.
Diese Nummer ist der Primary Key zum Zugriff auf die Fertigungsdatenbank.
*/
class ROHRKONSTDLL Werkauftrag : public WaxBase  
{
public:

	
	enum FERTIGUNGSSTAETTE 
	{  
		OLDESLOE = 0,
		MAGDEBURG = 1,
	};

	/// Binärer Anhang zum Werkauftrag
	/// Enthält einen Pointer auf binäre Daten, ein Längenfeld und die Bezeichnung des 
	/// Anhangtyps
	class ROHRKONSTDLL Anhang 
	{
	private:
		/// Kurzbeschreibung
		CString bezeichnung;

		/// Kennung des Anhangs (Dateiendung). Z.B. DXF für Anhang ist DXF Datei
		CString filename;

		/// Binäre Daten
		void* data;

		/// Länge des Anhanges
		UINT len;

		bool showAnhang;

		

	public:

		
		
		/// Konstante, die die Maximalgrösse des Anhangs festlegt
		unsigned int MAXLEN_ANHANG;

		/// Standardkonstruktor
		Anhang();

		/// Destruktor
		virtual ~Anhang();

		void copy(const Anhang& other);

		/// Liest die Daten einer Datei in den Anhang ein
		void setfile(const CString& fn);

		/// Speichern der Daten
		void serialize(MxFilesystem::BinaryArchive& ar);

		const CString& get_filename() const;

		void set_filename(const CString& value);

		const void* get_data() const;

		void set_data(void* value, int argname);

		const UINT& get_len() const;
		
		void set_bezeichnung(const CString& bez);

		const CString& get_bezeichnung() const;

		bool is_graphic() const;

		void set_showAnhang(bool val);
		const bool get_showAnhang()const;
	};


public:
	/// Standardkonstruktor
	Werkauftrag();

	/// Destruktor
	virtual ~Werkauftrag();

	int Get_Fertigungsstaette()
	{
		return m_iFertigungsstaette;
	};

	void Set_Fertigungsstaette(int iFertigungsstaette )
	{
		 m_iFertigungsstaette = iFertigungsstaette;
		 return;
	};

	int getPreiswertigkeit()
	{
		return m_iPreiswertigkeit;
	}
/// Set Methoden
	void set_stdbemerkung(const CString& value);
	void set_bezeichnung(const CString& value);
	void set_bemerkung(const CString& value);
	void set_beschichtung(const Beschichtung& value);
	void set_posNr(const CString& value);

	void set_KollilisteExport(int value);
		
	void set_maxlaenge(int value);
	void set_zeugnis(int value);
	void set_anzahl(int value);
	void set_waid(int value);
	void set_GUID(CString value);
	CString get_GUID();
	void set_sichtbereich(int value);
	
	//mds#425
	void set_zulassungsstelle(int value);
	
	// Wegen der Maschinenlisten für Gewinderohre, richtige Afos zälen und Preise aufaddieren
	void set_UseSammelauftrag(bool bUseSammelauftrag)
	{
		m_bUseSammelauftrag = bUseSammelauftrag;
	}

	bool get_UseSammelauftrag()
	{
		return m_bUseSammelauftrag;
	}

	void set_plausiOk(bool value);
	void set_einkauf(bool value);
	
	void set_mindermenge(bool value);

	void set_erstellungsdatum(const MXDate& value);

	/// / Muss bei Änderungen aufgerufen werden
	void set_aenderungsdatum(const MXDate& value = MXDate::get_current_date());

	void set_leitkarte(const Leitkarte& value);

	/// Get Methoden
	const CString&		get_stdbemerkung()		const;
	const CString&		get_stdbezeichnung()	const;
	const CString&		get_bemerkung()			const;
		  CString		get_bezeichnung()		const;		/// /< Liefert die Bezeichnung des Werkauftrages. Wenn bezeichnung nicht gesetzt wurde, wird die stdbezeichnung erzeugt.
	CString             getPosBezeichnung(bool sdnr) const;	
	const CString&		get_posNr()				const;
	
	const int			get_KollilisteExport()	const;
	const int			get_maxlaenge()			const;
	const int			get_anzahl()			const;		/// /< Liefert eine die Bestellanzahl, default ist 1
	const int			get_sichtbereich()		const;
	const int			get_zeugnis()			const;
	const int			get_waid()				const;

	const bool			get_plausiOk()			const;
	const bool			get_einkauf()			const;
	
	const bool			get_mindermenge()		const;

	const int			get_zulassungsstelle()	const;

	const string			get_barcodeStringForGuidLineStart(int rohrQuali, double ad, bool isStart)	const;

	const string			get_barcodeStringForGuidLineEnd(int rohrQuali, double ad, int wanr)	const;

	
	/// / Gibt das XML Modell zurück, falls vorhanden.
	virtual std::string get_xml_modell() const { return std::string(); }


	bool isSdMaterialNrVerteiler() const;
	bool isSdMaterialNrStrangrohr() const;
	bool isSdMaterialNrGewinderohr() const;
	bool isSdMaterialNrApax() const;

	virtual bool isVerteiler() const{ return false; }
	virtual bool isStrangrohr() const{ return false; }
	virtual bool isGewinderohr() const{ return false; }
	virtual bool isApax() const{ return false; }

	virtual bool isStationsverteiler() const { return false; }
	virtual bool isPumpenprobierleitung() const { return false; }
	virtual bool isPumpenverteiler() const { return false; }
	virtual bool isEntnahmeleitungDWB() const { return false; }
	virtual bool isSammelauftrag() const { return false; }
	virtual bool isRohr() const { return false; }

	/// Wird von CadWerkauftrag und Handelslaenge überschrieben
	virtual bool isHandelslaenge() const { return false; }

	/// Gesamtgewicht der Rohrleitung berechnen
	virtual double getGesGewicht(int teile = 0) const {teile = 0; return 0.0; }


	virtual int getMaxLaenge() const;

	const MXDate& get_erstellungsdatum() const;
	const MXDate& get_aenderungsdatum()  const;
	
	Preis& get_preis();	

	const Preis& get_preis() const
	{
		return preis;
	}

	Status&	get_status();
	const Status& get_status_const() const { return status; }

	const Beschichtung& getBeschichtung() const;		/// /< const Zugriff auf die Beschichtung	
	      Beschichtung& get_beschichtung();				/// /< Direkter Zugriff auf die Beschichtung

	Werkauftrag::Anhang* get_anhang();				/// /< Direkter Zugriff auf den Anhang

	const Werkauftrag::Anhang* get_anhang()const 
	{
		return &anhang;
	}


	Leitkarte&			 get_leitkarte();			/// /< Direkter Zugriff auf die Leitkarte des Werkauftrages

	const Leitkarte&	 get_leitkarte() const;		/// /< const Direkter Zugriff auf die Leitkarte des Werkauftrages

	Arbeitsplan& get_arbeit();				/// /< Direkter Zugriff auf den Arbeitsplan

	const Arbeitsplan& get_arbeit() const
	{
		return arbeit;
	}
	
	MaterialListe&		 get_material();			/// /< Direkter Zugriff auf die Materialliste (von Reiter Material)
	const MaterialListe& get_material() const;

	const EnStueckliste& get_stueckliste() const;
	EnStueckliste&		 get_stueckliste();

	/// / Liefert true, wenn nach der Freigabe Änderungen vorgenommen wurden
	bool geaendert_nach_freigabe() const;

	virtual void	serialize(MxFilesystem::BinaryArchive& ar);		/// /< Speichern und Laden des Werkauftrages.

	virtual void copy(const Werkauftrag& other);

	void			reset();						/// /< Setzt die Freigabedaten zurück, damit die Kopie eines Objektes freigegeben werden kann


	/// create Methoden
	void			createProfilAfos();				/// /< Erzeugt die Arbeitsfolgen fuer Profile	
	virtual void	createAutoArbeit(bool beschichtung = true);	/// /< Hier wird automatisch der Arbeitsplan generiert
	virtual void	createModell();					/// /< Modell (Rohrkonstruktion) erzeugen. Dabei werden auch alle Listen gefüllt


	/// Prüfungen
	virtual CString checkPlausis();					/// /< Werkauftrag auf korrekte Werte checken

	/// Prüft den Werkauftrag und liefert evtl. einen Hinweis
	virtual CString checkHinweis() const { return ""; }

	virtual bool	checkSDMatNr() const;			/// /< Prüft ob die eingegebene SD Nummer für diesen Werkauftrag erlaubt ist.
	bool			checkMindermenge();				/// /< Arbeitsplan auf Mindermenge checken

	virtual bool	isEmpty() const;				/// /< Liefert true zurück, wenn die Materialliste und die Arbeitsliste leer ist. 
	virtual bool	avNacharbeitsPruefung() const;	/// /<  Liefert true, wenn im Sammelauftrag die Nacharbeit überprüft werden muss

	/// Sonstiges
	void			initPosnr(const CString& pattern);/// /< Versucht aus der übergebenen Positionsnummer eine neue zu erzeugen. 


	/// Auftrag hat seinen Ursprung in der CAD, Infoflag
	bool get_cadpos() const { return cadpos; }

	/// Auftrag wurde aus CAD Tabellen erstellt, Konvertierung erforderlich
	virtual bool is_native_cadpos() const { return false; }

	void set_cadpos(bool val) { cadpos = val; }

	/// get vk3elementeListe
	EnCObListVk3& get_vk3elementeListe();
	
	const EnCObListVk3& get_vk3elementeListe() const;

	
	
	///vk3liste erstellen
	virtual bool createVk3Liste();

	
private:
	CString stdbemerkung;
	CString bemerkung;		/// /< Bemerkungsfeld
	CString bezeichnung;	/// /< Kurzbezeichnung, die im Baum angezeigt wird
	int m_iFertigungsstaette;
	bool m_bUseSammelauftrag;

	CString posNr;			/// /< Positionsnummer
	
	bool mindermenge;		/// /< Mindermengenzuschlag muss berechnet werde
	bool plausiOk;			/// /< Wird auf nur dann auf true gesetzt, wenn eine Eingabemaske erfolgreich verlassen werden konnte.
	
	bool einkauf;			/// /< EK-Teile zu SAP


	int sichtbereich;		/// /< Flag für Fertigung: Material wird im Sichtbereich verbaut.
	int zeugnis;			/// /< Flag für Qualitätswesen: Werkauftrag braucht ein Zeugnis
	int m_KollilisteExport;
	bool m_FertigungMagdeburgMoeglich;
	int maxlaenge;
	int anzahl;				/// /< Bestellmenge
	int waid;
	CString strGUID;

	//mds#425
	int zulassungsstelle;	/// /< Rohrkennzeichnung (Vds, FM)
	/// /< Zähler der Werkauftragstabelle. Dient zum Finden des Werkauftrags bei Statusabfragen

	MXDate erstellungsdatum;	/// /< Erstellungsdatum des Objektes, readonly
	MXDate aenderungsdatum;	/// /< Datum/Uhrzeit der letzten Änderung
	
	Status		  status;

	Beschichtung  beschichtung;
		
	Preis		  preis;			/// /< Preisobjekt zur Berechnung des WAP

	Anhang		  anhang;			/// /< Pointer auf binäre Daten, die einen Anhang enthalten
	
	Arbeitsplan   arbeit;			/// /< Der Arbeitsplan zum Werkauftrag
	
	MaterialListe material;			/// /< Materialliste des Werkauftrages

	Leitkarte	  leitkarte;		/// /< Leitkarte des Werkauftrages
	
	/// Position hat Ursprung in CAD
	bool cadpos;

protected:
	
	int m_iPreiswertigkeit;
/// Attribute	
	CString stdbezeichnung;		/// /< Kurzbezeichnung, die defaultmässig im Baum angezeigt wird, wenn bezeichnung nicht gesetzt ist.

	EnStueckliste stueckliste;	/// /< Stueckliste

	/// Liste mit VK3-Elemente
	EnCObListVk3 vk3elementeListe;

	/// get Methoden
	virtual double getPulverzeit();			/// /< Errechnet die Pulverzeit für den Werkauftrag
	virtual double getPutzzeit();			/// /< Errechnet die Putzzeit für den Werkauftrag
	virtual double getGrundierungszeit();	/// /< Errechnet die Grundierzeit für den Werkauftrag

	/// create Methoden
	virtual void createStdAfos(bool beschichtung = true);	/// /< Erzeugt sie Standardarbeitsfolgen
	virtual void createBeschAfos(Arbeitsplan& plan);		/// /< Erzeugen der Afos fuer die Beschichtung
	virtual void createAnliefAfos(Arbeitsplan& plan);		/// /< Erzeugen der Afos fuers Anliefern
	virtual void addRohrausschuss(CObList& list);			/// /< Berechnungs des Aufschlags fuer Rohrverschnitt
};


