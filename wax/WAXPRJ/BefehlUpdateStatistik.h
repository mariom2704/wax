#pragma once
#include "befehlbase.h"

class WAXPRJDLL BefehlUpdateStatistik :
	public BefehlBase
{
public:
	BefehlUpdateStatistik(void);
	virtual ~BefehlUpdateStatistik(void);

	///starten des Befehls
	virtual bool start();

	///Liste mit Werkaufträgen zuweisen
	void set_werkauftraege(WerkauftragListe *liste);

	///es kann ein Server Objekt übergeben werden
	void set_server(Server *pServer);

	///es kann eine Fortschrittsanzeige gesetzt werden
	void set_mitFortschrittsanzeige(bool bMitFortschrittsanzeige);

private:
	WerkauftragListe *m_pListeWerkauftraege;

	Server *m_pServer;

	bool m_bMitFortschrittsanzeige;

	bool m_bUpdateModell;
};
