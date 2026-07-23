#include "stdafx.h"


#include "textplot.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Konstruktor
TextPlot::TextPlot() : minXY((0,0)), maxXY((0,0))
{
	Set_font(10, "Arial");
	if (WaxSys::para_int(FONTGROESSE) >= 2 && WaxSys::para_int(FONTGROESSE) <= 20)
		fontGroesse = WaxSys::para_int(FONTGROESSE);
	set_color(TextPlot::BLACK);
	mittig = false;
	drehwinkel = 0;
}

/*
Konstruktor
*/
TextPlot::TextPlot(double X0, double Y0, const CString &Txt, TextPlot::colour Color, 
				   bool Mittig, int Drehwinkel, bool Relative) : GlObj(X0, Y0, Color)
{
	Set_font(10, "Arial", Txt);

	if (WaxSys::para_int(FONTGROESSE) >= 2 && WaxSys::para_int(FONTGROESSE) <= 20)
		fontGroesse = WaxSys::para_int(FONTGROESSE);
	mittig = Mittig;
	drehwinkel = Drehwinkel;
	if (drehwinkel == 360)
		drehwinkel = 0;
	calcMaxCoords();
	relative = Relative;
}

//Destruktor
TextPlot::~TextPlot()
{

}

//Serialisierungsfunktion
void TextPlot::serialize(GlFile* file)
{
	//void draw_text(int x0, int y0, const std::string& txt, const std::string& font, int fontgroesse, int zoom, int drehwinkel, int color, bool relative)
	file->draw_text((int)x0, (int)y0, (LPCSTR)text, (LPCSTR)font, fontGroesse, get_zoom(), drehwinkel, color, relative, mittig);
	calcMaxCoords();
}

//Font setzen
void TextPlot::Set_font(int Groesse, const CString &Fontname, const CString &Txt)
{
	fontGroesse = Groesse;
	font = Fontname;
	if (Txt.GetLength())
		text = Txt;
}

//Text setzen
void TextPlot::Set_text(double X, double Y, const CString &Txt)
{
	XASSERT(Txt.GetLength());
	text = Txt;
	setStart_x0y0(X, Y);
}

const int TextPlot::get_drehwinkel() const
{
	return drehwinkel;
}

void TextPlot::set_drehwinkel(int value)
{
	drehwinkel = value;
}


void TextPlot::getmin_xy(double & x, double & y) const
{
	GlObj::getmin_xy(x, y);
	if (minXY.x < x)
		x = minXY.x;
	if (minXY.y < y)
		y = minXY.y;
}

void TextPlot::getmax_xy(double & x, double & y) const
{
	GlObj::getmax_xy(x, y);
	if (maxXY.x > x)
		x = maxXY.x;
	if (maxXY.y > y)
		y = maxXY.y;
}



//Errechnet aus dem Text, der Fontgrösse und dem Drehwinkel die maximalen XY 
//Koordinaten
void TextPlot::calcMaxCoords()
{
	const double faktorx = 2.5;
	const int faktory = 8;

	double x = get_x0();
	double y = get_y0();
	int fg = get_fontGroesse();
	int winkel = get_drehwinkel();
	CString txt = get_text();
	bool mittig = get_mittig();

	double len = txt.GetLength() * fg * faktorx;
	if (mittig)
		len /= 2.0;
	
	int lenx = (int)(len * cos((double)winkel));
	int leny = (int)(len * sin((double)winkel));
	
	maxXY.x = get_LONG(x + lenx);
	minXY.x = get_LONG(x - lenx);
	

	if (leny)
	{
		maxXY.y = get_LONG(y + leny*2);
		minXY.y = get_LONG(y - leny*2);
	}
	else
	{
		maxXY.y = get_LONG(y + faktory*fg);
		minXY.y = get_LONG(y - fg);
	}	
}

const bool TextPlot::get_relative() const
{
	return relative;
}

void TextPlot::set_relative(bool value)
{
	relative = value;
}

