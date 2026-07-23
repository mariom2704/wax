#include "stdafx.h"


#include "bemassung.h"
#include "textplot.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//2. Konstruktor
Bemassung::Bemassung(double X0, double Y0, double X1, double Y1, int Ebene, bool Nachoben, bool Vertikal, double FaktorRohrlaenge, CString Text)  
: GlObj(X0, Y0, GlObj::BLACK), m_fontGroesse(0)
{
	x1 = X1;
	y1 = Y1;
	faktorRohrlaenge = FaktorRohrlaenge;
	index_ebene = Ebene;
	nachOben = Nachoben;
	vertikal = Vertikal; 
	double len = (vertikal ? (y1 - y0) : (x1 - x0)) * faktorRohrlaenge;

	// negative Längen werden disabled. Bei mehreren Rohrstücken und einer grossen Gesamt
	// länge wird sonst eine Gesamtlänge von -etlichen tausend angezeigt
	disabled = len > 0 ? false : true;
	text.Format("%s%d", Text, (int)(len+0.5));
	XASSERT((X1 != X0) || (Y1 != Y0));
	if ((X1 == X0) && (Y1 == Y0))
		X1 += 10;
}

//Destruktor
Bemassung::~Bemassung()
{

}
//Speichern des Objektes in eine Datei
void Bemassung::serialize(GlFile* file)
{
	if (disabled)
		return;
	const enum GlObj::colour farbe = GlObj::BLACK;
	const int strichstaerke = 1;

	double xa, ya, xb, yb;
	double txtx, txty;
	getCoords(xa, ya, xb, yb);
//	TRACE("->Bemassung '%s': %i, %i, %i, %i\n", text, xa, ya, xb, yb);
	getTxtCoords(txtx, txty);
	
	// horizontaler/vertikaler Strich
	if (WaxSys::para_int(BEMASSUNGSLINIE) == 1)
	{
		Linie l3(xa, ya, xb, yb, strichstaerke, farbe);
		l3.serialize(file);
	}
	
	// vertikale schräge Striche
	//Linie l1(xa-TRENNLINIENLAENGE, ya-TRENNLINIENLAENGE, xa+TRENNLINIENLAENGE, ya+TRENNLINIENLAENGE, strichstaerke, farbe);
	//Linie l2(xb-TRENNLINIENLAENGE, yb-TRENNLINIENLAENGE, xb+TRENNLINIENLAENGE, yb+TRENNLINIENLAENGE, strichstaerke, farbe);
	
	//senkrechte/waagerechte Striche
	if (get_vertikal())
	{
		Linie l1(xa-TRENNLINIENLAENGE, ya, xa+TRENNLINIENLAENGE, ya, strichstaerke, farbe);
		Linie l2(xb-TRENNLINIENLAENGE, yb, xb+TRENNLINIENLAENGE, yb, strichstaerke, farbe);
		l1.serialize(file);
		l2.serialize(file);
	}
	else
	{
		Linie l1(xa, ya-TRENNLINIENLAENGE, xa, ya+TRENNLINIENLAENGE, strichstaerke, farbe);
		Linie l2(xb, yb-TRENNLINIENLAENGE, xb, yb+TRENNLINIENLAENGE, strichstaerke, farbe);
		l1.serialize(file);
		l2.serialize(file);
	}
	TextPlot txt(txtx, txty, text, farbe, true, vertikal ? 90 : 0);
	txt.set_zoom(get_zoom());
	if(m_fontGroesse > 0)
	{
		txt.set_fontGroesse(txt.get_fontGroesse()+ m_fontGroesse);
	}
	if (text.GetLength() > 0)
		txt.serialize(file);
}

const bool Bemassung::get_vertikal() const
{
	return vertikal;
}



//Prüft die absoluten xy-Koordinaten der Bemassung auf einen minimalen 
//Abstand (EBENENABSTAND).
bool Bemassung::testCoordinates(const Bemassung& other)
{
	if (vertikal != other.get_vertikal() || this == &other)
		return false;

	double xa, ya, xb, yb;
	double xa1, ya1, xb1, yb1;
	getCoords(xa, ya, xb, yb);
	other.getCoords(xa1, ya1, xb1, yb1);
	double diff = abs(vertikal ? (xa1-xa) : (ya1-ya));
	if (diff < EBENENABSTAND)
	{
		// Ein Pixel Differenz wird toleriert
		const int tolerance = 1;
		// Ebene wird nur erhöht, wenn die x Koordinaten (bei horizontal) und
		// bzw. die y Koordinaten (bei vertikal) sich überschneiden
		double max, min, test1, test2;
		if (vertikal)
		{
			max = (ya > yb ? ya : yb) - tolerance;
			min = (ya < yb ? ya : yb) + tolerance;
			test1 = ya1;
			test2 = yb1;
		}
		else
		{
			max = (xa > xb ? xa : xb) - tolerance;
			min = (xa < xb ? xa : xb) + tolerance;
			test1 = xa1;
			test2 = xb1;
		}
		if (test1 <= min && test2 <= min || test1 >= max && test2 >= max)
				return false;
		index_ebene++;
		return true;
	}
	return false;
}


//Liefert die absoluten Koordinaten für die Bemassungslinie
void Bemassung::getCoords(double& x0, double& y0, double& x1, double& y1) const
{
	int eb = index_ebene;
	if (get_zoom() > 12)
		eb += 3;
	else if (get_zoom() > 10)
		eb += 2;
	else if (get_zoom() > 8)
		eb += 1;
	
	int Abstand = nachOben ? (eb) * EBENENABSTAND : - (eb) * EBENENABSTAND;
	int verbAbstd = nachOben ? -10 : 10;

	if (vertikal) 
	{
		x0 = this->x0 + Abstand + verbAbstd;
		y0 = this->y0;
		x1 = this->x0 + Abstand + verbAbstd;
		y1 = this->y1;
	}
	else
	{
		x0 = this->x0;
		y0 = this->y0 + Abstand + verbAbstd;
		x1 = this->x1;
		y1 = this->y0 + Abstand + verbAbstd;
	}
}



//Liefert die absoluten Koordinaten für den Bemassungstext
void Bemassung::getTxtCoords(double& x, double& y) const
{
	double xa, ya, xb, yb;
	getCoords(xa, ya, xb, yb);
	x = xa;
	y = ya;
	if (vertikal)
		y = ya + (yb - ya) / 2;
	else
		x = xa + (xb - xa) / 2;
}



//Prüft die absoluten xy-Koordinaten der Bemassung auf Gleichheit
bool Bemassung::sameCoordinates(const Bemassung& other)
{
	if (other.get_vertikal() != get_vertikal())
		return false;
	double xa, ya, xb, yb;
	double xa1, ya1, xb1, yb1;
	getCoords(xa, ya, xb, yb);
	other.getCoords(xa1, ya1, xb1, yb1);


	if (((ya1 == ya && yb1 == yb) || (ya1 == yb && yb1 == ya))
 	 && ((xa1 == xa && xb1 == xb) || (xa1 == xb && xb1 == xa)))
		return true;

	return false;
}



void Bemassung::getmax_xy(double& x, double& y) const
{
	Linie a(x0, y0, x1, y1);
	a.getmax_xy(x, y);

	double xa, ya, xb, yb;

	// Linienkoordinaten berechnen und prüfen
	// Die Koordinaten errechnen sich mit Hilfe von index_ebene, so dass hier nicht
	// einfach die x/y Koordinaten aus globj genommen werden dürfen
	getCoords(xa, ya, xb, yb);
	if (x < xa)
		x = xa;
	if (x < xb)
		x = xb;
	
	if (y < ya)
		y = ya;
	if (y < yb)
		y = yb;
// Textkoordinaten berechnen und prüfen
	getTxtCoords(xa, ya);
	TextPlot txt(xa, ya, text, (enum GlObj::colour)0, true, vertikal ? 90 : 0);
	txt.getmax_xy(x, y);	
}

void Bemassung::getmin_xy(double& x, double& y) const
{
	Linie a(x0, y0, x1, y1);
	a.getmin_xy(x, y);
	
	double xa, ya, xb, yb;

	// Linienkoordinaten berechnen und prüfen
	// Die Koordinaten errechnen sich mit Hilfe von index_ebene, so dass hier nicht
	// einfach die x/y Koordinaten aus globj genommen werden dürfen
	getCoords(xa, ya, xb, yb);
	if (x > xa)
		x = xa;
	if (x > xb)
		x = xb;
	
	if (y > ya)
		y = ya;
	if (y > yb)
		y = yb;

	// Textkoordinaten berechnen und prüfen
	getTxtCoords(xa, ya);
	TextPlot txt(xa, ya, text, (enum GlObj::colour)0, true, vertikal ? 90 : 0);
	txt.getmin_xy(x, y);
}

