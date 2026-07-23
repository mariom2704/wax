#pragma once

#include <vector>
#include "Globj.h"
#include "point.h"

class WAXGLDLL ConstructElement
{
//private
private:
	//showText, gibt an ob der Text fuer dieses Element gezeigt werden soll
	bool showText;

protected:

	int rohrbreite;

	int winkel;

	int dn;

	int andn;

	int maxlaengeAbg;

	int maxlaengeAbgOben;

	int maxlaengeAbgUnten;

	int maxlaengeAbgLinks;

	int maxlaengeAbgRechts;

	bool knickBemassung;

	//massText, gibt ein zusaetzlichen Text an der mit der Masszahl angezeigt 
	//werden soll
	//(bisher nur fuer Handelslaenge "~")
	CString masstext;


	//maxRohrDN, gibt die groesste in Rohrleitung vorkommende RohrDN an, damit 
	//bei Red. die Bemassungslinien auf einer Höhe sind
	int maxRohrDN;

	//elemerk, Merker-Element
	Element elemerk;

	//ist die Bemassung eine Ebene nach oben verschoben durch geringen rel. Abstand 
	static bool bemassungHochgestellt;

	//dn2zoll, gibt an ob die Nennweiten in DN oder in Zoll beschriftet werden 
	//sollen, User-Parameter
	static bool dn2zoll;

	//faktorElelaenge, durch diesen Faktor wird die Laenge der Abgaenge (nur 
	//Rohrstutzen) geteilt , User-Parameter
	static double faktorElelaenge;

	//faktorElebreite, durch diesen Faktor wird die Breite der Abgaenge geteilt, 
	//User-Parameter
	static double faktorElebreite;

	//faktorRohrlaenge, durch diesen Faktor wird die Rohrlaenge geteilt, 
	//User-Parameter
	static double faktorRohrlaenge;

	//faktorRohrbreite, durch diesen Faktor wird die Rohrbreite geteilt, 
	//User-Parameter
	static double faktorRohrbreite;

	//Merker-Element
	Element merkerRohrele;

	//Merker-Element
	Element merkerAbgele;

	std::vector<GlObj*> grafiklist;

	//lEndeLaenge, Laenge des Elements am Rohranfang
	int lEndeLaenge;

	//rEndeLaenge, Laenge des Elements am Rohrende
	int rEndeLaenge;

	//Max. Rohrlaenge in Drehrichtung 0
	int maxlaenge_0;

	//Max. Rohrlaenge in Drehrichtung 90
	int maxlaenge_90;

	//Max. Rohrlaenge in Drehrichtung 180
	int maxlaenge_180;

	//Max. Rohrlaenge in Drehrichtung 270
	int maxlaenge_270;
	
	//Max. Abganglaenge in Drehrichtung 0
	int maxlaengeAbg_0;

	//Max. Abganglaenge in Drehrichtung 90
	int maxlaengeAbg_90;
	
	//Max. Abganglaenge in Drehrichtung 180
	int maxlaengeAbg_180;
	
	//Max. Abganglaenge in Drehrichtung 270
	int maxlaengeAbg_270;

	//drehfaktor, gibt an in welche Richtung gezeichnet werden muss
	int drehfaktor;

	//bogenlaenge, gibt die laenge des aktuellen Bogens an
	int bogenlaenge;

	//bogen, gibt an ob ein Bogen in der Rohrleitung ist
	bool bogen;

	//laenge, Summe der bisher gezeichneten Rohrlaengen
	int laenge;

	//erstesElement, gibt an, ob das Element das erste einer neuen 
	//Teilkonstruktion ist
	bool erstesElement;

	double X0;

	double Y0;

	/// Evtl. Koordinatenänderungen gültig für den Rest der Konstruktion
	int deltaX0;
	int deltaY0;

	//rohrDn, Nennweite des aktuellen Rohrstuecks
	int rohrDn;

	int typ;

	//text fuer das Element
	CString text;

//public
public:
	// Rohrkonstruktion
	Rohrleitung* rohrleitung;

	enum TYP 
	{
		IMROHR,
		ABG,
		ABGABG
	};

	//Destruktor
	virtual ~ConstructElement();

	//Konstruktor
	ConstructElement();

	bool has_coords_changed() const
	{
		return deltaX0 || deltaY0;
	}



	void update_coords(double& x0, double& y0) const
	{
		x0 += deltaX0;
		y0 += deltaY0;
	}

	virtual void initEleDaten(const Element& ele);

	void initEleDatenMerker(Element ele);

	void initEleDatenAbgMerk(Element ele);

	void initGraphikDaten(double X0, double Y0, int drehfaktor, int bogenlaenge, bool bogen, bool erstesElement, int laenge, int lEndeLaenge, int rEndeLaenge, int typ);

	void initBemassung(int maxlaenge_0, int maxlaenge_90, int maxlaenge_180, int maxlaenge_270, 
					   int maxlaengeAbg_0, int maxlaengeAbg_90, int maxlaengeAbg_180, int maxlaengeAbg_270,
					   int maxlaengeAbg, int rohrDn, CString masstext = "");

	//Setzt die Faktoren der Grafikerstellung. Diese Faktoren sind statisch, da 
	//es systemweite Einstellungen sind.
	static void setGraphikfaktoren(double rohrlaenge = 1, double rohrbreite = 1, double elelaenge = 1, double elebreite = 1, bool dnzuzoll = false);

	void add_rectangle(double X0, double Y0, double Dx, double Dy, GlObj::colour Color);
	void add_line(double X0, double Y0, double X1, double Y1, 
						int Strichstaerke = 1, GlObj::colour Color = (GlObj::colour)0);

	void add_line(const Point& a, const Point& b, int Strichstaerke = 1, GlObj::colour Color = GlObj::BLACK);

	void add_dimensioning(double X0, double Y0, double X1, double Y1, 
						    int Ebene, bool Nachoben, bool Vertikal, CString masstext = "");

	void add_circle(double X0, double Y0, int Durchmesser, int Start, int End, bool gefuellt = false);

	void add_circle(const Point& p, int Durchmesser, GlObj::colour c = GlObj::NONE);

	void add_text(double X0, double Y0, CString Text, GlObj::colour Color, bool Mittig, int Drehwinkel, bool Relative = false);

	void add_text(double X0, double Y0, const std::string& txt, int Drehwinkel = 0, GlObj::colour Color = GlObj::BLACK, bool Mittig = false);
	
	void add_text(double X0, double Y0, const std::string& txt, bool Mittig);

	void add_text2(double X0, double Y0, const std::string& text, GlObj::colour Color = GlObj::RED, int fontgroesse = 8);

	const std::vector<GlObj*>& get_grafiklist() const
	{
		return grafiklist;
	}

	virtual void createElement() = 0;

	virtual void createBemassung() = 0;
	
	virtual void createBeschriftung() = 0;

	//set maxRohrDN
	void set_maxRohrDN(int value);

	const bool get_showText() const;

	void set_showText(bool value);

	//Ist Element Knick eingegeben, so ist dies eine Ebene höher als die 
	//Standardelemente zu bemassen.
	void set_knickBemassung(bool value);
};
