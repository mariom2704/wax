#include "stdafx.h"


#include ".\befehlupdatestatistik.h"
#include "..\waxprj\GpStatistik.h"
#include "..\rohrkonst\MxListe.h"


BefehlUpdateStatistik::BefehlUpdateStatistik(void)
{
	m_pListeWerkauftraege = NULL;
	m_pServer = NULL;
	m_bMitFortschrittsanzeige = false;

	m_bUpdateModell = true;
}

BefehlUpdateStatistik::~BefehlUpdateStatistik(void)
{
}


///Liste mit Werkaufträgen zuweisen
void BefehlUpdateStatistik::set_werkauftraege(WerkauftragListe *liste)
{
	m_pListeWerkauftraege = liste;
}

///es kann ein Server Objekt übergeben werden
void BefehlUpdateStatistik::set_server(Server *pServer)
{
	m_pServer = pServer;
}

///es kann eine Fortschrittsanzeige gesetzt werden
void BefehlUpdateStatistik::set_mitFortschrittsanzeige(bool bMitFortschrittsanzeige)
{
	m_bMitFortschrittsanzeige = bMitFortschrittsanzeige;
}



/**
Starten des Befehls UpdateStatistik.
Aus der Liste der Werkaufträge werden die Statistikdaten in die Db geschrieben
Zur Zeit:
	- Stückliste 
*/
bool BefehlUpdateStatistik::start()
{
	if (!m_pListeWerkauftraege)
		return false;

	Fortschrittanzeige fortschritt;

	GpStatistik gpStatistik;
	if (m_pServer)
		gpStatistik.set_server(m_pServer);

	bool bReturn = true;


	if (m_bMitFortschrittsanzeige)
		gpStatistik.set_Fortschritt(&fortschritt);


	//Modell erzeigen
	if (m_bUpdateModell)
	{
		MxIterator iter(*m_pListeWerkauftraege);
		while (iter.next())
		{
			Werkauftrag* pWerkauftrag = dynamic_cast<Werkauftrag*>(m_pListeWerkauftraege->getAt(iter));
			if (!pWerkauftrag)
				continue;

			pWerkauftrag->createModell();	
		}
	}



	//Materialstückliste in die Datenbank schrieben. Tabelle stuecklistewa
	bReturn = gpStatistik.writeStuecklistenInDb(m_pListeWerkauftraege);


	//Stücklisten für vk3 Preise in die Datenbank schreiben
	bReturn = gpStatistik.writeVk3ListeInDb(m_pListeWerkauftraege);


	//Fehlerliste zuweisen
	appendError(gpStatistik.getErrorListe());

	return bReturn;
}