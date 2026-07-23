#pragma once
#include "befehlbasezeitraum.h"

/**
Befehl "Material kumzlieren"

Liest aus dem Wax das Material der Wax Positionen und kumuliert es pro Artikelnummer.
Die Ausgabe erfolgt in einer Exceldatei
*/
class BefehlMaterialKumuliert :
	public BefehlBaseZeitraum
{
public:
	BefehlMaterialKumuliert(void);
	virtual ~BefehlMaterialKumuliert(void);

	virtual bool start();
};
