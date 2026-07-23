#pragma once
#include "..\waxprj\gpbase.h"
#include "DbWerkauftrag.h"
#include "..\rohrkonst\WerkauftragListe.h"
#include "..\waxprj\Fortschrittanzeige.h"

/**
Geschäftprozesse zum Werkauftrag.

Achtung: Der Speicherplatz der intern erzeugten Listen MUSS mit deleteListen() wieder freigegeben werden.

Zur Zeit werden die Werkaufträge noch in den WaxPart Objekten verwaltet. Dies sollte in Zukunft voneinander 
getrennt werden. WaxPart ist ein Treecontrol und gehört zur Präsentationsschicht und sollte keine Prozesse darstellen.
*/
class WAXINT GpWerkauftrag :
	public GpBase
{
public:
	GpWerkauftrag(void);
	virtual ~GpWerkauftrag(void);


	WerkauftragListe* readWerkauftraegeFromDb(const CUIntArray &arWaid,bool append);// Werkaufträge aus der DB lesen

	Werkauftrag* readWerkauftragFromDb(long waid);		//Zu der waid (Position) den Werkauftrag aus der Db lesen

	
	void appendWerkauftrag(Werkauftrag *wa);			//Werkauftrag an interne Werkauftragsliste hängen

	//Werkaufträge anhängen
	void appendWerkauftragListe(WerkauftragListe &was);

	virtual bool appendWerkauftraege(const CUIntArray &arNewWa); //Werkaufträge zu den WA Nummern aus der DB lesen und an interne Liste hängen
	
	void deleteListen();								// Speicher der erzeugten Listen freigeben

	WerkauftragListe* get_werkauftraege();				// Pointer auf Liste mit Werkaufträgen


	


	//Hilfsfunktionen im Zusammenhang mit den Werkaufträgen
	bool readAllWaidsFromDb(CUIntArray &arWaid);						// alle Waid aus der DB lesen

	bool readWaidsFromDb(const CUIntArray &arWaNr,CUIntArray &arWaid);  // zu der Liste mit WA Nummern die Liste mit zugehörigen Waids erzeugen


protected:


	bool createWerkauftraegeZuWaids(WerkauftragListe &liste);
	//Schnittstelle zur Datenbank
	DbWerkauftrag m_dbWerkauftrag;

private:
	WerkauftragListe m_werkauftragListe;



};
