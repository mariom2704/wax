#include "stdafx.h"


#include "gpstatistik.h"
#include "Server.h"




GpStatistik::GpStatistik(void)
{
	m_pServer = NULL;
}

GpStatistik::~GpStatistik(void)
{
}

///es kann ein Server Objekt übergeben werden
void GpStatistik::set_server(Server *pServer)
{
	m_pServer = pServer;
}

/**
Stücklisten der einzelnen Werkaufträge in die Datenbank schreiben
*/
bool GpStatistik::writeStuecklistenInDb(WerkauftragListe *liste)
{
	if (!liste)
		return false;

	bool bOk			= true,
		 bDeleteServer	= false;

	//Wenn kein Serverobjekt übergeben wurde, eines erstellen
	if (!m_pServer)
	{
		m_pServer = new Server(Server::save, NULL);
		bDeleteServer = true;
	}
	else
		m_pServer->clearErrorList();
	
	m_pServer->set_modus(DbBase::save);

	//wenn ein Fortschrittsobjekt gesetzt wurde, dieses übergeben
	if (get_Fortschritt())
		m_pServer->set_Fortschritt(get_Fortschritt());

	if (!m_pServer->updateStuecklisteWerkauftraege(liste))
	{
		appendError(m_pServer->getErrorListe());
		bOk = false;
	}

	if (m_pServer && bDeleteServer)
	{
		delete m_pServer;
		m_pServer = NULL;
	}

	return bOk;
}


bool GpStatistik::writeVk3ListeInDb(WerkauftragListe *liste)
{
	if (!liste)
		return false;

	bool bOk			= true,
		 bDeleteServer	= false;


	//Wenn kein Serverobjekt übergeben wurde, eines erstellen
	if (!m_pServer)
	{
		m_pServer = new Server(Server::save, NULL);
		bDeleteServer = true;
	}
	else
		m_pServer->clearErrorList();

	m_pServer->set_modus(Server::save);


	//wenn ein Fortschrittsobjekt gesetzt wurde, dieses übergeben
	if (get_Fortschritt())
		m_pServer->set_Fortschritt(get_Fortschritt());


	//vk3 Stücklisten in die Datenbank schreiben
	if (!m_pServer->updateVK3liste(liste))
	{
		appendError(m_pServer->getErrorListe());
		bOk = false;
	}

	if (m_pServer && bDeleteServer)
	{
		delete m_pServer;
		m_pServer = NULL;
	}

	return bOk;
}
