#include "stdafx.h"


#include "befehlzusammenfassungwerkauftraegerobbi.h"
#include "DialogZusammenfassenWerkauftraegeRobbi.h"
#include "GpWerkauftrag.h"
#include "GpUebergabeStrangrohrstrasse.h"
#include "DialogHtmlView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BefehlZusammenfassungWerkauftraegeRobbi::BefehlZusammenfassungWerkauftraegeRobbi(void)
{
}

BefehlZusammenfassungWerkauftraegeRobbi::~BefehlZusammenfassungWerkauftraegeRobbi(void)
{
}


/**
Befehl zum Zusammenfassen von Werkaufträgen für die Strangrohrstrasse ausführen
*/
bool BefehlZusammenfassungWerkauftraegeRobbi::start()
{
	
	WerkauftragListe *werkauftraegeGesamt;  //Gesamtliste. Alle Positionen die zu einem Werkauftrag (Sammelauftrag) gehören

	GpUebergabeStrangrohrstrasse gpUebergabeStr;


	//WA Nummern abfragen
	DialogZusammenfassenWerkauftraegeRobbi dlg;

	//Dialog ermittelt in gpWa die Werkaufträge, die in die Exceldatei geschrieben werden sollen
	dlg.set_gpUebergabeStrStrasse(&gpUebergabeStr);
	if (dlg.DoModal() != IDOK)
		return false;
	

	//Werkaufträge auslesen
	werkauftraegeGesamt = gpUebergabeStr.get_werkauftraege();
	if (!werkauftraegeGesamt)
		return false;

	//Exceldatei erstellen
	if (!gpUebergabeStr.writeUebergabeDaten())
	{
		return false;
	}


	//wenn Aufträge zusammengefasst wurden, alte Einzelaufträge löschen
	gpUebergabeStr.deleteOldUebergabeWerte();


	//Liste mit den Werkaufträgen in eine HtmlDatei schreiben
	gpUebergabeStr.writeUebersicht();

	//html Übersichtsdatei drucken
	DialogHtmlView dlgHtmlView;
	dlgHtmlView.set_url(gpUebergabeStr.get_gesPfadUebersichtsDatei());
	dlgHtmlView.DoModal();

	return true;
}