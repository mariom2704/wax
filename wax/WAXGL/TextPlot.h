#pragma once

#include "globj.h"


/// Textausgabe
/// Die Klasse TextPlot bildet die Basisklasse für alle Textausgaben.
class WAXGLDLL TextPlot : public GlObj  
{
private:
	/// Errechnet aus dem Text, der Fontgrösse und dem Drehwinkel die maximalen XY 
	/// Koordinaten
	void calcMaxCoords();

	CPoint minXY;

	CPoint maxXY;

	int drehwinkel;

	bool relative;


public:
	virtual void getmin_xy(double & x, double & y) const;

	virtual void getmax_xy(double & x, double & y) const;

	const int get_drehwinkel() const;

	void set_drehwinkel(int value);

	const bool get_relative() const;

	void set_relative(bool value);
	
	/// Konstruktor
	TextPlot();
	
	/// Konstruktor
	TextPlot(double X0, double Y0, const CString &Txt, GlObj::colour Color = GlObj::BLACK, bool Mittig = false, int Drehwinkel = 0, bool Relative = false);
	
	/// Destruktor
	virtual ~TextPlot();

	inline const bool get_mittig() const;

	inline void set_mittig(bool value);

	inline const int get_fontGroesse() const;

	inline void set_fontGroesse(int value);

	inline const CString& get_font() const;

	inline void set_font(CString& value);

	inline const CString& get_text() const;

	inline void set_text(CString& value);

	/// Text setzen
	void Set_text(double X, double Y, const CString& Txt);

	/// Font setzen
	void Set_font(int Groesse, const CString& Fontname, const CString &Txt = "");

	/// Serialisierungsfunktion
	virtual void serialize(GlFile* file);

	/// Reale Grösse des Fonts, Faktor im Moment geschätzt
	int get_fontpixel() const
	{
		// @todo Reale Koordinatengroessen ermitteln (DWF Toolkit?)
		return (int)(fontGroesse * 5);
	}

protected:
	bool mittig;
	
	/// Fontgrösse
	int fontGroesse;
	
	/// Fontname
	CString font;
	
	/// auszugebender Text
	CString text;
};

inline const bool TextPlot::get_mittig() const
{
	return mittig;
}

inline void TextPlot::set_mittig(bool value)
{
	mittig = value;
	return;
}

inline const int TextPlot::get_fontGroesse() const
{
	return fontGroesse;
}

inline void TextPlot::set_fontGroesse(int value)
{
	fontGroesse = value;
	return;
}

inline const CString& TextPlot::get_font() const
{
	return font;
}

inline void TextPlot::set_font(CString& value)
{
	font = value;
	return;
}

inline const CString& TextPlot::get_text() const
{
	return text;
}

inline void TextPlot::set_text(CString& value)
{
	text = value;
	return;
}
