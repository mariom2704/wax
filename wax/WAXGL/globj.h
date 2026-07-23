#pragma once


#define WHIP_STATIC_LIB
#include <dwf\whip_toolkit.h>

#include "GlFile.h"

#include <math.h>

/// Basisklasse für alle Grafikklassen
/// Basis für alle Grafikelemente ist GlObj. 
/// Eigenschaften sind eine Farbe und die Startkoordinaten
/// 
/// Die Wax+ Grafikschnittstelle besteht aus einer Klassenhierarchie, die auf dem GlObj
/// als Basisklasse beruht. Ableitungen sind Polygon, Kreis und Ellipse, Rechteck und Quadrat.
/// 
/// Diese Klassen benutzen die Klassenbibliothek von Whiptk. Whiptk ist eine Klassenbibliothek von Autodesk 
/// zur Erzeugung von DWF Dateien. Das DWF Format wird von Autodesk als Standard für die Bereitstellung
/// von Zeichnungen übers Internet gefördert. 
/// 
/// Es wäre eine Möglichkeit, die Klassen des Whiptk als Basisklassen zu benutzen. 
/// Darauf wird hier verzichtet,
/// da nur kleine Teile des Toolkits überhaupt benutzt werden. Ausserdem wird so 
/// eine Portierung erleichtert, 
/// falls eine andere Schnittstelle als DWF benutzt werden soll.
class WAXGLDLL GlObj : public CObject  
{
private:
	int zoom;

	
public:
	/// Farbdefinitionen laut Whiptk Standardpalette
	enum colour { 
		NONE = -1, /// Leer, nicht gefüllt, ohne Farbe
		BLACK = 0, /// schwarz
		GREY = 240, /// grau
		RED = 249, /// rot
		WHITE = 246, /// weiss
		BLUE = 50, /// blau
		GREEN = 42,/// grün
		YELLOW = 251, /// gelb
		LIGHTGREY = 242, /// hellgrau
		BROWN = 130,	/// braun
		UNBEKANNT = 255, // ??????????? 
	} ;

	/// Konstruktor
	GlObj();
	
	/// Konstruktor
	GlObj(double X0, double Y0, enum colour Color);

	/// Startkoordinaten des Objektes setzen
	void setStart_x0y0(double X0, double Y0);
		

	static std::string colour_str(colour c)
	{
		switch (c)
		{
			case NONE: return "none";
			case BLACK: return "black";
			case GREY: return "grey";
			case RED: return "red";
			case WHITE: return "white";
			case BLUE: return "blue";
			case GREEN: return "green";
			case YELLOW: return "yellow";
			case LIGHTGREY: return "lightgrey";
			case BROWN: return "brown";
			case UNBEKANNT: return "white";
		}
		return "black";
	}

	inline const int get_color() const;

	inline void set_color(enum GlObj::colour value);

	inline const int get_drehwinkel() const;

	inline void set_drehwinkel(int value);

	inline const double get_x0() const;

	inline void set_x0(double value);

	inline const double get_y0() const;

	inline void set_y0(double value);

protected:
	/// Drehwinkel
	int drehwinkel;

	/// Farbe
	int color;
	
	/// Startkoordinate y
	double y0;
	
	/// Startkoordinate x
	double x0;

public:
	WT_Logical_Point get_WT_Logical_Point(double x, double y);

	LONG get_LONG(double x);

	const int get_zoom() const;

	void set_zoom(int value);

	/// Serialisierungsfuntion,
	virtual void serialize(GlFile* file) = NULL;

	virtual void getmin_xy(double & x, double & y) const;

	virtual void getmax_xy(double & x, double & y) const;

};

inline const int GlObj::get_color() const
{
	return color;
}

inline void GlObj::set_color(enum GlObj::colour value)
{
	color = value;
	return;
}

inline const int GlObj::get_drehwinkel() const
{
	return drehwinkel;
}

inline void GlObj::set_drehwinkel(int value)
{ ///  Mehr geht im Moment rnoch nicht
	XASSERT(value == 0 || value == 90 || value == 180 || value == 270 || value == 360);
	drehwinkel = value;
	return;
}

inline const double GlObj::get_x0() const
{
	return x0;
}

inline void GlObj::set_x0(double value)
{
	x0 = value;
	return;
}

inline const double GlObj::get_y0() const
{
	return y0;
}

inline void GlObj::set_y0(double value)
{
	y0 = value;
	return;
}

