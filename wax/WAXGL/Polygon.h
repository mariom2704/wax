#pragma once

#include "globj.h"

#include <vector>
#include "Point.h"
/**	
In diesem Modul wird die Schnittstelle zur Klasse GlObj definiert
Die Wax+ Grafikschnittstelle besteht aus einer Klassenhierarchie, die auf dem GlObj
als Basisklasse beruht. Ableitungen sind Polygon, Kreis und Ellipse, Rechteck und Quadrat.

Diese Klassen benutzen die Klassenbibliothek von Whiptk. Whiptk ist eine Klassenbibliothek von Autodesk 
zur Erzeugung von DWF Dateien. Das DWF Format wird von Autodesk als Standard für die Bereitstellung
von Zeichnungen übers Internet gefördert. 

Es wäre eine Möglichkeit, die Klassen des Whiptk als Basisklassen zu benutzen. Darauf wird hier verzichtet,
da nur kleine Teile des Toolkits überhaupt benutzt werden. Ausserdem wird so eine Portierung erleichtert, 
falls eine andere Schnittstelle als DWF benutzt werden soll.
*/

/// Ein Polygon hat folgende Eigenschaften: Mindesten vier Punkte, die miteinander 
/// verbunden werden, umrandet und mit der Farbe gefüllt werden.
/// 
///  Erzeugt wird ein Polygon, indem das Array m_polygon mit Koordinaten gefüllt 
/// wird. Diese Koordinaten sind die Abmessungen des Objektes. Plaziert wird es, 
/// indem <mf GlObj::setStart_x0y0> aufgerufen wird. Die Startkoordinaten in GlObj 
/// beschreiben den Offset des Objektes in der Ausgabe. Standardmässig wird als 
/// erster Polygonpunkt x = 0 und y = 0 angenommen
/// 
/// Beispiel: Quadrat der Seitenlänge 3000 an den Koordinaten x = 100, y = 300 in 
/// Grün anzeigen. |
/// TRY
/// {
/// 	DWFFile f;
/// 	Polygon pol(100, 300, GlObj::color::GREEN);
/// 	CPoint pt[4] = {
/// 		0, 3000,
/// 		3000, 3000,
/// 		3000, 0,
/// 		0,0
/// 	};
/// 	
/// 	for (int i = 0; i < 4; i++)
/// 		pol.m_polygon.Add(pt[i]);
/// 	
/// 	pol.serialize(f);
/// 	f.close();  
/// }
/// CATCH_ALL(e)
/// {
/// 	e->ReportError();
/// }
/// END_CATCH_ALL

class WAXGLDLL WaxPolygon : public GlObj  
{
private:

public:
	virtual void getmin_xy(double & x, double & y) const;

	virtual void getmax_xy(double & x, double & y) const;

	/// Polygonpunkte
	std::vector<Point> points;

	/// Konstruktor
	WaxPolygon();
	
	/// 2. Konstruktor
	WaxPolygon(double X0, double Y0, enum colour Color = GlObj::BLACK) ;
	
	/// Destruktor
	virtual ~WaxPolygon();
	
	/// Speichern des Objektes in eine Datei
	virtual void serialize(GlFile* file);

	void add_point(Point p);

#ifdef _DEBUG
	/// void SerializeColorSquares(DWFFile *file);
#endif

protected:

};

/**
Spezialisierte Klasse : Ein Rechteck
*/
class Rechteck : public WaxPolygon     
{
public:
	/// Default Konstruktor
	Rechteck();	
	
	/// Konstruktor
	Rechteck(double X0, double Y0, double Dx, double Dy, enum colour Color) ;

	/// Destruktor
	virtual ~Rechteck();

};

/**
Dat Quadrat
*/
class Quadrat : public Rechteck     
{
public:
	/// Default Konstruktor
	Quadrat();
	
	/// Konstruktor
	Quadrat(double X0, double Y0, double Dx, enum colour Color) ;

	/// Destruktor
	virtual ~Quadrat();

};
