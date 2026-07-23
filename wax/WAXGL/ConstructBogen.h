#pragma once

#include "ConstructRohrEle.h"

#include <afxcoll.h>

#include "ConstructElement.h"

class WAXGLDLL ConstructBogen : public ConstructRohrEle  
{
public:
	ConstructBogen();

	ConstructBogen(CObList &entleerung);
	
	virtual ~ConstructBogen();

	virtual void createBeschriftung();

	virtual void createElement();


private:
	//Entleerung kann Muffe oder Rohr sein
	Element ele_entleer;
	
	// Bogen zeichnen mit Elementwinkel 0
	void drawBogenNachOben(int drehfaktor);
	
	// Bogen zeichnen mit Elementwinkel 180
	void drawBogenNachUnten(int drehfaktor);

	// Bogen zeichnen mit Elementwinkel 90 und 270 Grad
	void drawBogenDummy(int drehfaktor, bool filled);

	// Allgemeinen Bogen zeichnen als zwei konzentrische Kreise
	void drawBogen(double x0, double y0, int gradbegin, int gradend);

	// Entleerung zeichnen, gilt bis jetzt nur für EntnahmeleitungDWB
	void drawEntleerung(double x0, double y0, int r0);
};

