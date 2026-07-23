
#include "stdafx.h"


	#include "GpVk3Preis.h"



GpVk3Preis::GpVk3Preis(void)
{
}



GpVk3Preis::~GpVk3Preis(void)
{
}



double GpVk3Preis::getVk3Preis(WaxPart *ptHead)
{
	double preis = 0.0;

	if (!ptHead || ptHead->get_deleted())
		return 0.0;

	if (ptHead->get_werkauftrag() && 
	   (ptHead->get_werkauftrag()->isRohr() || ptHead->get_werkauftrag()->isApax()))
	{
		ptHead->get_werkauftrag()->createModell();	
		preis += ptHead->get_werkauftrag()->get_preis().get_gesamtpreisVK3();
	}
	else if (dynamic_cast<SammelWerkauftrag*>(ptHead->get_werkauftrag()))
	{
		
		SammelAuftrag *sammel = dynamic_cast<SammelAuftrag*>(ptHead);
		if (sammel)
			sammel->copyAfosToSammelauftrag(true);
	}



	for (POSITION pos = ptHead->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart* next = dynamic_cast<WaxPart*>(ptHead->get_subobjects().GetNext(pos));
		if (next && next->get_werkauftrag()  && 
	       (next->get_werkauftrag()->isRohr() || next->get_werkauftrag()->isApax()))
		{
			if (next->get_deleted())
				continue;

			next->get_werkauftrag()->createModell();
			next->get_werkauftrag()->get_preis().calc_gesamtpreisVK3();

			preis += next->get_werkauftrag()->get_preis().get_gesamtpreisVK3();

		}

		//Wenn Ordner Unterordner enthält, von diesen auch den Vk3 Preis einlesen
		if (next->get_subobject_count() > 0)
		{
			preis += getVk3Preis(next);
		}
	}

	return preis;
}




