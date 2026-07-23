#include "stdafx.h"


#include "constructkred.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructKred::~ConstructKred()
{

}

ConstructKred::ConstructKred()
{

}

void ConstructKred::createBeschriftung()
{
	ConstructRohrEle::createBeschriftung();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	
	if (elemerk.get_id() == KRED) 
	{
		if (!dn2zoll)
			text.Format("KRED %d", elemerk.get_dn());
		else
			text.Format("KRED %s''", WaxStamm::dn2zoll(dn));
	}
	else 
	{
		if (!dn2zoll)
			text.Format("EINZ %d", elemerk.get_dn());
		else
			text.Format("EINZ %s''", WaxStamm::dn2zoll(elemerk.get_dn()));
	}

	switch (typ) 
	{
	case IMROHR:
		if (drehfaktor == 0) 
			add_text(	X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1,	
								Y0 + rohrDn + 5, 
								text, color, false, 45);
		else if (drehfaktor == 180) 
			add_text(	X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1,					
								Y0 + rohrDn + 5, 
								text, color, false, 45);
		else if (drehfaktor == 90) 
			add_text(	X0 + rohrDn + 45,		
								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
								text, color, false, 45);
		else if (drehfaktor == 270)
			add_text(	X0 + rohrDn + 45,		
								Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1, 
								text, color, false, 45);
		break;
	}
}

void ConstructKred::createElement()
{
	ConstructRohrEle::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;

	typ = IMROHR;

	switch (typ) 
	{
	case IMROHR:
		if (drehfaktor == 0) 
		{
			// verkleinern
			if (dn < andn) 
			{ 
				// untere Linie
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1,							Y0, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + (andn - dn)/2,
							   1, color);
				// obere Linie
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1,							Y0 + andn, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn - (andn - dn)/2,
							   1, color);
				// senkrechte Linie
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + (andn - dn)/2, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn - (andn - dn)/2,
							   1, color);
			// vergroessern	
			} 
			else
			{
				// untere Linie
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1,							Y0, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 - (dn - andn)/2,
							   1, color);
				// obere Linie
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1,							Y0 + andn, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn + (dn - andn)/2,
							   1, color);
				// senkrechte Linie
				add_line(X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + (andn - dn)/2, 
							   X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn - (andn - dn)/2,
							   1, color);
			}
		} 
		else if (drehfaktor == 180)
		{
			// verkleinern
			if (dn < andn)
			{ 
				// untere Linie
				add_line(X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1,							Y0, 
							   X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + (andn - dn)/2,
							   1, color);
				// obere Linie
				add_line(X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1,							Y0 + andn, 
							   X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn - (andn - dn)/2,
							   1, color);
				// senkrechte Linie
				add_line(X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + (andn - dn)/2, 
							   X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn - (andn - dn)/2,
							   1, color);
			// vergroessern	
			}
			else 
			{
				// untere Linie
				add_line(X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1,							Y0, 
							   X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 - (dn-andn)/2,
							   1, color);
				// obere Linie
				add_line(X0 + (double)(- elemerk.get_abstAbs() + laenge)/f1,							Y0 + andn, 
							   X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn + (dn - andn)/2,
							   1, color);
				// senkrechte Linie
				add_line(X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + (andn - dn)/2, 
							   X0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,	Y0 + andn - (andn - dn)/2,
							   1, color);
			}
		} 
		else if (drehfaktor == 90) 
		{
			// verkleinern
			if (dn < andn) 
			{ 
				// untere Linie
				add_line(X0,																					Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
							   X0 + (andn - dn)/2,								Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1,
							   1, color);
				// obere Linie
				add_line(X0 + andn,														Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
							   X0 + andn - (andn - dn)/2,	Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1,
							   1, color);
				// senkrechte Linie
				add_line(X0 + (andn - dn)/2,								Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1, 
							   X0 + andn - (andn - dn)/2,	Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1,
							   1, color);
			// vergroessern	
			} 
			else 
			{
				// untere Linie
				add_line(X0,																					Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
							   X0 - (dn-andn)/2,									Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1,
							   1, color);
				// obere Linie
				add_line(X0 + andn,														Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
							   X0 + andn + (dn - andn)/2,	Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1,
							   1, color);
				// senkrechte Linie
				add_line(X0 + (andn - dn)/2,								Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1, 
							   X0 + andn - (andn - dn)/2,	Y0 + (double)(elemerk.get_abstAbs() - laenge + elemerk.get_laenge())/f1,
							   1, color);
			}
		}
		else if (drehfaktor == 270)
		{
			// verkleinern
			if (dn < andn)
			{ 
				// untere Linie
				add_line(X0,																					Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1, 
							   X0 + (andn - dn)/2,								Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,
							   1, color);
				// obere Linie
				add_line(X0 + andn,														Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1, 
							   X0 + andn - (andn - dn)/2,	Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,
							   1, color);
				// senkrechte Linie
				add_line(X0 + (andn - dn)/2,								Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1, 
							   X0 + andn - (andn - dn)/2,	Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,
							   1, color);
			// vergroessern	
			} 
			else 
			{
				// untere Linie
				add_line(X0,																					Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1, 
							   X0 - (dn-andn)/2,									Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,
							   1, color);
				// obere Linie
				add_line(X0 + andn,														Y0 + (double)(- elemerk.get_abstAbs() + laenge)/f1, 
							   X0 + andn + (dn - andn)/2,	Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,
							   1, color);
				// senkrechte Linie
				add_line(X0 + (andn - dn)/2,								Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1, 
							   X0 + andn - (andn - dn)/2,	Y0 + (double)(- elemerk.get_abstAbs() + laenge - elemerk.get_laenge())/f1,
							   1, color);
			}
		}
		break;
	}
}

