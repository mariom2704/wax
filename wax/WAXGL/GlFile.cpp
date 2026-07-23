#include "stdafx.h"


#include "GlFile.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



GlFile::GlFile(std::string fn, bool bFileAutodelete)
{
	autodelete = bFileAutodelete;
	if (!fn.length()) // Wenn kein Dateiname übergeben wurde, neuen erzeugen
	{
		fn = WaxSys::GetTempFilename(get_file_extension());
		boost::filesystem::remove(fn);	// Wenn die Datei schon existiert, löschen

		if (boost::filesystem::exists(fn))
		{
			fn = "fallback." + get_file_extension(); // Wenn das Löschen nicht geklappt hat, Standardnamen benutzen
			XASSERT(!boost::filesystem::exists(fn));
		}
	}
	fname = fn;
}


GlFile::~GlFile()
{
	if (autodelete)
		boost::filesystem::remove(fname);
}

void GlFile::draw_rectangle(int x0, int y0, int dx, int dy, int color)
{
	vector<Point> pts;
	pts.push_back(Point(0,0));
	pts.push_back(Point(0,dy));
	pts.push_back(Point(dx, dy));
	pts.push_back(Point(dx, 0));
	pts.push_back(Point(0,0));
	
	
	draw_polygon(x0, y0, pts, color);
}