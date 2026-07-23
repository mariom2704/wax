#include "stdafx.h"


#include "constructboden.h"
#include "kreis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructBoden::~ConstructBoden()
{

}

ConstructBoden::ConstructBoden()
{

}

void ConstructBoden::createBeschriftung()
{
	switch (typ) 
	{
	case IMROHR:
		text = "BOD";
		break;
	case ABG:
		text = "+BOD";
		break;
	}
	ConstructEndEle::createBeschriftung();
}


void ConstructBoden::createElement()
{
	ConstructEndEle::createElement();

	double f1 = faktorRohrlaenge;
	// Farbe
	GlObj::colour color = GlObj::BLACK;
	// Punkte
	double x = 0, y = 0;
	// Kreis
	int durchmesser = 0, start = 0, end = 0;

	switch (typ) 
	{
	case IMROHR:
		// Boden links
		if (erstesElement) 
		{ 
			add_line(X0,										Y0, 
						   X0 - elemerk.get_laenge(),				Y0,							
						   1, color);
			add_line(X0,										Y0 + andn,				
						   X0 - elemerk.get_laenge(),				Y0 + andn,				
						   1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 + dn/2 - elemerk.get_laenge();
			y     = Y0 + dn/2; 
		} 
		// Boden links
		else if (winkel == 180) 
		{ 
			add_line(X0 - elemerk.get_abstAbs(),							Y0, 
						   X0 - elemerk.get_abstAbs() - elemerk.get_laenge(),	Y0,								
						   1, color);
			add_line(X0 - elemerk.get_abstAbs(),							Y0 + andn,				
						   X0 - elemerk.get_abstAbs() - elemerk.get_laenge(),	Y0 + andn,				
						   1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0  - elemerk.get_abstAbs() + dn/2 - elemerk.get_laenge();
			y     = Y0 + dn/2; 
		} 
		// Boden oben
		else if (winkel == 90) 
		{ 
			add_line(X0,										Y0 + elemerk.get_abstAbs(), 
						   X0,										Y0 + elemerk.get_abstAbs() + elemerk.get_laenge(),
						   1, color);
			add_line(X0 + dn,					Y0 + elemerk.get_abstAbs(), 
						   X0 + dn,					Y0 + elemerk.get_abstAbs() + elemerk.get_laenge(),
						   1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + dn/2;
			y     = Y0 + elemerk.get_abstAbs() - dn/2 + elemerk.get_laenge(); 	
		} 
		// Boden unten
		else if (winkel == 270)
		{ 
			add_line(X0,										Y0 - elemerk.get_abstAbs(), 
						   X0,										Y0 - elemerk.get_abstAbs() - elemerk.get_laenge(),
						   1, color);
			add_line(X0 + dn,					Y0 - elemerk.get_abstAbs(), 
						   X0 + dn,					Y0 - elemerk.get_abstAbs() - elemerk.get_laenge(),
						   1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + dn/2;
			y     = Y0 - elemerk.get_abstAbs() + dn/2 - elemerk.get_laenge() ; 	
		} 
		// Boden rechts
		else if (winkel == 0) 
		{ 
			add_line(	X0 + (double)elemerk.get_abstAbs()/f1,							Y0, 
							X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge(),	Y0,				
							1, color);
			add_line(	X0 + (double)elemerk.get_abstAbs()/f1,							Y0 + andn, 
							X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge(),	Y0 + andn,	
							1, color);

			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge() - dn/2;
			y     = Y0 + dn/2;
		}
		break;

	case ABG:
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) 
		{
			add_line(	X0 - merkerRohrele.get_laenge(), 								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 - dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 		Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 - dn/2, 
							1, color);
			add_line(	X0 - merkerRohrele.get_laenge(), 								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 + dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 		Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 + dn/2, 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 - merkerRohrele.get_laenge() + dn/2 - elemerk.get_laenge();
			y     = Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1; 	
		}
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90) 
		{
			add_line(	X0 + andn + merkerRohrele.get_laenge(), 								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 - dn/2, 
							X0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 		Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 - dn/2, 
							1, color);
			add_line(	X0 + andn + merkerRohrele.get_laenge(), 								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 + dn/2, 
							X0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 		Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1 + dn/2, 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + andn + merkerRohrele.get_laenge() - dn/2 + elemerk.get_laenge();
			y     = Y0 +(double)( elemerk.get_abstAbs() - laenge)/f1; 	
		} 
		// rechts und Rohr nach unten
		else if (winkel == 0 && drehfaktor == 270) 
		{
			add_line(	X0 + andn + merkerRohrele.get_laenge(), 								Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 
							X0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 		Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 
							1, color);
			add_line(	X0 + andn + merkerRohrele.get_laenge(), 								Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 
							X0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 		Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_315;
			end   = Kreis::GRAD_45;
			x     = X0 + andn + merkerRohrele.get_laenge() - dn/2 + elemerk.get_laenge();
			y     = Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1; 	
		} 
		// links und Rohr nach unten
		else if (winkel == 180  && drehfaktor == 270)
		{
			add_line(	X0 - merkerRohrele.get_laenge(), 								Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 		Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 
							1, color);
			add_line(	X0 - merkerRohrele.get_laenge(), 								Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 		Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_135;
			end   = Kreis::GRAD_225;
			x     = X0 - merkerRohrele.get_laenge() + dn/2 - elemerk.get_laenge();
			y     = Y0 - elemerk.get_abstAbs() + laenge; 	
		} 
		// oben und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180) 
		{ 
			add_line(	X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 			Y0 + andn + merkerRohrele.get_laenge(), 
							X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 			Y0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 
							1, color);
			add_line(	X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 			Y0 + andn + merkerRohrele.get_laenge(), 
							X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 			Y0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 ;
			y     = Y0 + andn + merkerRohrele.get_laenge() - dn/2 + elemerk.get_laenge(); 	
		}
		// unten und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180) 
		{ 
			add_line(	X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 			Y0 - merkerRohrele.get_laenge(), 
							X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 - dn/2, 			Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 
							1, color);
			add_line(	X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 			Y0 - merkerRohrele.get_laenge(), 
							X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1 + dn/2, 			Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1;
			y     = Y0 - merkerRohrele.get_laenge()  + dn/2 - elemerk.get_laenge(); 	
		} 
		// oben und Rohr nach rechts
		else if (winkel == 0 && drehfaktor == 0) 
		{ 
			add_line(	X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, 			Y0 + andn + merkerRohrele.get_laenge(), 
							X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, 			Y0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 
							1, color);
			add_line(	X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, 			Y0 + andn + merkerRohrele.get_laenge(), 
							X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, 			Y0 + andn + merkerRohrele.get_laenge() + elemerk.get_laenge(), 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_45;
			end   = Kreis::GRAD_135;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 + andn + merkerRohrele.get_laenge() - dn/2 + elemerk.get_laenge(); 	
		}
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0)
		{ 
			add_line(	X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, 			Y0 - merkerRohrele.get_laenge(), 
							X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, 			Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 
							1, color);
			add_line(	X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, 			Y0 - merkerRohrele.get_laenge(), 
							X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, 			Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge(), 
							1, color);
			durchmesser = (int)sqrt((double)(2 * dn * dn));
			start = Kreis::GRAD_225;
			end   = Kreis::GRAD_315;
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y     = Y0 - merkerRohrele.get_laenge()  + dn/2 - elemerk.get_laenge(); 	
		} 
		break;
		default:
			XASSERT(FALSE);
			return;
	}
	add_circle(x, y, durchmesser, start, end);
}
