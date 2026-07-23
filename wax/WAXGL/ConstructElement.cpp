#include "stdafx.h"


#include "constructelement.h"
#include "linie.h"
#include "bemassung.h"
#include "kreis.h"
#include "textplot.h"
#include "polygon.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//ist die Bemassung eine Ebene nach oben verschoben durch geringen rel. Abstand 
bool ConstructElement::bemassungHochgestellt = false;

//faktorRohrlaenge, durch diesen Faktor wird die Rohrlaenge geteilt, 
//User-Parameter
double ConstructElement::faktorRohrlaenge;

//faktorRohrbreite, mit diesen Faktor wird die Rohrbreite multipliziert, 
//User-Parameter
double ConstructElement::faktorRohrbreite;

//faktorElelaenge, mit diesen Faktor wird die Laenge der Abgaenge multipliziert (nicht Rohrstutzen)
double ConstructElement::faktorElelaenge;

//faktorElebreite, durch diesen Faktor wird die Breite der Abgaenge geteilt, 
//User-Parameter
double ConstructElement::faktorElebreite;

//dn2zoll, gibt an ob die Nennweiten in DN oder in Zoll beschriftet werden 
//sollen, User-Parameter
bool ConstructElement::dn2zoll;

//Konstruktor
ConstructElement::ConstructElement()
{
	erstesElement = true; 
	lEndeLaenge = 0, rEndeLaenge = 0; 
	drehfaktor = 0;
	bogenlaenge = 0;
	bogen = false; 
	X0 = 0; Y0 = 0;
	typ = 0;
	text = "";
	showText = true;
	laenge = 0;

	winkel = 0;
	dn = 0;
	andn = 0;
	
	maxRohrDN = 0;
	maxlaengeAbg = 0;
	maxlaengeAbgOben = 0;
	maxlaengeAbgUnten = 0;
	maxlaengeAbgLinks = 0;
	maxlaengeAbgRechts = 0;

	maxlaenge_0 = 0;
	maxlaenge_90 = 0;
	maxlaenge_180 = 0;
	maxlaenge_270 = 0;
	maxlaengeAbg_0 = 0;
	maxlaengeAbg_90 = 0;
	maxlaengeAbg_180 = 0;
	maxlaengeAbg_270 = 0;
	deltaX0 = 0;
	deltaY0 = 0;
	knickBemassung = true;

}

//Destruktor
ConstructElement::~ConstructElement()
{
	for (std::vector<GlObj*>::iterator it = grafiklist.begin(); it != grafiklist.end(); it++)
		delete *it;
}

void ConstructElement::initEleDaten(const Element& ele)
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

	andn = elemerk.get_anschlussDN();
	andn = (int)((double)andn * faktorRohrbreite);

	dn = elemerk.get_dn();
	dn = (int)((double)dn * faktorRohrbreite);
}

void ConstructElement::initEleDatenMerker(Element ele)
{
	merkerRohrele.copy(ele);
	merkerRohrele.set_anschlussDN((int)((double)merkerRohrele.get_anschlussDN()*faktorRohrbreite));
	merkerRohrele.set_dn((int)((double)merkerRohrele.get_dn()*faktorRohrbreite));
	merkerRohrele.set_laenge((merkerRohrele.get_id() != ROHR) ? (int)((double)merkerRohrele.get_laenge()/faktorElelaenge) : merkerRohrele.get_laenge());
}

void ConstructElement::initEleDatenAbgMerk(Element ele)
{
	merkerAbgele.copy(ele);
}

void ConstructElement::initGraphikDaten(double X0, double Y0, int drehfaktor, int bogenlaenge, bool bogen, bool erstesElement, int laenge, int lEndeLaenge, int rEndeLaenge, int typ)
{
	this->X0 = X0;
	this->Y0 = Y0;
	if (Y0 < -2000 && Y0 > 2000)
		this->Y0 = 0;
	this->drehfaktor = drehfaktor;
	this->bogenlaenge = bogenlaenge;
	this->bogen = bogen;
	this->erstesElement = erstesElement;
	this->laenge = laenge;
	this->lEndeLaenge = lEndeLaenge;
	this->rEndeLaenge = rEndeLaenge;
	this->typ = typ;
}

void ConstructElement::initBemassung(int maxlaenge_0, int maxlaenge_90, int maxlaenge_180, int maxlaenge_270, 
									 int maxlaengeAbg_0, int maxlaengeAbg_90, int maxlaengeAbg_180, int maxlaengeAbg_270,
									 int maxlaengeAbg, int rohrDn, CString masstext)
{
	this->maxlaenge_0	= maxlaenge_0;
	this->maxlaenge_90	= maxlaenge_90;
	this->maxlaenge_180 = maxlaenge_180;
	this->maxlaenge_270 = maxlaenge_270;

	this->maxlaengeAbg_0   = maxlaengeAbg_0;
	this->maxlaengeAbg_90  = maxlaengeAbg_90;
	this->maxlaengeAbg_180 = maxlaengeAbg_180;
	this->maxlaengeAbg_270 = maxlaengeAbg_270;

	this->maxlaengeAbg = maxlaengeAbg;
	
	maxlaengeAbgOben = 0;
	maxlaengeAbgUnten = 0;
	maxlaengeAbgLinks = 0;
	maxlaengeAbgRechts = 0;

	if (drehfaktor == 0)
	{
		maxlaengeAbgOben  = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgUnten = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	else if (drehfaktor == 90)
	{
		maxlaengeAbgLinks  = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgRechts = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	else if (drehfaktor == 180)
	{
		maxlaengeAbgUnten = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgOben  = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}
	else if (drehfaktor == 270)
	{
		maxlaengeAbgRechts = (maxlaengeAbg_0>maxlaengeAbg_90)?maxlaengeAbg_0:maxlaengeAbg_90;
		maxlaengeAbgLinks  = (maxlaengeAbg_180>maxlaengeAbg_270)?maxlaengeAbg_180:maxlaengeAbg_270;
	}

	this->rohrDn = (int)((double)rohrDn * faktorRohrbreite);
	this->masstext = masstext;
}

//Setzt die Faktoren der Grafikerstellung. Diese Faktoren sind statisch, da 
//es systemweite Einstellungen sind.
void ConstructElement::setGraphikfaktoren(double rohrlaenge, double rohrbreite, double elelaenge, double elebreite, bool dnzuzoll)
{
	faktorRohrlaenge = rohrlaenge;
	faktorRohrbreite = rohrbreite;
	faktorElelaenge = elelaenge;
	faktorElebreite = elebreite;
	dn2zoll = dnzuzoll;
}

//set maxRohrDN
void ConstructElement::set_maxRohrDN(int value)
{
	XASSERT(value > 0);
	maxRohrDN = (int)((double)value * faktorRohrbreite);
}



void ConstructElement::add_rectangle(double X0, double Y0, double Dx, double Dy, GlObj::colour Color)
{
	grafiklist.push_back(new Rechteck(X0, Y0, Dx, Dy, Color));
}

void ConstructElement::add_line(double X0, double Y0, double X1, double Y1, int Strichstaerke, GlObj::colour Color)
{
	grafiklist.push_back( new Linie(X0, Y0, X1, Y1, Strichstaerke, Color));
}

void ConstructElement::add_line(const Point& a, const Point& b, int Strichstaerke, GlObj::colour Color)
{
	grafiklist.push_back( new Linie(a.x, a.y, b.x, b.y, Strichstaerke, Color));
}

void ConstructElement::add_dimensioning(double X0, double Y0, double X1, double Y1, int Ebene, bool Nachoben, bool Vertikal, CString masstext)
{
	grafiklist.push_back(new Bemassung(X0, Y0, X1, Y1, Ebene, Nachoben, Vertikal, faktorRohrlaenge, masstext));
}

void ConstructElement::add_circle(double X0, double Y0, int Durchmesser, int Start, int End, bool gefuellt)
{
	grafiklist.push_back(new Kreis(X0, Y0, Durchmesser,  GlObj::BLACK, Start, End, gefuellt));
}

/**
Kreis mit Durchmesser an Punkt p mit Füllfarbe c (default NONE)
*/
void ConstructElement::add_circle(const Point& p, int Durchmesser, GlObj::colour c)
{
	grafiklist.push_back(new Kreis(p.x, p.y, Durchmesser,  c == GlObj::NONE ? GlObj::BLACK : c, 0, 65536, c == GlObj::NONE ? false : true));
}


void ConstructElement::add_text(double X0, double Y0, CString Text, GlObj::colour Color, bool Mittig, int Drehwinkel, bool Relative)
{
	grafiklist.push_back(new TextPlot(X0, Y0, Text, Color, Mittig, Drehwinkel, Relative));
}

void ConstructElement::add_text(double X0, double Y0, const string& txt, int Drehwinkel, GlObj::colour Color, bool Mittig)
{
	grafiklist.push_back(new TextPlot(X0, Y0, txt.c_str(), Color, Mittig, Drehwinkel));
}


void ConstructElement::add_text(double X0, double Y0, const string& txt, bool mittig)
{
	grafiklist.push_back(new TextPlot(X0, Y0, txt.c_str(), GlObj::BLACK, mittig, 0));
}

 

void ConstructElement::add_text2(double X0, double Y0, const std::string& text, GlObj::colour Color, int fontgroesse )
{
	TextPlot* txt = new TextPlot(X0, Y0, text.c_str(), Color);
	txt->set_fontGroesse(8);
	grafiklist.push_back(txt);
}

const bool ConstructElement::get_showText() const
{
	return showText;
}

void ConstructElement::set_showText(bool value)
{
	showText = value;
}

/*
void ConstructElement::set_anfendid(int value)
{
	anfendid = value;
}

*/

	
void ConstructElement::set_knickBemassung(bool value)
{
	knickBemassung = value;
}