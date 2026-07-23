#include "stdafx.h"


#include "kreis.h"
#include "constructrohr.h"
#include "constructrohrnippel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructRohrNippel::~ConstructRohrNippel()
{

}

ConstructRohrNippel::ConstructRohrNippel()
{
}

void ConstructRohrNippel::createBeschriftung()
{
	CString str, id;
		
	if (!dn2zoll)
	{
		str.Format("%s %d", id, elemerk.get_dn());
	}
	else
	{
		str.Format("%s %s''", id, WaxStamm::dn2zoll(elemerk.get_dn()));
	}
	
	if (elemerk.get_id() == ROHRNIPPEL_COII)
	{
		str = _T("G 2 3/4");
		strArtikelnummer.Format("(ANr %d)", elemerk.get_artikelnr());
	}
	
	if (elemerk.get_winkel() != 0 && elemerk.get_winkel() != 180)
	{
		text.Format("%s %d'", str, elemerk.get_winkel());
	}
	else
	{
		text.Format("%s", str);
	}
	

	GlObj::colour color = GlObj::BLACK;
	
	int ebene = 0, textwinkel = 0;
	double f1 = faktorRohrlaenge;
	

	int ebene3= maxRohrDN + maxlaengeAbgOben + 140; 
	//Wenn Abgaenge zu dicht nebeneinander wird Text verschoben
	//Achtung: createBemassung muss fuer bemassungHochgestellt vorher aufgerufen werden!!!
	int tiefer = 0;
	if (bemassungHochgestellt && elemerk.get_winkel() == merkerAbgele.get_winkel()) 
		tiefer = 50;
	
	textwinkel = 45;
	ebene = ebene3 - tiefer;
	add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1-30, 
					  Y0 + ebene, 
					  text, color, false, textwinkel);
	if(!strArtikelnummer.IsEmpty())
	{
		add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1+45, 
					  Y0 + ebene, 
					  strArtikelnummer, color, false, textwinkel);

	}
}



void ConstructRohrNippel::createElement()
{
	ConstructAbgEle::createElement();

	double f1 = faktorRohrlaenge;

	GlObj::colour color = GlObj::BLACK;
	// Punkte
	double x  = 0, y  = 0;
	double x1 = 0, y1 = 0;
	double x2 = 0, y2 = 0; 
	double dx = 0, dy = 0;
	// Kreis
	int durchmesser = 0, start = 0, end = 0;
	
	switch (typ) 
	{
	case ABG:
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) 
		{
			// Element
			x  = X0;
			y  = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2;
			dx = -elemerk.get_laenge();
			dy = dn;
			add_rectangle(x, y, dx, dy, color);

			color = GlObj::UNBEKANNT;
			x1 = X0;
			y1 = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2;	
			x2 = X0;
			y2 = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2;
			add_line(x1, y1, x2, y2, 1, color);
			color = GlObj::BLACK;

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 - dn/2;
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);

			
		} 
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90)
		{
			x  = X0 + andn;
			y  = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2;
			dx = elemerk.get_laenge();
			dy = dn;
			add_rectangle(x, y, dx, dy, color);

			color = GlObj::UNBEKANNT;
			x1 = X0 + andn;
			y1 = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2; 
			x2 = X0 + andn;
			y2 = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2;
			add_line(x1, y1, x2, y2, 1, color);
			color = GlObj::BLACK;
			
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + andn + dn/2;
			y     = Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);

		
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
			x  = X0 + andn;
			y  = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2;
			dx = elemerk.get_laenge();
			dy = -dn;
			add_rectangle(x, y, dx, dy, color);
	
			color = GlObj::UNBEKANNT;
			x1 = X0 + andn;
			y1 = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2;
			x2 = X0 + andn;
			y2 = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2;
			add_line(x1, y1, x2, y2, 1, color);

			color = GlObj::BLACK;
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + andn + dn/2;
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);

		
		}
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270)
		{
			x  = X0;
			y  = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2;
			dx = -elemerk.get_laenge();
			dy = -dn;
			add_rectangle(x, y, dx, dy, color);

			color = GlObj::UNBEKANNT;
			x1 = X0;
			y1 = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2;
			x2 = X0;
			y2 = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2;
			add_line(x1, y1, x2, y2, 1, color);
			color = GlObj::BLACK;

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 - dn/2;
			y     = Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1; 	
			add_circle(x, y, durchmesser, start, end);

		}
		// oben	und Rohr nach rechts 
		else if (winkel == 0 && drehfaktor == 0) 
		{
			x = X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2;
			y = Y0 + andn;
			add_rectangle(x, y, dn, elemerk.get_laenge(), color);

			color = GlObj::UNBEKANNT;
			x1 = X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2;
			y1 = Y0 + andn;
			x2 = X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2;
			y2 = Y0 + andn;
			add_line(x1, y1, x2, y2, 1, color);
			color = GlObj::BLACK;

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1;
			y     = Y0 + andn + dn/2; 	
			add_circle(x, y, durchmesser, start, end);

		}
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0)
		{
			x     = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2;
			y     = Y0 - elemerk.get_laenge();
			add_rectangle(x, y, dn, elemerk.get_laenge(), color);
	
			color = GlObj::UNBEKANNT;
			add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2, Y0, 
						   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2, Y0, 
						   1, color);
			color = GlObj::BLACK;

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
			y     = Y0 - dn/2; 	
			add_circle(x, y, durchmesser, start, end);

		
		} 
		// hinten/vorne und Rohr nach rechts
		else if ((winkel == 90 || winkel == 270) && drehfaktor ==0) 
		{
			durchmesser = dn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1;
			y     = Y0 + andn/2; 	
			add_circle(x, y, durchmesser, start, end, winkel == 90 ? true : false);
		} 
		// oben	und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180) 
		{
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2;
			y     = Y0 - elemerk.get_laenge();
			add_rectangle(x, y, dn, elemerk.get_laenge(), color);

			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0, 
						   1, color);
			color = GlObj::BLACK;

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1;
			y     = Y0 - dn/2; 	
			add_circle(x, y, durchmesser, start, end);

		}
		// unten und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180)
		{
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2;
			y     = Y0 + andn;
			add_rectangle(x, y, dn, elemerk.get_laenge(), color);

			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, Y0 + andn, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, Y0 + andn, 
						   1, color);
			color = GlObj::BLACK;

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1;
			y     = Y0 + andn + dn/2; 	
			add_circle(x, y, durchmesser, start, end);

	
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
		// Nut für Nutstutzen
		if (elemerk.get_id() == NUTST || elemerk.get_id() == STDSTUTZEN)
		{ 
			// links und Rohr nach oben
			if (winkel == 0 && drehfaktor == 90)
			{		// obere Rohrlinie weiss									
				color = GlObj::UNBEKANNT;												
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								1, color);						
				// untere Rohrlinie weiss									
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								1, color);
				color = GlObj::BLACK;

				// linke Linie
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								1, color);						
				// rechte Linie												
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								1, color);						
				// obere Linie												
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - 2*dn/5, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - 2*dn/5, 
								1, color);						
				// untere Linie												
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + 2*dn/5, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + 2*dn/5, 
								1, color);						
		
			}
			// rechts und Rohr nach oben
			else if (winkel == 180 && drehfaktor == 90)
			{	
				// obere Rohrlinie weiss
				color = GlObj::UNBEKANNT;
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								1, color);
				// untere Rohrlinie weiss
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								1, color);
				color = GlObj::BLACK;

				// linke Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								1, color);
				// rechte Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
								1, color);
				// obere Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - 2*dn/5, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - 2*dn/5, 
								1, color);
				// untere Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + 2*dn/5, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + 2*dn/5, 
								1, color);
		
			}
			// rechts und Rohr nach unten
			else if (winkel ==   0 && drehfaktor == 270)
			{	
				// obere Rohrlinie weiss
				color = GlObj::UNBEKANNT;
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								1, color);
				// untere Rohrlinie weiss
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								1, color);
				color = GlObj::BLACK;

				// linke Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								1, color);
				// rechte Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								1, color);
				// obere Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5, 
								1, color);
				// untere Linie
				add_line(	X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5,		Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5, 
								X0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10,	Y0  + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5, 
								1, color);
		
			} 
			// links und Rohr nach unten
			else if (winkel == 180 && drehfaktor == 270)
			{		
				// obere Rohrlinie weiss								
				color = GlObj::UNBEKANNT;											
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								1, color);					
				// untere Rohrlinie weiss								
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								1, color);
				color = GlObj::BLACK;

				// linke Linie
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								1, color);					
				// rechte Linie											
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
								1, color);					
				// obere Linie											
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5, 
								1, color);					
				// untere Linie											
				add_line(	X0 - elemerk.get_laenge() + elemerk.get_laenge()/5,		Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5, 
								X0 - elemerk.get_laenge() + elemerk.get_laenge()/10,	Y0 + (double)( - elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5, 
								1, color);					
		
			} 
			// oben und Rohr nach rechts
			else if (winkel == 0 && drehfaktor == 0)
			{ 	
				// linke Rohrlinie weiss
				color = GlObj::UNBEKANNT;
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				// rechte Rohrlinie weiss
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				color = GlObj::BLACK;

				// obere Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				// untere Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2, 	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								1, color);
				// linke Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - 2*dn/5,	Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - 2*dn/5, Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				// rechte Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + 2*dn/5, Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + 2*dn/5, Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);

			} 
			// unten und Rohr nach rechts
			else if (winkel == 180 && drehfaktor == 0)
			{ 			
				// linke Rohrlinie weiss
				color = GlObj::UNBEKANNT;
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				// rechte Rohrlinie weiss
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				color = GlObj::BLACK;

				// untere Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				// obere Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,   Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								1, color);
				// linke Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - 2*dn/5, Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - 2*dn/5, Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				// rechte Linie
				add_line(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + 2*dn/5, Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + 2*dn/5, Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
	
			}
			// oben und Rohr nach links
			else if (winkel == 180 && drehfaktor == 180)
			{ 	
				// linke Rohrlinie weiss
				color = GlObj::UNBEKANNT;
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				// rechte Rohrlinie weiss	
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				color = GlObj::BLACK;

				// obere Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				// untere Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								1, color);
				// linke Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
				// rechte Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5,			Y0 + andn + elemerk.get_laenge() - elemerk.get_laenge()/10, 
								1, color);
	

			}
			// unten und Rohr links
			else if (winkel == 0 && drehfaktor == 180)
			{ 
				// linke Rohrlinie weiss
				color = GlObj::UNBEKANNT;
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				// rechte Rohrlinie weiss
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				color = GlObj::BLACK;

				// untere Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				// obere Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								1, color);
				// linke Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - 2*dn/5,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);
				// rechte Linie
				add_line(	X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/5, 
								X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + 2*dn/5,			Y0 - elemerk.get_laenge() + elemerk.get_laenge()/10, 
								1, color);

			}
		}
		// Gewinde für Gewindestutzen DN65
		else if (elemerk.get_id() == GWRST) 
		{ 
			// links und Rohr nach oben
			if (winkel == 0 && drehfaktor == 90) 
			{
				add_line(X0 - elemerk.get_laenge(),											Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
							   X0 - elemerk.get_laenge() +   dn/8,									Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 - elemerk.get_laenge() +	  dn/8,									Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
							   X0 - elemerk.get_laenge() + 2*dn/8,									Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 - elemerk.get_laenge() + 2*dn/8,									Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
							   X0 - elemerk.get_laenge() + 3*dn/8,									Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							   1, color);								
			} 
			// rechts und Rohr nach oben															
			else if (winkel == 180 && drehfaktor == 90) 
			{						
				add_line(X0 + andn + elemerk.get_laenge(),				Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
							   X0 + andn + elemerk.get_laenge() -   dn/8,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 + andn + elemerk.get_laenge() -   dn/8,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
							   X0 + andn + elemerk.get_laenge() - 2*dn/8,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 + andn + elemerk.get_laenge() - 2*dn/8,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
							   X0 + andn + elemerk.get_laenge() - 3*dn/8,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							   1, color);								
			} 
			// rechts und Rohr nach unten															
			else if (winkel ==   0 && drehfaktor == 270)
			{						
				add_line(X0 + andn + elemerk.get_laenge(),				Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
							   X0 + andn + elemerk.get_laenge() -   dn/8,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 + andn + elemerk.get_laenge() -   dn/8,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
							   X0 + andn + elemerk.get_laenge() - 2*dn/8,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 + andn + elemerk.get_laenge() - 2*dn/8,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
							   X0 + andn + elemerk.get_laenge() - 3*dn/8,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							   1, color);								
			}
			// links und Rohr nach unten															
			else if (winkel == 180 && drehfaktor == 270) 
			{						
				add_line(X0 - elemerk.get_laenge(),											Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
							   X0 - elemerk.get_laenge() +   dn/8,									Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 - elemerk.get_laenge() +	  dn/8,									Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
							   X0 - elemerk.get_laenge() + 2*dn/8,									Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							   1, color);								
																					
				add_line(X0 - elemerk.get_laenge() + 2*dn/8,									Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
							   X0 - elemerk.get_laenge() + 3*dn/8,									Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							   1, color);
			}
			// unten und Rohr nach rechts
			else if (winkel == 180 && drehfaktor == 0)
			{
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 - elemerk.get_laenge(), 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 - elemerk.get_laenge() +   dn/8,
							   1, color);								
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 - elemerk.get_laenge() +   dn/8, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 - elemerk.get_laenge() + 2*dn/8,
							   1, color);					
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 - elemerk.get_laenge() + 2*dn/8, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 - elemerk.get_laenge() + 3*dn/8,
							   1, color);					
			} 
			// oben	 und Rohr nach rechts													
			else if (winkel == 0 && drehfaktor == 0) 
			{								
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge(), 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 + andn + elemerk.get_laenge() -   dn/8,
							   1, color);					
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge() -   dn/8, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 + andn + elemerk.get_laenge() - 2*dn/8,
							   1, color);					
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 - dn/2,	Y0 + andn + elemerk.get_laenge() - 2*dn/8, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + dn/2,	Y0 + andn + elemerk.get_laenge() - 3*dn/8,
							   1, color);
			} 
			// unten und Rohr nach links
			else if (winkel == 0 && drehfaktor == 180) 
			{
				add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,					Y0 - elemerk.get_laenge(), 
							   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,					Y0 - elemerk.get_laenge() +   dn/8,
							   1, color);									
				add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,					Y0 - elemerk.get_laenge() +   dn/8, 
							   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,					Y0 - elemerk.get_laenge() + 2*dn/8,
							   1, color);									
				add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,					Y0 - elemerk.get_laenge() + 2*dn/8, 
							   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,					Y0 - elemerk.get_laenge() + 3*dn/8,
							   1, color);												
			}
			// oben	 und Rohr nach links																				
			else if (winkel == 180 && drehfaktor == 180)
			{															
				add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,					Y0 + andn + elemerk.get_laenge(), 
							   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,					Y0 + andn + elemerk.get_laenge() -   dn/8,
							   1, color);									
				add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,					Y0 + andn + elemerk.get_laenge() -   dn/8, 
							   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,					Y0 + andn + elemerk.get_laenge() - 2*dn/8,
							   1, color);									
				add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,					Y0 + andn + elemerk.get_laenge() - 2*dn/8, 
							   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,					Y0 + andn + elemerk.get_laenge() - 3*dn/8,
							   1, color);
			}
		}
		break;
	}
}

