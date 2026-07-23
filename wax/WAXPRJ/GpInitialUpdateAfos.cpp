#include "stdafx.h"


#include "GpInitialUpdateAfos.h"


GpInitialUpdateAfos::GpInitialUpdateAfos(WaxPartListe& l) : waxpartListe(l), server(Server::save, NULL)
{
	//laengeListe = waxpartListe.getSize();
	//progress.set_maxWert(laengeListe);	
	//progress.set_ueberschrift("Afos in DB");

	///nächster Step,(braucht nicht Prozentual sein), muß im Range liegen
	///Rückgabewert ist wert in Prozent vom MaxWert
	//int nextStep(double wert);
}


/**
Generiert für jeden WA in liste die Arbeitsfolgen (Standard) und schreibt diese in die 
Datenbanktabelle 'arbeit'. 

Dieser Prozess läuft nur für neu eingelesene Werkaufträge, so dass in der NL die Arbeitsfolgen
nicht mehr an den Server übertragen werden müssen.
*/
/*
bool GpInitialUpdateAfos::write_afos()
{
	int anzahl = liste.getSize();
	progress.start();
	
	MxIterator iter(liste);
	while (iter.next())
	{			
		Werkauftrag* pWerkauftrag = dynamic_cast<Werkauftrag*>(liste.getAt(iter));
		if (!pWerkauftrag)
			continue;

		int prozent = progress.nextStep(iter.getIndex());
		string stMeldung = boost::str(boost::format("%d%%, Datensatz %d von %d, (WaID = %d)") % prozent % iter.getIndex() % anzahl % pWerkauftrag->get_waid());
		progress.setAnzeige1(stMeldung.c_str());
		
		pWerkauftrag->createAutoArbeit(true);
		server.set_werkauftrag(pWerkauftrag);
		if (!server.updateArbeit("arbeit", &pWerkauftrag->get_arbeit().get_arbeitsfolgen()))
		{
			progress.ende();
			return false;
		}
	}
	progress.ende();
	return true;
}
*/

bool GpInitialUpdateAfos::writeAfos()
{
	//Anzahl der Positionen in der WaxPartliste
	WerkauftragListe& listWa = waxpartListe.getListeWerkauftraege(true, false);
	laengeListe = listWa.getSize();

	progress.set_maxWert(laengeListe);	
	progress.set_ueberschrift("Afos in DB");


	progress.start();
	counter = 0;

	bool result = writeAfos(waxpartListe, true);
	
	progress.ende();
	
	return result;
}


bool GpInitialUpdateAfos::writeAfos(WaxPartListe& liste, bool createAfos)
{
	Werkauftrag *wa = NULL;

	MxCObListIterator i(liste);
	while (i.next())
	{
		WaxPart *waxPart = dynamic_cast<WaxPart*>(liste.getAt(i));
		XASSERT(waxPart);
		if (!waxPart)
			continue;

		SammelAuftrag *sammel = dynamic_cast<SammelAuftrag*>(waxPart);
		if (sammel)
		{
			sammel->get_werkauftrag()->get_arbeit().clean();
			sammel->createAutoArbeit();
			
			if(sammel->copyAfosToSammelauftrag(false))
			{
				if (!sammel->setAfosAndMoveToFirstPos())
					return false;
		
			}
			else
			{
				return false;
			}
			

			if (!writeAfos(sammel->get_subobjects(), false))
				return false;
		}
		else
		{
			//prüfen ob WaxPart noch Unterelemente hat (Bauabschnitt)
			WaxPartListe& listWp = waxPart->get_subobjects();
			if (!listWp.isEmpty())
			{
				if (!writeAfos(listWp, true))
					return false;
			}
			else
			{
				//Arbeitsgänge einer einzelnen Position erzeugen und in DB speichern
				wa = waxPart->get_werkauftrag();
				XASSERT(wa);
				if (!wa)
					return false;

				if (createAfos)
					wa->createAutoArbeit(true);

				if (!updateArbeit(wa))
					return false;
			}
		}
	}

	return true;
}


bool GpInitialUpdateAfos::updateArbeit(Werkauftrag *wa)
{
	counter++;
	int prozent = progress.nextStep(counter);
	string stMeldung = boost::str(boost::format("%d%%, Datensatz %d von %d, (WaID = %d)") % prozent % counter % laengeListe % wa->get_waid());
	progress.setAnzeige1(stMeldung.c_str());

	server.set_werkauftrag(wa);
	if (!server.updateArbeit("arbeit", &wa->get_arbeit().get_arbeitsfolgen()))
		return false;
		
	return true;
}