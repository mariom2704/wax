#include "stdafx.h"


#include "SVGFile.h"
#include "GlObj.h"


#define _USE_MATH_DEFINES
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning ( disable: 4244)
#pragma warning ( disable: 4100)

#define WIDTHKORR 4


//Konstruktor
SVGFile::SVGFile(std::string fname, bool bFileAutodelete) : GlFile(fname, bFileAutodelete)
{
	width = 1;
}

//Destruktor
SVGFile::~SVGFile()
{
	//const int rand = (max_xy.x - min_xy.x) / 30;
	const int rand = abs(min.x-max.x) / 15; // 6,7 % Rand
	min.x -= rand;
	max.x += rand;
	min.y -= rand;
	max.y += rand;
	int DeltaX = (max.x-min.x) * 1.07;
	int MarginX = min.x * 1.10;

	//DeltaX = (max.x-min.x);
	//MarginX = min.x;
	string viewbox = boost::str( boost::format("viewBox = \"%1% %2% %3% %4%\" zoomAndPan=\"magnify\" ") %  MarginX % min.y % DeltaX % (max.y-min.y)  );

	std::string header = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?> \r\n"
	" <!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> \r\n"
	" <svg "  
	+ viewbox + 
	" encoding=\"ISO-8859-1\" \r\n"
	" xmlns=\"http://www.w3.org/2000/svg\" \r\n"
	" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:ev=\"http://www.w3.org/2001/xml-events\" \r\n"
	" version=\"1.1\" baseProfile=\"full\" >\r\n";
	
	//width=\"800mm\" height=\"600mm\">\r\n";
 
	boost::iostreams::stream<boost::iostreams::file_sink> out(fname);
	out << header;
	out << "<wax/> ";
	out << xml;
	out << "</svg>";
    out.close();	
}




///File schliessen
void SVGFile::SVGClose()
{

}

//Neue Ansicht beginnen
void SVGFile::StartView(const CString &name)
{
}

void SVGFile::draw_line(int x0, int y0, int x1, int y1, int color, int width)
{
	y0 *= -1;
	y1 *= -1;
	update_minmaxpos(Point(x0,y0));
	update_minmaxpos(Point(x1,y1));
	string svg_color = "black";
	int w = (width*WIDTHKORR);
	svg_color = GlObj::colour_str((GlObj::colour)color);
	
	string line = boost::str( boost::format("<line x1=\"%1%\" y1=\"%2%\" x2=\"%3%\" y2=\"%4%\" style=\"stroke:%5%;stroke-width:%6%;\" /> \r\n") % x0 % y0 % x1 % y1 % svg_color % w);
	 
	xml += line;
	this->width = width;
}


double correct_angle(double angle, int& quadrant) 
{
	if (angle <= M_PI / 2.0)
	{
		quadrant = 1;
		return M_PI / 2.0 - angle;
	}
	if (angle <= M_PI)
	{
		quadrant = 4;
		return angle - M_PI / 2.0;
	}
	if (angle <= (M_PI * 1.5))
	{
		quadrant = 3;
		return M_PI * 1.5 - angle;
	}
	quadrant = 2;
	return angle - M_PI * 1.5;
}

void SVGFile::draw_circle(double x0, double y0, int durchmesser, int color, int start, int end, bool filled)
{
	update_minmaxpos(Point(x0,y0*-1));
	std::string line;
	//static int cnt = 0;
	//string txt = boost::str( boost::format("%1%") % cnt);
	if (start == 0 && end == 65536 || start == 65536 && end == 0)
	{
		string filling = filled ? GlObj::colour_str((GlObj::colour)color) : "none";
		//draw_text(x0, y0, txt+ " Kreis", "arial", 3, 0, 0, 0, 0, true);

		line = boost::str( boost::format("<circle cx =\"%1%\" cy =\"%2%\" r =\"%3%\" style=\"fill:%4%;stroke:black;stroke-width:%5%\" />\r\n") % x0 % (y0*-1) % (durchmesser/2) % filling % (width*WIDTHKORR) );
	}
	else
	{
		int quadrant, quadrant1;
		double startangle = correct_angle((double)start / 65536.0 * 2 * M_PI, quadrant);
		double endangle = correct_angle((double)end / 65536.0 * 2 * M_PI, quadrant1);

		int dx = sin(startangle) * (double)durchmesser / 2.0;
		if (quadrant > 2)
			dx = dx*-1;
		int dy = cos(startangle) * (double)durchmesser / 2.0;
		if (quadrant == 2 || quadrant == 3)
			dy = dy*-1;

		Point startpoint(x0+dx, y0+dy);

		int dx1 = sin(endangle) * (double)durchmesser / 2.0;
		if (quadrant1 > 2)
			dx1 = dx1*-1;
		int dy1 = cos(endangle) * (double)durchmesser / 2.0;
		if (quadrant1 == 2 || quadrant1 == 3)
			dy1 = dy1*-1;

		Point endpoint(x0+dx1, y0+dy1);
//		draw_line(startpoint.x, startpoint.y, endpoint.x, endpoint.y, 0, 6);
		//int fontgroesse, int zoom, int drehwinkel, int color, bool relative, bool mittig)
		//draw_text(startpoint.x, startpoint.y, txt, "arial", 3, 0, 0, 0, 0, true);

		line = boost::str( boost::format("<path d=\" M%1%,%2% A%3%,%3% 0 0,1 %4%,%5%\" style=\"fill:none;fill-opacity: 1;stroke:black;stroke-width:%6%\"/>") 
			% endpoint.x % (endpoint.y*-1) % (durchmesser/-2.0) % startpoint.x % (startpoint.y*-1) % (width*WIDTHKORR));

//		<path class="cur" d="M 50,100 C 50,50 125,50 125,100"/>

	}
	xml += line;
//	cnt += 1;
}




void SVGFile::draw_polygon(int x0, int y0, std::vector<Point> polygon, int color)
{
	y0 *= -1;
	update_minmaxpos(Point(x0,y0));

	string point;
	string tag = "<polygon points=\"";
	
	for (vector<Point>::iterator it = polygon.begin(); it != polygon.end(); it++)
	{
		point = boost::str( boost::format("%1%,%2% ") % (x0+it->x) % (y0+(it->y*-1)) );
		tag += point;
	}

	string stroke = boost::str( boost::format("\" style=\"stroke:black;stroke-width:%1%;fill:none\" />\r\n") % (width*WIDTHKORR));
	tag += stroke;
	xml += tag;
}

void SVGFile::draw_text(int x0, int y0, const std::string& txt, const std::string& font, int fontgroesse, int zoom, int drehwinkel, int color, bool relative, bool mittig)
{
	y0 *= -1;
	update_minmaxpos(Point(x0,y0));
	
	Point pos(x0, y0);	
	drehwinkel = 360 - drehwinkel; // Auf DWF Logik anpassen

	string textanchor = mittig ? "text-anchor=\"middle\"" : "text-anchor=\"start\"";
	std::string line = boost::str( boost::format("<text font-family=\"Arial\" x=\"%1%\" y=\"%2%\" font-size=\"%3%px\" transform=\"rotate(%4% %1% %2%)\" %5% >%6%</text>\r\n") % pos.x % pos.y % (fontgroesse*zoom) % drehwinkel % textanchor % txt);
	xml += line;
	
}


void SVGFile::update_minmaxpos(const Point& p)
{
	if (p.x > max.x)
			max.x = p.x;
	if (p.x < min.x)
			min.x = p.x;
	if (p.y > max.y)
			max.y = p.y;
	if (p.y < min.y)
			min.y = p.y;
	//TRACE("Point p: %i,%i -> min.x = %i, min.y = %i | max.x = %i, max.y = %i\n", p.x, p.y, min.x, min.y, max.x, max.y);
}
