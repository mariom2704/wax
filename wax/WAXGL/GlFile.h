#pragma once

#include <string>
#include <vector>
#include "point.h"

class WAXGLDLL GlFile
{
public:
	/// Konstruktor
	GlFile(std::string fn, bool bFileAutodelete = false);

	/// Destruktor
	virtual ~GlFile();
	
public:
	virtual void draw_line(int x0, int y0, int x1, int y1, int color = 0, int width = 1) = 0;
	virtual void draw_circle(double x0, double y0, int durchmesser, int color = 0, int start = 0, int end = 0, bool filled = false) = 0;
	virtual void draw_polygon(int x0, int y0, std::vector<Point> polygon, int color = 0) = 0;
	virtual void draw_rectangle(int x0, int y0, int x1, int y1, int color = 0);
	virtual void draw_text(int x0, int y0, const std::string& txt, const std::string& font, int fontgroesse, int zoom, int drehwinkel, int color, bool relative, bool mittig) = 0;
		
	
public:
	const Point& get_max_xy() const
	{
		return max_xy;
	}

	const Point& get_min_xy() const
	{
		return min_xy;
	}

	void set_max_xy(const Point& value)
	{
		max_xy = value;
	}

	void set_min_xy(const Point& value)
	{
		min_xy = value;
	}

	std::string get_file_name() const
	{
		return fname;
	}

protected:
	Point max_xy;
	Point min_xy;

	/// Dateiname
	std::string fname;

	bool autodelete;

	virtual std::string get_file_extension() const
	{
		return "";
	}
};
