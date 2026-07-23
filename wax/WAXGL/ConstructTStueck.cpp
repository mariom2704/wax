#include "stdafx.h"


#include "constructtstueck.h"
#include "kreis.h"
#include "math.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructTStueck::~ConstructTStueck()
{

}

ConstructTStueck::ConstructTStueck()
{

}

void ConstructTStueck::createBeschriftung()
{
	// Text fuer Nennweite
	setDNText();

	if (text2.GetLength())
	{
		double f1 = faktorRohrlaenge;
		GlObj::colour color = GlObj::BLACK;
		int winkel = 0;
		double x = 0, y = 0;

		if (elemerk.get_firstele() && !elemerk.get_kopfstueck())
		{
			x = X0 - 200;
			y = Y0;
		}
		else if (elemerk.get_firstele() && elemerk.get_kopfstueck())
		{
			x = X0;
			y = Y0 - 100;
		}
		else
		{
			if (drehfaktor == 0 && bogen) 
			{
				if (elemerk.get_lastele() && !elemerk.get_kopfstueck())
				{
					x = X0 + elemerk.get_abstAbs() - laenge + 100;
					y = Y0;
				}
				else if (elemerk.get_lastele() && elemerk.get_kopfstueck())
				{
					x = X0 + elemerk.get_abstAbs() - laenge;
					y = Y0 - 100;
				}
			}
			else if (drehfaktor == 0) 
			{
				if (elemerk.get_lastele() && !elemerk.get_kopfstueck())
				{
					x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1 + 100;	
					y = Y0; 
				}
				else if (elemerk.get_lastele() && elemerk.get_kopfstueck())
				{
					x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;	
					y = Y0 - 100; 
				}
			}
			else if (drehfaktor == 180) 
			{
				if (elemerk.get_lastele() && !elemerk.get_kopfstueck())
				{
					x = X0 - elemerk.get_abstAbs() + laenge - 250;					
					y = Y0; 
				}
				else if (elemerk.get_lastele() && elemerk.get_kopfstueck())
				{
					x = X0 - elemerk.get_abstAbs() + laenge;					
					y = Y0 - 100; 
				}
			}
			else if (drehfaktor == 90) 
			{
				if (elemerk.get_lastele() && !elemerk.get_kopfstueck())
				{
					x = X0;		
					y = Y0 + elemerk.get_abstAbs() - laenge + 100; 
				}
				else if (elemerk.get_lastele() && elemerk.get_kopfstueck())
				{
					x = X0 + rohrDn + 100;		
					y = Y0 + elemerk.get_abstAbs() - laenge; 
				}
			}
			else if (drehfaktor == 270)
			{
				if (elemerk.get_lastele() && !elemerk.get_kopfstueck())
				{
					x = X0;		
					y = Y0 - elemerk.get_abstAbs() + laenge - 100; 
				}
				else if (elemerk.get_lastele() && elemerk.get_kopfstueck())
				{
					x = X0 - 200;		
					y = Y0 - elemerk.get_abstAbs() + laenge; 
				}
			}
		}
		add_text(x, y, text2, color, false, winkel);
	}

	
	if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270)
	{
		CString winkel;
		winkel.Format(" %d'", elemerk.get_winkel());
		text += winkel;
	}
	
	ConstructFitting::createBeschriftung();
}

void ConstructTStueck::createElement()
{
	ConstructFitting::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	
	double x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	int dn = 0;
	
	if (elemerk.get_kopfstueck() && elemerk.get_firstele())
		dn = elemerk.get_dn();
	else if (elemerk.get_kopfstueck() && elemerk.get_lastele())
		dn = elemerk.get_anschlussDN();
	else if (elemerk.get_dn() > elemerk.get_anschlussDN())
		dn = elemerk.get_dn();
	else 
		dn = elemerk.get_anschlussDN();
	
	switch (typ) 
	{
	case IMROHR:
		// Rohr nach rechts
		if (drehfaktor == 0) 
		{
			// und Kopfstueck rechts
			if (elemerk.get_kopfstueck() && !elemerk.get_firstele() && (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180))
			{
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.6*dn;
				x1 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y1 = Y0 + 1.6*dn;
				add_line(x0, y0, x1, y1, 1); 

				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.6*dn;
				x1 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y1 = Y0 - 0.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 + 1.1*dn;
				x1 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y1 = Y0 + 1.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1;y0 = Y0 - 0.1*dn;
				x1 = X0 + (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1;y1 = Y0 + 1.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.1*dn;
				x1 = X0 + (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1;y1 = Y0 - 0.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 + 1.1*dn;
				x1 = X0 + (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1;y1 = Y0 + 1.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.6*dn;
				x1 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y1 = Y0 - 0.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 + 1.6*dn;
				x1 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y1 = Y0 + 1.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
			}
			// und Kopfstueck links
			else if (elemerk.get_kopfstueck() && elemerk.get_firstele())
			{
				x0 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.6*dn;
				x1 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y1 = Y0 + 1.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.6*dn;
				x1 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y1 = Y0 - 0.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 + 1.1*dn;
				x1 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y1 = Y0 + 1.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + 1.5*elemerk.get_laenge())/f1;y0 = Y0 - 0.1*dn;
				x1 = X0 + (elemerk.get_abstRel() + 1.5*elemerk.get_laenge())/f1;y1 = Y0 + 1.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.1*dn;
				x1 = X0 + (elemerk.get_abstRel() + 1.5*elemerk.get_laenge())/f1;y1 = Y0 - 0.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 + 1.1*dn;
				x1 = X0 + (elemerk.get_abstRel() + 1.5*elemerk.get_laenge())/f1;y1 = Y0 + 1.1*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 - 0.6*dn;
				x1 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y1 = Y0 - 0.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
				
				x0 = X0 + (elemerk.get_abstRel() + elemerk.get_laenge()/2)/f1;	y0 = Y0 + 1.6*dn;
				x1 = X0 + (elemerk.get_abstRel() - elemerk.get_laenge()/2)/f1;	y1 = Y0 + 1.6*dn;	
				add_line(x0, y0, x1, y1, 1); 
			}
			// und Element nach oben
			else if (elemerk.get_winkel() == 0)
			{
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 - dn/10,		
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 - dn/10,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 + dn*1.1,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + dn*1.1,		
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + 2*dn,		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + 2*dn,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + dn*1.1,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + 2*dn,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + dn*1.1,		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + 2*dn,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 - dn*0.1,	
								X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 - dn*0.1,	
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
			}
			// und Element nach unten
			else if (elemerk.get_winkel() == 180) 
			{
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0-dn/10,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0-dn/10,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0-dn/10,		
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0-dn/10,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 + dn*1.1,		
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - dn,		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - dn,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - dn*0.1,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - dn,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - dn*0.1,		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - dn,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 - dn*0.1,	
								X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
				add_line(	X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 - dn*0.1,	
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 + dn*1.1,			
								1);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270)
			{
				add_rectangle(	X0 + (elemerk.get_abstRel() - elemerk.get_laenge())/f1, Y0 - dn/10,  
									2*elemerk.get_laenge()/f1,							   dn*1.2,	
									color);
		
				int durchmesser = (int)(dn/f1);
				int start		= Kreis::GRAD_0;
				int end			= Kreis::GRAD_360;
				double x		= X0 + elemerk.get_abstRel()/f1;
				double y		= Y0 - dn/10 + dn*0.6; 	
				add_circle(x, y, durchmesser, start, end, elemerk.get_winkel() == 90 ? true : false);
			}
		}
		// Rohr nach links
		else if (drehfaktor == 180)
		{
			 // und Element nach oben
			if (elemerk.get_winkel() == 0)
			{
				add_line(	X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0-dn/10,		
								X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0-dn/10,			
								1);
				add_line(	X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0 + dn*1.1,		
								X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
				add_line(	X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 + dn*1.1,		
								X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
				add_line(	X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 + 2*dn,		
								X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 + 2*dn,			
								1);
				add_line(	X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 + dn*1.1,		
								X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 + 2*dn,			
								1);
				add_line(	X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 + dn*1.1,		
								X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 + 2*dn,			
								1);
				add_line(	X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0 - dn*0.1,	
								X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
				add_line(	X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0 - dn*0.1,	
								X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
			}
			// und Element nach unten
			else if (elemerk.get_winkel() == 180) 
			{
				add_line(	X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0-dn/10,		
								X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0-dn/10,			
								1);
				add_line(	X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0-dn/10,		
								X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0-dn/10,			
								1);
				add_line(	X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0 + dn*1.1,		
								X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
				add_line(	X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 - dn,		
								X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 - dn,			
								1);
				add_line(	X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 - dn*0.1,		
								X0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	Y0 - dn,			
								1);
				add_line(	X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 - dn*0.1,		
								X0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	Y0 - dn,			
								1);
				add_line(	X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0 - dn*0.1,	
								X0 - elemerk.get_abstRel() +     elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
				add_line(	X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0 - dn*0.1,	
								X0 - elemerk.get_abstRel() -     elemerk.get_laenge(),	Y0 + dn*1.1,			
								1);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) 
			{
				add_rectangle(	X0 - elemerk.get_abstRel() + elemerk.get_laenge(), Y0 - dn/10,  
									-2*elemerk.get_laenge(),						   dn*1.2,	
									color);
	
				int durchmesser = dn;
				int start = Kreis::GRAD_0;
				int end   = Kreis::GRAD_360;
				double x     = X0 - elemerk.get_abstRel();
				double y     = Y0 - dn/10 + dn*0.6; 	
				add_circle(x, y, durchmesser, start, end, elemerk.get_winkel() == 90 ? true : false);
			}
		}
		// Rohr nach oben
		else if (drehfaktor == 90)
		{
			// und Element nach unten
			if (elemerk.get_winkel() == 0) 
			{
				add_line(	X0 - dn/10,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),	
								X0 - dn/10,		Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn/10,		Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 - dn/10,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),	
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn,			Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),
								X0 - dn,			Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								1);
				add_line(	X0 - dn*0.1,		Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 - dn,			Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								1);
				add_line(	X0 - dn*0.1,		Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 - dn,			Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								1);
				add_line(	X0 - dn*0.1,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn*0.1,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
			}
			 // und Element nach oben
			else if (elemerk.get_winkel() == 180)
			{
				add_line(	X0 - dn/10,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),	
								X0 - dn/10,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),	
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
				add_line(	X0 + 2*dn,		Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 + 2*dn,		Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 + 2*dn,		Y0	+ elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 + 2*dn,		Y0	+ elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn*0.1,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn*0.1,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	+ elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) 
			{
				add_rectangle(	X0 - dn/10,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge(),  
									dn*1.2,		2*elemerk.get_laenge(),						   	
									color);

				int durchmesser = dn;
				int start = Kreis::GRAD_0;
				int end   = Kreis::GRAD_360;
				double x     = X0 - dn/10 + dn*0.6;
				double y     = Y0 + elemerk.get_abstRel(); 	
				add_circle(x, y, durchmesser, start, end, elemerk.get_winkel() == 90 ? true : false);
			}
		}
		// Rohr nach unten
		else if (drehfaktor == 270)
		{
			 // und Element nach oben
			if (elemerk.get_winkel() == 0)
			{
				add_line(	X0 - dn/10,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),	
								X0 - dn/10,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),	
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
				add_line(	X0 + 2*dn,		Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 + 2*dn,		Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 + 2*dn,		Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 + dn*1.1,		Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 + 2*dn,		Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn*0.1,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn*0.1,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
			}
			// und Element nach unten
			else if (elemerk.get_winkel() == 180) 
			{
				add_line(	X0 - dn/10,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),	
								X0 - dn/10,		Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn/10,		Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 - dn/10,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);

				add_line(	X0 + dn*1.1,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),	
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn,			Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),
								X0 - dn,			Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								1);
				add_line(	X0 - dn*0.1,		Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								X0 - dn,			Y0	- elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),	
								1);
				add_line(	X0 - dn*0.1,		Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								X0 - dn,			Y0	- elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),	
								1);
				add_line(	X0 - dn*0.1,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() +     elemerk.get_laenge(),		
								1);
				add_line(	X0 - dn*0.1,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),
								X0 + dn*1.1,		Y0	- elemerk.get_abstRel() -     elemerk.get_laenge(),		
								1);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) 
			{
				add_rectangle(	X0 - dn/10,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge(),  
									dn*1.2,		-2*elemerk.get_laenge(),						   	
									color);
	
				int durchmesser = dn;
				int start = Kreis::GRAD_0;
				int end   = Kreis::GRAD_360;
				double x     = X0 - dn/10 + dn*0.6;
				double y     = Y0 - elemerk.get_abstRel(); 	
				add_circle(x, y, durchmesser, start, end, elemerk.get_winkel() == 90 ? true : false);
			}
		}
	}
}



void ConstructTStueck::setDNText()
{
	/**
	DN Beschriftung eines T-Stücks erzeugen. Zweiter Text mit DN wird nur gesetzt, wenn das
	Element am Anfang oder Ende steht
	Wenn der Winkel sich ändert, muss für ein Grafikupdate gesorgt werden.
	Damit da eine Änderung durch den CRC erkannt wird, wird bei text2 und Winkel >= 180
	ein Leerzeichen an den Text gehängt
	*/
	int dn_text = 0, dn_text2 = 0;

	if (elemerk.get_firstele())
	{
		if (!elemerk.get_kopfstueck())
		{
			dn_text = elemerk.get_dn2();
			dn_text2 = elemerk.get_dn1();
		}
		else
		{
			if (elemerk.get_winkel() < 180)
			{
				dn_text = elemerk.get_dn1();
				dn_text2 = elemerk.get_dn3();
			}
			else
			{
				dn_text = elemerk.get_dn3();
				dn_text2 = elemerk.get_dn1();
			}
		}
	}
	else if (elemerk.get_lastele())
	{
		if (!elemerk.get_kopfstueck())
		{
			dn_text = elemerk.get_dn2();
			dn_text2 = elemerk.get_dn3();
		}
		else
		{
			if (elemerk.get_winkel() < 180)
			{
				dn_text = elemerk.get_dn3();
				dn_text2 = elemerk.get_dn1();
			}
			else
			{
				dn_text = elemerk.get_dn1();
				dn_text2 = elemerk.get_dn3();
			}
		}
	}
	else
		dn_text = elemerk.get_dn2();

	// DN in String umwandeln, Zoll oder DN Angabe, Typ der Zoll Ausgabe ist hier immer
	// 0: also z.B. 1.1/4'
	text = WaxStamm::dn2str(dn2zoll, dn_text);
	if (dn_text2)
	{
		text2 = WaxStamm::dn2str(dn2zoll, dn_text2);
		if (elemerk.get_winkel() >= 180)
			text2 += " ";
	}
}

