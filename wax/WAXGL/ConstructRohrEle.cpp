#include "stdafx.h"


#include "constructrohrele.h"
#include "bemassung.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructRohrEle::ConstructRohrEle()
{

}

ConstructRohrEle::~ConstructRohrEle()
{

}

void ConstructRohrEle::createBemassung()
{
	double f1 = faktorRohrlaenge, ebene1 = 0.0;
	double x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	double ebene = 0;
	bool nachoben = false;
	bool vertikal = false;

	// Bemassungsebene bestimmen
	XASSERT(maxRohrDN > 0);
	//rohrleitung->isRohrstutzen(): 1=ohne Ende, 2=mit Ende
	int value = (rohrleitung->isRohrstutzen(drehfaktor) == 2) ? 6: (rohrleitung->isRohrstutzen() == 1) ? 5 : 5;	

	if (rohrleitung->isRohrstutzen(drehfaktor) > 0)
	{
		if (rohrleitung->get_laenge() > 4000)
			value += 2;
		if (rohrleitung->get_laenge() > 5000)
			value += 2;
		if (rohrleitung->get_laenge() > 6000)
			value++;
		if (rohrleitung->get_laenge() > 8000)
			value++;
	}
	
	if (drehfaktor == 0) 
		ebene1 = value + (double)(maxRohrDN + maxlaengeAbgOben)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 90) 
		ebene1 = value + (double)(maxRohrDN + maxlaengeAbgRechts)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 180) 
		ebene1 = value + (double)(maxRohrDN + maxlaengeAbgRechts)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 270) 
		ebene1 = value + (double)(maxRohrDN + maxlaengeAbgOben)/Bemassung::EBENENABSTAND;

	// immer aufrunden
	int e = (int)ebene1/1;
	if (e<ebene1)
		ebene1 += 1;

	const Gewinderohr* grohr = dynamic_cast<const Gewinderohr*>(rohrleitung);
	if (grohr)
	{
		ebene1 = 3;
	}

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
			ebene	= ebene1;
			nachoben= true;
			vertikal= true;
		} 
		// Rohr nach unten
		else if (drehfaktor == 270)
		{
			x0		= X0;
			y0		= Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1;
			x1		= X0;
			y1		= Y0 + (double)(- elemerk.get_abstAbs() + laenge + elemerk.get_abstRel())/f1;
			ebene	= ebene1;
			nachoben= true;
			vertikal= true;
		}
		// Rohr nach rechs
		else if (drehfaktor == 0 ) 
		{
			if ((merkerRohrele.get_abstAbs() > laenge) && (elemerk.get_abstAbs() > laenge)) 
			{	
				x0		= X0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel() - lEndeLaenge )/f1;			
				y0		= Y0;			
				x1		= X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge )/f1;			
				y1		= Y0;			
				ebene	= ebene1;			
				nachoben= true;			
				vertikal= false;			
			} 
			else if (merkerRohrele.get_abstAbs() > laenge) 
			{
				x0		= X0 + (double)(-lEndeLaenge + merkerRohrele.get_abstAbs() - laenge)/f1;
				y0		= Y0;			
				x1		= X0 + (double)(-lEndeLaenge + elemerk.get_abstAbs() - laenge)/f1;				
				y1		= Y0;				
				ebene	= ebene1;			
				nachoben= true;			
				vertikal= false;			
			}
			else if (elemerk.get_abstAbs() > laenge)
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
				}
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
		}
		break;
	}
	add_dimensioning(	x0, y0, x1, y1, (int)ebene, nachoben, vertikal);
}



