#include "stdafx.h"


#include "linie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Defaultkonstruktor
Linie::Linie(): GlObj(0, 0, GlObj::BLACK)
{
	x1 = 0;
	y1 = 0;
	strichstaerke = 50;
}

//Konstruktor
Linie::Linie(double X0, double Y0, double X1, double Y1, int Strichstaerke, enum colour Color) 
			: GlObj(X0, Y0, Color)
{
	x1 = X1;
	y1 = Y1;
	strichstaerke = Strichstaerke;
}

//Destruktor
Linie::~Linie()
{

}
	
//Speichern des Objektes in eine Datei
void Linie::serialize(GlFile* file)
{
	file->draw_line((int)x0, (int)y0, (int)x1, (int)y1, color, strichstaerke);
}



void Linie::getmax_xy(double & x, double & y) const
{
	GlObj::getmax_xy(x, y);
	if (x < x1)
		x = x1;
	if (y < y1)
		y = y1;
}

void Linie::getmin_xy(double & x, double & y) const
{
	GlObj::getmin_xy(x, y);
	if (x > x1)
		x = x1;
	if (y > y1)
		y = y1;
}

