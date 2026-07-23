#include "stdafx.h"


#include "constructkbod.h"
#include "kreis.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructKbod::~ConstructKbod()
{

}

ConstructKbod::ConstructKbod()
{

}

void ConstructKbod::createBeschriftung()
{
	switch (typ) 
	{
	case IMROHR:
		if (elemerk.get_id() == KB40) 
			text = "KB40";
		else 
			text = "KB50";

		break;
	case ABG:
		if (elemerk.get_id() == KB40) 
			text = "+KB40";
		else 
			text = "+KB50";
		break;
	}
	ConstructEndEle::createBeschriftung();
}

void ConstructKbod::createElement()
{
	ConstructEndEle::createElement();

	double f1 = faktorRohrlaenge;
	// Farbe
	GlObj::colour color = GlObj::BLACK;
	// Punkte
	double dx = 0, dy = 0;
	double x = 0, y = 0;
	// Kreis
	int durchmesser = 0, start = 0, end = 0;

	switch (typ) 
	{
	case IMROHR:
		// Spülanschluss links
		if (erstesElement) { 
			add_line(X0,					Y0, 
						   X0 - elemerk.get_laenge(),		Y0,			1, color);
			add_line(X0,					Y0 + andn, 
						   X0 - elemerk.get_laenge(),		Y0 + andn, 1, color);
			
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + dn/2 - elemerk.get_laenge();
			y     = Y0 + dn/2; 
			add_circle(x, y, durchmesser, start, end);
			
			x     = X0 - elemerk.get_laenge();
			y     = Y0;
			dx    = -60;
			if (elemerk.get_id() == KB40)
				dy = 40;
			else 
				dy = 50; 
			add_rectangle(x, y, dx, dy, color);
			
		// Spülanschluss links
		} else if (winkel == 180) { 
			add_line(X0 - elemerk.get_abstAbs(),				Y0, 
						   X0 - elemerk.get_abstAbs() - elemerk.get_laenge(), Y0,				1, color);
			add_line(X0 - elemerk.get_abstAbs(),				Y0 + andn, 
						   X0 - elemerk.get_abstAbs() - elemerk.get_laenge(), Y0 + andn,	1, color);
			
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 - elemerk.get_abstAbs() + dn/2 - elemerk.get_laenge();
			y     = Y0 + dn/2; 
			add_circle(x, y, durchmesser, start, end);
			
			x     = X0 - elemerk.get_abstAbs() - elemerk.get_laenge();
			y     = Y0;
			dx    = -60;
			if (elemerk.get_id() == KB40)
				dy = 40;
			else 
				dy = 50; 
			add_rectangle(x, y, dx, dy, color);

		// Spülanschluss oben
		} else if (winkel == 90) { 
			add_line(X0,			Y0 + elemerk.get_abstAbs(), 
						   X0,			Y0 + elemerk.get_abstAbs() + elemerk.get_laenge(),	1, color);
			add_line(X0 + dn,	Y0 + elemerk.get_abstAbs(), 
						   X0 + dn,	Y0 + elemerk.get_abstAbs() + elemerk.get_laenge(),	1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + dn/2;
			y     = Y0 + elemerk.get_abstAbs() - dn/2 + elemerk.get_laenge(); 	
			add_circle(x, y, durchmesser, start, end);
	
			x     = X0;
			y     = Y0 + elemerk.get_abstAbs() + elemerk.get_laenge();
			dy    = 60;
			if (elemerk.get_id() == KB40)
				dx = 40;
			else 
				dx = 50; 
			add_rectangle(x, y, dx, dy, color);

		// Spülanschluss unten
		} else if (winkel == 270) { 
			add_line(X0,			Y0  -elemerk.get_abstAbs(), 
						   X0,			Y0 - elemerk.get_abstAbs() - elemerk.get_laenge(),	1, color);
			add_line(X0 + dn,	Y0 - elemerk.get_abstAbs(), 
						   X0 + dn,	Y0 - elemerk.get_abstAbs() - elemerk.get_laenge(),	1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + dn/2;
			y     = Y0 - elemerk.get_abstAbs() + dn/2 - elemerk.get_laenge() ; 	
			add_circle(x, y, durchmesser, start, end);

			x     = X0;
			y     = Y0 - elemerk.get_abstAbs() - elemerk.get_laenge();
			dy    = -60;
			if (elemerk.get_id() == KB40)
				dx = 40;
			else 
				dx = 50; 
			add_rectangle(x, y, dx, dy, color);

		// Spülanschluss rechts
		} else if (winkel == 0) { 
			add_line(	X0 + (double)elemerk.get_abstAbs()/f1,             Y0, 
							X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge(), Y0,			1, color);
			add_line(	X0 + (double)elemerk.get_abstAbs()/f1,             Y0 + andn, 
							X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge(), Y0 + andn,	1, color);
			
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge() - dn/2 ;
			y     = Y0 + dn/2;
			add_circle(x, y, durchmesser, start, end);

			x     = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge();
			y     = Y0;
			dx    = 60;
			if (elemerk.get_id() == KB40)
				dy = 40;
			else 
				dy = 50; 
			add_rectangle(x, y, dx, dy, color);
		}
		break;
	}
}

