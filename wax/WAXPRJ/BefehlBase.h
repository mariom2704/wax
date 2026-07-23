#pragma once
#include "afx.h"

class WAXPRJDLL BefehlBase :
	public WaxBase
{
public:
	BefehlBase(void);
	virtual ~BefehlBase(void);

	///Starten des Befehls. Muß in den abgeleiteten Klassen überschrieben werden
	virtual bool start();
};
