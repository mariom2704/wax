#include "stdafx.h"


#include "constructgewind.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructGewind::~ConstructGewind()
{

}

ConstructGewind::ConstructGewind()
{

}

void ConstructGewind::createBeschriftung()
{
	BauelementGruppen bg;

	switch (typ) 
	{
	case IMROHR:
		text = bg.getKurzbezeichnung(GEW);
		break;
	case ABG:
		text = "+";
		text += bg.getKurzbezeichnung(GEW);
		break;
	}
	ConstructEndEle::createBeschriftung();
}

void ConstructGewind::createElement()
{
	ConstructEndEle::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;

	switch (typ) 
	{
	case IMROHR:
		// Gewinde links
		if (erstesElement) 
		{ 
			add_line(X0 + elemerk.get_abstAbs()/f1,						 Y0, 
						   X0 + elemerk.get_abstAbs()/f1 +   dn/8, Y0 + dn, 1, color);
			add_line(X0 + elemerk.get_abstAbs()/f1 +   dn/8, Y0, 
						   X0 + elemerk.get_abstAbs()/f1 + 2*dn/8, Y0 + dn, 1, color);
			add_line(X0 + elemerk.get_abstAbs()/f1 + 2*dn/8, Y0, 
						   X0 + elemerk.get_abstAbs()/f1 + 3*dn/8, Y0 + dn, 1, color);
		} 
		// Gewinde links
		else if (winkel == 180) 
		{ 
			add_line(X0 - elemerk.get_abstAbs(),						  Y0, 
						   X0 - elemerk.get_abstAbs() +   dn/8, Y0 + dn, 1, color);
			add_line(X0 - elemerk.get_abstAbs() +   dn/8, Y0, 
						   X0 - elemerk.get_abstAbs() + 2*dn/8, Y0 + dn, 1, color);
			add_line(X0 - elemerk.get_abstAbs() + 2*dn/8, Y0, 
						   X0 - elemerk.get_abstAbs() + 3*dn/8, Y0 + dn, 1, color);
		}
		// Gewinde oben
		else if (winkel == 90) 
		{ 
			add_line(X0,						Y0 + elemerk.get_abstAbs(), 
						   X0 + dn,	Y0 + elemerk.get_abstAbs() -   dn/8, 1, color);
			add_line(X0,						Y0 + elemerk.get_abstAbs() -   dn/8, 
						   X0 + dn,	Y0 + elemerk.get_abstAbs() - 2*dn/8, 1, color);
			add_line(X0,						Y0 + elemerk.get_abstAbs() - 2*dn/8, 
						   X0 + dn,	Y0 + elemerk.get_abstAbs() - 3*dn/8, 1, color);
		} 
		// Gewinde unten
		else if (winkel == 270)
		{ 
			add_line(X0,						Y0 - elemerk.get_abstAbs(), 
						   X0 + dn,	Y0 - elemerk.get_abstAbs() +   dn/8, 1, color);
			add_line(X0,						Y0 - elemerk.get_abstAbs() +   dn/8, 
						   X0 + dn,	Y0 - elemerk.get_abstAbs() + 2*dn/8, 1, color);
			add_line(X0,						Y0 - elemerk.get_abstAbs() + 2*dn/8, 
						   X0 + dn,	Y0 - elemerk.get_abstAbs() + 3*dn/8, 1, color);
		} 
		// Gewinde rechts
		else if (winkel == 0)
		{ 
			add_line(X0 + (double)elemerk.get_abstAbs()/f1,						Y0, 
						   X0 + (double)elemerk.get_abstAbs()/f1 -   dn/8, Y0 + dn,	1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 -   dn/8, Y0, 
						   X0 + (double)elemerk.get_abstAbs()/f1 - 2*dn/8, Y0 + dn,	1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - 2*dn/8, Y0, 
						   X0 + (double)elemerk.get_abstAbs()/f1 - 3*dn/8, Y0 + dn,	1, color);
		}
		break;

	case ABG:
	
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) 
		{
			add_line(X0 - merkerRohrele.get_laenge(),							Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() +   dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 - merkerRohrele.get_laenge() +   dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + 2*dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 - merkerRohrele.get_laenge() + 2*dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + 3*dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
						   1, color);
		} 
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90)
		{
			add_line(X0 + andn + merkerRohrele.get_laenge(),							Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() -   dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 + andn + merkerRohrele.get_laenge() -   dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - 2*dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 + andn + merkerRohrele.get_laenge() - 2*dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - 3*dn/8,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
						   1, color);
		} 
		// rechts und Rohr nach unten
		else if (winkel ==   0 && drehfaktor == 270) 
		{
			add_line(X0 + andn + merkerRohrele.get_laenge(),							Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() -   dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 + andn + merkerRohrele.get_laenge() -   dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - 2*dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 + andn + merkerRohrele.get_laenge() - 2*dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - 3*dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
						   1, color);
		} 
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270) 
		{
			add_line(X0 - merkerRohrele.get_laenge(),							Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() +   dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 - merkerRohrele.get_laenge() +   dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + 2*dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
						   1, color);
			add_line(X0 - merkerRohrele.get_laenge() + 2*dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + 3*dn/8,	Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
						   1, color);
		} 
		// unten und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180) 
		{
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge(), 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() +   dn/8,
						   1, color);
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() +   dn/8, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() + 2*dn/8,
						   1, color);
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() + 2*dn/8, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() + 3*dn/8,
						   1, color);
		} 
		// oben	 und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180) 
		{
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 + andn + merkerRohrele.get_laenge(), 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 + andn + merkerRohrele.get_laenge() -   dn/8,
						   1, color);
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 + andn + merkerRohrele.get_laenge() -   dn/8, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 + andn + merkerRohrele.get_laenge() - 2*dn/8,
						   1, color);
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 + andn + merkerRohrele.get_laenge() - 2*dn/8, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 + andn + merkerRohrele.get_laenge() - 3*dn/8,
						   1, color);
		} 
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0)
		{
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() +   dn/8,
						   1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() +   dn/8, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() + 2*dn/8,
						   1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() + 2*dn/8, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() + 3*dn/8,
						   1, color);
		}
		// oben	 und Rohr nach rechts
		else if (winkel == 0 && drehfaktor == 0) 
		{
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() -   dn/8,
						   1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() -   dn/8, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - 2*dn/8,
						   1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - 2*dn/8, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - 3*dn/8,
						   1, color);
		}
		break;
	}
}

