// Copyright (C) 2001 Minimax GmbH

#include "stdafx.h"


#include "VK3Berechnung.h"
#include "Werkauftrag.h"
#include "Rohrleitung.h"
#include "Handelslaenge.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//Destruktor
VK3Berechnung::~VK3Berechnung()
{

}

//Konstruktor
VK3Berechnung::VK3Berechnung()
{
	preisElelist = 0.0;
	preis = 0.0;
	preisPulvern = 0.0;
	preisPasslaenge = 0.0;
}

void VK3Berechnung::copy(const VK3Berechnung& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	preisElelist = other.preisElelist;
	preis = other.preis;
	preisPulvern = other.preisPulvern;
	preisPasslaenge = other.preisPasslaenge;
}

//Speichern und Laden der Preisdaten
void VK3Berechnung::serialize(BinaryArchive & ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	if (ar.is_storing())
	{
		ar << preis << preisElelist << preisPulvern << preisPasslaenge; 
	}
	else
	{
		if (version > 19)
		{
			ar >> preis >> preisElelist >> preisPulvern >> preisPasslaenge; 
		}
	}
}

void VK3Berechnung::calc_preis()
{
	calc_preisElelist();
	calc_preisPulvern();
	calc_preisPasslaenge();

	preis = preisElelist + preisPulvern + preisPasslaenge;
}

void VK3Berechnung::calc_preisPulvern()
{
	preisPulvern = 0.0;
	
	EnCObListVk3 vk3Liste;
	
	Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(werkauftrag);


	//XASSERT(rohr);
	if (!rohr)
		return;

	
	EnCObListVk3& elementList = rohr->get_pulverlaengenListe();

	//Liste kopieren, damit Originalliste nicht verändert wird
	vk3Liste.copy(elementList/*,true,true*/);

	//Alle Elemente des Sammelauftrages kumulieren und danach aufrunden
	vk3Liste.kumulieren();
	//vk3Liste.rundenVollMeter(); //Ab Version3.2 kein Runden mehr

	//Pulverpreis in Membervariable schreiben
	preisPulvern = vk3Liste.getGesamtpreis();

	//Speicher der Templiste freigeben
	vk3Liste.deleteAllElemente();
}

void VK3Berechnung::calc_preisPasslaenge()
{
	preisPasslaenge = 0.0;

	Rohrleitung *rohr = dynamic_cast<Rohrleitung *>(werkauftrag);
	//XASSERT(rohr);
	if (!rohr)
		return;

	// Preis berechnen
	for (POSITION pos = rohr->get_passlaengenListe().GetHeadPosition(); pos != NULL;)	
	{
		Element *ele = dynamic_cast<Element*>(rohr->get_passlaengenListe().GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;
	
		preisPasslaenge += ele->get_festPreis()*(double)ele->get_anzahl();
	}
}

void VK3Berechnung::calc_preisElelist()
{
	preisElelist = 0.0;
	
	Werkauftrag *rohr = dynamic_cast<Werkauftrag*>(werkauftrag);
	if (!rohr)
		return;	

	EnCObListVk3& elementList = rohr->get_vk3elementeListe();

	preisElelist = elementList.getGesamtpreis();
}

void VK3Berechnung::set_werkauftrag(Werkauftrag* value)
{
	werkauftrag = value;
}

const double VK3Berechnung::get_preisPulvern() const
{
	return preisPulvern;
}

void VK3Berechnung::set_preisPulvern(double value)
{
	preisPulvern = value;
}

const double VK3Berechnung::get_preis() const
{
	return preis;
}

void VK3Berechnung::set_preis(double value)
{
	preis = value;
}

const double VK3Berechnung::get_preisElelist() const
{
	return preisElelist;
}

void VK3Berechnung::set_preisElelist(double value)
{
	preisElelist = value;
}

const double VK3Berechnung::get_preisPasslaenge() const
{
	return preisPasslaenge;
}

void VK3Berechnung::set_preisPasslaenge(double value)
{
	preisPasslaenge = value;
}

