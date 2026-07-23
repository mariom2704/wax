#include "stdafx.h"


#include "dwffile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Konstruktor
DWFFile::DWFFile(std::string fn, bool bFileAutodelete) : GlFile(fn, bFileAutodelete)
{
	/*
	Konstruktor
	Hier wird eine temporäre Datei (DWF*.tmp) erzeugt, die die Grafikdatei darstellt. Es wird eine "Named view"
	erstellt ("Standard"). 
	Der Hintergrund wird auf Weiss gesetzt.
	*/
	
	WT_File *dwf_file = new WT_File;

    dwf_file->set_filename (fname.c_str());
    dwf_file->set_file_mode (WT_File::File_Write);
	dwf_file->heuristics().set_allow_binary_data(false);// Ascii Dateien erzwingen
	dwf_file->heuristics().set_allow_data_compression(false);
	dwf_file->heuristics().set_allow_drawable_merging(false);
	dwf_file->open();
	WT_Logical_Point min, max;
	
	dwf_file->desired_rendition().color() = WT_Color (1, dwf_file->desired_rendition().color_map());
	dwf_file->desired_rendition().line_weight () = 1;
	dwf_file->desired_rendition().fill() = WT_Fill(WD_False);

	// Hintergrundfarbe setzen. Funktioniert zwar, im IE sieht aber weiss wie hellgrau aus
	// Dann lieber gar nicht setzen...
    //WT_Background background (WT_Color (GlObj::WHITE, dwf_file->desired_rendition().color_map()));
    //background.serialize (*dwf_file);

	m_pWtFile = (DWORD)dwf_file;
	m_iLayer = 0;
}

//Destruktor
DWFFile::~DWFFile()
{
	const int rand = (max_xy.x - min_xy.x) / 30;
	WT_Logical_Point    min, max;
	min.m_x = min_xy.x - rand;
    min.m_y = min_xy.y;
    max.m_x = max_xy.x + rand;
    max.m_y = max_xy.y;
    WT_Logical_Box        box(min, max);
    WT_Named_View        named_view (box, "GESAMTZEICHNUNG");
	named_view.serialize(*(WT_File*)m_pWtFile);

	WT_View view(box);
	view.serialize(*(WT_File*)m_pWtFile);
	/*
	Destruktor
	Schliesst die WT_File DWF-Datei und löscht das Objekt. Das Löschen der erzeugten Datei 
	muss dann passieren, wenn sie nicht mehr gebraucht wird. Durch das Flag m_bFileAutodelete kann
	das automatische Löschen der Datei erzwungen werden. Im Moment werden alle temporären Dateien gesammelt
	und bei Programmende gelöscht.
	*/
	DWFClose();
}



///File schliessen
void DWFFile::DWFClose()
{
	WT_File *f = (WT_File*)m_pWtFile;
	f->close();

	delete f;
}

//Neue Ansicht beginnen
void DWFFile::StartView(const CString &name)
{
	/*
	Beginn einer neuen Ansicht
	Es wird ein neues DWF File angelegt. Dabei wird eine named_view erzeugt und das Koordinaten
	system auf x/y -> -10000 bis +10000 gesetzt. Dieses Koordinatensystem ist beim Erzeugen von 
	Grafikobjekten zu beachten. Ungeachtet dessen, wird die Grafik sowieso zentriert ausgegeben.
	*/
	WT_File *dwf_file = (WT_File *)m_pWtFile;
	WT_Layer  lay(*dwf_file, m_iLayer++,  name);
	lay.serialize(*dwf_file);
	
	WT_Logical_Point    min, max;

    min.m_x = -10000;
    min.m_y = -10000;
    max.m_x = 10000;
    max.m_y = 10000;

	WT_Logical_Box        box(min, max);
    WT_Named_View        named_view (box, name);

    named_view.serialize(*dwf_file);
}

WT_Logical_Point DWFFile::get_WT_Logical_Point(double x, double y)
{
	WT_Logical_Point lp;
	lp.m_x = (int)x;
	lp.m_y = (int)y; 
	return lp;
}

void DWFFile::draw_line(int x0, int y0, int x1, int y1, int color, int width)
{
	WT_File *f = (WT_File*)m_pWtFile;
	f->desired_rendition().line_weight () = 1;
	f->desired_rendition().fill() = WT_Fill(FALSE);

	WT_Logical_Point vPoints[2];

	vPoints[0] = get_WT_Logical_Point(x0, y0);
	vPoints[1] = get_WT_Logical_Point(x1, y1);
	
	f->desired_rendition().line_weight() = width;
    f->desired_rendition().color() = WT_Color (color, f->desired_rendition().color_map());

	WT_Polyline pg(2, &vPoints[0], WD_True);
	f->desired_rendition().color() = WT_Color (color, f->desired_rendition().color_map());
	pg.serialize(*f);
}


void DWFFile::draw_circle(double x0, double y0, int durchmesser, int color, int start, int end, bool filled )
{
	WT_File *f = (WT_File*)m_pWtFile;
	f->desired_rendition().line_weight () = 1;
	f->desired_rendition().fill() = WT_Fill(FALSE);
	
	WT_Logical_Point pt;
	pt = get_WT_Logical_Point(x0, y0);
	WT_Outline_Ellipse elli(pt, durchmesser/2, durchmesser/2, (WT_Unsigned_Integer16)start, (WT_Unsigned_Integer16)end);

	WT_Fill fill(FALSE);
	f->desired_rendition().color() = WT_Color(color, f->desired_rendition().color_map());
	
	if (filled && start == 0 && end == 65536)
	{
		CPoint umkreis;

		int radius = durchmesser /2;
		while (--radius > 1)
		{
			WT_Logical_Point pt;
			pt = get_WT_Logical_Point(x0, y0);
			WT_Outline_Ellipse elli(pt, radius, radius, (WT_Unsigned_Integer16)start, (WT_Unsigned_Integer16)end);

			WT_Fill fill(FALSE);
			f->desired_rendition().color() = WT_Color(color, f->desired_rendition().color_map());
			f->desired_rendition().line_weight() = 10;
		
			fill.serialize(*f);
			elli.serialize(*f);
		}
	}
	elli.serialize(*f);
	
}

void DWFFile::draw_polygon(int x0, int y0, std::vector<Point> polygon, int color)
{
	WT_File *f = (WT_File*)m_pWtFile;
	f->desired_rendition().line_weight () = 1;
	f->desired_rendition().fill() = WT_Fill(FALSE);
	
	Point lastpt = polygon.back();

	if (lastpt.x != 0 || lastpt.y != 0)
		polygon.push_back(Point(0,0));

	const int ipts = polygon.size();
	const int len =  ipts * sizeof(WT_Logical_Point);

	WT_Logical_Point *vPoints = (WT_Logical_Point *)new char[len];


    //WT_Logical_Point3D *vPoints = (WT_Logical_Point *)new char[len];
	WT_Logical_Point transpt;
	transpt = get_WT_Logical_Point(x0, y0);
	
	int drehwinkel = 0;
	WT_Transform trans(transpt, 1, 1, drehwinkel);

	int i = 0;
	for (vector<Point>::iterator it = polygon.begin(); it != polygon.end(); it++, i++)
	{
		vPoints[i].m_x = it->x;
		vPoints[i].m_y = it->y;
	}

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
}


void DWFFile::draw_text(int x0, int y0, const std::string& txt, const std::string& font, int fontgroesse, int zoom, int drehwinkel, int color, bool relative, bool mittig)
{
	WT_File *f = (WT_File*)m_pWtFile;
	f->desired_rendition().line_weight () = 1;
	f->desired_rendition().fill() = WT_Fill(FALSE);
	const double eingrad = 16380.0 / 90.0;
	int dreh = (int)(eingrad * drehwinkel);

	WT_Logical_Point transpt;
	transpt = get_WT_Logical_Point(x0, y0);
	
	// Text mittig ausgeben, erste Näherung
	if (mittig)
	{
		double offset = (double)fontgroesse*1.5;
		int offset2 = txt.length();

		if (drehwinkel != 90)
			transpt.m_x -= (int)(offset*offset2);
		else
			transpt.m_y -= (int)(offset*offset2);
	}
	
	WT_Text text(transpt, txt.c_str());
	f->desired_rendition().color() = WT_Color (color, f->desired_rendition().color_map());
	
	if (relative)
		text.relativize(*f);

	WT_Font fnt(font.c_str(), false, false, false, 0, 0, 0, fontgroesse * zoom, (WT_Unsigned_Integer16)dreh, 700, 1000, 0, 1);
	fnt.serialize(*f);
/*
	// Transform
	WT_Logical_Point pt;
	pt.m_x = 0;
	pt.m_y = 0;
	WT_Transform trans(pt, 1,1,0);
	//WT_Transform trans(transpt, 1,1,0);
	text.transform(trans);
*/
	text.serialize(*f);
}