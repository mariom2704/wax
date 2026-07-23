#pragma once

#include "ConstructFitting.h"

class ConstructWinkel : public ConstructFitting  
{
private:
	// Bogen zeichnen mit Elementwinkel 0
	void drawBogenNachOben(int drehfaktor);
	
	// Bogen zeichnen mit Elementwinkel 180
	void drawBogenNachUnten(int drehfaktor);

	// Bogen zeichnen mit Elementwinkel 90 und 270 Grad
	void drawBogenDummy(int drehfaktor, bool filled);

	// Allgemeinen Bogen zeichnen als zwei konzentrische Kreise
	void drawBogen(double x0, double y0, int gradbegin, int gradend);

public:
	virtual void createBeschriftung();

	virtual void createElement();

	virtual ~ConstructWinkel();

	ConstructWinkel();

};

