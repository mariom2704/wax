#include "stdafx.h"


#include "resource.h"
#include "dwfreader.h"
#include "math.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// statische Variablen zur Benutzung in den statischen Callbackfunktionen
CPoint DWFReader::max;
CPoint DWFReader::min;
bool DWFReader::create_emf = false;
CMetaFileDC *DWFReader::meta = 0;
int  DWFReader::grad = 0;
int DWFReader::font_height = 60;


DWFReader::DWFReader(bool create_metafile, bool framed, CString fname) 
{
	create_emf = create_metafile;
	frame = framed;
	WT_File *my_input_file =  new WT_File;
	my_input_file->set_filename(fname);
	file = (DWORD)my_input_file;
	filename = fname;
	meta = NULL;
}




DWFReader::~DWFReader()
{
	delete (WT_File*)file;
	if (meta)
	{
		::DeleteEnhMetaFile(meta->CloseEnhanced());
		delete meta;
	}
}

void DWFReader::create_meta(CRect &r)
{
	r.bottom += abs(r.top);
	r.top = 0;
	r.right += abs(r.left);
	r.left = 0;

	meta = new CMetaFileDC;
	CWnd* desktop = CWnd::GetDesktopWindow();
	CDC *dc = desktop->GetDC();
	CString emf_fname(filename + ".emf");
	::DeleteFile(emf_fname);;
	WaxSys::add_tmpfile(emf_fname);
	meta->CreateEnhanced(dc, emf_fname, NULL, "Wax+\0EMF_INTERFACE\0\0" );
	meta->SetBkColor(RGB(255, 255, 255));
	meta->SetBkMode(OPAQUE);//(TRANSPARENT);


/*
	// todo : Grafik drehen
	SetGraphicsMode(meta->m_hDC, GM_ADVANCED);
	XFORM xform;
	xform.eM11 = cos(3.141529/2.0);
	xform.eM12 = sin(3.141529/2.0);
	xform.eM21 = -sin(3.141529/2.0);
	xform.eM22 = cos(3.141529/2.0);
	xform.eDx = 1;
	xform.eDy = 1;
	SetWorldTransform(meta->m_hDC, &xform);
*/
	/*

	LOGPEN lpen = { PS_SOLID, {8,8}, RGB(0,0,0) };
	LOGBRUSH lbrush = { BS_SOLID, RGB(0x0,0x0,0x0), HS_HORIZONTAL };

	static CBrush b;
	if (!b.m_hObject)
		b.CreateBrushIndirect( &lbrush );

	static CPen p;
	if (!p.m_hObject)
		p.CreatePenIndirect( &lpen );

	static CFont f;
	if (!f.m_hObject)
		f.CreateFont( -1100, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET,
			  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			  DEFAULT_PITCH | FF_DONTCARE, "Helvetica" );

	meta->SelectObject(&f);
	meta->SelectObject(&p);
	meta->SelectObject(&b);
	*/
}


WT_Result DWFReader::my_process_Polyline (WT_Polyline & line, WT_File & file)
{
	CPoint pts[100];

	XASSERT(line.count() < 100);
	if (line.count() >= 100)
		return WT_Result::Internal_Error;

	for (int i = 0; i < line.count(); i++)
	{
		check_minmax(line.points()[i]);
		pts[i].x = line.points()[i].m_x ;
		pts[i].y = line.points()[i].m_y * -1  ;
	}
	if (create_emf)
	{
		meta->Polyline(&pts[0], line.count());
	}
	return WT_Result::Success;
}


WT_Result DWFReader::my_process_Text (WT_Text & text, WT_File & file)
{
	WT_Logical_Point pos = text.position();
	
	if (create_emf)	
	{
		CString txt = text.string().ascii();
		CSize dy(0,0);

		//if (text.m_relativized == WD_True)
		//	pos.m_x = -50 - dy.cx;

		dy = meta->GetOutputTextExtent(txt);
		check_minmax(pos);

		// dy.cy/2 ist die halbe Texthöhe, die der Text nach oben geschoben werden muss
		int y = pos.m_y*-1 - dy.cy/2;
		if (grad)
		{
			y = pos.m_y*-1;
			if (y<0) y -= 60;
			if (y>0) y += 30;
		}

		meta->ExtTextOut(pos.m_x , y, NULL, NULL, txt, NULL);
		
		if (!grad)
		{
			pos.m_x += dy.cx;
			pos.m_y += dy.cy/2;
			check_minmax(pos);
			pos.m_y -= dy.cy;
		}
		else
		{
			pos.m_x += (int)(dy.cx + 0.5*dy.cx); // 50 Prozent als Rand
			pos.m_y += 2*dy.cy; 
		}			
	}
	check_minmax(pos);
	return WT_Result::Success;
}

WT_Result DWFReader::my_process_Font (WT_Font & text, WT_File & file)
{
	if (create_emf)
	{
		grad = text.rotation().rotation() *3600 / 65500;
		static CFont font;
		font.DeleteObject();
		font.CreateFont(font_height, font_height/3, grad, grad, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		meta->SelectObject(&font);	
	}
	return WT_Result::Success;
}

WT_Result DWFReader::my_process_Ellipse (WT_Outline_Ellipse &ellipse, WT_File & file)
{
	if (create_emf)
	{
		float startdegree = (float)(ellipse.start_degree()*360.0);
		float enddegree = (float)(ellipse.end_degree()*360.0 - startdegree);
		double startdegree_rad = (180.0-startdegree) * 3.1415298 / 180;
		double startx = ellipse.position().m_x - cos(startdegree_rad) * ellipse.major();
		double starty = ellipse.position().m_y*-1 - sin(startdegree_rad) * ellipse.major();
		meta->MoveTo((int)startx, (int)starty);
		meta->AngleArc(ellipse.position().m_x , ellipse.position().m_y*-1 , ellipse.major(), startdegree , enddegree);
	}
	check_minmax(ellipse.position());
	return WT_Result::Success;
}

void DWFReader::check_minmax(WT_Logical_Point point)
{
	if (point.m_x > max.x)
		max.x = point.m_x;
	if (point.m_x < min.x)
		min.x = point.m_x;
	if (point.m_y > max.y)
		max.y = point.m_y;
	if (point.m_y < min.y)
		min.y = point.m_y;
}

void DWFReader::init_gdiobjects(int dx)
{
	/**
	Fontgrösse in Abhängigkeit der Breite berechnen. Der Teiler wurde durch Ausprobieren
	ermittelt. 
	*/

	const int FONTGROESSE_TEILER = 30;
	font_height = dx / FONTGROESSE_TEILER;

	LOGPEN lpen[PEN_COUNT] = {
		{ PS_SOLID, {2,2}, RGB(0,0,0) },
		{ PS_SOLID, {4,4}, RGB(0,0,0) },
		{ PS_SOLID, {6,6}, RGB(0,0,0) },
		{ PS_SOLID, {8,8}, RGB(0,0,0) },
	};
	
	for (int i = 0; i < PEN_COUNT; i++)
	{
		if (!pen[i].m_hObject)
			pen[i].CreatePenIndirect( &lpen[i] );
	}
	

	if (dx < 1000)
		meta->SelectObject(&pen[0]);
	else if (dx <= 2000)
		meta->SelectObject(&pen[1]);
	else if (dx <= 6000)
		meta->SelectObject(&pen[2]);
	else meta->SelectObject(&pen[3]);
}

RECT DWFReader::get_maxrect(int rand) const
{
	/**
	Erzeugt aus den maximalen und minimalen Koordinaten eine
	RECT Struktur. Über Rand kann optional ein Prozentsatz für 
	einen Rand übergeben werden. Der Rand wird an jeder Seite addiert.
	*/
	RECT r;
	int dx = (max.x - min.x) / 100 * rand;
	int dy = (max.y - min.y) / 100 * rand;
	r.left = min.x - dx;
	r.top = max.y*-1 - dy;

	r.right = max.x  + dx;
	r.bottom = min.y*-1  + dy;
	return r;
}

void DWFReader::get_maxrect(int rand, CPoint &maxdxy, CPoint &mindxy) const
{
	RECT r = get_maxrect(rand);
	maxdxy.x = r.right;
	maxdxy.y = r.bottom;
	mindxy.x = r.left;
	mindxy.y = r.top;
}

void DWFReader::get_maxrect(CPoint &maxdxy, CPoint &mindxy) const
{
	get_maxrect(0, maxdxy, mindxy);
}

void DWFReader::process_file()
{
	/**
	Öffnet das DWF File und sorgt für einen Aufruf der Callbackfunktionen.
	*/
	WT_File *my_input_file = (WT_File*)file;
	my_input_file->set_file_mode(WT_File::File_Read);
	my_input_file->open();

	WT_Result result;
	do 
	{
		result = my_input_file->process_next_object();
	}  while (result == WT_Result::Success);

	my_input_file->close();
}

void DWFReader::create_frame()
{
	XASSERT(meta);
	if (!meta) 
		return;
	int dx = 4;

	RECT r = get_maxrect(0);
	POINT p[5];
	p[0].x = r.left-dx;
	p[0].y = r.top-dx;
	p[1].x = r.right+dx;
	p[1].y = r.top-dx;
	p[2].x = r.right+dx;
	p[2].y = r.bottom+dx;
	p[3].x = r.left-dx;
	p[3].y = r.bottom+dx;
	p[4].x = r.left-dx;
	p[4].y = r.top-dx;

	InflateRect(&r, 10, 10);
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,255));
	meta->FrameRect(&r, &br);
	meta->Polyline(p, 5);
}

bool DWFReader::process()
{
	/**
	Erstellt aus der DWF Datei die EMF Files. Falls keine EMF Dateien gewünscht sind (Systemparameter)
	werden nur die Max. Koordinaten erzeugt.
	*/
	bool tmp_create_emf = create_emf;
	WT_File *my_input_file = (WT_File*)file;

	max.x = -999999;
	max.y = -999999;
	min.x =  999999;
	min.y =  999999;

	my_input_file->set_outline_ellipse_action(my_process_Ellipse);
	my_input_file->set_polyline_action(my_process_Polyline);
	my_input_file->set_text_action(my_process_Text);
	my_input_file->set_font_action(my_process_Font);

	// Fürs erste Lesen emf ausschalten, damit die maximalen Koordinaten erzeugt werden können
	create_emf = false;
	process_file();
	create_emf = tmp_create_emf;

	if (create_emf)
	{
		RECT r = get_maxrect(2);
		create_meta((CRect&)r);

		int diffx = max.x - min.x;
		int diffy = max.y - min.y;

		init_gdiobjects((diffx>diffy)?diffx:diffy);
		process_file();
		if (frame)
			create_frame();
	}
	return true;
}

