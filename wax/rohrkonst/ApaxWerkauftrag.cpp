#include "stdafx.h"
#include "resource.h"

	#include "ApaxWerkauftrag.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ApaxWerkauftrag::ApaxWerkauftrag()
{
	get_leitkarte().set_vk3(true); 
	stdbezeichnung = getStammdaten().getText("IDS_APAX_STDBEZEICHNUNG").c_str(); 
	vk3Preis = 0.0;
}



ApaxWerkauftrag::~ApaxWerkauftrag()
{}



void ApaxWerkauftrag::set_vk3Preis(double val)
{
	vk3Preis = val;
}



double ApaxWerkauftrag::get_vk3Preis()
{
	return vk3Preis;
}



//Speichern und Laden der Rohrleitung.
void ApaxWerkauftrag::serialize(MxFilesystem::BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	Werkauftrag::serialize(ar);
	if (ar.is_storing()) 
	{
		ar << vk3Preis;
	} 
	else 
	{
		if (version >= 45)
		{
			ar >> vk3Preis;
		}

		//Preislisten erzeugen
		createModell();
	}
}


void ApaxWerkauftrag::copy(ApaxWerkauftrag& other)
{
	Werkauftrag::copy(other);

	vk3Preis = other.vk3Preis;
}


void ApaxWerkauftrag::copy(const Werkauftrag& other)
{
	Werkauftrag::copy(other);
}

void ApaxWerkauftrag::createModell()
{
	createVk3Liste();
}

bool ApaxWerkauftrag::createVk3Liste()
{
	int vk3ApaxElementId = 99;

	// erstmal alte loeschen
	vk3elementeListe.deleteAllElemente();

	Element* ele = new Element;
	ele->set_id(vk3ApaxElementId);
	ele->set_festPreis(get_anzahl() * get_vk3Preis());

	vk3elementeListe.append(ele);

	get_preis().calc_gesamtpreisVK3();

	return true;
}

CString ApaxWerkauftrag::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString fehler;

	if (WaxSys::isAV())
	{
		if (get_vk3Preis() < 0.01)
			fehler = getStammdaten().getText("IDS_KEIN_PREIS").c_str(); 
		if (!fehler.IsEmpty())
			return fehler;

		if (get_anzahl() <= 0)
			fehler = getStammdaten().getText("IDS_KEINE_MENGE").c_str(); 
		if (!fehler.IsEmpty())
			return fehler;
	}
	return fehler;
}



bool ApaxWerkauftrag::isEmpty() const
{
	return false;
}

double ApaxWerkauftrag::getGesGewicht(int teile) const 
{
	UNREFERENCED_PARAMETER(teile);

	double gesGew = 0.0;
	

	for (POSITION p = get_material().get_matlist().GetHeadPosition(); p != NULL;)	
	{
		const Material *material = dynamic_cast<const Material*>(get_material().get_matlist().GetNext(p));
		XASSERT(material);
		if (!material)
			return false;

		double gew = material->get_gewicht() * material->get_anzahl();
		if (material->get_mass1() > 0)
			gew *= (material->get_mass1() / 1000);

		gesGew += gew;
	}


	return gesGew;
}



int ApaxWerkauftrag::getMaxLaenge()const
{
	int laenge = 0;

	for (POSITION p = get_material().get_matlist().GetHeadPosition(); p != NULL;)	
	{
		const Material *material = dynamic_cast<const Material*>(get_material().get_matlist().GetNext(p));
		XASSERT(material);
		if (!material)
			return false;

		if (material->get_mass1() > laenge)
			laenge = material->get_mass1();		
	}

	return laenge;
}