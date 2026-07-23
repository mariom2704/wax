#pragma once

#include "GlFile.h"

class WAXGLDLL SVGFile : public GlFile
{
private:
	///  Die GlObj Klassen brauchen zugriff auf das WT_File Objekt in m_pWtFile
	friend class WaxPolygon;
	friend class TextPlot;
	friend class Kreis;
	friend class Linie;
	friend class Bemassung;

public:
	/// Konstruktor
	SVGFile(std::string fname, bool bFileAutodelete = false);

	/// Destruktor
	virtual ~SVGFile();
	
public:
	/// Neue Ansicht beginnen
	void StartView(const CString& name);

	/// File schliessen
	void SVGClose();

	/// Dateiname abfragen
	CString GetFileName() const;

	CString readBlock();

	void set_max(const Point& p)
	{
		max = p;
	}
	void set_min(const Point& p)
	{
		min = p;
	}

	virtual void draw_line(int x0, int y0, int x1, int y1, int color = 0, int width = 1);
	virtual void draw_circle(double x0, double y0, int durchmesser, int color = 0, int start = 0, int end = 0, bool filled = false);
	virtual void draw_polygon(int x0, int y0, std::vector<Point> polygon, int color = 0);
	virtual void draw_text(int x0, int y0, const std::string& txt, const std::string& font, int fontgroesse, int zoom, int drehwinkel, int color, bool relative, bool mittig);
protected:

	virtual std::string get_file_extension() const
	{
		return "SVG";
	}
private:
	int width;
	std::string xml;

	Point min;
	Point max;

	void update_minmaxpos(const Point& p);
};

