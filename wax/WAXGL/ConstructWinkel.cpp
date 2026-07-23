#include "stdafx.h"


#include "constructwinkel.h"
#include "globj.h"
#include "kreis.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructWinkel::~ConstructWinkel()
{
}

ConstructWinkel::ConstructWinkel()
{
}
 
void ConstructWinkel::createBeschriftung()
{
	if (elemerk.get_firstele() || elemerk.get_lastele())
	{
		int dn = (elemerk.get_dn2() > 0) ? elemerk.get_dn2() : elemerk.get_dn1();
		if (!dn2zoll)
			text.Format("%d", dn);
		else
			text.Format("%s''", WaxStamm::dn2zoll(dn));
	}

	switch (typ) 
	{
	case IMROHR:
		if (drehfaktor == 0 && bogen)
			add_text(	X0 + elemerk.get_abstRel() + rohrDn + 50,					
								Y0 + rohrDn, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 0)
			add_text(	X0 - lEndeLaenge + elemerk.get_abstRel() + rohrDn + 50,	
								Y0, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 180)
			add_text(	X0 - elemerk.get_abstRel() - 50,					
								Y0 + rohrDn, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 90 && elemerk.get_winkel() == 0) 
			add_text(	X0 + rohrDn + 50,		
								Y0 + elemerk.get_abstRel(), 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 90 && elemerk.get_winkel() == 180) 
			add_text(	X0 - 50,		
								Y0 + elemerk.get_abstRel(), 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 270 && elemerk.get_winkel() == 180) 
			add_text(	X0 + rohrDn + 50,		
								Y0 - elemerk.get_abstRel(), 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 270 && elemerk.get_winkel() == 0) 
			add_text(	X0 - 150,		
								Y0 - elemerk.get_abstRel(), 
								text, GlObj::BLACK, false, 45);
		break;
	}
	if (elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270)
	{
		CString winkel;
		winkel.Format(" %d'", elemerk.get_winkel());
		text += winkel;
	}

	set_showText(false);
	ConstructFitting::createBeschriftung();
}

void ConstructWinkel::createElement()
{
	ConstructFitting::createElement();

	if (elemerk.get_dn1() > elemerk.get_dn2())
		andn = (int)(elemerk.get_dn1()*1.2);
	else
		andn = (int)(elemerk.get_dn2()*1.2);

	GlObj::colour color = GlObj::BLACK;

	// Punkte
	double x = 0, y = 0;
	// Kreis
	int durchmesser = 0, start = 0, end = 0;
	bool tilt = false;
	/*  
	//Wie Bogen
	switch (elemerk.get_winkel())
	{
	case 0: 
		drawBogenNachOben(drehfaktor);
		break;
	case 90: 
		drawBogenDummy(drehfaktor, true);
		break;
	case 270: 
		drawBogenDummy(drehfaktor, false);
		break;
	case 180:
		drawBogenNachUnten(drehfaktor);
		break;
	default:
		XASSERT(false);
	}
	return;
	*/
	switch (typ) 
	{
	case IMROHR:
		// unten und Bogen nach rechts
		if (elemerk.get_winkel() == 0 && drehfaktor == 270) 
		{
			add_line(X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn,
						   1, color);
			add_line(X0 - andn/10 +   andn,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - 2*andn, 
						   X0 - andn/10 + 2*andn,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - 2*andn, 
						   1, color);
		
			add_line(X0 - andn/10 + andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   X0 - andn/10 + andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn/2,
						   1, color);
			add_line(X0 - andn/10 + 1.5*andn,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn, 
						   X0 - andn/10 + 2*andn,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn, 
						   1, color);
			
			add_line(X0 - andn/10 + 2*andn,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() -   andn, 
						   X0 - andn/10 + 2*andn,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - 2*andn, 
						   1, color);
			
			add_line(X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   X0 + andn*9/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   1, color);

			durchmesser = 2*andn;
			start = Kreis::GRAD_180;
			end   = Kreis::GRAD_270;
			x     = X0 - andn/10 + andn;
			y     = Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn;
			add_circle(x, y, durchmesser, start, end, tilt);

			durchmesser = andn;
			start = Kreis::GRAD_180;
			end   = Kreis::GRAD_270;
			x     = X0 - andn/10 + andn + andn/2;
			y     = Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn/2;
			add_circle(x, y, durchmesser, start, end, tilt);
		} 
		// unten und Bogen nach links
		else if (elemerk.get_winkel() == 180 && drehfaktor == 270) 
		{
			add_line(X0 - andn/10 + andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   X0 - andn/10 + andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge() -   andn,
						   1, color);
			add_line(X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - 2*andn, 
						   X0 - andn/10 - andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - 2*andn, 
						   1, color);
		
			add_line(X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn/2,
						   1, color);
			add_line(X0 - andn/10 - andn/2,	Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn, 
						   X0 - andn/10 - andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn, 
						   1, color);
			
			add_line(X0 - andn/10 - andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - 2*andn, 
						   X0 - andn/10 - andn,		Y0 - elemerk.get_abstRel() + elemerk.get_laenge() -   andn, 
						   1, color);
			
			add_line(X0 - andn/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   X0 + andn*9/10,			Y0 - elemerk.get_abstRel() + elemerk.get_laenge(), 
						   1, color);
			
			durchmesser = 2*andn;
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_0;
			x     = X0 - andn/10;
			y     = Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn;
			add_circle(x, y, durchmesser, start, end, tilt);

			durchmesser = andn;
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_0;
			x     = X0 - andn/10 - andn/2;
			y     = Y0 - elemerk.get_abstRel() + elemerk.get_laenge() - andn/2;
			add_circle(x, y, durchmesser, start, end, tilt);
	
		} 
		// oben und Bogen nach links
		else if (elemerk.get_winkel() == 0 && drehfaktor == 90) 
		{
			add_line(X0 - andn/10 + andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 - andn/10 + andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn,
						   1, color);
			add_line(X0,						Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn, 
						   X0 - andn/10 - andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn, 
						   1, color);
		
			add_line(X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2,
						   1, color);
			add_line(X0 - andn/10 - andn/2,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn, 
						   X0 - andn/10 - andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn, 
						   1, color);
			
			add_line(X0 - andn/10 - andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() +   andn, 
						   X0 - andn/10 - andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn, 
						   1, color);
			
			add_line(X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 + andn*9/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   1, color);

			durchmesser = 2*andn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_90;
			x     = X0 - andn/10;
			y     = Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn;
			add_circle(x, y, durchmesser, start, end, tilt);

			durchmesser = andn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_90;
			x     = X0 - andn/10 - andn/2;
			y     = Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2;
			add_circle(x, y, durchmesser, start, end, tilt);
		} 
		// oben und Bogen nach rechts
		else if (elemerk.get_winkel() == 180 && drehfaktor == 90) 
		{
			add_line(X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge() +   andn,
						   1, color);
			add_line(X0 - andn/10 +   andn,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn, 
						   X0 - andn/10 + 2*andn,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn, 
						   1, color);
		
			add_line(X0 - andn/10 + andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 - andn/10 + andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2,
						   1, color);
			add_line(X0 - andn/10 + 1.5*andn,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn, 
						   X0 - andn/10 + 2*andn,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn, 
						   1, color);
			
			add_line(X0 - andn/10 + 2*andn,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn, 
						   X0 - andn/10 + 2*andn,	Y0 + elemerk.get_abstRel() - elemerk.get_laenge() +   andn, 
						   1, color);
			
			add_line(X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 + andn*9/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   1, color);

			durchmesser = 2*andn;
			start = Kreis::GRAD_90;
			end   = Kreis::GRAD_180;
			x     = X0 - andn/10 + andn;
			y     = Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn;
			add_circle(x, y, durchmesser, start, end, tilt);

			durchmesser = andn;
			start = Kreis::GRAD_90;
			end   = Kreis::GRAD_180;
			x     = X0 - andn/10 + andn + andn/2;
			y     = Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2;
			add_circle(x, y, durchmesser, start, end, tilt);
		} 
		// oben und Bogen nach vorne/hinten
		else if ((elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) && drehfaktor == 90) 
		{
			add_line(X0 - andn/10 + andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge(),
						   X0 - andn/10 + andn,		Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn*3/2,
						   1, color);
			add_line(X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge(), 
						   X0 - andn/10,			Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn*3/2,
						   1, color);
		
			durchmesser = andn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 - andn/10 + andn/2;
			y     = Y0 + elemerk.get_abstRel() - elemerk.get_laenge() + andn*3/2;
			add_circle(x, y, durchmesser, start, end, elemerk.get_winkel() == 90 ? true : false);
		} 
		// rechts und Bogen nach oben
		else if (elemerk.get_winkel() == 0 && drehfaktor == 0) 
		{
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),			Y0 - andn/10, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() +   andn,	Y0 - andn/10,
						   1, color);
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn,	Y0 - andn/10 +   andn, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn,	Y0 - andn/10 + 2*andn, 
						   1, color);
		
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),			Y0 - andn/10 + andn, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2,	Y0 - andn/10 + andn,
						   1, color);
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn,	Y0 - andn/10 + andn+andn/2, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn,	Y0 - andn/10 + 2*andn, 
						   1, color);
			
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn,	Y0 - andn/10 + 2*andn, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() +   andn,	Y0 - andn/10 + 2*andn, 
						   1, color);
			
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),			Y0 - andn/10, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),			Y0 + andn*9/10, 
						   1, color);
			
			durchmesser = 2*andn;
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_0;
			x     = X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn;
			y     = Y0 - andn/10 + andn;
			add_circle(x, y, durchmesser, start, end, tilt);

			durchmesser = andn;
			start = Kreis::GRAD_270;
			end   = Kreis::GRAD_0;
			x     = X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2;
			y     = Y0 - andn/10 + andn + andn/2;
			add_circle(x, y, durchmesser, start, end, tilt);
		}
		// rechts und Bogen nach unten 
		else if (elemerk.get_winkel() == 180 && drehfaktor == 0) 
		{
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),								Y0 - andn/10 + andn, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() +   andn,Y0 - andn/10 + andn,
						   1, color);
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn,Y0 - andn/10, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn,Y0 - andn/10 - andn, 
						   1, color);
		
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),								Y0 - andn/10, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2,Y0 - andn/10,
						   1, color);
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn,	Y0 - andn/10 - andn/2, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn,	Y0 - andn/10 - andn, 
						   1, color);
			
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + 2*andn,Y0 - andn/10 - andn, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() +   andn,Y0 - andn/10 - andn, 
						   1, color);
		
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),								Y0 - andn/10, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),								Y0 + andn*9/10, 
						   1, color);
		
			durchmesser = 2*andn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_90;
			x     = X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn;
			y     = Y0 - andn/10;
			add_circle(x, y, durchmesser, start, end, tilt);

			durchmesser = andn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_90;
			x     = X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn/2;
			y     = Y0 - andn/10 - andn/2;
			add_circle(x, y, durchmesser, start, end, tilt);
		} 
		// rechts und Bogen nach vorne/hinten 
		else if ((elemerk.get_winkel() == 90 || elemerk.get_winkel() == 270) && drehfaktor == 0) 
		{
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),									Y0 - andn/10 + andn, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn*3/2,	Y0 - andn/10 + andn,
						   1, color);
			add_line(X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge(),									Y0 - andn/10, 
						   X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn*3/2,	Y0 - andn/10,
						   1, color);
		
			durchmesser = andn;
			start = Kreis::GRAD_0;
			end   = Kreis::GRAD_360;
			x     = X0 - lEndeLaenge + elemerk.get_abstRel() - elemerk.get_laenge() + andn*3/2;
			y     = Y0 - andn/10 + andn/2 ;
			add_circle(x, y, durchmesser, start, end, elemerk.get_winkel() == 90 ? true : false);
		} 
		break;
	}
}

// Bogen zeichnen mit Elementwinkel 0
void ConstructWinkel::drawBogenNachOben(int drehfaktor)
{
	int gradbegin = 0, gradend = 0;
	double rx0 = 0, ry0 = 0;
	double x0 = 0, x1 = 0;
	double y0 = 0, y1 = 0;

	double x = -lEndeLaenge + elemerk.get_abstAbs() - laenge - elemerk.get_laenge();
	double o1 = 1.0; 
	double o2 = 2.0; 

	switch (drehfaktor)
	{
		case 0:
			// Anfang
			x0 = X0 + x; 
			y0 = Y0;
			x1 = X0 + x;
			y1 = Y0 + o1*andn;
			add_line(x0, y0, x1, y1);
			
			// Ende	
			x0 = X0 + x + o1*andn; 
			y0 = Y0 + o2*andn;
			x1 = X0 + x + o2*andn;
			y1 = Y0 + o2*andn;
			add_line(x0, y0, x1, y1);

			// Kreise
			rx0 = X0 + x;
			ry0 = Y0 + o2*andn;
			gradbegin = Kreis::GRAD_270;
			gradend = Kreis::GRAD_0;

			break;
		case 90:
			// Anfang
			x0 = X0; 
			y0 = Y0 + x;
			x1 = X0 + o1*andn;
			y1 = Y0 + x;
			add_line(x0, y0, x1, y1);

			// Ende
			x0 = X0 - o1*andn; 
			y0 = Y0 + x + o1*andn;
			x1 = X0 - o1*andn;
			y1 = Y0 + x + o2*andn;
			add_line(x0, y0, x1, y1);
			
			// Kreise
			rx0 = X0 - o1*andn;
			ry0 = Y0 + x;
			gradbegin = Kreis::GRAD_0;
			gradend = Kreis::GRAD_90;
			break;
		case 180:
			// Anfang
			x0 = X0 - x; 
			y0 = Y0;
			x1 = X0 - x;
			y1 = Y0 + o1*andn;
			add_line(x0, y0, x1, y1);

			// Ende
			x0 = X0 - x - o1*andn; 
			y0 = Y0 - o1*andn;
			x1 = X0 - x - o2*andn;
			y1 = Y0 - o1*andn;
			add_line(x0, y0, x1, y1);

			// Kreise
			rx0 = X0 - x;  
			ry0 = Y0 - o1*andn;
			gradbegin = Kreis::GRAD_90;
			gradend = Kreis::GRAD_180;
			break;
		case 270:
			// Anfang
			x0 = X0; 
			y0 = Y0 - x;
			x1 = X0 + o1*andn;
			y1 = Y0 - x;
			add_line(x0, y0, x1, y1);

			// Ende
			x0 = X0 + o1*andn - o1*andn; 
			y0 = Y0 - x;
			x1 = X0 + o1*andn - o1*andn;
			y1 = Y0 - x + o1*andn;
			add_line(x0, y0, x1, y1);

			// Kreise
			ry0 = Y0 - x; 
			rx0 = X0 + o2*andn;
			gradbegin = Kreis::GRAD_180;
			gradend = Kreis::GRAD_270;
			break;
	}

	drawBogen(rx0, ry0, gradbegin, gradend);	
}

// Bogen zeichnen mit Elementwinkel 180
void ConstructWinkel::drawBogenNachUnten(int drehfaktor)
{
	int gradbegin = 0, gradend = 0;
	double rx0 = 0, ry0 = 0;
	double x0 = 0, x1 = 0;
	double y0 = 0, y1 = 0;

	double x = -lEndeLaenge + elemerk.get_abstAbs() - laenge - elemerk.get_laenge();
	double o1 = 1.0; 
	double o2 = 2.0; 

	switch (drehfaktor)
	{
		case 0:
			// Anfang
			x0 = X0 + x; 
			y0 = Y0;
			x1 = X0 + x;
			y1 = Y0 + o1*andn;
			add_line(x0, y0, x1, y1);
			
			// Ende	
			x0 = X0 + x + o1*andn; 
			y0 = Y0 - o1*andn;
			x1 = X0 + x + o2*andn;
			y1 = Y0 - o1*andn;
			add_line(x0, y0, x1, y1);

			// Kreise
			rx0 = X0 + x;
			ry0 = Y0 - o1*andn;
			gradbegin = Kreis::GRAD_0;
			gradend = Kreis::GRAD_90;

			break;
		case 90:
			// Anfang
			x0 = X0; 
			y0 = Y0 + x;
			x1 = X0 + o1*andn;
			y1 = Y0 + x;
			add_line(x0, y0, x1, y1);

			// Ende
			x0 = X0 + o2*andn; 
			y0 = Y0 + x + o1*andn;
			x1 = X0 + o2*andn;
			y1 = Y0 + x + o2*andn;
			add_line(x0, y0, x1, y1);
			
			// Kreise
			rx0 = X0 + o2*andn;
			ry0 = Y0 + x;
			gradbegin = Kreis::GRAD_90;
			gradend = Kreis::GRAD_180;
			break;
		case 180:
			// Anfang
			x0 = X0 - x; 
			y0 = Y0;
			x1 = X0 - x;
			y1 = Y0 + o1*andn;
			add_line(x0, y0, x1, y1);

			// Ende
			x0 = X0 - x - o1*andn; 
			y0 = Y0 + o2*andn;
			x1 = X0 - x - o2*andn;
			y1 = Y0 + o2*andn;
			add_line(x0, y0, x1, y1);

			// Kreise
			rx0 = X0 - x;  
			ry0 = Y0 + o2*andn;
			gradbegin = Kreis::GRAD_180;
			gradend = Kreis::GRAD_270;
			break;
		case 270:
			// Anfang
			x0 = X0; 
			y0 = Y0 - x;
			x1 = X0 + o1*andn;
			y1 = Y0 - x;
			add_line(x0, y0, x1, y1);

			// Ende
			x0 = X0 + o1*andn - o1*andn; 
			y0 = Y0 - x;
			x1 = X0 + o1*andn - o1*andn;
			y1 = Y0 - x;
			add_line(x0, y0, x1, y1);

			// Kreise
			rx0 = X0 - o1*andn;
			ry0 = Y0 - x; 
			gradbegin = Kreis::GRAD_270;
			gradend = Kreis::GRAD_0;
			break;
	}
	drawBogen(rx0, ry0, gradbegin, gradend);	
}

// Bogen zeichnen mit Elementwinkel 90 und 270 Grad
void ConstructWinkel::drawBogenDummy(int drehfaktor, bool filled)
{
	/*
	Muss noch  vereinfacht, erweitert und getestet werden!!!
	*/
	double x1 = 0, y1 = 0;
	double x2 = 0, y2 = 0; 
	double x  = 0, y  = 0;
	
	double value = - lEndeLaenge + elemerk.get_abstAbs() - laenge - elemerk.get_laenge();
	switch (drehfaktor)
	{
		case 0:
			{
			x1 = X0 + value;
			y1 = Y0 + andn;
			x2 = X0 + value + andn*3/2;
			y2 = Y0 + andn;
			add_line(x1, y1, x2, y2);

			x1 = X0 + value;
			y1 = Y0;
			x2 = X0 + value + andn*3/2;
			y2 = Y0;
			add_line(x1, y1, x2, y2);

			x = X0 + value +  andn*3/2;
			y = Y0 + andn/2;
			add_circle(x, y, andn, Kreis::GRAD_0, Kreis::GRAD_360, filled);
			}
			break;
		case 90:
			{
			y1 = Y0 + value;
			x1 = X0 + andn;
			y2 = Y0 + value + andn*3/2;
			x2 = X0 + andn;
			add_line(x1, y1, x2, y2);

			y1 = Y0 + value;
			x1 = X0;
			y2 = Y0 + value + andn*3/2;
			x2 = X0;
			add_line(x1, y1, x2, y2);

			y = Y0 + value + andn*3/2;
			x = X0 + andn/2;
			add_circle(x, y, andn, Kreis::GRAD_0, Kreis::GRAD_360, filled);
			}
			break;
		case 180:
			{
			x1 = X0 - value;
			y1 = Y0 + andn;
			x2 = X0 - value + andn*3/2;
			y2 = Y0 + andn;
			add_line(x1, y1, x2, y2);

			x1 = X0 - value;
			y1 = Y0;
			x2 = X0 - value + andn*3/2;
			y2 = Y0;
			add_line(x1, y1, x2, y2);

			x = X0 - value + andn*3/2;
			y = Y0 + andn/2;
			add_circle(x, y, andn, Kreis::GRAD_0, Kreis::GRAD_360, filled);
			}
			break;
		case 270:
			{
			y1 = Y0 - value;
			x1 = X0 + andn;
			y2 = Y0 - value +  andn*3/2;
			x2 = X0 + andn;
			add_line(x1, y1, x2, y2);

			y1 = Y0 - value;
			x1 = X0;
			y2 = Y0 - value +  andn*3/2;
			x2 = X0;
			add_line(x1, y1, x2, y2);

			y = Y0 - value +  andn*3/2;
			x = X0 + andn/2;
			add_circle(x, y, andn, Kreis::GRAD_0, Kreis::GRAD_360, filled);
			}
			break;
		default:
			XASSERT(false);
	}
}

// Allgemeinen Bogen zeichnen als zwei konzentrische Kreise
void ConstructWinkel::drawBogen(double x0, double y0, int gradbegin, int gradend)
{
	int r0 = 0;

	r0 = 2*andn;
	add_circle(x0, y0, r0, gradbegin, gradend, false);
	add_circle(x0, y0, 2*r0, gradbegin, gradend, false);
}


