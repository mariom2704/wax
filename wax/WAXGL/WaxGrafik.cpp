#include "stdafx.h"


#include "ConstructGewinderohr.h"
#include "WaxGrafik.h"


using namespace WaxGrafik;


std::string Creator::create(Rohrleitung* rohr,  const std::string& grafik_file)
{
	if (!rohr)
		return "";
	
	Gewinderohr *grohr = dynamic_cast<Gewinderohr*>(rohr);
	
	string grafic_file;

	Rohrleitung* rtmp = NULL;

	if (rohr->is_native_cadpos())
	{
		if (rohr->isGewinderohr())
		{
			rtmp = new Gewinderohr();
		}
		else if (rohr->isStrangrohr())
		{
			rtmp = new Strangrohr();
		}
		else if (rohr->isVerteiler())
		{
			rtmp = new Verteiler();
		}
		if (rtmp)
		{
			rtmp->copy(*rohr);
			rtmp->createModell();
		}
	}

	if (rohr->is_native_cadpos() && !rtmp)
	{
		rohr->set_fertigungshinweis("ACHTUNG! FEHLER BEI DER WAX-UMWANDLUNG!!!");
	}

	if (rtmp)
	{
		if (rtmp->get_elementList().GetSize() != rohr->get_elementList().GetSize())
			rtmp->set_fertigungshinweis("ACHTUNG! UMWANDLUNGSFEHLER");
	}
		
	if (grohr || rohr->isGewinderohr())
	{
		ConstructGewinderohr construct(rtmp ? rtmp : rohr, WaxGrafik::SVG, grafik_file);
		construct.createGraphik();
		grafic_file = construct.get_filename();
	}
	else
	{
		ConstructGraphik construct(rtmp ? rtmp : rohr, WaxGrafik::SVG, grafik_file);
		construct.createGraphik();
		grafic_file = construct.get_filename();
	}

	delete rtmp;
	
	return grafic_file;
}
