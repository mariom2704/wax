#include "stdafx.h"


#include "constructendele.h"
#include "bemassung.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructEndEle::~ConstructEndEle()
{

}

ConstructEndEle::ConstructEndEle()
{

}

void ConstructEndEle::createBemassung()
{
	double f1 = faktorRohrlaenge;
	double f3 = faktorElelaenge; 

	double ebene1 = 3 + (maxRohrDN + maxlaenge_0*f3)/Bemassung::EBENENABSTAND;
	double ebene2 = (maxlaenge_180*f3>0)? (3 + (maxlaenge_180*f3)/Bemassung::EBENENABSTAND) : 3;
	double ebene3 = 3 + (maxRohrDN + maxlaenge_0*f3)/Bemassung::EBENENABSTAND;

	if (rohrleitung && rohrleitung->isGewinderohr())
	{
		ebene1 = 1;
		ebene2 = 1;
		ebene3 = 1;
	}
	double x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	double ebene = 0;
	bool nachoben = false, vertikal = false;
	bool fillbemassung = false;

	switch (typ) 
	{
	case IMROHR:
		// Rohr nach oben
		if (drehfaktor == 90) 
		{
			x0		= X0;
			y0		= Y0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel())/f1; 
			x1		= X0;
			y1		= Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1; 
			ebene	= ebene2;
			nachoben= true;
			vertikal= true;
			fillbemassung = true;
		} 
		// rechts und Rohr nach unten
		else if (drehfaktor == 270)
		{
			x0		= X0;
			y0		= Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1; 
			x1		= X0;
			y1		= Y0 + (double)(- elemerk.get_abstAbs() + laenge + elemerk.get_abstRel())/f1; 
			ebene	= ebene3; 
			nachoben= true; 
			vertikal= true;
			fillbemassung = true;
		}
		// Rohr nach rechs
		else if (drehfaktor == 0) 
		{
			if  ((merkerRohrele.get_abstAbs() > laenge) && (elemerk.get_abstAbs() > laenge)) 
			{	
				x0		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge + elemerk.get_abstRel())/f1;
				y0		= Y0; 
				x1		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
				y1		= Y0;
				ebene	= ebene1;
				nachoben= true; 
				vertikal= false;
				fillbemassung = true;
			} 
			else if (merkerRohrele.get_abstAbs() > laenge) 
			{
				x0		= X0 - (double)(lEndeLaenge - merkerRohrele.get_abstAbs() + laenge)/f1;					
				y0		= Y0;
				x1		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
				y1		= Y0;
				ebene	= ebene1;
				nachoben= true;
				vertikal= false;
				fillbemassung = true;
			}
			else if ((elemerk.get_abstAbs() > laenge) && (merkerRohrele.get_abstAbs() == 0 || merkerRohrele.get_abstAbs() < laenge)) 
			{
				if (X0 == 0)	
				{
					x0		= X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel() - laenge + merkerRohrele.get_abstAbs())/f1;
					y0		= Y0;
					x1		= X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs())/f1;
					y1		= Y0;
					ebene	= ebene1;
					nachoben= true;
					vertikal= false;
					fillbemassung = true;
				}
				else 
				{
					x0		= ((X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1) < 0)?0:X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1;
					y0		= Y0;
					x1		= X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs())/f1;
					y1		= Y0;
					ebene	= ebene1;
					nachoben= true;
					vertikal= false;
					fillbemassung = true;
				}
			}
			else if (elemerk.get_abstAbs() > laenge)	
			{
				x0		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge + merkerRohrele.get_abstRel())/f1;
				y0		= Y0;
				x1		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
				y1		= Y0;
				ebene	= ebene1;
				nachoben= true;
				vertikal= false;
				fillbemassung = true;
			}
			else 
			{ 
				x0		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1;
				y0		= Y0;
				x1		= X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
				y1		= Y0;
				ebene	= ebene1;
				nachoben= true;
				vertikal= false;
				fillbemassung = true;
			}
		}
		// Rohr nach links	
		else if (drehfaktor == 180 )
		{
			x0		= X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1;
			y0		= Y0;
			x1		= X0 + (double)(- elemerk.get_abstAbs() + laenge + elemerk.get_abstRel())/f1;
			y1		= Y0;
			ebene	= ebene1;
			nachoben= true;
			vertikal= false;
			fillbemassung = true;
		}
		break;
	}
	XASSERT(fillbemassung);

	if (fillbemassung)
		add_dimensioning(	x0,	y0, x1, y1,	(int)ebene, nachoben, vertikal);
	else
		TRACE("Warnung: ElementID = %d konnte nicht bemasst werden!", elemerk.get_id());
}

void ConstructEndEle::createBeschriftung()
{
	if (text.GetLength() == 0)
		return;

	int dx = 500;
	if (dynamic_cast<const Gewinderohr*>(rohrleitung))
		dx = 200;

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	double x=0, y=0;

	if (typ == IMROHR)
	{
		// Flansch am Anfang, jetzt bei Rohrbeschriftung gesetzt
		if (erstesElement)
		{
			x = X0 - dx;
			y = Y0;
		}
		// Flansch links
		else if (winkel == 180) 
		{
			x = X0 - elemerk.get_abstAbs() - elemerk.get_laenge() - 400;
			y = Y0;
		}
		// Flansch oben 
		else if (winkel == 90) 
		{
			x = X0;
			y = Y0 + elemerk.get_abstAbs() + 100;
		}
		// Flansch unten
		else if (winkel == 270)
		{
			x = X0;
			y = Y0 - elemerk.get_abstAbs() - 100;
		}
		// Flansch rechts	
		else if (winkel == 0) 
		{
			x = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1 + 100;
			y = Y0;
		}
		add_text(x, y, text, color, false, 360);

	}
}

void ConstructEndEle::createElement()
{
}

void ConstructEndEle::initEleDaten(const Element& ele)
{
	if (typ == IMROHR)
	{
		ConstructElement::initEleDaten(ele);
		return;
	}
	elemerk.copy(ele);
	
	// Winkel setzen
	if (elemerk.get_winkel() >= 0 && elemerk.get_winkel() <= 89)
		winkel = 0;
	else if (elemerk.get_winkel() >= 90 && elemerk.get_winkel() <= 179)
		winkel = 90;
	else if (elemerk.get_winkel() >= 180 && elemerk.get_winkel() <= 269)
		winkel = 180;
	else if (elemerk.get_winkel() >= 270 && elemerk.get_winkel() <= 359)
		winkel = 270;

	// Nennweiten anpassen
	andn = merkerAbgele.get_anschlussDN();
	andn = (int)((double)andn * faktorRohrbreite);
	dn = andn/2;
	andn = dn;
}
