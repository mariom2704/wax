#include "stdafx.h"


#include "resource.h"
#include "befehlgesamtstatistkneuschreiben.h"
#include "GpWerkauftrag.H"
#include "..\waxprj\BefehlUpdateStatistik.h"
#include "..\waxprj\BefehlShowError.h"
#include "..\waxprj\Fortschrittanzeige.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BefehlGesamtStatistkNeuSchreiben::BefehlGesamtStatistkNeuSchreiben(void)
{
}

BefehlGesamtStatistkNeuSchreiben::~BefehlGesamtStatistkNeuSchreiben(void)
{
}

/**
Zu allen Positionen in der Datenbank die Statistik neu generieren und in die Datenbank schreiben
*/
bool BefehlGesamtStatistkNeuSchreiben::start()
{
	WerkauftragListe		*pListeWa;

	CUIntArray				arWaid,arTemp;

	Fortschrittanzeige fortschritt;

	GpWerkauftrag			gpWerkauftrag;

	BefehlShowError			showError;

	int			iProzent = 0;

	CString		stMeldung;





	//Alle Werkaufträge aus der Datenbank lesen
	if (!gpWerkauftrag.readAllWaidsFromDb(arWaid))
		return false;

	long lAnzahl = arWaid.GetSize();

	CString st; st = getStammdaten().getText("IDS_WA_EINLESEN").c_str();//"Werkaufträge aus der Datenbank einlesen..."
	fortschritt.set_maxWert(lAnzahl);
	fortschritt.set_minWert(0);
	fortschritt.set_ueberschrift(st);
	fortschritt.start();


	//Waid einzeln abarbeiten. (Erst das komplette Einlesen der Werkaufträge und danach das Erstellen der Stückliste ergab Probleme!!)
	for (long lCount=0; lCount<arWaid.GetSize(); lCount++)
	{
		//Fortschrittsanzeige
		iProzent = fortschritt.nextStep(lCount);
		stMeldung.Format("%d%s, waid = %d, Datensatz %d von %d",iProzent,"%",arWaid[lCount],lCount,lAnzahl);
		fortschritt.setAnzeige1(stMeldung);


		long waid = arWaid[lCount];
		//Templiste erstellen
		arTemp.Add(waid);

		//Werkaufträge aus der Db einlesen (einzeln, da beim Einlesen der Gesamtliste Probleme auftraten)
		pListeWa = gpWerkauftrag.readWerkauftraegeFromDb(arTemp,false);
		arTemp.RemoveAll();

		//evt. auftgetretene Fehler speichern
		showError.appendError(gpWerkauftrag.getErrorListe());


		if (pListeWa)
		{
			//Statistik neu schreiben
			BefehlUpdateStatistik befehlUpdStat;

			befehlUpdStat.set_werkauftraege(pListeWa);
			befehlUpdStat.start();
			
			//evt. auftgetretene Fehler speichern
			showError.appendError(befehlUpdStat.getErrorListe());
		}

		//Speicher der intern erzeugten Listen wieder freigeben
		gpWerkauftrag.deleteListen();
	}


	//evt. aufgetretene Fehler anzeigen
	if (!showError.getErrorListe().empty())
		showError.start();


	return true;
}