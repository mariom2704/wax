#include "stdafx.h"


#include "globj.h"

#ifdef _DEBUG

#define new DEBUG_NEW
#endif

//Konstruktor
GlObj::GlObj()
{
	x0 = 0;
	y0 = 0;
	color = WHITE;
	set_drehwinkel(0);
	zoom = 10;
}


GlObj::GlObj(double X0, double Y0, enum GlObj::colour Color)
{
	/*
	2. Kontruktor
	Initialisiert alle Membervariablen nach Vorgabe
	*/	
	x0= X0;
	y0 = Y0;
	color = Color;
	set_drehwinkel(0);
	zoom = 10;
}


//Startkoordinaten des Objektes setzen
void GlObj::setStart_x0y0(double X0, double Y0)
{
	x0 = X0;
	y0 = Y0;
}



void GlObj::getmax_xy(double & x, double & y) const
{
	if (x < (double)x0)
		x = (double)x0;
	if (y < (double)y0)
		y = (double)y0;
}



void GlObj::getmin_xy(double & x, double & y) const
{
	/*
	defaultmässig gibts nur zwei Werte
	*/
	if (x > (double)x0)
		x = (double)x0;
	if (y > (double)y0)
		y = (double)y0;
}



const int GlObj::get_zoom() const
{
	return zoom;
}

void GlObj::set_zoom(int value)
{
	zoom = value;
}

WT_Logical_Point GlObj::get_WT_Logical_Point(double x, double y)
{
	WT_Logical_Point lp;
	lp.m_x = (int)x;
	lp.m_y = (int)y; 
	return lp;
}

LONG GlObj::get_LONG(double x)
{
	return (LONG)x;
}

