#pragma once
#include "gpanlage.h"
#include "DbAnlage.h"

class DialogGpAnlageSaegeNut :
	public GpAnlage
{
public:
	DialogGpAnlageSaegeNut(MxCObList *auftraege);
	virtual ~DialogGpAnlageSaegeNut(void);

	enum anlagen
	{
		AnlageSaegeNut = 1,					//nur Sägenut
		AnlageVerteiler = 2,				//Verteileranlage ohne Sägenut (kommt eigentlich nicht vor)
		AnlageSaegeNutUndVerteiler = 3		//Sägenut und Verteileranlage
	};

	enum ablageplatz
	{
		AblageHandelslaenge = 1,
		AblageVerteilerAnlage = 2,
		AblageVerteilerHand3 = 3,
		AblageVerteilerHand4 = 4,
		AblageSonder = 5
	};

	enum typ
	{
		TypHandelslaenge = 0,
		TypVerteilerAnlage = 1,
		TypVerteilerhandschweissung = 2
	};

	enum lochTyp
	{
		Muffe = 0,
		Nutstutzen = 1,
		Rohr = 2,	//Stutzen mit variabler Länge
		Sonder = 3,  //Manuelle Eingabe
		Bohrung = 4
	};

	//Aufträge für Sägenut und Verteiler in die Datenbank schrieben
	bool createSaegeNutVerteilerDb();



	//Aufträge aus der Datenbank lesen
	bool readAuftraege(GpAnlage::Status status, list<DbAnlage::kopfsatz> &data, MXDate date);

	//zu der Wanr alle Datensätze löschen
	bool cleanTables(int wanr);

	static bool greater(DbAnlage::saegeNutRohre r1, DbAnlage::saegeNutRohre r2);

protected:
	DbAnlage::kopfsatz kopfData;

	//Listen mit den Datensätzen
	list<DbAnlage::saegeNutRohre>		saegeNutRohrListe;
	list<DbAnlage::saegeNutRohre>		saegeNutRohrListeLegacy;
	list<DbAnlage::saegeNutLoecher>		saegeNutLoecherListe;

	list<DbAnlage::verteilerRohre>		verteilerRohrListe;
	list<DbAnlage::verteilerNutAbgang>	verteilerNutAbgangListe;

	//ersten Werkauftrag aus Auftragsliste holen und Prüfen ob Wanr gesetzt ist
	Werkauftrag *getFirstAuftrag();

	//Alle Werte erzeugen
	bool createWerte(Werkauftrag *werkauftrag);

	//Listen und Kopfsatz mit Werten füllen
	bool createKopfsatz(Werkauftrag *werkauftrag);
	bool createRohre(Werkauftrag *werkauftrag);
	bool createVerteilerRohre(Rohrleitung *rohr, DbAnlage::verteilerRohre &vertRohrDaten);
	bool createLoecher(int absAbstandRohr, bool drehen, DbAnlage::saegeNutRohre rohrDaten, DbAnlage::verteilerRohre vertRohrDaten, EnCObListModell &unterelemente, int &lochCounter);

	//Status prüfen
	bool checkStatus(int wanr);

	//zu der Waid alle Datensätze löschen
	bool cleanTablesWaid(int waid);

	//Daten in die Db schreiben
	bool writeDatenInDb();


	void changeBoolValue(bool &wert1, bool &wert2);

	DbAnlage dbAnlage;



	//min max Werte
	int minDnMuffe;
	int maxDnMuffe;
	int minDnStutzen;
	int maxDnStutzen;

	int minTeilrohrLaenge; //Mindestrohrlänge
	int maxTeilrohrLaenge; //Maximale Länge des Rohres auf der Sägenutanlage
	int minDn;			   //Mindesdurchmesser des Rohres
	int maxDnSaegeNut;	   //Max Durchmesser des Rohres auf der Sägenutanlage
	int maxDnVerteiler;    //Max Durchmesser des Rohres auf der Verteilerrohrstrasse
	int maxLaengeAblage4;  //Maximale Länge der Ablage 4

	int minAbstAnfangVerteiler; //Mindestabstand der 1. Elementes auf dem Verteiler
	int minAbstEndeVerteiler;	//Mindestabstand des letzten Elementes auf dem Verteiler

	int minAbstRohrAnfangEndeSaegeNut;
	bool isMinAbstandVonMitteLoch;
};
