#include "stdafx.h"


#include "bemassung.h"
#include "RohrQx.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxGrafik;


RohrQx::RohrQx()
{
	show_laenge = (WaxSys::para_int(BESCHRIFTUNGLAENGE) == 0) ? false : true;
	show_dn = true;
	einz = OHNE_EINZIEHUNG;
	show_text = false;
}

void RohrQx::init_rohrqx(enum Einziehung einziehung, int count)
{
	einz = einziehung;
	rohrcounter = count;
}

void RohrQx::createBemassung()
{
	double f1 = faktorRohrlaenge;

	if (-maxlaenge_270 > Y0)
		maxlaenge_270 = 200;

	int richtung = (drehfaktor == 0) ? 180 : 0;

	double unten = (double)(maxlaenge_270+maxlaengeAbgUnten)/Bemassung::EBENENABSTAND;
	double abgunten = (double)(maxlaengeAbgUnten)/Bemassung::EBENENABSTAND;

	//rohrleitung->isRohrstutzen(): 1=ohne Ende, 2=mit Ende
	int ebene1 = (rohrDn + maxlaenge_0 + maxlaengeAbg > 0)? 1 + (rohrDn + maxlaenge_0 + maxlaengeAbg)/Bemassung::EBENENABSTAND : 1;

    int ebene2 = (int)ceil(double((rohrleitung->isRohrstutzen(richtung) == 2) ? 7+unten : (rohrleitung->isRohrstutzen(richtung) == 1) ? 6+unten: (abgunten) ? 6+unten : (unten) ? unten : (rohrleitung->isRed()) ? 5: 2));

	const COIIVerteiler* pCOIIRohr = dynamic_cast<const COIIVerteiler*>(rohrleitung);
	if (pCOIIRohr)
	{
		ebene2 = 5;
	}
	//Prüfen ob Knick nach unten
	if (ebene2 < 6)
	{
		const Element *ele = rohrleitung->getKnick();
		if (ele && ele->get_winkel() >= 180)
			ebene2 = 6;
	}

	const Gewinderohr* grohr = dynamic_cast<const Gewinderohr*>(rohrleitung);
	if (grohr)
	{
		// Wann soll denn diese Stelle hier aufgerufen werden? PR
		// Konnte ich nicht nachvollziehen
		XASSERT(grohr == NULL);
		ebene1 = 1;
		ebene2 = 1;
	}

	double x1=0, y1=0, x2=0, y2=0;
	int ebene = 0; 
	bool ok = true;  

	// Rohr nach rechts
	switch(winkel)
	{
		case 0:
			y1 = Y0;
			y2 = Y0;
			ebene = ebene2;
			if (X0 != 0 && bogen)
			{
				x1 = X0 - bogenlaenge - (double)lEndeLaenge/f1;
				x2 = X0 + (double)(elemerk.get_laenge() + rEndeLaenge)/f1;
			}
			else
			{
				x1 = X0 - (double)lEndeLaenge/f1;
				x2 = X0 + (double)(elemerk.get_laenge() + rEndeLaenge)/f1;
				x1 = X0 - (double)lEndeLaenge/f1;
			}
			break;

		case 90:
			x1 = X0;
			x2 = X0;
			if (X0 == 0)
			{
				y1 = Y0 - bogenlaenge;
				y2 = Y0;
				ebene = -ebene2;
			}
			else
			{
				y1 = Y0 - bogenlaenge - (double)lEndeLaenge/f1;
				y2 = Y0 + elemerk.get_laenge() + rEndeLaenge;
				ebene = ebene1;
			}
			break;
		
		case 180:
			// Rohr nach links
			y1 = Y0;
			y2 = Y0;
			if (X0 == 0) 
			{
				x1 = X0 + lEndeLaenge;
				x2 = X0 - elemerk.get_laenge() - rEndeLaenge;
				ebene = -ebene1;
			}
			else
			{
				x1 = X0 - elemerk.get_laenge() - rEndeLaenge;
				x2 = X0 + bogenlaenge + (double)lEndeLaenge/f1;
				ebene = ebene1;
			}
			break;

		case 270:
			x1 = X0;
			x2 = X0;
			ebene = ebene1;
			if (X0 == 0)
			{
				y1 = Y0 + bogenlaenge;
				y2 = Y0 - elemerk.get_laenge() - rEndeLaenge;
				
			}
			else
			{
				y1 = Y0 - elemerk.get_laenge() - rEndeLaenge;
				y2 = Y0 + bogenlaenge + (double)lEndeLaenge/f1;
			}
			break;
		default:
			ok = false;
			break;

	}

	XASSERT(typ == IMROHR);
	// Rohre werden immer in der gleichen Richtung wie das Rohr selber bemasst
	bool nachoben_vertikal = (winkel == 270) || (winkel == 90); 
	if (ok && (typ == IMROHR))
		add_dimensioning(x1, y1, x2, y2, ebene, nachoben_vertikal, nachoben_vertikal, masstext);
	else 
		TRACE("Element 'RohrQx-Bemassung' mit typ=%d, winkel=%d, bogen=%d konnte nicht gezeichnet werden!", typ, winkel, bogen?1:0);
}

void RohrQx::createBeschriftung()
{
	ConstructRohrEle::createBeschriftung();
	
	GlObj::colour color = GlObj::BLACK;
	double f1 = faktorRohrlaenge;
	int dx = 400;
	int dy = 100;

	if (dynamic_cast<const Gewinderohr*>(rohrleitung))
		dx = 200;

	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 200)
	{
		dx = 50;
		dy = 50;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 400)
	{
		dx = 100;
		dy = 50;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 600)
	{
		dx = 150;
		dy = 50;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 800)
	{
		dx = 250;
		dy = 50;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 1000)
	{
		dx = 250;
		dy = 50;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 2000)
	{
		dx = 300;
		dy = 80;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 4000)
	{
		dx = 600;
		dy = 100;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 6000)
	{
		dx = 700;
		dy = 180;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 8000)
	{
		dx = 900;
		dy = 180;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 10000)
	{
		dx = 1000;
		dy = 180;
	}
	else if (rohrleitung->get_laenge()/faktorRohrlaenge <= 12000)
	{
		dx = 1100;
		dy = 200;
	}
/*
	if (anfendid == GLATT)
		dx /= 2;
*/
	switch (typ) 
	{
	case IMROHR:

		// Dieser Text wird nur einmal fürs erste Rohr am Beginn der Zeichnung ausgegeben
		if (show_text)
		{
			// Text mit Positionsnummer
			string posnrtxt = str(format("%s") % (LPCSTR)rohrleitung->get_posNr());
			if (rohrcounter)
				posnrtxt += str(format(".%i") %rohrcounter);
			add_text(X0-dx, Y0+dy, posnrtxt);
						
			// Text mit DN und ID des Anfangselements
			if (!rohrleitung->isGewinderohr()) // Bei Gewinderohr andere Beschriftung
			{
				string txt = rohrleitung->get_beschriftung(elemerk.get_dn(), dn2zoll, rohrcounter);
				add_text(X0-dx, Y0, txt, true);
			}

			// Text mit Anzahl
			string dntxt = str(format("%dX") % rohrleitung->get_anzahl());
			add_text(X0-dx, Y0-dy, dntxt);

		}
		if (show_laenge || show_dn)
		{
			CString laengeDn = "";
			CString laengestr = "";
			
			if (show_laenge && elemerk.get_laenge() > 0)
				laengestr.Format("%d*", elemerk.get_laenge());

			if (show_dn)
			{
				if (!dn2zoll)
					laengeDn.Format("%sDN%d", laengestr, elemerk.get_dn());
				else 
					laengeDn.Format("%s%s''", laengestr, WaxStamm::dn2zoll(elemerk.get_dn()));
			}

			if (drehfaktor == 0)
				add_text(X0+elemerk.get_laenge()/2/f1, Y0-dy, laengeDn, color, true, 360);
			
			else if (drehfaktor == 180)
				add_text(X0-elemerk.get_laenge()/2/f1, Y0-dy, laengeDn, color, true, 360);
			
			else if (drehfaktor == 90)
				add_text(X0+elemerk.get_dn()+dy, Y0+elemerk.get_laenge()/2/f1, laengeDn, color, true, 90);
			
			else if (drehfaktor == 270)
				add_text(X0+elemerk.get_dn()+dy, Y0-elemerk.get_laenge()/2/f1, laengeDn, color, true, 90);
		}
		break;
	}
}

void RohrQx::createElement()
{
	// Bei Gewinderohr wird das Rohr um das ZMass nach rechts verschoben.
	ConstructRohrEle::createElement();

	double f1 = faktorRohrlaenge;
	double f2 = faktorRohrbreite;
	GlObj::colour color = GlObj::BLACK;
	double dnEinz = 0;

	if (elemerk.get_dn() == 50)
		dnEinz = 40;
	else if (elemerk.get_dn() == 40)
		dnEinz = 32;
	else if (elemerk.get_dn() == 32)
		dnEinz = 25;
	
	dnEinz *= f2;

	switch (typ) 
	{
	case IMROHR:
		// damit bei Rohr ohne Abgaenge die obere Rohrlinie gezeichnet wird bei metafile
		if (rohrleitung->isRohrGlatt() && !(dynamic_cast<const Gewinderohr*>(rohrleitung)))
			add_line(X0, Y0+dn+5, X0, Y0+dn+6, 1,  color);			

		//const Verteiler* vert = dynamic_cast<const Verteiler*>(rohrleitung);
/*
		// damit bei Rohr mit glatt am Ende die rechte Rohrlinie gezeichnet wird bei metafile
		if ((rohrleitung->get_rohrEnde() == GLATT || (vert && vert->get_rohrDefLaengen().GetLength() && vert->get_rohrVerb() == SNAHT)) && winkel == 0) 
			add_line(X0+(double)elemerk.get_laenge()/f1+5, Y0, X0+(double)elemerk.get_laenge()/f1+6, Y0, 1,  color);
*/	
		// keine Einziehung
		if (einz == OHNE_EINZIEHUNG)
		{
			// Rohr nach rechts
			if (winkel == 0) 
				add_rectangle(	X0 + elemerk.get_zmass()/f1,	Y0,  
									(double)elemerk.get_laenge()/f1,dn,	
									color);
			// Rohr nach oben
			else if (winkel == 90) 
				add_rectangle(	X0,								Y0 + elemerk.get_zmass()/f1,  
									dn,								(double)elemerk.get_laenge()/f1,	
									color);
			// Rohr nach links
			else if (winkel == 180) 
				add_rectangle(	X0 - elemerk.get_zmass()/f1,	Y0, 
									-(double)elemerk.get_laenge()/f1,dn,	
									color);
			// Rohr nach unten
			else if (winkel == 270) 
				add_rectangle(	X0,								Y0 - elemerk.get_zmass()/f1,  
									dn,								-(double)elemerk.get_laenge()/f1,	
									color);
		}
		// Einziehung links 
		else if (einz == ANFANG)
		{
			// Rohr nach rechts
			if (winkel == 0) 
			{
				add_line(X0,									Y0+((dn-dnEinz)/2),		
							   X0,									Y0+((dn-dnEinz)/2)+dnEinz,			
							    1,  color);			
				add_line(X0,									Y0+((dn-dnEinz)/2),		
							   X0+50,								Y0,			
								1,  color);			
				add_line(X0,									Y0+((dn-dnEinz)/2)+dnEinz, 
							   X0+50,								Y0+dn,	
								1,  color);			
				add_line(X0+50,								Y0,       
							   (double)(X0+elemerk.get_laenge()/f1),Y0, 
								1,  color);
				add_line(X0+50,								Y0+dn, 
							   (double)(X0+elemerk.get_laenge()/f1),Y0+dn, 
								1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1),Y0, 
							   (double)(X0+elemerk.get_laenge()/f1),Y0+dn, 
								1,  color);
			} 
			// Rohr nach oben
			else if (winkel == 90) 
			{
				add_line(X0+((dn-dnEinz)/2),					Y0,		
							   X0+((dn-dnEinz)/2)+dnEinz,			Y0,			
							    1,  color);
				add_line(X0+((dn-dnEinz)/2),					Y0,		
							   X0,									Y0+50,			
								1,  color);
				add_line(X0+((dn-dnEinz)/2)+dnEinz,			Y0, 
							   X0+dn,								Y0+50,	
								1,  color);
				add_line(X0,									Y0+50,       
							   X0,									(double)(Y0+elemerk.get_laenge()/f1), 
								1,  color);
				add_line(X0+dn,								Y0+50, 
							   X0+dn,								(double)(Y0+elemerk.get_laenge()/f1), 
								1,  color);
				add_line(X0,									(double)(Y0+elemerk.get_laenge()/f1), 
							   X0+dn,								(double)(Y0+elemerk.get_laenge()/f1), 
								1,  color);
			} 
			// Rohr nach links
			else if (winkel == 180) 
			{
				add_line(X0,									Y0+((dn-dnEinz)/2),		
							   X0,									Y0+((dn-dnEinz)/2)+dnEinz,			
							    1,  color);
				add_line(X0,									Y0+((dn-dnEinz)/2),		
							   X0-50,								Y0,			
								1,  color);
				add_line(X0,									Y0+((dn-dnEinz)/2)+dnEinz, 
							   X0-50,								Y0+dn,	
								1,  color);
				add_line(X0-50,								Y0,       
							   (double)(X0-elemerk.get_laenge()/f1),Y0, 
								1,  color);
				add_line(X0-50,								Y0+dn, 
							   (double)(X0-elemerk.get_laenge()/f1),Y0+dn, 
								1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1),Y0, 
							   (double)(X0-elemerk.get_laenge()/f1),Y0+dn, 
								1,  color);
			} 
			// Rohr nach unten
			else if (winkel == 270) 
			{
				add_line(X0+((dn-dnEinz)/2),					Y0,		
							   X0+((dn-dnEinz)/2)+dnEinz,			Y0,			
							    1,  color);
				add_line(X0+((dn-dnEinz)/2),					Y0,		
							   X0,									Y0-50,			
								1,  color);
				add_line(X0+((dn-dnEinz)/2)+dnEinz,			Y0, 
							   X0+dn,								Y0-50,	
								1,  color);
				add_line(X0,									Y0-50,       
							   X0,									(double)(Y0-elemerk.get_laenge()/f1), 
								1,  color);
				add_line(X0+dn,								Y0-50, 
							   X0+dn,								(double)(Y0-elemerk.get_laenge()/f1), 
								1,  color);
				add_line(X0,									(double)(Y0-elemerk.get_laenge()/f1), 
							   X0+dn,								(double)(Y0-elemerk.get_laenge()/f1), 
								1,  color);
			}
		}
		// Einziehung rechts
		else if (einz == ENDE)
		{
			
			// Rohr nach rechts
			if (winkel == 0) 
			{

				add_line(X0,										Y0,		
							   X0,										Y0+dn,			
							   1,  color);
				add_line(X0,										Y0,       
							   (double)(X0+elemerk.get_laenge()/f1-50),	Y0, 
							   1,  color);
				add_line(X0,										Y0+dn, 
							   (double)(X0+elemerk.get_laenge()/f1-50),	Y0+dn, 
							   1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1-50),	Y0,		
							   (double)(X0+elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2),			
							   1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1-50),	Y0+dn, 
							   (double)(X0+elemerk.get_laenge()/f1),	Y0+dn-((dn-dnEinz)/2),	
							   1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2), 
							   (double)(X0+elemerk.get_laenge()/f1),	Y0-((dn-dnEinz)/2)+dn, 
							   1,  color);
				TRACE("X0=%i, elemerk->get_laenge() = %i, f1=%f\n", X0, elemerk.get_laenge(), f1);
			
			} 
			// Rohr nach oben
			else if (winkel == 90) 
			{
				add_line(X0,										Y0,		
							   X0+dn,									Y0,			
							   1,  color);
				add_line(X0,										Y0,       
							   X0,										(double)(Y0+elemerk.get_laenge()/f1-50), 
							   1,  color);
				add_line(X0+dn,									Y0,       
							   X0+dn,									(double)(Y0+elemerk.get_laenge()/f1-50), 
							   1,  color);
				add_line(X0,										(double)(Y0+elemerk.get_laenge()/f1-50),		
							   X0+((dn-dnEinz)/2),						(double)(Y0+elemerk.get_laenge()/f1),			
							   1,  color);
				add_line(X0+dn,									(double)(Y0+elemerk.get_laenge()/f1-50), 
							   X0+dn-((dn-dnEinz)/2),					(double)(Y0+elemerk.get_laenge()/f1),	
							   1,  color);
				add_line(X0+((dn-dnEinz)/2),						(double)(Y0+elemerk.get_laenge()/f1), 
							   X0-((dn-dnEinz)/2)+dn,					(double)(Y0+elemerk.get_laenge()/f1), 
							   1,  color);
			} 
			// Rohr nach links
			else if (winkel == 180) 
			{
				add_line(X0,										Y0,		
							   X0,										Y0+dn,			
							   1,  color);
				add_line(X0,										Y0,       
							   (double)(X0-elemerk.get_laenge()/f1+50),	Y0, 
							   1,  color);
				add_line(X0,										Y0+dn, 
							   (double)(X0-elemerk.get_laenge()/f1+50),	Y0+dn, 
							   1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1+50),	Y0,		
							   (double)(X0-elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2),			
							   1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1+50),	Y0+dn, 
							   (double)(X0-elemerk.get_laenge()/f1),	Y0+dn-((dn-dnEinz)/2),	
							   1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2), 
							   (double)(X0-elemerk.get_laenge()/f1),	Y0-((dn-dnEinz)/2)+dn, 
							   1,  color);
			} 
			// Rohr nach unten
			else if (winkel == 270) 
			{
				add_line(X0,										Y0,		
							   X0+dn,									Y0,			
							   1,  color);
				add_line(X0,										Y0,       
							   X0,										(double)(Y0-elemerk.get_laenge()/f1+50), 
							   1,  color);
				add_line(X0+dn,									Y0,       
							   X0+dn,									(double)(Y0-elemerk.get_laenge()/f1+50), 
							   1,  color);
				add_line(X0,										(double)(Y0-elemerk.get_laenge()/f1+50),		
							   X0+((dn-dnEinz)/2),						(double)(Y0-elemerk.get_laenge()/f1),			
							   1,  color);
				add_line(X0+dn,									(double)(Y0-elemerk.get_laenge()/f1+50), 
							   X0+dn-((dn-dnEinz)/2),					(double)(Y0-elemerk.get_laenge()/f1),	
							   1,  color);
				add_line(X0+((dn-dnEinz)/2),						(double)(Y0-elemerk.get_laenge()/f1), 
							   X0-((dn-dnEinz)/2)+dn,					(double)(Y0-elemerk.get_laenge()/f1), 
							   1,  color);
			}
		}
		// Einziehung links & rechts
		else if (einz == ANFANG_ENDE)
		{
			// Rohr nach rechts
			if (winkel == 0)
			{
				add_line(X0,										Y0+((dn-dnEinz)/2),		
							   X0,										Y0+((dn-dnEinz)/2)+dnEinz,			
							    1,  color);
				add_line(X0,										Y0+((dn-dnEinz)/2),		
							   X0+50,									Y0,			
								1,  color);
				add_line(X0,										Y0+((dn-dnEinz)/2)+dnEinz, 
							   X0+50,									Y0+dn,	
								1,  color);
				add_line(X0+50,									Y0,       
							   (double)(X0+elemerk.get_laenge()/f1-50),	Y0, 
								1,  color);
				add_line(X0+50,									Y0+dn, 
							   (double)(X0+elemerk.get_laenge()/f1-50),	Y0+dn, 
								1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1-50),	Y0,		
							   (double)(X0+elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2),			
								1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1-50),	Y0+dn, 
							   (double)(X0+elemerk.get_laenge()/f1),	Y0+dn-((dn-dnEinz)/2),	
								1,  color);
				add_line((double)(X0+elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2), 
							   (double)(X0+elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2)+dnEinz, 
								1,  color);
			} 
			// Rohr nach oben
			else if (winkel == 90) 
			{
				add_line(X0+((dn-dnEinz)/2),						Y0,		
							   X0+((dn-dnEinz)/2)+dnEinz,				Y0,			
							    1,  color);
				add_line(X0+((dn-dnEinz)/2),						Y0,		
							   X0,										Y0+50,			
								1,  color);
				add_line(X0+((dn-dnEinz)/2)+dnEinz,				Y0, 
							   X0+dn,									Y0+50,	
								1,  color);
				add_line(X0,										Y0+50,       
							   X0,										(double)(Y0+elemerk.get_laenge()/f1-50), 
								1,  color);
				add_line(X0+dn,									Y0+50, 
							   X0+dn,									(double)(Y0+elemerk.get_laenge()/f1-50), 
								1,  color);
				add_line(X0,										(double)(Y0+elemerk.get_laenge()/f1-50),		
							   X0+((dn-dnEinz)/2),						(double)(Y0+elemerk.get_laenge()/f1),			
								1,  color);
				add_line(X0+dn,									(double)(Y0+elemerk.get_laenge()/f1-50), 
							   X0+dn-((dn-dnEinz)/2),					(double)(Y0+elemerk.get_laenge()/f1),	
								1,  color);
				add_line(X0+((dn-dnEinz)/2),						(double)(Y0+elemerk.get_laenge()/f1), 
							   X0+((dn-dnEinz)/2)+dnEinz,				(double)(Y0+elemerk.get_laenge()/f1), 
								1,  color);
			} 
			// Rohr nach links
			else if (winkel == 180) 
			{
				add_line(X0,										Y0+((dn-dnEinz)/2),		
							   X0,										Y0+((dn-dnEinz)/2)+dnEinz,			
							    1,  color);
				add_line(X0,										Y0+((dn-dnEinz)/2),		
							   X0-50,									Y0,			
								1,  color);
				add_line(X0,										Y0+((dn-dnEinz)/2)+dnEinz, 
							   X0-50,									Y0+dn,	
								1,  color);
				add_line(X0-50,									Y0,       
							   (double)(X0-elemerk.get_laenge()/f1+50),	Y0, 
								1,  color);
				add_line(X0-50,									Y0+dn, 
							   (double)(X0-elemerk.get_laenge()/f1+50),	Y0+dn, 
								1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1+50),	Y0,		
							   (double)(X0-elemerk.get_laenge()/f1),		Y0+((dn-dnEinz)/2),			
								1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1+50),	Y0+dn, 
							   (double)(X0-elemerk.get_laenge()/f1),	Y0+dn-((dn-dnEinz)/2),	
								1,  color);
				add_line((double)(X0-elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2), 
							   (double)(X0-elemerk.get_laenge()/f1),	Y0+((dn-dnEinz)/2)+dnEinz, 
								1,  color);
			} 
			// Rohr nach unten
			else if (winkel == 270) 
			{
				add_line(X0+((dn-dnEinz)/2),						Y0,		
							   X0+((dn-dnEinz)/2)+dnEinz,				Y0,			
							    1,  color);
				add_line(X0+((dn-dnEinz)/2),						Y0,		
							   X0,										Y0-50,			
								1,  color);
				add_line(X0+((dn-dnEinz)/2)+dnEinz,				Y0, 
							   X0+dn,									Y0-50,	
								1,  color);
				add_line(X0,										Y0-50,       
							   X0,										(double)(Y0-elemerk.get_laenge()/f1+50), 
								1,  color);
				add_line(X0+dn,									Y0-50, 
							   X0+dn,									(double)(Y0-elemerk.get_laenge()/f1+50), 
								1,  color);
				add_line(X0,										(double)(Y0-elemerk.get_laenge()/f1+50),		
							   X0+((dn-dnEinz)/2),						(double)(Y0-elemerk.get_laenge()/f1),			
								1,  color);
				add_line(X0+dn,									(double)(Y0-elemerk.get_laenge()/f1+50), 
							   X0+dn-((dn-dnEinz)/2),					(double)(Y0-elemerk.get_laenge()/f1),	
								1,  color);
				add_line(X0+((dn-dnEinz)/2),						(double)(Y0-elemerk.get_laenge()/f1), 
							   X0+((dn-dnEinz)/2)+dnEinz,				(double)(Y0-elemerk.get_laenge()/f1), 
								1,  color);
			}
		}
		break;
	}
}


void RohrQx::set_show_dn(int value)
{
	show_dn = value;
}
