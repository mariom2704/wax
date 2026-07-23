#include "stdafx.h"


#include "kreis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Konstruktor
Kreis::Kreis(double x0, double y0, int durchmesser, enum colour color, int start, int end, bool gefuellt)
			: GlObj(x0, y0, color)
{
	this->durchmesser = durchmesser;
	this->start = start;
	this->end = end;
	this->gefuellt = gefuellt;
}

//Destruktor
Kreis::~Kreis()
{

}

//Serialisierungsfuntion
void Kreis::serialize(GlFile* file)
{
	file->draw_circle(x0, y0, durchmesser, color, start, end, gefuellt ? true : false );
}

