#include "stdafx.h"


#include ".\waxpartliste.h"

WaxPartListe::WaxPartListe(void)
{
	auto_delete = false; // Kein automatisches Löschen der Elemente 
}

WaxPartListe::~WaxPartListe(void)
{
}


/**
Aus der WaxPartliste eine Liste der Werkaufträge zurückgeben. 

\param mitUnterListen = true: Werkaufträge der Untergeordneten WaxPart Objekte werden berücksichtigt

\param mitDummyWp	  = true: Dummy Werkaufträge von z.B der Projektleitkarte oder Sammelaufträgen
                              werden auch in der Liste übergeben

\return Liste mit allen Werkaufträgen, die hierarchische Struktur der WaxPartliste ist dabei aufgehoben. 
*/
WerkauftragListe& WaxPartListe::getListeWerkauftraege(bool mitUnterListen,bool mitDummyWp)
{
	Werkauftrag *pWa = NULL;


	m_listWp.resetListe();

	MxCObListIterator i(*this);
	while (i.next())
	{
		WaxPart *waxPart = dynamic_cast<WaxPart*>(getAt(i));
		XASSERT(waxPart);
		if (!waxPart)
			continue;

		pWa = waxPart->get_werkauftrag();
		XASSERT(pWa);
		if (!pWa)
			continue;

		//prüfen ob Werkauftrag in Liste geschrieben werden soll
		if (!mitDummyWp)
		{
			if (pWa->get_waid() > 0)
				m_listWp.append(pWa);
		}
		else
			m_listWp.append(pWa);


		if (mitUnterListen)
		{
			//prüfen ob WaxPart noch Unterelemente hat
			WaxPartListe& listWp = waxPart->get_subobjects();
			if (!listWp.isEmpty())
			{
				//Werkaufträge von untergeordneten WaxParts holen
				WerkauftragListe& tmpListeWp = listWp.getListeWerkauftraege(mitUnterListen,mitDummyWp);

				if (!tmpListeWp.isEmpty())
					m_listWp.appendListe(tmpListeWp); //Werkaufträge an Liste hängen
			}
		}
	}

	return m_listWp;
}

/**
nur die Liste löschen. Die Elemente müssen extra mit delete gelöscht werden
*/

void WaxPartListe::resetListe()
{
	//nur Liste löschen
	RemoveAll();
}
