#ifndef _INC_FREIGABE_
#define _INC_FREIGABE_

class LeitkarteData;
class WaxPart;

#include "Server.h"
#include "dbBase.h"


/**
DoTo: Prüfung der Plausis in GpFreigabe verlagern und von dort die DbFreigabe aufrufen.
*/


//Freigabe und Statusaktualisierung 
//Realisiert die Freigabefunktionalität
class DbFreigabe : DbBase
{
public:
	bool checkBeschichtung();

	//Konstruktor
	DbFreigabe(WaxPart* wa, DWORD dbhandle);
	
	//Test auf vollständige Eingabe der Leitkarte
	bool check_freigabe_plausis();

	//Liest aus der Serverdatenbank den Status aus und setzt die Werte in der 
	//Leitkarte
	bool updateStatus(Server& serverdb);

	//Freigabe des Werkauftrages durchführen, bzw. Freigabedaten aktualisieren
	bool freigeben();

	//Freigeben der Preisanfrage
	bool preisanfrageFreigeben(Fortschrittanzeige *fortschritt);
	
	
private:
	WaxPart* pt;

	///  Programm wird in der AV verwendet
	bool av;

	///  Versionsinfo des Dateiformates
	UINT version;

	///  freizugebender Werkauftrag
	Werkauftrag& werkauftrag;

	///  Referenz auf die Leitkarte des WA
	Leitkarte& leitkarte;

	//Öffnet oder erstellt einen neuen Freigabedatensatz
	unsigned long getfreigabeid(WaxPart *pt);
	unsigned long getfreigabeid(DWORD dbhandle);

	// Wenn ges_fortschritt >= 0 ist, wird der Wert als Gesamtfortschritt angezeigt
	// sonst wird der Fortschritt bei der Freigabe der Position angezeigt
	int ges_fortschritt;

	bool insertNewPreisanfrage(int &panr, int papos);

	string tabPreisanfrage;

	bool freigabeWerkauftrag(WaxPart *pt, bool writeStl);

	
};

#endif
