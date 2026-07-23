#pragma once

#include "dwffile.h"


#define PEN_COUNT 4



class WAXGLDLL DWFReader 
{
public:
	DWFReader(bool create_metafile, bool framed, CString fname);

	virtual ~DWFReader();

	void get_maxrect(CPoint &maxdxy, CPoint &mindxy) const;

	bool process();

private:
	/// Rahmen um die Zeichnung erzeugen, nur EMF
	void create_frame();

	/// Punkte mit den maximalen und minimalen Koordinaten errechnen
	void get_maxrect(int rand, CPoint &maxdxy, CPoint &mindxy) const;

	/// Schalter, ob ein Rahmen gezeichnet werden soll, nur EMF
	bool frame;
	
	CString filename;

	/// Wrapper für den Aufruf der Callbackfunktionen
	void process_file();

	/// Maximale Koordinaten als RECT mit rand Prozent Rand
	RECT get_maxrect(int rand) const;

	void create_meta(CRect &r);

	//Gecasteter Pointer auf ein WT_File Objekt
	DWORD file;

	/// aktuelle Gradzahl der Schrift
	static int grad; 

	static CPoint max;

	static CPoint min;

	/// Wird gesetzt, wenn als Viewer metafile eingetragen ist
	static bool create_emf;

	/// Metafile DC für die Erzeugung eines EMF Files
	static CMetaFileDC *meta;

	static WT_Result my_process_Polyline (WT_Polyline & line, WT_File & file);
	static WT_Result my_process_Ellipse (WT_Outline_Ellipse &ellipse, WT_File & file);
	static WT_Result my_process_Font (WT_Font & text, WT_File & file);
	static WT_Result my_process_Text (WT_Text & text, WT_File & file);

	static void check_minmax(WT_Logical_Point point);

	static int font_height;

	/// Fontgrösse und Linienbreite in Abhängigkeit von der Grafikbreite setzen
	void init_gdiobjects(int dx);
	
	/// Strichstärke dünn, mittel und dick
	CPen pen[PEN_COUNT];

};
