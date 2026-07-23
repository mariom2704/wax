#include "stdafx.h"


#include "constructbogen.h"
#include "kreis.h"
#include "textplot.h"

#include "..\wax\resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructBogen::~ConstructBogen()
{

}

ConstructBogen::ConstructBogen()
{
	Element nullelement;
	ele_entleer.copy(nullelement);
}

ConstructBogen::ConstructBogen(CObList &entleerung)
{
	Element nullelement;
	XASSERT(entleerung.GetCount() == 1 || !entleerung.GetCount());
	if (entleerung.GetCount() == 1)
		ele_entleer.copy(*dynamic_cast<Element*>(entleerung.GetHead()));
	else ele_entleer.copy(nullelement);
}


void ConstructBogen::createBeschriftung()
{
	ConstructRohrEle::createBeschriftung();

	double f1 = faktorRohrlaenge;
	if (elemerk.get_id() == BOG2S) 
		text = "BOG2S";
	else
		return;

	switch (typ) 
	{
	case IMROHR:
		if (drehfaktor == 0 && bogen)
			add_text(	X0 + (double)(elemerk.get_abstAbs() - laenge + rohrDn)/f1 + 50,					
								Y0 + rohrDn, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 0)
			add_text(	X0 + (double)(-lEndeLaenge + elemerk.get_abstAbs() - laenge + rohrDn)/f1 + 50,	
								Y0, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 180)
			add_text(	X0 + (double)(-elemerk.get_abstAbs() + laenge)/f1 - 50,					
								Y0 + rohrDn, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 90 && elemerk.get_winkel() == 0) 
			add_text(	X0 + rohrDn + 50,		
								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 90 && elemerk.get_winkel() == 180) 
			add_text(	X0 - 50,		
								Y0 + (double)(elemerk.get_abstAbs() - laenge)/f1, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 270 && elemerk.get_winkel() == 180) 
			add_text(	X0 + rohrDn + 50,		
								Y0 + (double)(-elemerk.get_abstAbs() + laenge)/f1, 
								text, GlObj::BLACK, false, 45);
		else if (drehfaktor == 270 && elemerk.get_winkel() == 0) 
			add_text(	X0 - 150,		
								Y0 + (double)(-elemerk.get_abstAbs() + laenge)/f1, 
								text, GlObj::BLACK, false, 45);
		break;
	}
}

void ConstructBogen::createElement()
{
	ConstructRohrEle::createElement();

	XASSERT(typ == IMROHR); 

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
}

// Bogen zeichnen mit Elementwinkel 0
void ConstructBogen::drawBogenNachOben(int drehfaktor)
{
	int gradbegin = 0, gradend = 0;
	double rx0 = 0, ry0 = 0;
	double x0 = 0, x1 = 0;
	double y0 = 0, y1 = 0;

	double x = -lEndeLaenge + elemerk.get_abstAbs() - laenge - elemerk.get_laenge();
	double o1 = (elemerk.get_id() == BOG2S) ? 0.5:1.0; 
	double o2 = (elemerk.get_id() == BOG2S) ? 1.5:2.0; 

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
void ConstructBogen::drawBogenNachUnten(int drehfaktor)
{
	int gradbegin = 0, gradend = 0;
	double rx0 = 0, ry0 = 0;
	double x0 = 0, x1 = 0;
	double y0 = 0, y1 = 0;

	double x = -lEndeLaenge + elemerk.get_abstAbs() - laenge - elemerk.get_laenge();
	double o1 = (elemerk.get_id() == BOG2S) ? 0.5:1.0; 
	double o2 = (elemerk.get_id() == BOG2S) ? 1.5:2.0; 

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
void ConstructBogen::drawBogenDummy(int drehfaktor, bool filled)
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
			y2 = Y0 - value -  andn*3/2;
			x2 = X0 + andn;
			add_line(x1, y1, x2, y2);

			y1 = Y0 - value;
			x1 = X0;
			y2 = Y0 - value -  andn*3/2;
			x2 = X0;
			add_line(x1, y1, x2, y2);

			y = Y0 - value -  andn*3/2;
			x = X0 + andn/2;
			add_circle(x, y, andn, Kreis::GRAD_0, Kreis::GRAD_360, filled);
			}
			break;
		default:
			XASSERT(false);
	}
}

// Allgemeinen Bogen zeichnen als zwei konzentrische Kreise
void ConstructBogen::drawBogen(double x0, double y0, int gradbegin, int gradend)
{
	int r0 = (int)((double)andn / faktorRohrlaenge);
	int r1 = 3 * r0;
	if (elemerk.get_id() == ELEMENT_BOGEN || elemerk.get_id() == BOGVERZ)
	{
		r0 *= 2;
		r1 = 2 * r0;
	}	
	add_circle(x0, y0, r0, gradbegin, gradend, false);
	add_circle(x0, y0, r1, gradbegin, gradend, false);
	drawEntleerung(x0, y0, r0);
}

// Entleerung zeichnen, gilt bis jetzt nur für EntnahmeleitungDWB
void ConstructBogen::drawEntleerung(double x0, double y0, int r0)
{
	// Nur Muffe und Rohrnippel sind gültige Entleerungen
	if (ele_entleer.get_id() == MUFFE || ele_entleer.get_id() == ROHR)
	{
		CString text;
		CString elementname;
		if (ele_entleer.get_id() == MUFFE)
			elementname = getStammdaten().getText("IDS_MUFFE").c_str();
		else if (ele_entleer.get_id() == ROHR)
			elementname = getStammdaten().getText("IDS_ROHRNIPPEL").c_str();
		
		CString str; str = getStammdaten().getText("IDS_ENTLEERUNGa").c_str();
		text.Format(str, elementname, ele_entleer.get_anschlussDN(), ele_entleer.get_laenge());
	
		TRACE("Entleerung\n");
	
		// dx = dy = cos45  * ro, gilt auch für ya
		double xa = x0 + 0.7071 * (double)r0 ;

		// Nach oben oder nach unten vom Mittelpunkt des Bogens, je nach Drehrichtung
		double cos45 = (drehfaktor == 0) ? -0.7071 : 0.7071;
		double ya = y0 + cos45 * (double)r0;
		double fontx = 0, fonty = 0;

		if (drehfaktor == 0)
		{
			if (ele_entleer.get_winkel() == -90)
			{
				add_line(xa-30, ya-ele_entleer.get_dn()/2, xa+ele_entleer.get_laenge(), ya-ele_entleer.get_dn()/2);
				add_line(xa+22, ya+ele_entleer.get_dn()/2, xa+ele_entleer.get_laenge(), ya+ele_entleer.get_dn()/2);
				// Endebegrenzung
				add_line(xa+ele_entleer.get_laenge(), ya+ele_entleer.get_dn()/2, xa+ele_entleer.get_laenge(), ya-ele_entleer.get_dn()/2);

				fontx = xa+ele_entleer.get_laenge() + 20;
				fonty = ya-ele_entleer.get_dn()/2;
			}
			else if (ele_entleer.get_winkel() == -180)
			{
				double yend = ya-ele_entleer.get_dn()/2-ele_entleer.get_laenge()+30;
				
				add_line(xa-30, ya-ele_entleer.get_dn()/2, xa-30, yend);
				add_line(xa+22, ya+ele_entleer.get_dn()/2, xa+22, yend);
				// Endebegrenzung
				add_line(xa-30, yend, xa+22, yend);

				fontx = xa;
				fonty = yend-30;
			}
		}
		else if (drehfaktor == 90)
		{
			if (ele_entleer.get_winkel() == -90)
			{
				double yend = ya+ele_entleer.get_dn()/2+ele_entleer.get_laenge()-30;

				// Beide Startpunkte auf dem Kreisbogen, jeweils dn/2 vom Mittelpunkt entfernt
				double xkreis0 = xa-ele_entleer.get_dn()/2;
				double xkreis1 = xa+ele_entleer.get_dn()/2;

				add_line(xkreis0, ya+22, xkreis0, yend);
				add_line(xkreis1, ya-30, xkreis1, yend);
				// Endebegrenzung
				add_line(xkreis0, yend, xkreis1, yend);

				fontx = xa;
				fonty = yend+30;
			}
			else
				XASSERT(false);
		}
		else
			XASSERT(false);

		add_text2(fontx, fonty, (LPCSTR)text);
		/*TextPlot *textplot = new TextPlot(fontx, fonty, text, GlObj::colour::RED);
		textplot->set_fontGroesse(8);
		textplotList.AddHead( textplot );
		*/
	}
}
