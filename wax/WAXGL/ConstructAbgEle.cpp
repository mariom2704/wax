#include "stdafx.h"


#include "constructabgele.h"
#include "bemassung.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructAbgEle::~ConstructAbgEle()
{

}

ConstructAbgEle::ConstructAbgEle()
{
	avele = false;
	abstKnickbem = 2;
}


void ConstructAbgEle::createBemassung()
{
	
	COIIVerteiler* pCOIIVerteilerUsedAsBase =  dynamic_cast<COIIVerteiler*>(rohrleitung);
	
	if(pCOIIVerteilerUsedAsBase)
	{
		createCOIIMx1230u200Bemassung( pCOIIVerteilerUsedAsBase);
		return;
	}

	double f1 = faktorRohrlaenge, ebene = 0.0;
	double x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	bool nachoben = false;
	bool vertikal = false;

	// Bemassungsebene bestimmen
	XASSERT(maxRohrDN > 0);
	//rohrleitung->isRohrstutzen(): 1=ohne Ende, 2=mit Ende
	int value = (rohrleitung->isRohrstutzen(drehfaktor) == 2) ? 6: (rohrleitung->isRohrstutzen() == 1) ? 5 : 5;	

	if (rohrleitung->isRohrstutzen(drehfaktor) > 0)
	{
		if (rohrleitung->get_laenge() > 500 && rohrleitung->get_laenge() < 1000)
			value++;
		else if (rohrleitung->get_laenge() > 4000)
			value++;
		else if (rohrleitung->get_laenge() > 6000)
			value += 2;
	}
	
	//Bemassung eine Ebene nach oben verschieben durch zu geringen rel. Abstand 
	//Wenn mehrmals aufeinanderfolgend, dann im abwechselnd
	if ((elemerk.get_abstRel() < 300 && elemerk.get_abstRel() != 0 && merkerAbgele.get_abstRel() != 0 && rohrleitung->get_laenge() > 2000
	  || elemerk.get_abstRel() < 400 && elemerk.get_abstRel() != 0 && merkerAbgele.get_abstRel() != 0 && rohrleitung->get_laenge() > 4000
	  || elemerk.get_abstRel() < 500 && elemerk.get_abstRel() != 0 && merkerAbgele.get_abstRel() != 0 && rohrleitung->get_laenge() > 6000)
	  && !bemassungHochgestellt)
	{
		value++; 
		bemassungHochgestellt = true;
	}
	else
		bemassungHochgestellt = false;
	
	if (drehfaktor == 0) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgOben)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 90) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgRechts)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 180) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgRechts)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 270) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgOben)/Bemassung::EBENENABSTAND;

	// immer aufrunden
	int e = (int)ebene/1;
	if (e<ebene)
		ebene += 1;

	// Bemassung der AV-Elemente 1 Ebene hoeher
	if (avele)
		ebene += 2;

	if (elemerk.isKnick() && knickBemassung)
	{
		ebene += abstKnickbem;
		
		if (rohrleitung && rohrleitung->get_avelementArray().GetSize() > 0)
			ebene += 2;
	}

	// Rohr nach oben
	if (drehfaktor == 90) 
	{
		x0		= X0;
		y0		= Y0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel())/f1;
		x1		= X0;
		y1		= Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1;
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
		nachoben= true;		
		vertikal= true;		
	}
	// Rohr nach rechs
	else if (drehfaktor == 0 ) 
	{
		if  ((merkerRohrele.get_abstAbs() > laenge) && (elemerk.get_abstAbs() > laenge)) 
		{	
			x0		= X0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel() - lEndeLaenge)/f1;
			y0		= Y0;
			x1		= X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y1		= Y0;
			nachoben= true;
			vertikal= false;
		} 
		else if (merkerRohrele.get_abstAbs() > laenge) 
		{
			x0		= X0 + (double)(-lEndeLaenge + merkerRohrele.get_abstAbs() - laenge)/f1;
			y0		= Y0;			
			x1		= X0 + (double)(-lEndeLaenge + elemerk.get_abstAbs() - laenge)/f1;				
			y1		= Y0;				
			nachoben= true;			
			vertikal= false;			
		}
		else if (elemerk.get_abstAbs() > laenge)
		{
			if (X0 == 0)	
			{
				x0		= X0 + (double)(-lEndeLaenge - laenge + elemerk.get_abstAbs() - elemerk.get_abstRel() + laenge - merkerRohrele.get_abstAbs())/f1;	
				y0		= Y0;					
				x1		= X0 + (double)(-lEndeLaenge - laenge + elemerk.get_abstAbs())/f1;					
				y1		= Y0;					
				nachoben= true;					
				vertikal= false;					
			}
			else 
			{
				x0		= ((X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1) < 0)?0:X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1;	
				y0		= Y0;					
				x1		= X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs())/f1;					
				y1		= Y0;					
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
			nachoben= true;			
			vertikal= false;			
	}
	add_dimensioning(	x0, y0, x1, y1, (int)ebene, nachoben, vertikal);
}




void ConstructAbgEle::createCOIIMx1230u200Bemassung(COIIVerteiler* pVerteilerCOII)
{
	double f1 = faktorRohrlaenge, ebene = 0.0;
	double x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	bool nachoben = false;
	bool vertikal = false;

	// Bemassungsebene bestimmen
	XASSERT(maxRohrDN > 0);
	
	double  PointElementBefore = pVerteilerCOII->GetRohrPointElementBeforeWinkel();

	int value = (rohrleitung->isRohrstutzen(drehfaktor) == 2) ? 6: (rohrleitung->isRohrstutzen() == 1) ? 5 : 5;	

	if (rohrleitung->isRohrstutzen(drehfaktor) > 0)
	{
		if (rohrleitung->get_laenge() > 500 && rohrleitung->get_laenge() < 1000)
			value++;
		else if (rohrleitung->get_laenge() > 4000)
			value++;
		else if (rohrleitung->get_laenge() > 6000)
			value += 2;
	}
	
	
	value++;
	value++;
	bemassungHochgestellt = false;

	if(!pVerteilerCOII->GetFirstGraficAbgang())
	{
		if(pVerteilerCOII->GetHochgestellt())
		{
			value++;
			bemassungHochgestellt = true;
		}
	}
	else
	{
		pVerteilerCOII->SetFirstGraficAbgang(false);
		pVerteilerCOII->SetHochgestellt(false);
	}
	pVerteilerCOII->SetHochgestellt(!pVerteilerCOII->GetHochgestellt());
	
	if (drehfaktor == 0) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgOben)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 90) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgRechts)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 180) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgRechts)/Bemassung::EBENENABSTAND;
	else if (drehfaktor == 270) 
		ebene = value + (double)(maxRohrDN + maxlaengeAbgOben)/Bemassung::EBENENABSTAND;

	// immer aufrunden
	int e = (int)ebene/1;
	if (e<ebene)
		ebene += 1;

	// Bemassung der AV-Elemente 1 Ebene hoeher
	if (avele)
		ebene += 2;

	if (elemerk.isKnick() && knickBemassung)
	{
		ebene += abstKnickbem;
		
		if (rohrleitung && rohrleitung->get_avelementArray().GetSize() > 0)
			ebene += 2;
	}

	// Rohr nach oben
	if (drehfaktor == 90) 
	{
		x0		= X0;
		y0		= Y0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel())/f1;
		x1		= X0;
		y1		= Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1;
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
		nachoben= true;		
		vertikal= true;		
	}
	// Rohr nach rechs
	else if (drehfaktor == 0 ) 
	{
		if  ((merkerRohrele.get_abstAbs() > laenge) && (elemerk.get_abstAbs() > laenge)) 
		{	
			if(PointElementBefore > -1)
			{
				x0 = PointElementBefore;
				pVerteilerCOII->ResetRohrElementePointElementBeforeWinkel();
			}
			else
			{
				x0 = X0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel() - lEndeLaenge)/f1;
			}
			y0		= Y0;
			x1		= X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1;
			y1		= Y0;
			nachoben= true;
			vertikal= false;
		} 
		else if (merkerRohrele.get_abstAbs() > laenge) 
		{
			x0		= X0 + (double)(-lEndeLaenge + merkerRohrele.get_abstAbs() - laenge)/f1;
			y0		= Y0;			
			x1		= X0 + (double)(-lEndeLaenge + elemerk.get_abstAbs() - laenge)/f1;				
			y1		= Y0;				
			nachoben= true;			
			vertikal= false;			
		}
		else if (elemerk.get_abstAbs() > laenge)
		{
			if (X0 == 0)	
			{
				if(PointElementBefore > -1)
				{
					x0	= PointElementBefore;
					pVerteilerCOII->ResetRohrElementePointElementBeforeWinkel();
				}
				else
				{
					x0	= 	X0 + (double)(-lEndeLaenge - laenge + elemerk.get_abstAbs() - elemerk.get_abstRel() + laenge - merkerRohrele.get_abstAbs())/f1;	
				}
				y0		= Y0;					
				x1		= X0 + (double)(-lEndeLaenge - laenge + elemerk.get_abstAbs())/f1;					
				y1		= Y0;					
				nachoben= true;					
				vertikal= false;					
			}
			else 
			{
				x0		= ((X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1) < 0)?0:X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs() + elemerk.get_abstRel())/f1;	
				y0		= Y0;					
				x1		= X0 - (double)(lEndeLaenge + laenge - elemerk.get_abstAbs())/f1;					
				y1		= Y0;					
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
			nachoben= true;			
			vertikal= false;			
	}
	add_dimensioning(	x0, y0, x1, y1, (int)ebene, nachoben, vertikal);
	bemassungHochgestellt = false;
}

void ConstructAbgEle::createBeschriftung()
{
	if (text.GetLength() == 0)
		return;

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	int ebene = 0, textwinkel = 0;
	
	int ebene1= maxlaengeAbgUnten + 120; 
	int ebene2= 50; 
	int ebene3= maxRohrDN + maxlaengeAbgOben + 140; 
	int ebene4= maxRohrDN + maxlaengeAbgOben + 50; 

	int ebene5= maxlaengeAbgLinks + 100; 
	int ebene9= maxlaengeAbgLinks + 80; 
	int ebene6= 100; 
	int ebene7= maxRohrDN + maxlaengeAbgRechts + 80; 
	int ebene10=maxRohrDN + maxlaengeAbgRechts + 80; 
	int ebene8= maxRohrDN + 60; 

	//Wenn Abgaenge zu dicht nebeneinander wird Text verschoben
	//Achtung: createBemassung muss fuer bemassungHochgestellt vorher aufgerufen werden!!!
	int tiefer = 0;
	if (bemassungHochgestellt && elemerk.get_winkel() == merkerAbgele.get_winkel()) 
		tiefer = 50;
	//else if (bemassungHochgestellt && (elemerk.get_winkel() ==   0 && merkerAbgele.get_winkel() ==  90
	//								|| elemerk.get_winkel() == 180 && merkerAbgele.get_winkel() == 270))
	//	tiefer = 200;

	switch (typ) 
	{
	case ABG:
		// links und Rohr nach oben
		if (winkel == 0 && drehfaktor == 90)
		{
			ebene = ebene5 - tiefer;
			textwinkel = 135;
			add_text(X0 - ebene, 
							  Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// hinten und Rohr nach oben
		else if (winkel == 90 && drehfaktor == 90)
		{
			ebene = ebene6 - tiefer;
			textwinkel = 135;//225; //135;
			add_text(X0 - ebene, 
							  Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// rechts und Rohr nach oben
		else if (winkel == 180 && drehfaktor == 90) 
		{
			ebene = ebene7 - tiefer;
			textwinkel = 45;
			add_text(X0 + ebene, 
							  Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// vorne und Rohr nach oben
		else if (winkel == 270 && drehfaktor == 90)
		{
			ebene = ebene8 - tiefer;
			textwinkel = 45;//315; //45;
			add_text(X0 + ebene, 
							  Y0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// rechts und Rohr nach unten
		else if (winkel == 0 && drehfaktor == 270) 
		{
			ebene = ebene10 - tiefer;
			textwinkel = 45;
			add_text(X0 + ebene, 
							  Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// hinten und Rohr nach unten
		else if (winkel == 90 && drehfaktor == 270) 
		{
			ebene = ebene8 - tiefer;
			textwinkel = 45;//315;//45;
			add_text(X0 + ebene, 
							  Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// links und Rohr nach unten
		else if (winkel == 180 && drehfaktor == 270) 
		{
			ebene = ebene9 - tiefer;
			textwinkel = 225;
			add_text(X0 - ebene, 
							  Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// vorne und Rohr nach unten
		else if (winkel == 270 && drehfaktor == 270) 
		{
			ebene = ebene6 - tiefer;
			textwinkel = 225;//135;//225;
			add_text(X0 - ebene, 
							  Y0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  text, color, false, textwinkel);
		}
		// unten und Rohr nach links
		else if (winkel == 0 && drehfaktor == 180) 
		{
			ebene = ebene1 - tiefer;
			textwinkel = 315;
			add_text(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  Y0 - ebene, 
							  text, color, false, textwinkel);
		}
		// hinten und Rohr nach links
		else if (winkel == 90 && drehfaktor == 180) 
		{
			ebene = ebene2 - tiefer;
			textwinkel = 315;//225; //315;
			add_text(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  Y0 - ebene, 
							  text, color, false, textwinkel);
		}
		// oben und Rohr nach links
		else if (winkel == 180 && drehfaktor == 180) 
		{
			ebene = ebene3 - tiefer;
			textwinkel = 45;
			add_text(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  Y0 + ebene, 
							  text, color, false, textwinkel);
		}
		// vorne und Rohr nach links
		else if (winkel == 270 && drehfaktor == 180) 
		{
			ebene = ebene4 - tiefer;
			textwinkel = 45;//135;//45;
			add_text(X0 + (double)(- elemerk.get_abstAbs() + lEndeLaenge + laenge)/f1, 
							  Y0 + ebene, 
							  text, color, false, textwinkel);
		}
		// oben und Rohr nach rechts + Bogen 
		else if (winkel == 0 && drehfaktor == 0 && bogen) 
		{
			ebene = ebene3 - tiefer;
			textwinkel = 45;
			add_text(X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  Y0 + ebene, 
							  text, color, false, textwinkel);
		}
		// hinten und Rohr nach rechts + Bogen 
		else if (winkel == 90 && drehfaktor == 0 && bogen) 
		{
			ebene = ebene4 - tiefer;
			textwinkel = 45;//135;//45;
			add_text(X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  Y0 + ebene, 
							  text, color, false, textwinkel);
		}
		// unten und Rohr nach rechts + Bogen 
		else if (winkel == 180 && drehfaktor == 0 && bogen) 
		{
			ebene = ebene1 - tiefer;
			textwinkel = 315;
			add_text(X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  Y0 - ebene, 
							  text, color, false, textwinkel);
		}
		// vorne und Rohr nach rechts + Bogen 
		else if (winkel == 270 && drehfaktor == 0 && bogen) 
		{
			ebene = ebene2 - tiefer;
			textwinkel = 315;//225;//315;
			add_text(X0 + (double)(elemerk.get_abstAbs() - lEndeLaenge - laenge)/f1, 
							  Y0 - ebene, 
							  text, color, false, textwinkel);
		}
		// oben und Rohr nach rechts	
		else if (winkel == 0 && drehfaktor == 0) 
		{
			ebene = ebene3 - tiefer;
			textwinkel = 45;
			add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1, 
							  Y0 + ebene, 
							  text, color, false, textwinkel);
		}
		// hinten und Rohr nach rechts
		else if (winkel == 90 && drehfaktor == 0) 
		{
			ebene = ebene4 - tiefer;
			textwinkel = 45;//135;//45;
			add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1, 
							  Y0 + ebene, 
							  text, color, false, textwinkel);
		}
		// unten und Rohr nach rechts
		else if (winkel == 180 && drehfaktor == 0) 
		{
			ebene = ebene1 - tiefer;
			textwinkel = 315;
			add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1, 
							  Y0 - ebene, 
							  text, color, false, textwinkel);
		}
		// vorne und Rohr nach rechts
		else if (winkel == 270 && drehfaktor == 0) 
		{
			ebene = ebene2 - tiefer;
			textwinkel = 315;//225;//315;
			add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1, 
							  Y0 - ebene, 
							  text, color, false, textwinkel);
		}
		break;
	}
}

void ConstructAbgEle::createElement()
{
}

const bool ConstructAbgEle::get_avele() const
{
	return avele;
}

//Ist Element Nachbearbeit eingegeben, so ist dies eine Ebene höher als die 
//Standardelemente zu bemassen.
void ConstructAbgEle::set_avele(bool value)
{
	avele = value;
}

void ConstructAbgEle::initEleDaten(const Element& ele)
{
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
	andn = elemerk.get_anschlussDN();
	andn = (int)((double)andn * faktorRohrbreite);
	dn = andn / 2;
	
	if( elemerk.get_id() == SPRINKLERSCHELLE_VIROTEC)
	{
		elemerk.set_dn1(elemerk.get_laenge()); // der Wert Dn1 muss hier genommen werden, da die Laenge immer mit einem Faktor gerechnet wird
	}
	double f3 = (elemerk.get_id() != ROHR) ? faktorElelaenge : 1;
	elemerk.set_laenge((int)((double)elemerk.get_laenge() * f3));
	
}
