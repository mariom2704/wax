#include "stdafx.h"



//#include "..\waxgl\dwfreader.h"

#ifdef __WAX_FUER_DRITTE__		
#else
	#include "GpBarcode.h"		
#endif


#include "WaxHtmlFile.h"

//#include <tools/tracer.h>

#include <fstream>
using namespace std;



#ifdef _DEBUG
#undef THIS_FILE
#endif


//Konstruktor
WaxHtmlFile::WaxHtmlFile(const CString* file, CString title)
{
	viewer = WaxSys::para_str(VIEWERDRUCK);
	viewer.MakeLower();
	openFile(file ? *file : WaxSys::GetTempFilename("html").c_str(), title);
	font = "Arial";
	nonull = false;
}


void WaxHtmlFile::openFile(const CString& file, const CString& title)
{
	closeFile(); 
	/*
	Öffnet eine temporäre Datei und schreibt Basis Html Tags
	*/
	CFileException pError;
	if (!Open(file, modeReadWrite | modeCreate | modeNoTruncate, &pError))
	{
		pError.ReportError();
		AfxThrowUserException();
	}
	SeekToEnd( ); // Ausgaben werden immer drangehängt
	if (title.GetLength())
		set_title(title);	// Wird vielleicht später erst gesetzt
}


//Destruktor
WaxHtmlFile::~WaxHtmlFile()
{
	/*
	Schliesst die Html Datei
	*/
	closeFile();
}

//Dateinamen aus CStdioFile zurückgeben
const CString WaxHtmlFile::getFileName()
{
	/*
	Gibt den Dateiname zurück
	*/
	return CFile::GetFilePath( );
}

//Zeile einfügen
void WaxHtmlFile::appendString(const CString &line)
{
	/*
	Zeile anfügen
	*/
	CString align = "LEFT";
	CString txt;
	txt.Format("<B><div align=%s><FONT FACE=\"%s\" SIZE=2>%s<BR></B></FONT>", align, font, line);
	WriteString(txt);
}

//Titel zweiter Ebene einfügen
void WaxHtmlFile::setSubTitle(const CString &title, const int size)
{
	/*
	Subtitel einfügen
	Standardmässig wird als Grösse 3 übergeben
	*/
	setTitle(title, size);
}

//Titel erster Ebene einfügen
void WaxHtmlFile::setTitle(const CString &title, const int size)
{
	/*
	Überschrift setzen
	Standardgrösse ist 4
	*/
	CString tit;
	tit.Format("<B><FONT FACE=\"%s\" SIZE=%i><P>%s</P></B></FONT>", font, size, title);
	WriteString(tit);
}

void WaxHtmlFile::set_font(const CString &value)
{
	font = value;
}

//Link auf eine DWF Datei einfügen
void WaxHtmlFile::insertDWFLink(const string& fname)
{	
	int rahmen = WaxSys::para_int(RAHMEN);

	int height = 0, iwidth = 0;
	if (viewer == "svg")
	{
		get_size_svg(iwidth, height, fname);
		if( iwidth < height - 100)
		{
			iwidth = 660;
			height = 880;
		}
		else
		{
			iwidth = 660;
			height = 400;
		}
	}

	string link;

	// Bei Link und Voloview muss URL Name eingefügt werden
	string url;
	url = "file://" + fname;
	replace_all(url, "\\", "/");
	

	if (viewer == "adview")
	{
		link = createDwfLink(rahmen, height, fname);
	}
	else if (viewer == "link")
	{
		//<img src="filename.png" Vorschauimage wenn verfügbar
		link = str(format("<a href=\"%s\"><img src=\"%s\" width=100%% height=%i border=%i "
		"alt=\"click to view full screen\"></a>") % url % url % height % rahmen ); 
	}
	else if (viewer == "metafile")
	{
		//<img src="filename.png" Vorschauimage wenn verfügbar
		link = str(format("<a href=\"%s.emf\"><img src=\"%s.emf\" width=%i height=%i border=%i "
		"alt=\"Grafikvollansicht\"></a>") % url % url % iwidth % height % 0); 
	}
	else if (viewer == "svg")
	{
		link = str(format("<embed\r\n width=%i \r\n height=%i\r\n "
		"src=\"file://%s\" </embed>\r\n ") % iwidth % height % fname);
	}
	else // default Viewer
	{
		link = str(format("<embed\r\n width=100% \r\n height=%i\r\n "
		"src=\"file://%s\" </embed>\r\n ") % height % fname);
	}
	WriteString(link);
}

string WaxHtmlFile::createBarcode(int breite, int hoehe, string txt, string align, CString* strfn)
{
	string st;

	#ifdef __WAX_FUER_DRITTE__
		UNREFERENCED_PARAMETER(breite);
		UNREFERENCED_PARAMETER(hoehe);
		UNREFERENCED_PARAMETER(txt);
		UNREFERENCED_PARAMETER(align);
	#else
		string fn = WaxSys::GetTempFilename("jpg");
		
		if(strfn)
		{
			*strfn = fn.c_str();
		}
		WaxSys::add_tmpfile(fn.c_str());

		//Die Auflösung der Jpg Grafik muss größer als die Größe in der HTML Datei sein!
		GpBarcode barcode;
		barcode.getGrafik(txt, false, breite * 2, hoehe * 2, GpBarcode::winkel0, fn); 

		st = MxString("{0}{1}{2}{3}{4}{5}{6}{7}{8}").arg("<p align='").arg(align.c_str()).arg("'><img src='")
	 												.arg(fn.c_str())
													.arg("' width=\"").arg(breite)
													.arg("\" height=\"").arg(hoehe)
													.arg("\">").getString();
		#endif
	
	return st;
}

void WaxHtmlFile::insertDWF(const std::string& fname)
{
	int rahmen = WaxSys::para_int(RAHMEN);

	int height = 880;

	string link = createDwfLink(rahmen, height, fname);
	WriteString(link);
}


string WaxHtmlFile::createDwfLink(int rahmen, int height, const std::string& fname)
{
	// Bei Link und Voloview muss URL Name eingefügt werden
	string url;
	url = "file://" + fname;
	replace_all(url, "\\", "/");
	
	string link = str( format("<object id = \"AdView\"\r\n " 
	"classid = \"clsid:A662DA7E-CCB7-4743-B71A-D817F6D575DF\" \r\n "
	"border = \"%i\"\r\n "
    "width = 660\r\n "
    "height = \"%i\">\r\n "
	"<PARAM NAME=\"Src\" VALUE=\"%s\">"
	"<param name=\"PaperVisible\" value=\"false\">"
	"<param name=\"BackColor\" value=\"16777215\">"
	"<param name=\"ToolbarVisible\" value=\"false\">"
    "</object>\r\n\r\n") % rahmen % height % url );

	return link;
}

void WaxHtmlFile::insertJpg(const std::string& fname)
{
	int width, height;
	//get_size_jpg(fname, width, height); //todo
	width = 640;
	height = 480;


	WriteString("<table width='100%' border='0' cellpadding='2' cellspacing='0'>");
	WriteString("<tr><TD WIDTH='100%' VALIGN='MIDDLE' COLSPAN=1>\n");

	string link;
	link = str(format("<P ALIGN='CENTER'><IMG SRC='%s' WIDTH=%i HEIGHT=%i ></TD>\n</tr>") % fname % width % height); 
	WriteString(link);
}


//Tabelle beginnen, defaultwert führt zur Skalierung auf 100%
void WaxHtmlFile::startTable(int breite, int border)
{
	/*
	Tabelle starten
	Warum hier eine Breite angegeben werden muss und in <mf .AppendCol> auch.
	*/
	CString tblbreite;
	if (breite > 0)
		tblbreite.Format("%i", breite);
	else
		tblbreite = "'100%'";
	CString tblstart;
	tblstart.Format("<table border=%d width=%s cellspacing=-1 cellpadding=3>", border, tblbreite);
	WriteString(tblstart);
}

//Tabelle beenden
void WaxHtmlFile::endTable(bool zeilenwechsel)
{
	/*
	Tabelle beenden
	*/
	if (zeilenwechsel)
		WriteString("</table> </P> \r\n");
	else
		WriteString("</table>");

	bool printword = (WaxSys::para_int(PRINT_WORD) == 0) ? false : true;
	if (printword)
		WriteString("<br>");
}

//Spalte beginnen
void WaxHtmlFile::startCol(int width, const CString &align)
{
	CString out, w;
	
	if (width > 0)
		w.Format("%d", width);
	else
		w = "'100%'";
	
	out.Format("<td align=%s width=%s><font face=\"%s\"SIZE=2>", align, w, font);
	WriteString(out);
}

//Spalte beenden
void WaxHtmlFile::endCol()
{
	CString out = "</TD>";
	WriteString(out);
}

void WaxHtmlFile::appendCol(const char* txt, int width, const CString &align, bool prozent)
{
	appendCol((string)txt, width, align, prozent);
}


void WaxHtmlFile::appendCol(const CString& txt, int width, const CString &align, bool prozent)
{
	appendCol((string)(LPCSTR)txt, width, align, prozent);
}

//Spalte einfügen
void WaxHtmlFile::appendCol(const string& txt, int width, const CString &align, bool prozent)
{
	/*
	Spaltentext einfügen
	Um eine neue Zeile zu beginnen muss vorher <mf .StartRow> aufgerufen werden
	*/
	CString out;
	if (prozent)
		out.Format("<td align=%s width='%i%%'><font face=\"%s\"SIZE=2>%s</TD> \r\n", align, width, font, txt.length() ? txt.c_str() : "-");
	else
		out.Format("<td align=%s width=%i><font face=\"%s\"SIZE=2>%s</TD> \r\n", align, width, font, txt.length() ? txt.c_str() : "-");
	WriteString(out);
}

//Spalte einfügen, Zahl in String umwandeln
void WaxHtmlFile::appendCol(const int i, int len, const CString &align, bool prozent)
{
	/*
	Spaltentext einfügen, hier eine Zahl
	Um eine neue Zeile zu beginnen muss vorher <mf .startRow> aufgerufen werden
	*/
	CString asc("-");
	if (!nonull || i)
		asc.Format("%i", i);
	appendCol(asc, len, align, prozent);
}


//Spalte einfügen, Zeit als String ausgeben (Systemeinstellung)
void WaxHtmlFile::appendCol(const MXDate tm, int len, const CString &align, bool prozent)
{
	/*
	Spaltentext einfügen, hier ein Datum
	Um eine neue Zeile zu beginnen muss vorher <mf .startRow> aufgerufen werden
	*/
	CString asc(tm.Format(VAR_DATEVALUEONLY));
	appendCol(asc, len, align, prozent);
}


//Spalte einfügen, Double in String umwandeln
void WaxHtmlFile::appendCol(const double i, int len, const CString& align, bool prozent)
{
	CString asc("-");
	if (!nonull || i < -0.0001 || i > 0.0001)
		asc.Format("%.2f", i);
	appendCol(asc, len, align, prozent);
}

//Zeile beginnen
void WaxHtmlFile::startRow()
{
	/*
	Zeile neu beginnen
	Die Spalten werden durch <mf .AppendCol> gesetzt. Es muss dabei aufgepasst werden,
	dass die Summe der Spaltenbreiten beim Aufruf von <mf .StartTable> richtig angegeben werden
	*/

	//WriteString("<tr style=\"page-break-after:avoid\"> \r\n");
	WriteString("<tr>");
}

//Zeile beenden
void WaxHtmlFile::endRow(bool zeilenwechsel)
{
	/*
	Spalte beenden
	*/
	if (zeilenwechsel)
		WriteString("</tr> \r\n");
	else
		WriteString("</tr>");
}

//Html Dateien zusammenfügen
WaxHtmlFile& WaxHtmlFile::operator +=(WaxHtmlFile &other)
{
	/*
	Dateien zusammenkopieren
	Html Dateien lassen sich einfach aus mehreren Dateien zusammenstellen, indem man sie 
	zusammenkopiert.
	Beide Filepointer stehen hinterher auf dem Dateiende. 
	*/
	CString content;
	other.SeekToBegin();
	SeekToEnd();
	while (other.ReadString(content))
		WriteString(content);

	return *this;
}

//Datei schliessen, dabei wird vorher die Html Datei durch /html abgeschlossen
void WaxHtmlFile::closeFile()
{
	/*
	Datei schliessen
	vor dem Schliessen wird der Html Endetag geschrieben
	*/
	if ((UINT)m_hFile != (UINT)hFileNull)
	{
		WriteString("</BODY></HTML>");
		Close();
	}
}



void WaxHtmlFile::get_size_svg(int& dx, int& dy, const string& fname) const
{
	/**
	Liest die SVG Datei aus und liefert die errechnete Höhe des Links aus.
	Breite ist default 600.
	*/

	// Defaulthöhe, maximale Höhe
	const int default_val = 150;
	const int max_val = 800;

	dx = 650;
	dy = default_val;
	
	std::ifstream f(fname.c_str(), ios_base::binary);
	
	if (!f.is_open())
		return;

	string dim, str;
	for (; !f.eof(); f >> str)
	{
		// Bis zum Tag viewBox lesen
		if (to_lower_copy(str) != "viewbox")
			continue;
					
		char next = ' ';
		while (next != '"')
			f.read(&next, 1);
		
		// Die vier Koordinaten einlesen
		for (f.read(&next, 1); next != '"'; f.read(&next, 1))
			dim += next;
		
		if (dim.length())
			break;
	}
	if (!dim.length())
		return;

	// Koordinaten aus string lesen
	vector<string> res;
	split(res, dim, is_any_of(" "));

	if (res.size() != 4)
		return;

	double diff_x = abs((atoi(res.at(0).c_str()) - atoi(res.at(2).c_str())));
	double diff_y = abs((atoi(res.at(1).c_str()) - atoi(res.at(3).c_str())));

	// Relative Höhe bezogen auf Breite dx
	dy = (int)(diff_y / diff_x * dx);
		
	if (dy < default_val)
		dy = default_val;
	else if (dy > max_val)
		dy = max_val;

	//MxUtil::Tracer::trace( boost::str(format("SVG Grafik Dimension in Html Datei: %i - %i\n") % dx % dy) );
}

//Seitenumbruch einfuegen
void WaxHtmlFile::insertSeitenumbruch()
{
	CString txt = "<h1 style=";
	txt +="\"page-break-before:always\"";
	//txt +=">Und die zweite Seite<"
	txt += "/h2>";

	WriteString(txt);
}

//Trennlinie einfuegen
void WaxHtmlFile::insertTrennlinie()
{
	CString txt = "<hr width=\"100%%\" size=\"2\" align=\"center\">";
	WriteString(txt);
}

void WaxHtmlFile::insertHtmlLink(CString filename, CString linkname, CString ziel, CString listname)
{
	// Beispiel: "<a href=\"http://www.minimax.de/\" target="_blank">MINIMAX GbmH</a> Homepage<br>"

	CString target;
	if (!ziel.IsEmpty())
		target.Format("target=\"%s\"",ziel);

	CString txt;	
	txt.Format("<p> <a href=\"%s\"%s>%s</a>%s<br></p>", filename, target, linkname, listname);

	WriteString(txt);
}

void WaxHtmlFile::set_title(const CString& title)
{
	CString trennung = "<style type=\"text/css\"> table,p,blockqoute { orphans:20; }</style>";

	CString head;
	head.Format("<script type=\"text/javascript\" src=\"HTML/LoaderForImagesDwfSvg.js\">\n\r</script><HTML><HEAD><META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=windows-1252\">"
	"<META NAME=\"Druckausgabe\" CONTENT=\"Wax+ 2000\">"
	"<TITLE>%s</TITLE>%s</HEAD><BODY>", title, trennung);
	//"<P><IMG SRC=\"Image2.gif\" WIDTH=1017 HEIGHT=326>Dflkasdj</P>"
	WriteString(head);
}

/// Tag für Fettschrift hinzufügen
string WaxHtmlFile::bold_string(const string& txt) const
{
	return "<b>" + txt + "</b>";
}

/// Tag für Fettschrift hinzufügen, String aus Resourcen laden
string WaxHtmlFile::bold_string_ids(const std::string& txt) const
{
	return bold_string(getStammdaten().getText(txt));
}
