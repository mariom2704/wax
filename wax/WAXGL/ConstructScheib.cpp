#include "stdafx.h"


#include "kreis.h"
#include "constructscheib.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructScheib::~ConstructScheib()
{

}

ConstructScheib::ConstructScheib()
{
	
}

void ConstructScheib::createBeschriftung()
{
	switch (typ) 
	{
	case IMROHR:
		text = "SCH";
		break;
	case ABG:
		text = "+SCH";
		break;
	}
	ConstructEndEle::createBeschriftung();
}

void ConstructScheib::createElement()
{
	ConstructEndEle::createElement();

	double f1 = faktorRohrlaenge;
	double x = 0, y = 0;
	int start = 0, end = 0, durchmesser = 0;

	switch (typ) 
	{
	case IMROHR:
		durchmesser = (int)sqrt((double)(2 * dn * dn));
		elemerk.set_winkel(drehfaktor);
		// Scheibe links
		if (erstesElement) { 
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + dn/2;
			y     = Y0 + dn/2; 
		// Scheibe links
		} else if (winkel == 180) { 
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 - elemerk.get_abstAbs() + dn/2;
			y     = Y0 + dn/2; 
		// Scheibe oben 
		} else if (winkel == 90) { 
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + dn/2;
			y     = Y0 + elemerk.get_abstAbs() - dn/2; 	
		// Scheibe unten
		} else if (winkel == 270) { 
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + dn/2;
			y     = Y0 - elemerk.get_abstAbs() + dn/2; 	
		// Scheibe rechts
		} else if (winkel == 0) { 
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + (double)elemerk.get_abstAbs()/f1 - dn/2;
			y     = Y0 + dn/2;
		}
		break;

	case ABG:
		durchmesser = (int)sqrt((double)(2 * dn * dn));

		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) {
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 - merkerRohrele.get_laenge() + dn/2;
			y     = Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge; 	
		// rechts und Rohr nach oben
		} else if (winkel == 180 && drehfaktor == 90) {
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/2;
			y     = Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge; 	
		// rechts und Rohr nach unten
		} else if (winkel ==   0 && drehfaktor == 270) {
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/2;
			y     = Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge; 	
		// links und Rohr nach unten
		} else if (winkel == 180 && drehfaktor == 270) {
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 - merkerRohrele.get_laenge() + dn/2;
			y     = Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge; 	
		// oben und Rohr nach links
		} else if (winkel == 180 && drehfaktor == 180) {
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge;
			y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/2; 	
		// unten und Rohr nach links
		} else if (winkel == 0 && drehfaktor == 180) {
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge;
			y     = Y0 - merkerRohrele.get_laenge() + dn/2; 	
		// oben und Rohr nach rechts
		} else if (winkel == 0 && drehfaktor == 0) {
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/2; 	
		// unten und Rohr nach rechts
		} else if (winkel == 180 && drehfaktor == 0) {
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 - merkerRohrele.get_laenge() + dn/2; 	
		}
		break;
	}
	add_circle(x, y, durchmesser, start, end);
}

