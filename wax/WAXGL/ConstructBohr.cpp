#include "stdafx.h"


#include "constructbohr.h"
#include "kreis.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructBohr::~ConstructBohr()
{
}

ConstructBohr::ConstructBohr()
{
}


void ConstructBohr::createBeschriftung()
{
	CString str;

	int d = (int)(elemerk.get_durchmesser() * 10.0);
	
	if (d%10>0)
		str.Format("Ø=%.1fmm", elemerk.get_durchmesser());
	else
	{
		if (elemerk.get_durchmesser() == 0)
			str = "Ø=50mm";
		else 
			str.Format("Ø=%dmm", (int)elemerk.get_durchmesser());
	}

	if (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180)
		text = str;
	else 
		text.Format("%s %d'", str, elemerk.get_winkel());

	ConstructAbgEle::createBeschriftung();
}

void ConstructBohr::createElement()
{
	ConstructAbgEle::createElement();
	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	// Punkte
	double x  = 0, y  = 0;
	// Kreis
	int durchmesser = 0, start = 0, end = 0;

	if (elemerk.get_id() == BOHR24)
		dn = 25;
	else 
		dn = 50;

	if (dn > andn)
		dn = andn;

	switch (typ) 
	{
	case ABG:
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) 
		{
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (dn + elemerk.get_laenge()) * (dn + elemerk.get_laenge())));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 - dn/2 - elemerk.get_laenge()/2;
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0, Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 - elemerk.get_laenge()/2, 
						   X0, Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		} 
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90)
		{
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (dn + elemerk.get_laenge()) * (dn + elemerk.get_laenge())));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + andn + dn/2 + elemerk.get_laenge()/2;
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 + andn , Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 - elemerk.get_laenge()/2, 
						   X0 + andn , Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		} 
		// hinten/vorne und Rohr nach oben
		else if ((winkel == 90 || winkel == 270) && drehfaktor == 90) 
		{
			durchmesser = dn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + andn/2;
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end, winkel == 90 ? true : false);
		} 
		// hinten/vorne und Rohr nach unten
		else if ((winkel == 90 || winkel == 270) && drehfaktor == 270) 
		{
			durchmesser = dn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + andn/2;
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end, winkel == 90 ? true : false);
		} 
		// rechts und Rohr nach unten
		else if (winkel ==   0 && drehfaktor == 270) 
		{
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (dn + elemerk.get_laenge()) * (dn + elemerk.get_laenge())));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + andn + dn/2 + elemerk.get_laenge()/2;
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 + andn , Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 - elemerk.get_laenge()/2, 
						   X0 + andn , Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		}
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270)
		{
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (dn + elemerk.get_laenge()) * (dn + elemerk.get_laenge())));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 - dn/2 - elemerk.get_laenge()/2;
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0, Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 - elemerk.get_laenge()/2, 
						   X0, Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;

		} 
		// unten und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180)
		{ 
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1;
			y     = Y0 - dn/2; 	
			add_circle(x, y, durchmesser, start, end);

			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0, 
						   1, color);
			color = GlObj::BLACK;
		} 
		// oben und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180)
		{
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1;
			y     = Y0 + andn + dn/2; 	
			add_circle(x, y, durchmesser, start, end);

			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 + andn, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 + andn, 
						   1, color);
			color = GlObj::BLACK;
		} 
		// hinten/vorne und Rohr nach links
		else if ((winkel == 90 || winkel == 270) && drehfaktor == 180)
		{
			durchmesser = dn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1;
			y     = Y0 + andn/2; 	
			add_circle(x, y, durchmesser, start, end, winkel == 90 ? true : false);
		} 
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0)
		{ 
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
			y     = Y0 - dn/2; 	
			add_circle(x, y, durchmesser, start, end);

			color = GlObj::UNBEKANNT;
			add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2, Y0, 
						   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2, Y0, 
						   1, color);
			color = GlObj::BLACK;
		} 
		// oben und Rohr nach rechts
		else if (winkel == 0 && drehfaktor == 0) 
		{
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
			y     = Y0 + andn + dn/2; 	
			add_circle(x, y, durchmesser, start, end);

			color = GlObj::UNBEKANNT;
			add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2, Y0 + andn, 
						   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2, Y0 + andn, 
						   1, color);
			color = GlObj::BLACK;
		} 
		// hinten/vorne und Rohr nach rechts
		else if ((winkel == 90 || winkel == 270) && drehfaktor == 0)
		{
			durchmesser = dn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
			y     = Y0 + andn/2; 	
			//add_circle(x, y, durchmesser, start, end);
			add_circle(x, y, durchmesser, start, end, winkel == 90 ? true : false);
		}
		break;
	}
}

