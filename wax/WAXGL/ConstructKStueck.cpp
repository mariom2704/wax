#include "stdafx.h"


#include "constructkstueck.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructKStueck::~ConstructKStueck()
{}

ConstructKStueck::ConstructKStueck()
{

}

void ConstructKStueck::createBeschriftung()
{
	CString txt2, txt3;

	if (!dn2zoll)
	{
		if (elemerk.get_firstele() || elemerk.get_lastele())
		{
			//text.Format("%d %d %d", elemerk.get_dn2(), elemerk.get_dn3(), elemerk.get_dn4());
			text.Format("%d", elemerk.get_dn2());
			txt2.Format("%d", elemerk.get_dn3());
			txt3.Format("%d", elemerk.get_dn4());
		}
		else
		{
			//text.Format("%d %d", elemerk.get_dn2(), elemerk.get_dn4());
			text.Format("%d", elemerk.get_dn2());
			txt2.Format("%d", elemerk.get_dn4());
		}
	}
	else
	{
		if (elemerk.get_firstele() || elemerk.get_lastele())
		{
			//text.Format("%s'' %s'' %s''", WaxStamm::dn2zoll(elemerk.get_dn2()),
			//							  WaxStamm::dn2zoll(elemerk.get_dn3()), WaxStamm::dn2zoll(elemerk.get_dn4()));
			text.Format("%s''", WaxStamm::dn2zoll(elemerk.get_dn2()));
			txt2.Format("%s''", WaxStamm::dn2zoll(elemerk.get_dn3()));
			txt3.Format("%s''", WaxStamm::dn2zoll(elemerk.get_dn4()));
		}
		else
		{
			//text.Format("%s'' %s''", WaxStamm::dn2zoll(elemerk.get_dn2()), WaxStamm::dn2zoll(elemerk.get_dn4()));
			text.Format("%s''", WaxStamm::dn2zoll(elemerk.get_dn2()));
			txt2.Format("%s''", WaxStamm::dn2zoll(elemerk.get_dn4()));
		}
	}

	if (txt2.GetLength())
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
			y = Y0;
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
					y = Y0 - 150;
				}
				else if (!elemerk.get_lastele())
				{
					x = X0 + elemerk.get_abstAbs() - laenge;
					y = Y0 - 150;
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
					y = Y0 - 150; 
				}
				else if (!elemerk.get_lastele())
				{
					x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;	
					y = Y0 - 150; 
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
				else if (!elemerk.get_lastele())
				{
					x = X0 - elemerk.get_abstAbs() + laenge - 250;					
					y = Y0; 
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
				else if (!elemerk.get_lastele())
				{
					if (elemerk.get_winkel() == 180)
					{
						winkel = 135;
						x = X0;		
					}
					else
					{
						x = X0 + 200;		
						winkel = 45;
					}
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
				else if (!elemerk.get_lastele())
				{
					x = X0;		
					y = Y0 - elemerk.get_abstAbs() + laenge - 100; 
				}
			}
		}
		add_text(x, y, txt2, color, false, winkel);
	}

	if (txt3.GetLength())
	{
		double f1 = faktorRohrlaenge;
		GlObj::colour color = GlObj::BLACK;
		int winkel = 0;
		double x = 0, y = 0;

		if (elemerk.get_firstele())
		{
			x = X0 - 50;
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
					y = Y0 - 150;
				}
			}
			else if (drehfaktor == 0) 
			{
				if (elemerk.get_lastele() && !elemerk.get_kopfstueck())
				{
					x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;	
					y = Y0 - 150; 
				}
				else if (elemerk.get_lastele() && elemerk.get_kopfstueck())
				{
					x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;	
					y = Y0 - 150; 
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
		add_text(x, y, txt3, color, false, winkel);
	}
	
	if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270)
	{
		CString winkel;
		winkel.Format(" %d'", elemerk.get_winkel());
		text += winkel;
	}
	ConstructFitting::createBeschriftung();
}

void ConstructKStueck::createElement()
{
	ConstructFitting::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	
	switch (typ) 
	{
	case IMROHR:
		// Rohr nach rechts
		if (drehfaktor == 0) 
		{
			if (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180)
			{
				// unten
				// mittlere Linie links
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,			
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,			
								1,  color);
				// untere Linie
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),			
								1,  color);
				// oben
				// mittlere Linie links
				add_line(	X0 + (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 + (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
				// obere Linie
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),			
								1,  color);
				// links/rechts
				// untere senkrechte Linie links
				add_line(	X0 + (elemerk.get_abstRel() - elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()*0.1,	
								X0 + (elemerk.get_abstRel() - elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
				// untere senkrechte Linie rechts
				add_line(	X0 + (elemerk.get_abstRel() + elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()*0.1,	
								X0 + (elemerk.get_abstRel() + elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270)
			{
				add_rectangle(	X0 + (elemerk.get_abstRel() - elemerk.get_laenge())/f1, Y0 - elemerk.get_dn()/10,  
									2*elemerk.get_laenge()/f1,							    elemerk.get_dn()*1.2,	
									color);
			}
		}
		// Rohr nach links
		else if (drehfaktor == 180)
		{
			if (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180) 
			{
				// unten
				// mittlere Linie links
				add_line(	X0 - (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,			
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 - (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,			
								1,  color);
				// untere Linie
				add_line(	X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),		
								X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()/10,		
								X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn(),			
								1,  color);
				// oben
				// mittlere Linie links
				add_line(	X0 - (elemerk.get_abstRel() +     elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 - (elemerk.get_abstRel() -     elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
				// obere Linie
				add_line(	X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),		
								X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),			
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,		
								X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 + 2*elemerk.get_dn(),			
								1,  color);
				// links/rechts
				// untere senkrechte Linie links
				add_line(	X0 - (elemerk.get_abstRel() + elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()*0.1,	
								X0 - (elemerk.get_abstRel() + elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
				// untere senkrechte Linie rechts
				add_line(	X0 - (elemerk.get_abstRel() - elemerk.get_laenge())/f1,	Y0 - elemerk.get_dn()*0.1,	
								X0 - (elemerk.get_abstRel() - elemerk.get_laenge())/f1,	Y0 + elemerk.get_dn()*1.1,			
								1,  color);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) 
			{
				add_rectangle(	X0 - (elemerk.get_abstRel() + elemerk.get_laenge())/f1, Y0 - elemerk.get_dn()/10,  
									-2*elemerk.get_laenge()/f1,						        elemerk.get_dn()*1.2,	
									color);
			}
		}
		// Rohr nach oben
		else if (drehfaktor == 90)
		{
			if (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180) 
			{
				// unten
				// mittlere Linie links
				add_line(	X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() -     elemerk.get_laenge(),			
								X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),				
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() +     elemerk.get_laenge(),				
								1,  color);
				// untere Linie
				add_line(	X0 - elemerk.get_dn(),		Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),		
								X0 - elemerk.get_dn(),		Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 - elemerk.get_dn(),		Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 - elemerk.get_dn(),		Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								1,  color);
				// oben
				// mittlere Linie links
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() -     elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),				
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() +     elemerk.get_laenge(),				
								1,  color);
				// obere Linie
				add_line(	X0 + 2*elemerk.get_dn(),	Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 + 2*elemerk.get_dn(),	Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),				
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 + 2*elemerk.get_dn(),	Y0 + elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),				
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 + 2*elemerk.get_dn(),	Y0 + elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),				
								1,  color);
				// links/rechts
				// untere senkrechte Linie links
				add_line(	X0 - elemerk.get_dn()*0.1,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge(),					
								1,  color);
				// untere senkrechte Linie rechts
				add_line(	X0 - elemerk.get_dn()*0.1,	Y0 + elemerk.get_abstRel() + elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 + elemerk.get_abstRel() + elemerk.get_laenge(),					
								1,  color);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) 
			{
				add_rectangle(	X0 - elemerk.get_dn()/10,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge(),  
									elemerk.get_dn()*1.2,		2*elemerk.get_laenge(),						   	
									color);
			}
		}
		// Rohr nach unten
		else if (drehfaktor == 270)
		{
			if (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180) 
			{
				// unten
				// mittlere Linie links
				add_line(	X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() +     elemerk.get_laenge(),			
								X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),				
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() -     elemerk.get_laenge(),				
								1,  color);
				// untere Linie
				add_line(	X0 - elemerk.get_dn(),		Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),		
								X0 - elemerk.get_dn(),		Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 - elemerk.get_dn(),		Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 - elemerk.get_dn(),		Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								1,  color);
				// oben
				// mittlere Linie links
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() +     elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),				
								1,  color);
				// mittlere Linie rechts
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() -     elemerk.get_laenge(),				
								1,  color);
				// obere Linie
				add_line(	X0 + 2*elemerk.get_dn(),	Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 + 2*elemerk.get_dn(),	Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),				
								1,  color);
				// obere senkrechte Linie links
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),			
								X0 + 2*elemerk.get_dn(),	Y0 - elemerk.get_abstRel() + 0.5*elemerk.get_laenge(),				
								1,  color);
				// obere senkrechte Linie rechts
				add_line(	X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),			
								X0 + 2*elemerk.get_dn(),	Y0 - elemerk.get_abstRel() - 0.5*elemerk.get_laenge(),				
								1,  color);
				// links/rechts
				// untere senkrechte Linie links
				add_line(	X0 - elemerk.get_dn()*0.1,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge(),					
								1,  color);
				// untere senkrechte Linie rechts
				add_line(	X0 - elemerk.get_dn()*0.1,	Y0 - elemerk.get_abstRel() - elemerk.get_laenge(),			
								X0 + elemerk.get_dn()*1.1,	Y0 - elemerk.get_abstRel() - elemerk.get_laenge(),					
								1,  color);
			}
			else if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) 
			{
				add_rectangle(	X0 - elemerk.get_dn()/10,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge(),  
									elemerk.get_dn()*1.2,		-2*elemerk.get_laenge(),						   	
									color);
			}
		}
	}
}

