#pragma once

#include "globj.h"


//Ausgabe eines Kreises
class WAXGLDLL Kreis : public GlObj  
{
private:
	//Erzeugt die Füllung für den Kreis
	//void serialize_fill(DWFFile * file);

	//Durchmesser des Kreises in mm
	int durchmesser;

	//Startpunkt der Kreislinie in Grad
	int start;

	//Endpunkt der Kreislinie in Grad
	int end;

	int gefuellt;

public:


	//Konstruktor
	Kreis(double x0 = 0, double y0 = 0, int durchmesser = 0, enum colour color = GlObj::BLACK,
		  int start = 0, int end = 0, bool gefuellt = false);

	//Destruktor
	virtual ~Kreis();

	//Die  DWF Funktionen erwarten statt einer Gradzahl einen Wert von 0 bis
//65536. Für die gängigsten Gradzahlen sind hier Konstanten eingefügt.
	enum GRAD 
	{
	
		GRAD_0 = 0,

		GRAD_45 = 8192,

		GRAD_90 = 16384,

		GRAD_135 = 24576,

		GRAD_180 = 32768,

		GRAD_225 = 40960,

		GRAD_270 = 49152,

		GRAD_315 = 57344,

		GRAD_360 = 65536
	};



	//Serialisierungsfuntion
	virtual void serialize(GlFile* file);

};

