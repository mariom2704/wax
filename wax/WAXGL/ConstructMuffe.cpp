#include "stdafx.h"


#include "constructmuffe.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructMuffe::~ConstructMuffe()
{

}

ConstructMuffe::ConstructMuffe()
{

}

void ConstructMuffe::createBeschriftung()
{
	if (elemerk.get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL)
		set_offset(100);
	
	if (elemerk.get_dn2() > 0)
	{
		if (!dn2zoll)
			text.Format("%d %d", elemerk.get_dn1(), elemerk.get_dn2());
		else
			text.Format("%s'' %s''", WaxStamm::dn2zoll(elemerk.get_dn1()), WaxStamm::dn2zoll(elemerk.get_dn2()));
	}
	else
	{
		if (!dn2zoll)
			text.Format("%d", elemerk.get_dn1());
		else
			text.Format("%s''", WaxStamm::dn2zoll(elemerk.get_dn1()));
	}

	ConstructFitting::createBeschriftung();
}

void ConstructMuffe::createElement()
{
	ConstructFitting::createElement();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	double x, y, dx, dy;

	XASSERT(typ == IMROHR);
	XASSERT(drehfaktor == 0 || drehfaktor == 90 || drehfaktor == 180 || drehfaktor == 270);

	const double lappfaktor = 0.1; // 10 Prozent überragt die Muffe das Rohr 
	const double lappung = elemerk.get_dn() * lappfaktor;
	const double muffenbreite =  elemerk.get_laenge();//liefert Null?
	const double hoehe = elemerk.get_dn() * ( 1 + 2*lappfaktor); // Hoehe ist Durchmesser + 2*lappfaktor Prozent
	const double abstand = (elemerk.get_abstRel()>0)?elemerk.get_abstRel()- muffenbreite:muffenbreite-elemerk.get_abstAbs();
	
	if (typ == IMROHR)
	{
		switch (drehfaktor)
		{
			case 0:
				x = X0 + abstand/f1;
				x = X0 + abstand/f1;
				y = Y0 - lappung;
				dx = 2*muffenbreite/f1;
				dy = hoehe;
				break;

			case 90:
				x = X0 - lappung;
				y = Y0 + abstand;
				dx = hoehe;
				dy = 2*muffenbreite;
				break;

			case 180:
				x = X0 - abstand;
				y = Y0 - lappung;
				dx = -2*muffenbreite;
				dy = hoehe;
				break;

			case 270:
				x = X0 - lappung;
				y = Y0 - abstand;
				dx = hoehe;
				dy = -2*muffenbreite;
				break;

			default:
				return;	

		}
		add_rectangle(x, y, dx, dy, color);
	}
}
