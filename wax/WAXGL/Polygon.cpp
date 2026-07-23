#include "stdafx.h"


#include "polygon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



/*	
In diesem Modul wird die Schnittstelle zur Klasse GlObj implementiert
@ex Beispiel für die Ausgabe eines Kästchenrasters mit allen 256 Farben der Standardpalette |

DWFFile f;
Polygon p;

CPoint pt;
pt.x = 100;
pt.y = 0;
p.points.Add(pt);
pt.x = 100;
pt.y = 100;
p.points.Add(pt);
pt.x = 0;
pt.y = 100;
p.points.Add(pt);
pt.x = 0;
pt.y = 0;
p.points.Add(pt);
p.SerializeColorSquares(&f);

f.DWFClose();
	

void Polygon::SerializeColorSquares(DWFFile *file)
{
	const int ipts = points.GetSize( );
	const int len =  ipts * sizeof(WT_Logical_Point);
	
	WT_Logical_Point *vPoints = (WT_Logical_Point *)new char[len];
	
	for (int i = 0; i < points.GetSize(); i++)
	{
		vPoints[i].m_x = points.GetAt(i).x;
		vPoints[i].m_y = points.GetAt(i).y;
	}

	WT_File *f = (WT_File*)file->m_pWtFile;
    f->desired_rendition().line_weight () = 1;
    f->desired_rendition().fill() = WD_False;
    f->desired_rendition().color() = WT_Color (m_color, f->desired_rendition().color_map());

	WT_Polygon pg1 (ipts, vPoints, WD_True);
	WT_Polyline p (ipts, vPoints, WD_False);

	int dy = 0;
	int j = 0;
	for ( i = 0; i < 256; i++)
	{
		WT_Polygon pg2 (ipts, vPoints, WD_True);
		WT_Polyline p2 (ipts, vPoints, WD_False);
		j++;
		if (!(i%10))
		{
			dy += 100;
			j = 0;
		}

		WT_Logical_Point pt;
		pt.m_x = j*100;
		pt.m_y = j*100+dy;
		WT_Transform trans(pt, 1, 1, 0);
		p2.transform(trans);
		pg2.transform(trans);
		f->desired_rendition().color() = WT_Color (i, f->desired_rendition().color_map());
		pg2.serialize (*f);
		f->desired_rendition().color() = WT_Color (0, f->desired_rendition().color_map());
		p2.serialize(*f);
		// Index ausgeben (in Kästchen)
		CString index; index.Format("%i", i);

		WT_Logical_Point txtpt;
		txtpt.m_x = vPoints[0].m_x + 10;
		txtpt.m_y = vPoints[0].m_y + 10;
		
		WT_Text txt(txtpt, (LPCTSTR)index);
		txt.transform(trans);
		WT_Font ft("Arial", false, false, false, 0, 0, 0, 50, 0, 700, 1000, 0, 1);
		ft.serialize(*f);
		txt.serialize(*f);
	}
	delete (char*)vPoints;
}
*/

//Konstruktor
WaxPolygon::WaxPolygon()
{
	drehwinkel = 0;
}

//2. Konstruktor
WaxPolygon::WaxPolygon(double X0, double Y0, enum colour Color) : GlObj(X0, Y0, Color)
{
	drehwinkel = 0;
}

//Destruktor
WaxPolygon::~WaxPolygon()
{

}

void WaxPolygon::add_point(Point p)
{
	points.push_back(p); 
}
	
//Speichern des Objektes in eine Datei
void WaxPolygon::serialize(GlFile* file)
{
	file->draw_polygon((int)x0, (int)y0, points, color);
	/*
	GlObj::serialize(file);
	//CArray
	// Als letzter Punkt muss 0,0 eingetragen sein.
	CPoint lastpt = points.ElementAt(points.GetSize( )-1);

	if (lastpt.x != 0 || lastpt.y != 0)
		points.Add(CPoint(0,0));
	const int ipts = points.GetSize( );
	const int len =  ipts * sizeof(WT_Logical_Point);

	WT_Logical_Point *vPoints = (WT_Logical_Point *)new char[len];


    //WT_Logical_Point3D *vPoints = (WT_Logical_Point *)new char[len];
	WT_Logical_Point transpt;
	transpt = get_WT_Logical_Point(x0, y0);
	
	WT_Transform trans(transpt, 1, 1, drehwinkel);

	for (int i = 0; i < points.GetSize(); i++)
	{
		vPoints[i].m_x = points.GetAt(i).x;
		vPoints[i].m_y = points.GetAt(i).y;
	}

	WT_File *f = (WT_File*)file->m_pWtFile;
  //  f->desired_rendition().line_weight () = 1;
//    f->desired_rendition().fill() = WD_False;
	XASSERT(color < 256 && color >= 0);
    f->desired_rendition().color() = WT_Color (color, f->desired_rendition().color_map());

	//WT_Polygon pg1 (ipts, vPoints, WD_True);
	WT_Polyline p (ipts, vPoints, WD_True);
	//pg1.transform(trans);
	p.transform(trans);
	f->desired_rendition().color() = WT_Color (color, f->desired_rendition().color_map());
	//pg1.serialize (*f);
	//f->desired_rendition().color() = WT_Color (BLACK, f->desired_rendition().color_map());
	p.serialize(*f);
	delete (char*)vPoints;
	*/
}

void WaxPolygon::getmin_xy(double & x, double & y) const
{
	GlObj::getmin_xy(x, y);
	for (std::vector<Point>::const_iterator it = points.begin(); it != points.end(); it++)
	{
		if (x > it->x)
			x = it->x;
		if (y > it->y)
			y = it->y;
	}
}

void WaxPolygon::getmax_xy(double & x, double & y) const
{
	GlObj::getmax_xy(x, y);
	for (std::vector<Point>::const_iterator it = points.begin(); it != points.end(); it++)
	{
		if (x < it->x)
			x = it->x;
		if (y < it->y)
			y = it->y;
	}

}


/// Default Konstruktor
Quadrat::Quadrat()
{

}
/// Konstruktor
Quadrat::Quadrat(double X0, double Y0, double Dx, enum colour Color) : Rechteck(X0, Y0, Dx, Dx, Color)
{

}

/// Destruktor
Quadrat::~Quadrat()
{
}

//////////////////////////////////////////////////////////////////////
// Rechteck Klasse
//////////////////////////////////////////////////////////////////////

/// Default Konstruktor
Rechteck::Rechteck()
{

}
/// Konstruktor
Rechteck::Rechteck(double X0, double Y0, double Dx, double Dy, enum colour Color) : WaxPolygon(X0, Y0, Color)
{
	points.push_back(Point(0,0));
	points.push_back(Point(0, get_LONG(Dy)));
	points.push_back(Point(get_LONG(Dx), get_LONG(Dy)));
	points.push_back(Point(get_LONG(Dx), 0));
	points.push_back(Point(0, 0));
}

/// Destruktor
Rechteck::~Rechteck()
{

}

#ifdef _DEBUG
/*
void Polygon::SerializeColorSquares(DWFFile *file)
{
	const int ipts = points.GetSize( );
	const int len =  ipts * sizeof(WT_Logical_Point);
	
	WT_Logical_Point *vPoints = (WT_Logical_Point *)new char[len];
	
	for (int i = 0; i < points.GetSize(); i++)
	{
		vPoints[i].m_x = points.GetAt(i).x;
		vPoints[i].m_y = points.GetAt(i).y;
	}

	WT_File *f = (WT_File*)file->m_pWtFile;
    f->desired_rendition().line_weight () = 1;
    f->desired_rendition().fill() = WD_False;
    f->desired_rendition().color() = WT_Color (m_color, f->desired_rendition().color_map());

	WT_Polygon pg1 (ipts, vPoints, WD_True);
	WT_Polyline p (ipts, vPoints, WD_False);

	int dy = 0;
	int j = 0;
	for ( i = 0; i < 256; i++)
	{
		WT_Polygon pg2 (ipts, vPoints, WD_True);
		WT_Polyline p2 (ipts, vPoints, WD_False);
		j++;
		if (!(i%10))
		{
			dy += 100;
			j = 0;
		}

		WT_Logical_Point pt;
		pt.m_x = j*100;
		pt.m_y = j*100+dy;
		WT_Transform trans(pt, 1, 1, 0);
		p2.transform(trans);
		pg2.transform(trans);
		f->desired_rendition().color() = WT_Color (i, f->desired_rendition().color_map());
		pg2.serialize (*f);
		f->desired_rendition().color() = WT_Color (0, f->desired_rendition().color_map());
		p2.serialize(*f);
		// Index ausgeben (in Kästchen)
		CString index; index.Format("%i", i);

		WT_Logical_Point txtpt;
		txtpt.m_x = vPoints[0].m_x + 10;
		txtpt.m_y = vPoints[0].m_y + 10;
		
		WT_Text txt(txtpt, (LPCTSTR)index);
		txt.transform(trans);
		WT_Font ft("Arial", false, false, false, 0, 0, 0, 50, 0, 700, 1000, 0, 1);
		ft.serialize(*f);
		txt.serialize(*f);
	}
	delete (char*)vPoints;
}
*/
#endif

