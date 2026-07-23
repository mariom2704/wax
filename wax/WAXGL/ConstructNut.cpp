#include "stdafx.h"


#include "constructnut.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructNut::~ConstructNut()
{

}

ConstructNut::ConstructNut()
{

}

void ConstructNut::createBeschriftung()
{
	BauelementGruppen bg;

	switch (typ) 
	{
	case IMROHR:
		text = bg.getKurzbezeichnung(NUT); 
		break;
	case ABG:
		text = "+";
		text += bg.getKurzbezeichnung(NUT); 
		break;
	}

	ConstructEndEle::createBeschriftung();
}

void ConstructNut::createElement()
{
	ConstructEndEle::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;

	double x0, x1, y0, y1;

	switch (typ) 
	{
	case IMROHR:
		// Nut links
		if (erstesElement) 
		{ 
			x0 = X0 + dn/10;
			x1 = X0 + dn/10;
			y0 = Y0;
			y1 = Y0 + dn;
			color = GlObj::UNBEKANNT;
			add_line(X0 + dn/10 + (double)elemerk.get_abstAbs()/f1,	Y0,							
						   X0 + dn/ 5 + (double)elemerk.get_abstAbs()/f1,	Y0,										
						   1, color);
			add_line(X0 + dn/10 + (double)elemerk.get_abstAbs()/f1,	Y0 + dn,					
						   X0 + dn/ 5 + (double)elemerk.get_abstAbs()/f1,	Y0 + dn,					
						   1, color);
			color = GlObj::BLACK;
			
			add_line(x0, y0, x1, y1, 1, color);
					
			add_line(X0 + dn/5, Y0,							
						   X0 + dn/5, Y0 + dn,			
						   1, color);
				
			add_line(X0 + dn/10,	Y0 + dn/10,				
						   X0 + dn/ 5,	Y0 + dn/10,				
						   1, color);
			add_line(X0 + dn/10,	Y0 + dn - dn/10,		
						   X0 + dn/ 5,	Y0 + dn - dn/10,	
						   1, color);
		} 
		// Nut links
		else if (winkel == 180) 
		{ 
			color = GlObj::UNBEKANNT;															
			add_line(X0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	Y0,							
						   X0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	Y0,							1, color);
			add_line(X0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	Y0 + dn,					
						   X0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	Y0 + dn,					1, color);
			
			color = GlObj::BLACK;;																
			
			add_line(X0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	Y0, 
						   X0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	Y0 + dn,					1, color);
			add_line(X0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	Y0,							
						   X0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	Y0 + dn,					1, color);
						
			add_line(X0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	Y0 + dn/10,				
						   X0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	Y0 + dn/10,				1, color);
			add_line(X0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	Y0 + dn - dn/10,		
						   X0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	Y0 + dn - dn/10,		1, color);
		} 
		// Nut oben
		else if (winkel == 90) 
		{ 
			color = GlObj::UNBEKANNT;														
			add_line(X0,												Y0 + (double)elemerk.get_abstAbs()/f1 - dn/10, 
						   X0,												Y0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	1, color);
			add_line(X0 + dn,											Y0 + (double)elemerk.get_abstAbs()/f1 - dn/10, 
						   X0 + dn,											Y0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	1, color);
			
			color = GlObj::BLACK;;															
			add_line(X0,												Y0 + (double)elemerk.get_abstAbs()/f1 - dn/10, 
						   X0 + dn,											Y0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	1, color);
			add_line(X0,												Y0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5, 
						   X0 + dn,											Y0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	1, color);
			
			add_line(X0 + dn/10,										Y0 + (double)elemerk.get_abstAbs()/f1 - dn/10, 
						   X0 + dn/10,										Y0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	1, color);
			add_line(X0 + dn - dn/10,									Y0 + (double)elemerk.get_abstAbs()/f1 - dn/10,
						   X0 + dn - dn/10,									Y0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	1, color);
		} 
		// Nut unten
		else if (winkel == 270) 
		{ 
			color = GlObj::UNBEKANNT;														
			add_line(X0,												Y0 - (double)elemerk.get_abstAbs()/f1 + dn/10, 
						   X0,												Y0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	1, color);
			add_line(X0 + dn,											Y0 - (double)elemerk.get_abstAbs()/f1 + dn/10, 
						   X0 + dn,											Y0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	1, color);
			
			color = GlObj::BLACK;															
			add_line(X0,												Y0 - (double)elemerk.get_abstAbs()/f1 + dn/10, 
						   X0 + dn,											Y0 - (double)elemerk.get_abstAbs()/f1 + dn/10,	1, color);
			add_line(X0,												Y0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5, 
						   X0 + dn,											Y0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	1, color);
			
			add_line(X0 + dn/10,										Y0 - (double)elemerk.get_abstAbs()/f1 + dn/10, 
						   X0 + dn/10,										Y0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	1, color);
			add_line(X0 + dn - dn/10,									Y0 - (double)elemerk.get_abstAbs()/f1 + dn/10,
						   X0 + dn - dn/10,									Y0 - (double)elemerk.get_abstAbs()/f1 + dn/ 5,	1, color);
		} 
		// Nut rechts
		else if (winkel == 0) 
		{ 
			color = GlObj::UNBEKANNT;															
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	Y0,							
						   X0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	Y0,												1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	Y0 + dn,					
						   X0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	Y0 + dn,							1, color);
			
			color = GlObj::BLACK;
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	Y0, 
						   X0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	Y0 + dn,							1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	Y0,							
						   X0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	Y0 + dn,							1, color);
			
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	Y0 + dn/10,				
						   X0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	Y0 + dn/10,						1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1 - dn/10,	Y0 + dn - dn/10,		
						   X0 + (double)elemerk.get_abstAbs()/f1 - dn/ 5,	Y0 + dn - dn/10,	1, color);
		}
		break;

	case ABG:

		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90) 
		{
			color = GlObj::UNBEKANNT;													
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							1, color);							
			
			color = GlObj::BLACK;														
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							1, color);							
			
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 + dn/10, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 + dn/10,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2 - dn/10, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2 - dn/10,
							1, color);
		} 
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90) 
		{
			color = GlObj::UNBEKANNT;													
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2,
							1, color);							
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							1, color);							
			
			color = GlObj::BLACK;														
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							1, color);							
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2,
							1, color);							
			
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 + dn/10, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 - dn/2 + dn/10,
							1, color);							
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2 - dn/10, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1 + dn/2 - dn/10,
							1, color);							
		} 
		// rechts und Rohr nach unten										
		else if (winkel ==   0 && drehfaktor == 270)
		{	
			color = GlObj::UNBEKANNT;
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,
							1, color);							
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							1, color);							
			color = GlObj::BLACK;
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							1, color);							
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							1, color);
			
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10,
							1, color);							
			add_line(X0 + andn + merkerRohrele.get_laenge() - dn/10,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10, 
						   X0 + andn + merkerRohrele.get_laenge() - dn/ 5,		Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10,
							1, color);
		} 
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270)
		{
			color = GlObj::UNBEKANNT;													
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							1, color);							
			color = GlObj::BLACK;
			
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10,
							1, color);							
			add_line(X0 - merkerRohrele.get_laenge() + dn/10,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10, 
						   X0 - merkerRohrele.get_laenge() + dn/ 5,					Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10,
							1, color);
		} 
		// unten und Rohr nach links 
		else if (winkel == 0 && drehfaktor == 180) 
		{
			color = GlObj::UNBEKANNT;													
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 - merkerRohrele.get_laenge() + dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 - merkerRohrele.get_laenge() + dn/ 5,
							1, color);							
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 - merkerRohrele.get_laenge() + dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 - merkerRohrele.get_laenge() + dn/ 5,
							1, color);
			color = GlObj::BLACK;
			
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 - merkerRohrele.get_laenge() + dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 - merkerRohrele.get_laenge() + dn/10,
							1, color);							
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 - merkerRohrele.get_laenge() + dn/ 5, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 - merkerRohrele.get_laenge() + dn/ 5,
							1, color);							
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10,	Y0 - merkerRohrele.get_laenge() + dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10,	Y0 - merkerRohrele.get_laenge() + dn/ 5,
							1, color);							
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10,	Y0 - merkerRohrele.get_laenge() + dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10,	Y0 - merkerRohrele.get_laenge() + dn/ 5,
							1, color);
		} 
		// Nut oben und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180) 
		{
			color = GlObj::UNBEKANNT;													
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/ 5,
							1, color);							
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/ 5,
							1, color);
			color = GlObj::BLACK;
			
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/10,
								1, color);						
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/ 5, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2,				Y0 + andn + merkerRohrele.get_laenge() - dn/ 5,
							1, color);							
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10,	Y0 + andn + merkerRohrele.get_laenge() - dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 - dn/2 + dn/10,	Y0 + andn + merkerRohrele.get_laenge() - dn/ 5,
								1, color);						
			add_line(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10,	Y0 + andn + merkerRohrele.get_laenge() - dn/10, 
						   X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1 + dn/2 - dn/10,	Y0 + andn + merkerRohrele.get_laenge() - dn/ 5,
							1, color);
		} 
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0) 
		{
			color = GlObj::UNBEKANNT;
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/10, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/ 5,
							1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/10, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/ 5,
							1, color);
			color = GlObj::BLACK;
			
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/10, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/10,
							1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/ 5, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/ 5,
							1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 + dn/10, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/10, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 + dn/10, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/ 5,
							1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2 - dn/10, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/10, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2 - dn/10, Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge() + dn/ 5,
							1, color);
		} 
		// Nut oben und Rohr nach rechts
		else if (winkel == 0 && drehfaktor == 0) 
		{
			color = GlObj::UNBEKANNT;
			x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2;
			x1 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2;
			y0 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/10;
			y1 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/ 5;
			add_line(x0, y0, x1, y1, 1, color);																						
			
			x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2;
			x1 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2;
			y0 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/10;
			y1 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/ 5;
			add_line(x0, y0, x1, y1, 1, color);																						
	
			color = GlObj::BLACK;
			
			x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2;
			x1 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2;
			y0 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/10;
			y1 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/10;
			add_line(x0, y0, x1, y1, 1, color);																						
			
			x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2;
			x1 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2;
			y0 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/ 5;
			y1 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/ 5;
			add_line(x0, y0, x1, y1, 1, color);																						
			
			x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 + dn/10;
			x1 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 + dn/10;			
			y0 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/10;			
			y1 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/ 5;			
			add_line(x0, y0, x1, y1, 1, color);																						
			
			x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2 - dn/10;
			x1 = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2 - dn/10;
			y0 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/10;
			y1 = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() - dn/ 5;
			add_line(x0, y0, x1, y1, 1, color);																						
		}
		break;
	}
}

