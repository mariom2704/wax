#ifndef _INC_SERVER_
#define _INC_SERVER_

#include "..\rohrkonst\Werkauftrag.h"
#include "..\rohrkonst\WerkauftragListe.h"
#include "Fortschrittanzeige.h"
#include "DbBase.h"
#include "GpSapWittenbergeAuftragsdaten.h"

class MXTable;
class CObList;

/// Zugriff auf die Tabellen in der Serverdatenbank
class WAXPRJDLL Server : public DbBase
{
public:
	
	
	void set_modus(enum DbBase::MODE value);

	GpSapWittenbergeAuftragsdaten* sapWittenbergeAuftragsdaten;


	/// Konstruktor
	Server(enum MODE modus, Werkauftrag * wa, DWORD  dbhandle = NULL  );

	Server(enum MODE modus, Werkauftrag* wa, CString dbname, CString tabname);

	virtual ~Server();


	void SetGpSapWittenbergeAuftragsdaten(GpSapWittenbergeAuftragsdaten* sapWittenbergeAuftragsdatenLocal)
	{
		if(sapWittenbergeAuftragsdaten == NULL)
		{
			sapWittenbergeAuftragsdaten = sapWittenbergeAuftragsdatenLocal;
		}
	}


	/// Lädt die WA Nummer aus der Statustabelle
	bool get_wanr();

	/// Setzt, bzw. versucht eine bestimmte Wanr in der Tabelle Status zu setzen.
	bool setwanr(int newwanr = 0, bool testUnique = true);
	bool ResetWanr();

	const DWORD& get_dbhandle() const;

	void set_Fortschritt(Fortschrittanzeige* pFortschritt);

	/// Testet auf Connect zur Serverdatenbank
	static CString checkServerDB();

	/// Holt aus der Statustabelle eine neue WA Nummer. Der Nummernkreis wird durch 
	/// die Materialnummer festgelegt.
	UINT getNewWaNr(bool isNextGeneration, CString strIppeMatNr, int materialnummer, __int64 auftragsnummer, int positionsnummer) const;

	void set_werkauftrag(Werkauftrag* value);

	/// Zugriff auf die Status Tabelle der Serverdatenbank
	bool updateStatus(bool updateWaNr = false);

	/// Zugriff auf die Status Tabelle der Serverdatenbank. Die Datumsfelder werden 
	/// hier explizit gelesen/gesetzt.
	bool updateStatusDatum(bool updateWaNr = false);

	/// Zugriff auf die Soko Tabelle der Serverdatenbank
	bool updateSoko();

	/// Zugriff auf die Sap Tabelle der Serverdatenbank
	bool updateSap(int nr, double price);

	/// Zugriff auf die rohrleitung Tabelle der Serverdatenbank
	bool updateRohrleitung();

	/// Zugriff auf die Tabelle Stückliste für die Statistik
	bool updateStuecklisteWerkauftraege(WerkauftragListe *liste);

	/// Zugriff auf die VK3 Tabelle der Serverdatenbank
	bool updateVK3liste(WerkauftragListe *liste);

	/// Zugriff auf die Werkauftrag Tabelle der Serverdatenbank mit den 
	/// Informationen der Leitkarte, den Objektinformationen,  dem Bemerkungsfeld 
	/// und der Beschichtung.
	bool updateLeitkarte(CString classID);

	/// Zugriff auf die Elemente Tabelle der Serverdatenbank
	bool updateElemente();

	/// Zugriff auf die Arbeit Tabelle der Serverdatenbank
	bool updateArbeit(const CString& tabname, CObList* liste);

	/// Zugriff auf die AnhangTabelle der Serverdatenbank
	bool updateAnhang();

	/// Zugriff auf die Material Tabelle der Serverdatenbank
	bool updateMaterial();

	/// Zugriff auf die objektspezifischen Tabellen der Serverdatenbank, z.B. für 
	/// Verteiler die Verteilertabelle, für Strangrohr die Strangrohtabell usw.
	bool updateObject();

	/// Pruefung, ob Tabelle Status gelesen werden kann
	bool testStatusTab();


	bool updatePreisanfrage();

	bool updateSdNummer();

	bool updateLiefertermin();

	bool updateKcAuftragsdaten();

private:
	Werkauftrag* werkauftrag;

	
	/// Gesetzter Modus des Serverzugriffs: Lesen oder Schreiben
	enum DbBase::MODE modus;


	/// Prüft in der Statustabelle und der Tabelle mit gesperrten Wanummern, ob die 
	/// gewünschte Nummer vorhanden ist.
	/// Liefert true, wenn die Nummer verfügbar ist.
	bool checkUniqueWanr(int testwanr) const;

	/// Löscht alle Einträge des Werkauftrages aus einer Tabelle.
	/// Gibt false zurück, wenn nicht alle gelöscht werden.
	bool deleteWerkauftragInTable(MXTable& tab);

	/// öffnet die Tabelle
	bool openTable(MXTable& tab);

	/// Speichert eine Elementliste entweder in der avelement (Strangrohr, 
	/// Stationsverteiler)Tabelle oder der element Tabelle (Verteiler). Es sind 
	/// zwei Tabellen gleicher Struktur vorhanden, damit sich die Daten besser 
	/// verteilen.
	bool updateElementTab(const CString& tabname, EnContainerRohrElemente& elemlist);

	/// Zugriff auf die objektspezifischen Tabelle für Verteilerfreigaben
	bool updateVerteiler();

	/// Zugriff auf die objektspezifischen Tabelle für Strangrohrfreigaben
	bool updateStrangrohr();

	/// Zugriff auf die objektspezifischen Tabelle für Stationsverteilerfreigaben
	bool updateStationsverteiler();

	bool updateCOIIMX1230200Verteiler();
	/// Zugriff auf die Tabelle mit Entnahmeleitungen der Serverdatenbank
	bool updateEntnahmeleitung();

	/// Zugriff auf die Tabelle mit Pumpenverteiler der Serverdatenbank
	bool updatePumpenverteiler();

	/// Zugriff auf die Tabelle mit Pumpenprobierleitung der Serverdatenbank
	bool updatePumpenprobierleitung();


	/// Zugriff auf die objektspezifischen Tabelle für Gewinderohrfreigaben
	bool updateGewinderohr();

	///  Zugriff auf die objektspezifischen Tabelle für CadWerkaufträge aus dem Rohrnetzkonfigurator
	bool updateCadWerkauftrag();

	bool updateApax();

	/// Objekt zur Forttschrittsanzeige
	Fortschrittanzeige *m_pFortschritt;
};

#endif
