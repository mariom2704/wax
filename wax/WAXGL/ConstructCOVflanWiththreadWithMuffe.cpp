#include "stdafx.h"


#include "ConstructCOVflanWiththreadWithMuffe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructCOVflanWiththreadWithMuffe::ConstructCOVflanWiththreadWithMuffe()
{}



ConstructCOVflanWiththreadWithMuffe::~ConstructCOVflanWiththreadWithMuffe()
{}



void ConstructCOVflanWiththreadWithMuffe::createElement()
{
	calculateDimisions();
	int YHeightFLansch = (int) createFlansch();
	int YHeightStutzen = (int) createStutzen(YHeightFLansch);
	// Muffe
	add_rectangle(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1 +  MerkV*dn/2, YHeightFLansch + (YHeightStutzen -YHeightFLansch)/2- dn/2 , 3*dx/5, 1.3*MerkIV*dn, color);
	
	int iLaenge = elemerk.get_laenge();

	CString strLaenge;
	strLaenge.Format("%d", iLaenge/2);
	//int mm = cc +  element_laenge/2 + 1.3*MerkIV*dn/2 - YHeight;
	
	add_text(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 + 2* 3*dx/5,
					    YHeightFLansch + (YHeightStutzen -YHeightFLansch)*1/4, 
						strLaenge, color, true, 90);
	
	add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 + 2* 3*dx/5, YHeightFLansch+10, X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 + 2* 3*dx/5 - 30, YHeightFLansch+10);
	add_line(X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 + 2* 3*dx/5, YHeightFLansch + (YHeightStutzen -YHeightFLansch)/2,
		X0 + (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge)/f1  +  MerkV*dn/2 + 2* 3*dx/5 - 30,
		YHeightFLansch + (YHeightStutzen -YHeightFLansch)/2);

}
