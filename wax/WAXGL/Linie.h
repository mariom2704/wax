#pragma once

#include "globj.h"

/**
In diesem Modul wird die Schnittstelle zur Klasse Linie definiert
Die Wax+ Grafikschnittstelle besteht aus einer Klassenhierarchie, die auf dem GlObj
als Basisklasse beruht. Ableitungen sind Linie, Polygon, Kreis und Ellipse, 
Rechteck und Quadrat.
*/


class WAXGLDLL Linie : public GlObj  
{
private:

public:
	virtual void getmax_xy(double & x, double & y) const;

	virtual void getmin_xy(double & x, double & y) const;

	//Defaultkonstruktor
	Linie();
	
	//Konstruktor
	Linie(double X0, double Y0, double X1, double Y1, int Strichstaerke = 1, enum colour Color = GlObj::BLACK) ;
	
	//Destruktor
	virtual ~Linie();

	//Speichern des Objektes in eine Datei
	virtual void serialize(GlFile* file);
	
	inline const int get_colorLinie() const;

	inline void set_colorLinie(int value);

	inline const int get_strichstaerke() const;

	inline void set_strichstaerke(int value);

	inline const double get_x1() const;

	inline void set_x1(double value);

	inline const double get_y1() const;

	inline void set_y1(double value);


protected:
	//Endkoordinate x
	double x1;

	//Endkoordinate y
	double y1;

	//Strichstaerke der Linie
	int strichstaerke;

	//Farbe der Linie
	int colorLinie;

};

inline const int Linie::get_colorLinie() const
{
	return colorLinie;
}

inline void Linie::set_colorLinie(int value)
{
	colorLinie = value;
	return;
}

inline const int Linie::get_strichstaerke() const
{
	return strichstaerke;
}

inline void Linie::set_strichstaerke(int value)
{
	strichstaerke = value;
	return;
}

inline const double Linie::get_x1() const
{
	return x1;
}

inline void Linie::set_x1(double value)
{
	x1 = value;
	return;
}

inline const double Linie::get_y1() const
{
	return y1;
}

inline void Linie::set_y1(double value)
{
	y1 = value;
	return;
}
