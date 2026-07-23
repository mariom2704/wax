#pragma once

#include "linie.h"



class WAXGLDLL Bemassung  : public GlObj
{
public:
	//2. Konstruktor
	Bemassung(double X0, double Y0, double X1, double Y1, 
		      int Ebene = 0, bool Nachoben = true, bool Vertikal = false, double FaktorRohrlaenge = 1, CString Text = "");

	//Destruktor
	virtual ~Bemassung();

	

	void Set_BemassungFontGroesse(int Groesse)
	{
		m_fontGroesse = Groesse;
	}


	enum ABSTAND 
	{
	//Abstand zwischen zwei Bemassungsebenen
		EBENENABSTAND = 100,
	
	//Länge der Bemassungstrennlinien
		TRENNLINIENLAENGE = 15
	};

	//Speichern des Objektes in eine Datei
	virtual void serialize(GlFile* file);

protected:
	//Text
	CString text;

	//Bemassung erfolgt nach oben oder unten
	bool nachOben;

private:
	int index_ebene;
	//Bemassung erfolgt horizontal oder vertikal
	bool vertikal;

	//Faktor der Rohrlängen aus den Systemparametern
	double faktorRohrlaenge;

	//Liefert die absoluten Koordinaten für den Bemassungstext
	void getTxtCoords(double& x, double& y) const;

	/// Wird gesetzt, wenn die Bemassung ungültig ist
	bool disabled;
	
	//Endkoordinate x
	double x1;

	//Endkoordinate y
	double y1;

	int m_fontGroesse;

public:
	virtual void getmax_xy(double& x, double& y) const;

	virtual void getmin_xy(double& x, double& y) const;

	//Liefert die absoluten Koordinaten für die Bemassungslinie
	void getCoords(double& x0, double& y0, double& x1, double& y1) const;

	//Prüft die absoluten xy-Koordinaten der Bemassung auf Gleichheit
	bool sameCoordinates(const Bemassung& other);

	const bool get_vertikal() const;

	//Prüft die absoluten xy-Koordinaten der Bemassung auf einen minimalen 
	//Abstand (EBENENABSTAND).
	bool testCoordinates(const Bemassung& other);

};

