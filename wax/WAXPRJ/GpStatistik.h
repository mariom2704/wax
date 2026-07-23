#pragma once
#include "gpbase.h"
#include "..\rohrkonst\WerkauftragListe.h"


class WAXPRJDLL GpStatistik :
	public GpBase
{
public:
	GpStatistik(void);
	virtual ~GpStatistik(void);


	bool writeStuecklistenInDb(WerkauftragListe *liste);

	bool writeVk3ListeInDb(WerkauftragListe *liste);

	///es kann ein Server Objekt übergeben werden
	void set_server(Server *pServer);



private:
	Server *m_pServer;


};
