#pragma once


#include "Arbeitsfolge.h"
#include "Element.h"


class Beschichtung;
class Rohrleitung;
class Strangrohr;
//Arbeitsplan - Definition des Arbeitsplanes
class ROHRKONSTDLL Arbeitsplan  : public CObject
{

	
private:

	
	Rohrleitung*  m_pRohr;
	//Eine einzelne Liste mit Arbeitsfolgen
	MxCObList  arbeitsfolgen;

	//ob Afos automatisch kumuliert werden
	bool m_bKumulieren;

	double getAfoZeitWasser(int count, Element *ele, int afoid, int iFertigungsstätte, CString strArbeitsplatz, const Beschichtung beschichtung);
	double getAfoZeitCOII(int count, Element *ele, int afoid, const Beschichtung beschichtung);


	double minderMengenZeit;

	double minderMengenZeitzuschlag;


public:

	double getAfoZeit(int count, Element *ele, int afoid, const Beschichtung beschichtung, int iFertigungsstätte, CString strArbeitsplatz,  int iProduktTyp);
	void SetRohrType(Rohrleitung*  pLRohr)
	{	
		m_pRohr = pLRohr;
	};

		
	Rohrleitung* GetRohrType(){return m_pRohr;};
	static CString get_abpSchweissVorrichtplatz();//"32000"

	///Fuegt einem Material zugeordnete Afo hinzu 
	void addAfos(Material *mat);
	
	void setSap(int id, bool sap);

	//Standardkonstruktor
	Arbeitsplan();

	//Destruktor
	virtual ~Arbeitsplan();

	//Arbeitsplan löschen
	void clean();

	//Speichern und Laden der Daten
	void serialize(MxFilesystem::BinaryArchive& ar);

	//Kopierfunktion	
	void copy(const Arbeitsplan& other);

	//Addiert zwei Arbeitspläne
	void merge(const Arbeitsplan& other);

	///Afos fuer Schweisser zusammenfassen 
	void mergeSchweissafos();

	///Mindermenge setzen
	void setMindermenge();

	///Hat Arbeitsplan Afo Vorrichten
	bool hasAfo(int id);

	Arbeitsfolge* getAfo(Arbeitsfolge::ÂRBEITSFOLGENTYP id);
	
	///Hat Arbeitsplan Afo fuer angegebenen Arbeitsplatz
	bool hasArbeitsplatz(CString arbeitsplatz);

	//Hier wird die gesamte Fertigungszeit fuer einen bestimmten Arbeitsplatz ermittelt.
	double getZeitArbeitsplatz(CString arbeitsplatz);

	///Hier wird die gesamte Fertigungszeit fuer eine bestimmte Kostenstelle ermittelt.
	double getZeitKostenstelle(int kostenstelle);

	//Hier wird die gesamte Fertigungszeit ermittelt. Mit od. ohne Pulvern
	double getZeit(bool gesamt = true);

	///Hier wird die ermittelte Kranzeit als Ruestzeit bei Afo 16/17 gesetzt 
	void setKranzeit(double zeit, bool mitVerlaengerung = false);

	//Initialisiert den Text der Arbeitsfolgen 27/40/41, bzw. setzt den 
	//Standardtext wenn setDefault true ist.
	void updateSpecialAF(  Arbeitsfolge* af, const Beschichtung &werk, double gewicht, int maxlaenge, int wanr);

#ifdef __OLDAPAX
// PR 31.3.2008 Erstmal deaktiviert

	//Speichern und Laden der Daten aus einem Altwax Arbeitsplan
	CString serializeOldApax(BinaryArchive& ar);
#endif

	void getAfoZeit(Material *mat, Arbeitsfolge *afo);

	double getAfoZeit(int id, int dn, const Beschichtung &beschichtung, int iFertigungsstätte, CString strArbeitsplatz, int iProduktTyp);

	void addAfos( CArray<int,int> * afolist, CArray<int,int>* dnlist,  const Beschichtung &beschichtung, int iFertigungsstätte, CString strArbeitsplatz, int iProduktTyp);

	//Fügt die Arbeitsfolge 41 oder 42 (verzinkt) ein (Bündeln und anliefern). 
	//Der Unterschied der beiden Arbeitsfolgen ist nur der Arbeitsplatz.
	void addAfo_Anlieferung(bool verzinkt, bool pulveranlage = false);

	//Fügt eine Arbeitsfolge fürs Beschichten ein. Bei Pulvern ist dies Nr. 27.
	void addAfo_Beschichtung(int anzahl, const Beschichtung & beschichtung, double zeitPulvern, double zeitGrundieren, double zeitPutzen);

	//Liefert true zurück, wenn die Liste mit Arbeitsfolgen leer ist.
	bool isEmpty() const;

	//Direkter Zugriff auf die Arbeitsfolgen
	 MxCObList& get_arbeitsfolgen() ;

	 const MxCObList& get_arbeitsfolgen() const;

	//Sortiert die Afos nach Codenr.
	void sortAfos(MxCObList& afoliste);

	//Sortiert die Afos nach Codenr.
	void sortAfos();

	// Fügt die Arbeitsfolge 99 (Übergabe an Produktion) hinzu
	void addAfo_Produktion();

	// Fügt die Arbeitsfolge 8 (Rüstzeiten) hinzu
	void addAfo_Ruesten(int count);

	// Fügt alle einem Element zugeordneten Afos hinzu, afofirst und afolast 
	// bestimmen welche arbeitsfolgen tatsaechlich genommen werden
	void addAfos(int count, Element *ele, const Beschichtung beschichtung, int iFertigungsstätte, CString strArbeitsplatz, int iProduktTyp, int afofirst = 0, int afolast = 4);

	// Einfügen einer Standardarbeitsfolge. Die restlichen Werte werden den 
	// Stammdaten entnommen.
	void addAfo(Arbeitsfolge *af);


	void set_kumulieren(bool value);

	
	double get_minderMengenZeit();

	double get_minderMengenZeitzuschlag();

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;	///< Ausgabe der Attribute
	#endif
};

