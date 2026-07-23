// Copyright (C) 2001 Minimax GmbH

// Include files
#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ConstructSatst::~ConstructSatst()
{

}

ConstructSatst::ConstructSatst()
{

}

void ConstructSatst::createBeschriftung()
{
	if (winkel == 0 || winkel == 180)
	{
		if (!dn2zoll)
			text.Format("SAT DN%d", elemerk.get_dn());
		else
			text.Format("SAT %s''", WaxStamm::dn2zoll(elemerk.get_dn()));
	}	
	else 
	{
		if (!dn2zoll)
			text.Format("SAT DN%d %d°", elemerk.get_dn(), elemerk.get_winkel());
		else
			text.Format("SAT %s'' %d", WaxStamm::dn2zoll(elemerk.get_dn()), elemerk.get_winkel());
	}	

	ConstructAbgEle::createBeschriftung();
}

void ConstructSatst::createElement()
{
	ConstructAbgEle::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	// Punkte
	double x  = 0, y  = 0;
	double dx = 0, dy = 0;
	// Kreis
	int durchmesser = 0, start = 0, end = 0;


	switch (typ) 
	{
	case ABG:
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) {
			// linke Linie 
			add_line(	X0 - elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2, 
							X0 - elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2, 
							1, color);
			// untere Linie 
			add_line(	X0 - elemerk.get_laenge()/2,	Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2, 
							X0 - elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2, 
							1, color);
			// obere Linie 
			add_line(	X0 - elemerk.get_laenge()/2,	Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2, 
							X0 - elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2, 
							1, color);
			// unterer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_360;
			end   = Kreis::GRAD_90;
			x     = X0 - elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// oberer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_360;
			x     = X0 - elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 - elemerk.get_dn()/2 - elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0, Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2, 
						   X0, Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		// rechts und Rohr nach oben
		} else if (winkel == 180 && drehfaktor == 90) {
			// linke Linie 
			add_line(	X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2, 
							X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2, 
							1, color);
			// untere Linie 
			add_line(	X0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2,		Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2, 
							X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2, 
							1, color);
			// obere Linie 
			add_line(	X0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2,		Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2, 
							X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2, 
							1, color);
			// unterer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_90;
			end   = Kreis::GRAD_180;
			x     = X0 + elemerk.get_anschlussDN()  + elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// oberer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_180;
			end   = Kreis::GRAD_270;
			x     = X0 + elemerk.get_anschlussDN()  + elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + elemerk.get_anschlussDN() + elemerk.get_dn()/2 + elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 + elemerk.get_anschlussDN() , Y0 + elemerk.get_abstAbs() - laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2, 
						   X0 + elemerk.get_anschlussDN() , Y0 + elemerk.get_abstAbs() - laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		// hinten/vorne und Rohr nach oben
		} else if ((winkel == 90 || winkel == 270) && drehfaktor == 90) {
			durchmesser = elemerk.get_dn();
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + elemerk.get_anschlussDN()/2;
			y     = Y0 + elemerk.get_abstAbs() - laenge; 	
			add_circle(x, y, durchmesser, start, end);
		// hinten/vorne und Rohr nach unten
		} else if ((winkel == 90 || winkel == 270) && drehfaktor == 270) {
			durchmesser = elemerk.get_dn();
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + elemerk.get_anschlussDN()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge; 	
			add_circle(x, y, durchmesser, start, end);
		// rechts und Rohr nach unten
		} else if (winkel ==   0 && drehfaktor == 270) {
			// linke Linie 
			add_line(	X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, 
							X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, 
							1, color);
			// untere Linie 
			add_line(	X0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2,		Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, 
							X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, 
							1, color);
			// obere Linie 
			add_line(	X0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2,		Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, 
							X0 + elemerk.get_anschlussDN() + elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, 
							1, color);
			// unterer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_90;
			end   = Kreis::GRAD_180;
			x     = X0 + elemerk.get_anschlussDN()  + elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// oberer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_180;
			end   = Kreis::GRAD_270;
			x     = X0 + elemerk.get_anschlussDN()  + elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + elemerk.get_anschlussDN() + elemerk.get_dn()/2 + elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 + elemerk.get_anschlussDN() , Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2, 
						   X0 + elemerk.get_anschlussDN() , Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		// links und Rohr nach unten
		} else if (winkel == 180 && drehfaktor == 270) {
			// linke Linie 
			add_line(	X0 - elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, 
							X0 - elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, 
							1, color);
			// untere Linie 
			add_line(	X0 - elemerk.get_laenge()/2,	Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, 
							X0 - elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, 
							1, color);
			// obere Linie 
			add_line(	X0 - elemerk.get_laenge()/2,	Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, 
							X0 - elemerk.get_laenge(),		Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, 
							1, color);
			// unterer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_360;
			end   = Kreis::GRAD_90;
			x     = X0 - elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// oberer Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_360;
			x     = X0 - elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 - elemerk.get_dn()/2 - elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_abstAbs() + laenge; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0, Y0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2, 
						   X0, Y0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2, 
						   1, color);
			color = GlObj::BLACK;
		// unten und Rohr nach links
		} else if (winkel == 0 && drehfaktor == 180) { 
			// untere Linie
			add_line(X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   1, color);
			// linke Linie
			add_line(X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, Y0 - elemerk.get_laenge()/2, 
						   X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   1, color);
			// rechte Linie
			add_line(X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, Y0 - elemerk.get_laenge()/2, 
						   X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   1, color);
			// linker Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_90;
			x     = X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// rechter Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_90;
			end   = Kreis::GRAD_180;
			x     = X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 - elemerk.get_abstAbs() + laenge;
			y     = Y0 - elemerk.get_dn()/2 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2, Y0, 
						   X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2, Y0, 
						   1, color);
			color = GlObj::BLACK;
		// oben und Rohr nach links
		} else if (winkel == 180 && drehfaktor == 180) { 
			// obere Linie 
			add_line(X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   1, color);
			// linke Linie 
			add_line(X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2, 
						   X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   1, color);
			// rechte Linie 
			add_line(X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2, 
						   X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   1, color);
			// linker Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_360;
			x     = X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// rechter Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_180;
			end   = Kreis::GRAD_270;
			x     = X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 - elemerk.get_abstAbs() + laenge;
			y     = Y0 + elemerk.get_anschlussDN() + elemerk.get_dn()/2 + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 - elemerk.get_abstAbs() + laenge - elemerk.get_dn()/2 - elemerk.get_laenge()/2, Y0 + elemerk.get_anschlussDN(), 
						   X0 - elemerk.get_abstAbs() + laenge + elemerk.get_dn()/2 + elemerk.get_laenge()/2, Y0 + elemerk.get_anschlussDN(), 
						   1, color);
			color = GlObj::BLACK;
		// hinten/vorne und Rohr nach links
		} else if ((winkel == 90 || winkel == 270) && drehfaktor == 180) {  
			durchmesser = elemerk.get_dn();
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 - elemerk.get_abstAbs() + laenge;
			y     = Y0 + elemerk.get_anschlussDN()/2; 	
			add_circle(x, y, durchmesser, start, end);
		// unten und Rohr nach rechts
		} else if (winkel == 180 && drehfaktor == 0) { 
			// untere Linie
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   1, color);
			// linke Linie
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2, Y0 - elemerk.get_laenge()/2, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   1, color);
			// rechte Linie
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2, Y0 - elemerk.get_laenge()/2, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2, Y0 - elemerk.get_laenge(), 
						   1, color);
			// linker Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_90;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2 - elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// rechter Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_90;
			end   = Kreis::GRAD_180;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2 + elemerk.get_laenge()/2;
			y     = Y0 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 - elemerk.get_dn()/2 - elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2 - elemerk.get_laenge()/2, Y0, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2 + elemerk.get_laenge()/2, Y0, 
						   1, color);
			color = GlObj::BLACK;
		// oben und Rohr nach rechts
		} else if (winkel == 0 && drehfaktor == 0) { 
			// obere Linie 
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   1, color);
			// linke Linie 
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   1, color);
			// rechte Linie 
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2, Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge(), 
						   1, color);
			// linker Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_360;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2 - elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// rechter Bogen
			durchmesser = elemerk.get_laenge();
			start = Kreis::GRAD_180;
			end   = Kreis::GRAD_270;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2 + elemerk.get_laenge()/2;
			y     = Y0 + elemerk.get_anschlussDN() + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrausschnitt
			durchmesser = (int)sqrt((double)(2 * (elemerk.get_dn() + elemerk.get_laenge()) * (elemerk.get_dn() + elemerk.get_laenge())));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 + elemerk.get_anschlussDN() + elemerk.get_dn()/2 + elemerk.get_laenge()/2; 	
			add_circle(x, y, durchmesser, start, end);
			// Rohrlinie weiss
			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - elemerk.get_dn()/2 - elemerk.get_laenge()/2, Y0 + elemerk.get_anschlussDN(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + elemerk.get_dn()/2 + elemerk.get_laenge()/2, Y0 + elemerk.get_anschlussDN(), 
						   1, color);
			color = GlObj::BLACK;
		// hinten/vorne und Rohr nach rechts
		} else if ((winkel == 90 || winkel == 270) && drehfaktor == 0) {  
			durchmesser = elemerk.get_dn();
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 + elemerk.get_anschlussDN()/2; 	
			add_circle(x, y, durchmesser, start, end);
		}
		break;
	}
}

