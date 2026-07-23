#pragma once
#include "befehlbasezeitraum.h"

/**
Der Befehl ermittelt die Anzahl der Arbeitsfolgen pro DN-Wert und gibt sie in einer 
Matrix "Arbeitsfolgen - DnWerte" aus.

Die Ausgabe erfolgt in einer Exceldatei
*/
class BefehlArbeitsfolgenProDn :
	public BefehlBaseZeitraum
{
public:
	BefehlArbeitsfolgenProDn(void);
	virtual ~BefehlArbeitsfolgenProDn(void);


	///Startet den Befehl
	virtual bool start();

	
private:
	

};
