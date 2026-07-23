#include "stdafx.h"

#include "ConstructKnick.h"

ConstructKnick::ConstructKnick(void)
{
}

ConstructKnick::~ConstructKnick(void)
{
}



void ConstructKnick::createBeschriftung()
{
	
	if (elemerk.get_winkel() == 0 || elemerk.get_winkel() == 180)
		text.Format("KNICK %.0f'", elemerk.get_durchmesser());
	else
		text.Format("KNICK %.0f' (%d°)", elemerk.get_durchmesser(), elemerk.get_winkel());

	
	ConstructAbgEle::createBeschriftung();
}

void ConstructKnick::createElement()
{
	ConstructAbgEle::createElement();

	double x0, x1;
	double y0, y1;

	x0 = x1 = X0;
	y0 = y1 = Y0;

	double abstVonAnf = (elemerk.get_abstAbs()  - lEndeLaenge - laenge) / faktorRohrlaenge;

	//Knick mit einer senkrechten Linie darstellen
	if (drehfaktor == 0)
	{
		x0 = x1 = x0 + abstVonAnf;
		y1 = y0 + andn;
	}
	if (drehfaktor == 180)
	{
		x0 = x1 = x0 - abstVonAnf;
		y1 = y0 + andn; 
	}
	else if (drehfaktor == 90)
	{
		x1 = x0 + andn;
		y1 = y0 = y0 + abstVonAnf;
	}
	else if (drehfaktor == 270)
	{
		x1 = x0 + andn;
		y1 = y0 = y0 - abstVonAnf;
	}


	add_line(x0, y0, x1, y1);


	//Knickrichtung symbolisch andeuten
	double laengeAchse = 100; //Schenkellänge des Winkels
	double abstOkr = 50;	  //Abstand von Oberkante Rohr

	if (drehfaktor == 0)
	{
		if (elemerk.get_winkel() == 0)
		{
			x1 = x0 - laengeAchse;
			y1 = y0 = y1 + abstOkr;
			add_line(x1, y1, x0, y0);

			x1 = x0 + laengeAchse;
			y1 = y1 + laengeAchse * 0.5;
			add_line(x0, y0, x1, y1);
		}
		else if (elemerk.get_winkel() == 180)
		{
			x1 = x0 - laengeAchse;
			y1 = y0 = y0 - abstOkr * 0.7;
			add_line(x1, y1, x0, y0);

			x1 = x0 + laengeAchse;
			y1 = y1 - laengeAchse * 0.5;
			add_line(x0, y0, x1, y1);
		}
	}
	else if (drehfaktor == 90)
	{
		if (elemerk.get_winkel() == 0)
		{
			x1 = x0 = x0 - abstOkr;
			y1 = y0 - laengeAchse;
			add_line(x0, y0, x1, y1);

			x1 = x0 - laengeAchse * 0.5;
			y1 = y0 + laengeAchse;
			add_line(x0, y0, x1, y1);
		}
		else if (elemerk.get_winkel() == 180)
		{
			x1 = x0 = x1 + abstOkr * 0.7;
			y1 = y0 - laengeAchse;
			add_line(x0, y0, x1, y1);

			x1 = x1 + laengeAchse * 0.5;
			y1 = y0 + laengeAchse;
			add_line(x0, y0, x1, y1);
		}
	}
	else if (drehfaktor == 270)
	{
		if (elemerk.get_winkel() == 0)
		{
			x1 = x0 = x1 + abstOkr * 0.7;
			y1 = y0 + laengeAchse;
			add_line(x0, y0, x1, y1);

			x1 = x0 + laengeAchse * 0.5;
			y1 = y0 - laengeAchse;
			add_line(x0, y0, x1, y1);
		}
		else if (elemerk.get_winkel() == 180)
		{
			x1 = x0 = x0 - abstOkr * 0.7;
			y1 = y0 + laengeAchse;
			add_line(x0, y0, x1, y1);

			x1 = x1 - laengeAchse * 0.5;
			y1 = y0 - laengeAchse;
			add_line(x0, y0, x1, y1);
		}
	}
    else if (drehfaktor == 180)
	{
		if (elemerk.get_winkel() == 0)
		{
			x1 = x0 + laengeAchse;
			y1 = y0 = y0 - abstOkr;
			add_line(x1, y1, x0, y0);

			x1 = x0 - laengeAchse;
			y1 = y1 - laengeAchse * 0.5;
			add_line(x0, y0, x1, y1);
		}
		else if (elemerk.get_winkel() == 180)
		{
			x1 = x0 + laengeAchse;
			y1 = y0 = y1 + abstOkr;
			add_line(x1, y1, x0, y0);

			x1 = x0 - laengeAchse;
			y1 = y1 + laengeAchse * 0.5;
			add_line(x0, y0, x1, y1);
		}
	}
}