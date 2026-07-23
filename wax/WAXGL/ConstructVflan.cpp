#include "stdafx.h"


#include "constructvflan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructVflan::~ConstructVflan()
{

}

ConstructVflan::ConstructVflan()
{

}

void ConstructVflan::createBeschriftung()
{
	switch (typ) 
	{
	case IMROHR:
		if (elemerk.get_id() == VFL10) 
			text = "VFL10";
		else if (elemerk.get_id() == VFL10NEU) 
			text = "VFL10Neu";
		else if (elemerk.get_id() == VFL16NEU) 
			text = "VFL16Neu";
		else if (elemerk.get_id() == VFL16) 
			text = "VFL16";
		else if (elemerk.get_id() == FLANSCH_COII) 
			text = "FL";
		else
			text = "AFL";
		break;
	case ABG:
		if (elemerk.get_id() == VFL10) 
			text = "+VFL10";
		else if (elemerk.get_id() == VFL10) 
			text = "+VFL10";
		else if (elemerk.get_id() == VFL16) 
			text = "+VFL16";
		else if (elemerk.get_id() == VFL10NEU) 
			text = "+VFL10";
		else if (elemerk.get_id() == VFL16NEU) 
			text = "+VFL16";
		else 
			text = "+AFL";
		break;
	}
	ConstructEndEle::createBeschriftung();
}

void ConstructVflan::createElement()
{
	ConstructEndEle::createElement();
	
	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	double x1 = 0, y1 = 0;
	double x2 = 0, y2 = 0;
	double x = 0, y = 0;
	double dx = 0, dy = 0;

	switch (typ) 
	{
	case IMROHR:
		// V-Flansch links
		if (erstesElement) 
		{
			x1 = X0;
			y1 = Y0;
			x2 = X0 - elemerk.get_laenge()/f1*8/10;
			y2 = Y0 - dn/10;
			add_line(x1, y1, x2,	y2, 1, color);

			x1 = X0;
			y1 = Y0 + dn;
			x2 = X0 - elemerk.get_laenge()/f1*8/10;
			y2 = Y0 + dn + dn/10;
			add_line(x1, y1, x2,	y2, 1, color);
			
			x     = X0 - elemerk.get_laenge()/f1;
			y     = Y0 - dn/5;
			dx    = elemerk.get_laenge()/f1*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		} 
		// V-Flansch links
		else if (winkel == 180) 
		{
			add_line(X0 - elemerk.get_abstAbs(),									Y0, 
						   X0 - elemerk.get_abstAbs() - elemerk.get_laenge()/f1*8/10,	Y0 - dn/10,
						   1, color);
			add_line(X0 - elemerk.get_abstAbs(),									Y0 + dn, 
						   X0 - elemerk.get_abstAbs() - elemerk.get_laenge()/f1*8/10,	Y0 + dn + dn/10,
						   1, color);
			x     = X0 - elemerk.get_abstAbs() - elemerk.get_laenge()/f1;
			y     = Y0 - dn/5;
			dx    = elemerk.get_laenge()/f1*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		} 
		// V-Flansch oben 
		else if (winkel == 90) 
		{ 
			add_line(X0,											Y0 + elemerk.get_abstAbs(), 
						   X0 - dn/10,					Y0 + elemerk.get_abstAbs() + elemerk.get_laenge()/f1*8/10,
						   1, color);
			add_line(X0 + dn,						Y0 + elemerk.get_abstAbs(), 
						   X0 + dn + dn/10,	Y0 + elemerk.get_abstAbs() + elemerk.get_laenge()/f1*8/10,
						   1, color);
			x     = X0 - dn/5;
			y     = Y0 + elemerk.get_abstAbs() + elemerk.get_laenge()/f1*8/10;
			dx    = dn + dn/5*2;
			dy    = elemerk.get_laenge()/f1*2/10;
			add_rectangle(x, y, dx, dy, color);
		} 
		// V-Flansch unten
		else if (winkel == 270) 
		{ 
			add_line(X0,											Y0 - elemerk.get_abstAbs(), 
						   X0 - dn/10,					Y0 - elemerk.get_abstAbs() - elemerk.get_laenge()/f1*8/10,
						   1, color);
			add_line(X0 + dn,						Y0 - elemerk.get_abstAbs(), 
						   X0 + dn + dn/10,	Y0 - elemerk.get_abstAbs() - elemerk.get_laenge()/f1*8/10,
						   1, color);
			x     = X0 - dn/5;
			y     = Y0 - elemerk.get_abstAbs() - elemerk.get_laenge()/f1*8/10;
			dx    = dn + dn/5*2;
			dy    = -elemerk.get_laenge()/f1*2/10;
			add_rectangle(x, y, dx, dy, color);
		} 
		// V-Flansch rechts
		else if (winkel == 0) 
		{ 
			add_line(X0 + (double)elemerk.get_abstAbs()/f1,								Y0, 
						   X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1*8/10, Y0 - dn/10,
						   1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1, 								Y0 + dn, 
						   X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1*8/10, Y0 + dn + dn/10,
						   1, color);
			x     = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1*8/10;
			y     = Y0 - dn/5;
			dx    = elemerk.get_laenge()/f1*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		}
		break;

	case ABG:

		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90)
		{
			add_line(	X0 - merkerRohrele.get_laenge(),								Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge - dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,	Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge - dn/2 - dn/10,
							1, color);
			
			add_line(	X0 - merkerRohrele.get_laenge(),								Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge + dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,	Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge + dn/2 + dn/10,
							1, color);
			
			x     = X0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10;
			y     = Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge - dn/2 - dn/5;
			dx    = -elemerk.get_laenge()*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		} 
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90) 
		{
			add_line(	X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(),								Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge - dn/2, 
							X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,	Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge - dn/2 - dn/10,
							1, color);
			
			add_line(	X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(),								Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge + dn/2, 
							X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,	Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge + dn/2 + dn/10,
							1, color);
			
			x     = X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10;
			y     = Y0 + elemerk.get_abstAbs() - lEndeLaenge - laenge - dn/2 - dn/5;
			dx    = elemerk.get_laenge()*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		} 
		// rechts und Rohr nach unten
		else if (winkel ==   0 && drehfaktor == 270)
		{
			add_line(	X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(),								Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2, 
							X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,	Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/10,
							1, color);
			
			add_line(	X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(),								Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2,		
							X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,	Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2 + dn/10,
							1, color);
			
			x     = X0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10;
			y     = Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/5;
			dx    = elemerk.get_laenge()*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		}
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270)
		{
			add_line(	X0 - merkerRohrele.get_laenge(),								Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,	Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/10,
							1, color);
			
			add_line(	X0 - merkerRohrele.get_laenge(),								Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2, 
							X0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,	Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2 + dn/10,
							1, color);
			
			x     = X0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10;
			y     = Y0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/5;
			dx    = -elemerk.get_laenge()*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
		}
		// oben und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180)
		{ 
			add_line(X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2,						Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(), 
						   X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/10,	Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,
						   1, color);
			add_line(X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2,						Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(), 
						   X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2 + dn/10,	Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,
						   1, color);
			x     = X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/5;
			y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10;
			dx    = dn + dn/5*2;
			dy    = elemerk.get_laenge()*2/10;
			add_rectangle(x, y, dx, dy, color);
		}
		// unten und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180)
		{ 
			add_line(X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2,						Y0 - merkerRohrele.get_laenge(), 
						   X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/10,	Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,
						   1, color);
			add_line(X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge + dn/2,						Y0 - merkerRohrele.get_laenge(), 
						   X0 - elemerk.get_abstAbs() - lEndeLaenge + laenge + dn/2 + dn/10,	Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,
						   1, color);
			x     = X0 - elemerk.get_abstAbs() + lEndeLaenge + laenge - dn/2 - dn/5;
			y     = Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge();
			dx    = dn + dn/5*2;
			dy    = elemerk.get_laenge()*2/10;
			add_rectangle(x, y, dx, dy, color);
		} 
		// oben und Rohr nach rechts
		else if (winkel == 0 && drehfaktor == 0)
		{ 
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2,							Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 - dn/10,	Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,
						   1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2,							Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2 + dn/10,	Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10,
						   1, color);
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 - dn/5;
			y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge() + elemerk.get_laenge()*8/10;
			dx    = dn + dn/5*2;
			dy    = elemerk.get_laenge()*2/10;
			add_rectangle(x, y, dx, dy, color);

			if(elemerk.get_id() == FLANSCH_COII)
			{
				CString strText;
				strText.Format("DN %dx%d", merkerRohrele.get_dn(), merkerRohrele.get_laenge());
				add_text(x - 25,
							(Y0 + 2 * andn) + ((y+dy) - (Y0 + 2 * andn)) /2, 
						  strText, color, true, 90);
				
				add_line( x-15, y + dy, x-dx+15, y + dy,	
							1, color);
			}
		}
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0) 
		{ 
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2,							Y0 - merkerRohrele.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 - dn/10,	Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,
						   1, color);
			add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2,							Y0 - merkerRohrele.get_laenge(), 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn/2 + dn/10,	Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge()*8/10,
						   1, color);
			x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn/2 - dn/5;
			y     = Y0 - merkerRohrele.get_laenge() - elemerk.get_laenge();
			dx    = dn + dn/5*2;
			dy    = elemerk.get_laenge()*2/10;
			add_rectangle(x, y, dx, dy, color);
		}
		break;
	}
}

