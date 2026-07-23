#pragma once
#include "befehlbase.h"


/**
In einer Liste die angefallenen Fehler anzeigen.
Die Fehlerliste wird über appendError(..) gesetzt.
*/
class WAXPRJDLL BefehlShowError :
	public BefehlBase
{
public:
	BefehlShowError(void);
	virtual ~BefehlShowError(void);

	virtual bool start();
};
