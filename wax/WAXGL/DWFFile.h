#pragma once

#include "GlFile.h"
#define WHIP_STATIC_LIB
#include <dwf\whip_toolkit.h>

/**
Fileklasse zum Abspeichern von DWF Dateien
Basisklasse ist eigentlich WT_File. Dann müsste
man aber das Whiptoolkit überall einbinden.
*/
class DWFFile : public GlFile
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
	DWFFile(std::string fname, bool bFileAutodelete = false);

	/// Destruktor
	virtual ~DWFFile();
	
	/// Neue Ansicht beginnen
	void StartView(const CString& name);

	/// File schliessen
	void DWFClose();


	virtual void draw_line(int x0, int y0, int x1, int y1, int color = 0, int width = 1);
	virtual void draw_circle(double x0, double y0, int durchmesser, int color = 0, int start = 0, int end = 0, bool filled = false);
	virtual void draw_polygon(int x0, int y0, std::vector<Point> polygon, int color = 0);
	virtual void draw_text(int x0, int y0, const std::string& txt, const std::string& font, int fontgroesse, int zoom, int drehwinkel, int color, bool relative, bool mittig);

protected:
	int m_iLayer;

	virtual std::string get_file_extension() const
	{
		return "DWF";
	}
private:
	WT_Logical_Point get_WT_Logical_Point(double x, double y);
public:
	/// Zeiger auf das WT_File Objekt
	DWORD m_pWtFile;
};

