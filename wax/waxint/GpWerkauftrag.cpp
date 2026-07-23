#include "stdafx.h"


#include "resource.h"
#include "gpwerkauftrag.h"
#include "..\rohrkonst\cadwerkauftrag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GpWerkauftrag::GpWerkauftrag(void)
{
}

GpWerkauftrag::~GpWerkauftrag(void)
{
	//Speicher der Listen wieder freigeben
	deleteListen();
}



/**
Alle Waids aus der Db lesen
*/
bool GpWerkauftrag::readAllWaidsFromDb(CUIntArray &arWaid)
{
	//Waids aus der Datenbank lesen
	if (!m_dbWerkauftrag.readAllWaids(arWaid))
		return false;

	return true;
}

/**
Zu der Liste mit WA Nummern die Liste mit zugehörigen Waids erzeugen
*/
bool GpWerkauftrag::readWaidsFromDb(const CUIntArray &arWaNr,CUIntArray &arWaid)
{
	CUIntArray arTempWaid;

	CString str,error;


	for (int i=0; i<arWaNr.GetSize(); i++)
	{
		arTempWaid.RemoveAll();

		//Zu der WA Nummer die Positionen (Waids) aus der DB lesen
		m_dbWerkauftrag.readWaids(arWaNr[i],arTempWaid);

		if (arTempWaid.IsEmpty())
		{
			str = getStammdaten().getText("IDS_WERKAUFTRAG_NICHT_GEFUNDEN").c_str();
			error.Format(str,arWaNr[i]);
			appendErrorDescription((LPCSTR)error);
			continue;
		}

		arWaid.Append(arTempWaid);
	}

	return true;
}

/**
Werkaufträge aus der Datenbank einlesen

*/
WerkauftragListe* GpWerkauftrag::readWerkauftraegeFromDb(const CUIntArray &arWaid,bool append)
{

	if (!append)
	{
		m_werkauftragListe.deleteAllElemente();
	}

	if (arWaid.IsEmpty())
		return &m_werkauftragListe;


	Werkauftrag *pWerkauftrag = NULL;

	long lAnzahl = arWaid.GetSize();


	Fortschrittanzeige *pFortschritt = get_Fortschritt();

	//Fortschrittsanzeige ínitialisieren, wenn gesetzt
	if (pFortschritt)
	{
		CString st; st = getStammdaten().getText("IDS_WA_EINLESEN").c_str();//"Werkaufträge aus der Datenbank einlesen..
		pFortschritt->set_maxWert(lAnzahl);
		pFortschritt->set_minWert(0);
		pFortschritt->set_ueberschrift(st);
		pFortschritt->start();
	}

	CString stMeldung;

	for (long lCount=0; lCount<arWaid.GetSize(); lCount++)
	{   
		//Fortschritt
		if (pFortschritt)
		{
			int iProzent = pFortschritt->nextStep(lCount);

			stMeldung.Format("%d%s, waid = %d, Datensatz %d von %d",iProzent,"%",arWaid[lCount],lCount,lAnzahl);
			pFortschritt->setAnzeige1(stMeldung);
		}

		m_dbWerkauftrag.clearErrorList();

		//Werkauftrag aus den Daten aus der Db erzeugen
		pWerkauftrag = m_dbWerkauftrag.readWerkauftrag(arWaid[lCount]);

		//evt. aufgetretene Fehlermeldungen übergeben
		if (!m_dbWerkauftrag.getErrorListe().empty())	{
			appendError(m_dbWerkauftrag.getErrorListe());
		}

		if (!pWerkauftrag)
			continue;

		//Werkauftrag in Liste schreiben
		m_werkauftragListe.append(pWerkauftrag);
	}

	return &m_werkauftragListe;
}

//Zu der WA Nr den Werkauftrag aus der Db lesen
Werkauftrag* GpWerkauftrag::readWerkauftragFromDb(long waid)
{
	Werkauftrag* wa = NULL;

	//Werkauftrag aus den Daten aus der Db erzeugen
	wa = m_dbWerkauftrag.readWerkauftrag(waid);

	CadWerkauftrag *cad = dynamic_cast<CadWerkauftrag*>(wa);
	if (cad && (cad->isStrangrohr() || cad->isSdMaterialNrStrangrohr()))
	{
		Strangrohr *strang = new Strangrohr();
		strang->copy(*cad);
		delete wa;
		
		//damit alle Werte berechnet werden
		strang->createModell();

		wa = strang;
	}

	return wa;
}

//Werkauftrag an interne Werkauftragsliste hängen
void GpWerkauftrag::appendWerkauftrag(Werkauftrag *wa)
{
	m_werkauftragListe.append(wa);
}

void GpWerkauftrag::appendWerkauftragListe(WerkauftragListe &was)
{
	m_werkauftragListe.appendListe(was);
}


/**
Pointer auf Liste mit Werkaufträgen
*/
WerkauftragListe* GpWerkauftrag::get_werkauftraege()
{
	return &m_werkauftragListe;
}



/**
den Speicher der intern erzeugten Listen wieder freigeben. 
*/
void GpWerkauftrag::deleteListen()
{
	m_werkauftragListe.deleteAllElemente();
}

/**
Werkaufträge zu den WA Nummern aus der DB lesen und an interne Liste hängen
*/
bool GpWerkauftrag::appendWerkauftraege(const CUIntArray &arNewWa)
{
	CUIntArray arNewWaGeprueft,
			   arWaids;	


	//prüfen ob Werkauftraege schon in der Liste stehen
	for (int i=0; i<arNewWa.GetSize(); i++)
	{
		bool bFound = false;
		long newWa = arNewWa[i];

		MxIterator iter(*get_werkauftraege());
		while (iter.next())
		{
			Werkauftrag *pWa =  dynamic_cast<Werkauftrag*>(get_werkauftraege()->getAt(iter));
			if (!pWa)
				continue;

			//prüfen ob WA Nr schon in der Liste mit Werkaufträgen steht
			if (newWa == pWa->get_status().get_wanr())
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
			arNewWaGeprueft.Add(newWa);
	}

	//Werkaufträge aus DB lesen
	if (arNewWaGeprueft.IsEmpty())
		return false;

	
	//Zu den WA Nummern die Waids ermitteln. (Positionen der Werkaufträge)
	readWaidsFromDb(arNewWaGeprueft,arWaids);

	//Werkaufträge aus DB lesen und anhängen
	readWerkauftraegeFromDb(arWaids,true);


	return true;
}