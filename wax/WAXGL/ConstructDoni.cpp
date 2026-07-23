#include "stdafx.h"


#include "constructdoni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructDoni::~ConstructDoni()
{

}

ConstructDoni::ConstructDoni()
{

}


void ConstructDoni::createBeschriftung()
{
	set_showText(false);

	ConstructFitting::createBeschriftung();
}

void ConstructDoni::createElement()
{
	ConstructFitting::createElement();
	
	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	
	int laenge1 = elemerk.get_laenge()/2-elemerk.get_zmass()/2;
	int laenge2 = elemerk.get_zmass();

	switch (typ) 
	{
	case IMROHR:
		// Rohr nach rechts
		if (drehfaktor == 0) 
		{
			add_rectangle(	X0 + merkerAbgele.getZmass(Element::LINKS)/f1,	Y0,  
								laenge1/f1,								elemerk.get_dn(),	
								color);

			add_rectangle(	X0 + (merkerAbgele.getZmass(Element::LINKS)+laenge1+laenge2)/f1,	Y0,  
								laenge1/f1,								elemerk.get_dn(),	
								color);
			
			add_rectangle(	X0 + (merkerAbgele.getZmass(Element::LINKS)+laenge1)/f1,	Y0 - 0.2*elemerk.get_dn(),  
								laenge2/f1,								1.4*elemerk.get_dn(),	
								color);
		}
		else if (drehfaktor == 180) 
		{
			add_rectangle(	X0 - (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1,	Y0,  
								elemerk.get_laenge()/f1,									elemerk.get_dn(),	
								color);

			add_rectangle(	X0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,	Y0,  
								elemerk.get_laenge()/f1,									elemerk.get_dn(),	
								color);
			
			add_rectangle(	X0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,	Y0 - 0.2*elemerk.get_dn(),  
								elemerk.get_laenge()/f1,									1.4*elemerk.get_dn(),	
								color);
		}
		else if (drehfaktor == 90) 
		{
			add_rectangle(	X0 ,														Y0 + (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1,  
								elemerk.get_dn(),											elemerk.get_laenge()/f1,
								color);

			add_rectangle(	X0,															Y0 + (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,  
								elemerk.get_dn(),											elemerk.get_laenge()/f1,	
								color);
			
			add_rectangle(	X0 - 0.2*elemerk.get_dn(),									Y0 + (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,  
								1.4*elemerk.get_dn(),										elemerk.get_laenge()/f1,	
								color);
		}
		else if (drehfaktor == 270) 
		{
			add_rectangle(	X0 ,														Y0 - (elemerk.get_abstRel() - 1.5*elemerk.get_laenge())/f1,  
								elemerk.get_dn(),											elemerk.get_laenge()/f1,
								color);

			add_rectangle(	X0,															Y0 - (elemerk.get_abstRel() + 0.5*elemerk.get_laenge())/f1,  
								elemerk.get_dn(),											elemerk.get_laenge()/f1,	
								color);
			
			add_rectangle(	X0 - 0.2*elemerk.get_dn(),									Y0 - (elemerk.get_abstRel() - 0.5*elemerk.get_laenge())/f1,  
								1.4*elemerk.get_dn(),										elemerk.get_laenge()/f1,	
								color);
		}
	}
}

