#pragma once

#include "..\rohrkonst\WerkauftragListe.h"
#include "Server.h"
#include "Fortschrittanzeige.h"

class WAXPRJDLL GpInitialUpdateAfos
{
	/**
	Generierung der Afos und Schreiben der Afos in die Servertabelle Arbeit
	*/
public:
	GpInitialUpdateAfos(WaxPartListe& l);

	//bool write_afos();

	bool writeAfos();

private:
	/// Zugriff auf den Freigabeserver
	Server server;

	/// Liste mit Werkaufträgen, deren Afos erzeugt und gespeichert werden sollen
	WaxPartListe& waxpartListe;
	//WerkauftragListe& liste;

	Fortschrittanzeige progress;

	bool writeAfos(WaxPartListe& liste, bool createAfos);

	bool updateArbeit(Werkauftrag *wa);

	int laengeListe;
	int counter;
};
