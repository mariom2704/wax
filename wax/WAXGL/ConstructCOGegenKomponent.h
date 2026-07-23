#pragma once

#include "ConstructAbgEle.h"

class ConstructCOIIGegenKomponent : public ConstructAbgEle  
{
private:
	
public:
	
	ConstructCOIIGegenKomponent();
	virtual ~ConstructCOIIGegenKomponent();


	virtual void createBeschriftung();
	virtual void initEleDaten(const Element& ele);
	
	void calculateDimisions();
	double createFlansch();
	double createStutzen(int iFlanschHeight);

protected:
	double dMerkI, MerkII, MerkIII, MerkIV,MerkV;
		double f1;
	GlObj::colour color ;
	double x1, y1;
	double x2, y2;
	double x, y;
	double dx, dy;
	double cc;
	CString strSchwarztext;


};
