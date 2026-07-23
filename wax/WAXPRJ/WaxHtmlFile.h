#pragma once

#include "../rohrkonst/mxdate.h"


/// Diese Klasse dient als Frontend zur Erzeugung von Druckausgaben. Die erzeugte 
/// Html Datei
/// wird über ein Objekt der Klasse WaxHtmlView angezeigt und ausgedruckt. 
class WAXPRJDLL WaxHtmlFile : private CStdioFile  
{
public:
	/// Konstruktor
	WaxHtmlFile(const CString *file = NULL, CString title = "");

	/// Destruktor
	virtual ~WaxHtmlFile();

	/// Schaltet die Ausgabe bei appendCol so, dass Nullen als Leerzeichen ausgegeben werden
	inline void null2whitespace(bool val = true);

	void set_title(const CString& title);

	void set_font(const CString& value);

	void openFile(const CString& file, const CString& title);

	void insertHtmlLink(CString filename, CString linkname, CString ziel, CString listname);

	/// Seitenumbruch einfuegen
	void insertSeitenumbruch();

	/// Trennlinie einfuegen
	void insertTrennlinie();
	
	/// Datei schliessen, dabei wird vorher die Html Datei durch /html abgeschlossen
	void closeFile();

	/// Html Dateien zusammenfügen
	WaxHtmlFile& operator += (WaxHtmlFile &other);

	/// Tabelle beginnen, defaultwert führt zur Skalierung auf 100%
	void startTable(int breite = 0, int border = 1);

	/// Tabelle beenden
	void endTable(bool zeilenwechsel = true);

	/// Zeile beginnen
	void startRow();
	
	/// Zeile beenden
	void endRow(bool zeilenwechsel = true);

	/// Spalte beginnen
	void startCol(int width, const CString &align = "CENTER");

	/// Spalte beenden
	void endCol();

	void appendCol(const char* txt, int width, const CString &align = "LEFT", bool prozent = false);

	/// Spalte einfügen
	void appendCol(const CString& txt, int width, const CString &align = "LEFT", bool prozent = false);
	
	/// Spalte einfügen
	void appendCol(const std::string& txt, int width, const CString &align = "LEFT", bool prozent = false);
	
	/// Spalte einfügen, Zahl in String umwandeln
	void appendCol(const int i, int len, const CString &align = "RIGHT", bool prozent = false);

	/// Spalte einfügen, Zeit als String ausgeben (Systemeinstellung)
	void appendCol(const MXDate tm, int len, const CString &align = "CENTER", bool prozent = false);

	/// Spalte einfügen, Double in String umwandeln
	void appendCol(const double i, int len, const CString& align = "RIGHT", bool prozent = false);

	/// Link auf eine DWF Datei einfügen
	void insertDWFLink(const std::string& fname);

	/// Titel zweiter Ebene einfügen
	void setSubTitle(const CString& title, const int size = 3);

	/// Titel erster Ebene einfügen
	void setTitle(const CString& title, const int size = 4);

	/// Zeile einfügen
	void appendString(const CString& line);

	/// Tag für Fettschrift hinzufügen
	std::string bold_string(const std::string& txt) const;

	/// String aus Resourcen laden und in Fettschrift formatieren
	std::string bold_string_ids(const std::string& txt) const;
	
	/// Dateinamen aus CStdioFile zurückgeben
	const CString getFileName();

	void WriteString(const char* str) { CStdioFile::WriteString(str); }

	void WriteString(const CString& str) { CStdioFile::WriteString(str); }

	void WriteString(const std::string& str) { CStdioFile::WriteString(str.c_str()); }

	CString GetFilePath() const { return CStdioFile::GetFilePath(); }

	void insertJpg(const std::string& fname);

	void insertDWF(const std::string& fname);

	void get_size_svg(int& dx, int& dy, const std::string& fname) const;

	string createBarcode(int breite, int hoehe, string txt, string align, CString* fn = NULL);

private:
	
	/// wenn true, dann werden bei appendCol keine Nullen sondern Leerzeichen ausgegeben
	bool nonull;

	string createDwfLink(int rahmen, int height, const std::string& fname);


	/// Viewer können sein: Zing, Voloview oder Default. Default führt zu einem 
	/// generischen Eintrag in der HTML Datei, wodurch der jeweils im System 
	/// eingetragene DWF Viewer benutzt wird.
	CString viewer;

	CString font;
};

inline void WaxHtmlFile::null2whitespace(bool val)
{
	nonull = val;
}

