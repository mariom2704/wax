#pragma once

#include "..\waxprj\BefehlBase.h"


class WAXINT BefehlZusammenfassungWerkauftraegeRobbi : public BefehlBase
{
public:
	BefehlZusammenfassungWerkauftraegeRobbi(void);
	virtual ~BefehlZusammenfassungWerkauftraegeRobbi(void);

	//starten des Befehls
	virtual bool start();

};
