#include "stdafx.h"


#include "constructfflan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructFflan::~ConstructFflan()
{

}

ConstructFflan::ConstructFflan()
{

}

void ConstructFflan::createBeschriftung()
{
	switch (typ) 
	{
	case IMROHR:
		if (elemerk.get_id() == FFL) 
			text = "FFL";
		else if (elemerk.get_id() == SFL)
			text = "SFL";
		else if (elemerk.get_id() == FFL16)
			text = "FFL16";
		else if (elemerk.get_id() == FFL10)
			text = "FFL10";
		break;
	case ABG:
		if (elemerk.get_id() == FFL)	
			text = "/FFL";
		else if (elemerk.get_id() == SFL)
			text = "/SFL";
		else if (elemerk.get_id() == FFL16)
			text = "/FFL16";
		else if (elemerk.get_id() == FFL10)
			text = "/FFL10";
		break;
	}
	ConstructEndEle::createBeschriftung();
}

void ConstructFflan::createElement()
{
	ConstructEndEle::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	double x = 0, y = 0;
	double dx = 0, dy = 0;

	switch (typ) 
	{
	case IMROHR:
		// Flansch links
		if (erstesElement) 
		{ 
			x  = X0 - elemerk.get_laenge();
			y  = Y0 - 10;  
			dx = elemerk.get_laenge();
			dy = dn+2*10;		
		} 
		// Flansch links
		else if (winkel == 180) 
		{ 
			x  = X0 - elemerk.get_abstAbs()/f1 - elemerk.get_laenge();
			y  = Y0 - 10;  
			dx = elemerk.get_laenge();
			dy = dn+2*10;		
		} 
		// Flansch oben 
		else if (winkel == 90) 
		{ 
			x  = X0 - 10;
			y  = Y0 + elemerk.get_abstAbs()/f1;
			dx = dn+2*10;
			dy = elemerk.get_laenge();
		} 
		// Flansch unten
		else if (winkel == 270) 
		{ 
			x  = X0 - 10;
			y  = Y0 - elemerk.get_abstAbs()/f1;
			dx = dn+2*10;
			dy = -elemerk.get_laenge();
		} 
		// Flansch rechts	
		else if (winkel == 0) 
		{ 
			x  = X0 + (double)elemerk.get_abstAbs()/f1;
			y  = Y0 - 10;
			dx = elemerk.get_laenge();
			dy = dn+2*10;
		}
		add_rectangle(x, y, dx, dy, color);
		break;

	case ABG:
	
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) 
		{
			x     = X0 - merkerRohrele.get_laenge();
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 - 10; 	
			dx    = -elemerk.get_laenge();
			dy    = dn + 2*10;
		} 
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90) 
		{
			x     = X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge();
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 - 10; 	
			dx    = +elemerk.get_laenge();
			dy    = dn + 2*10;
		} 
		// rechts und Rohr nach unten
		else if (winkel ==   0 && drehfaktor == 270) 
		{
			x     = X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge();
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 - 10; 	
			dx    = +elemerk.get_laenge();
			dy    = dn + 2*10;
		} 
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270)
		{
			x     = X0 - merkerRohrele.get_laenge();
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 - 10; 	
			dx    = -elemerk.get_laenge();
			dy    = dn + 2*10;
		} 
		// oben und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180) 
		{ 
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 - 10;
			y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(); 	
			dx    = dn + 2*10;
			dy    = elemerk.get_laenge();
		} 
		// unten und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180)
		{
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 - 10;
			y     = Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge(); 	
			dx    = dn + 2*10;
			dy    = elemerk.get_laenge();
		}
		// oben und Rohr nach rechts 
		else if (winkel == 0 && drehfaktor == 0) 
		{ 
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 - 10;
			y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(); 	
			dx    = dn + 2*10;
			dy    = elemerk.get_laenge();
		} 
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0)
		{
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 - 10;
			y     = Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge(); 	
			dx    = dn + 2*10;
			dy    = elemerk.get_laenge();
		}
		add_rectangle(x, y, dx, dy, color);
		break;
	}
}

