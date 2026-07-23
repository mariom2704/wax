#include "stdafx.h"


#include "ConstructCOIIGegenKomponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructCOIIGegenKomponent::ConstructCOIIGegenKomponent()
{

}


ConstructCOIIGegenKomponent::~ConstructCOIIGegenKomponent()
{

}



void ConstructCOIIGegenKomponent::createBeschriftung()
{
	GlObj::colour color = GlObj::BLACK;
	
	int ebene = 0, textwinkel = 0;
	

	int ebene3= maxRohrDN + maxlaengeAbgOben + 140; 
	//Wenn Abgaenge zu dicht nebeneinander wird Text verschoben
	//Achtung: createBemassung muss fuer bemassungHochgestellt vorher aufgerufen werden!!!
	int tiefer = 0;
	if (bemassungHochgestellt && elemerk.get_winkel() == merkerAbgele.get_winkel()) 
		tiefer = 50;
	
	textwinkel = 45;
	ebene = ebene3 - tiefer;
	add_text(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1-70, 
					  Y0 + ebene, 
					  text, color, false, textwinkel);
}



void ConstructCOIIGegenKomponent::calculateDimisions()
{
	f1 = faktorRohrlaenge;	
	double xFa = 1;
	int iDn = merkerRohrele.get_anschlussDN();
	if(iDn == 80)
	{
		xFa = 0.8;
	}

	if(iDn == 65)
	{
		xFa = 0.6;
	}
	
	if(elemerk.get_dn() == 65)
	{
		dMerkI = 0.6*xFa;
		MerkII = 0.7*xFa;
		MerkIII= 0.85*xFa;
		MerkIV = 0.85*xFa;
		MerkV = 1.5*xFa;
	};

	if(elemerk.get_dn() == 80)
	{

		dMerkI = 0.4*xFa;
		MerkII = 0.4*xFa;
		MerkIII= 0.7*xFa;
		MerkIV = 0.7*xFa;
		MerkV = 1.2*xFa;
	};


	if(elemerk.get_dn() == 100)
	{

		dMerkI = 0.025;
		MerkII = 0.025;
		MerkIII= 0.5;
		MerkIV = 0.5;
		MerkV = 1;
	};
}



void ConstructCOIIGegenKomponent::initEleDaten(const Element& ele)
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
}



double ConstructCOIIGegenKomponent::createFlansch()
{
	x     = X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  - dn*MerkIII - 2*dn/10;
	y     = Y0 + merkerRohrele.get_anschlussDN() + merkerRohrele.get_laenge()*1.4;
	dx    = -((elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn *MerkIII - dn/10) + ((elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn*MerkIV + 3*dn/10);
	dy    = elemerk.get_laenge()*2/10;

	
	add_rectangle(x, y, dx, 14, color);
						  

	add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn *MerkIII - dn/10,	y+14, 
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - dn*MerkIV,	y+14 +  14*4,
						   1, color);
	add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn*MerkIII + dn/10, y+14,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + dn*MerkIV , y+14 +  14*4,
						   1, color);

	//Querline
	add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 -  MerkV*dn/2,	y+14 +  14*4,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + MerkV * dn/2 ,  y+14 +  14*4,
						   1, color);
	return  y;
}


double ConstructCOIIGegenKomponent::createStutzen(int iFlanschHeight)
{
	int element_laenge =  elemerk.get_laenge();
	// linke Seiten nach oben Stutzen
	add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - MerkV * dn/2,	y+14 +  14*4,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - MerkV* dn/2 ,	y+14 +  14*4 +  element_laenge,
						   1, color);
	// rechte Seiten nach oben Stutzen
	add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + MerkV*dn/2,  y+14 +  14*4,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + MerkV*dn/2 ,	y+14 +  14*4 +  element_laenge,
						   1, color);

	// Querseite Seiten nach oben Stutzen
	add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 -  MerkV * dn/2,	y+14 +  14*4 +  element_laenge,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 +  MerkV * dn/2 ,	y+14 +  14*4 +  element_laenge,
						   1, color);

	cc = 0;
	for(int z=0; z<5; z++)
	{ 
		cc = y+14 +  14*4  - z*5;
		add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - MerkV*dn/2,	cc +    element_laenge,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 + MerkV*dn/2 ,	cc-5 +  element_laenge,
						   1, color);
	}

	int iLaenge = elemerk.get_laenge();
	int iDN = elemerk.get_dn();
	CString strLaenge;
	strLaenge.Format("DN%dx%d", iDN, iLaenge);
	if(iLaenge < 170)
	{
		add_text(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 - 2* 3*dx/5,
						   y , 
						  strLaenge, color, false, 90);
	}
	else
	{
		add_text(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 - 2* 3*dx/5,
						   cc +  element_laenge/2 - dn/3, 
						  strLaenge, color, true, 90);
	}
		

	add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 -  MerkV * dn/2,	y+14 +  14*4 +  element_laenge,  
						   X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - MerkV * dn/2 -50 ,	y+14 +  14*4 +  element_laenge,
						   1, color);
	add_line( X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 -  MerkV * dn/2,
					iFlanschHeight,  
					X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 - MerkV * dn/2 - 50 ,
				    iFlanschHeight,
					1, color);

	return  y+14 +  14*4 + element_laenge;

}
