#pragma once
#include "..\waxprj\befehlbase.h"


/**
Zu allen Positionen in der Datenbank die Statistik neu generieren und in die Datenbank schreiben
*/
class WAXINT BefehlGesamtStatistkNeuSchreiben :
	public BefehlBase
{
public:
	BefehlGesamtStatistkNeuSchreiben(void);
	virtual ~BefehlGesamtStatistkNeuSchreiben(void);


	virtual bool start();
};
