#include "stdafx.h"


#include "ERed.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxGrafik;


void ERed::createBeschriftung()
{
	/**
	Beschriftung der EReduzierung besteht aus ERED + DN + evtl. Winkelangabe
	*/
	string txt, ext; 
	string einheit = dn2zoll ? "''" : "";
	
	if (elemerk.get_winkel())
		ext = boost::str( boost::format("%1%'") % elemerk.get_winkel() );
	
	txt = boost::str( boost::format("ERED %1%%2% %3%") % elemerk.get_dn() % einheit % ext );

	Point punused, p;
		
	get_start_coords(punused, p);
	
	const int xoffset = 45;
	const int yoffset = 25;

	switch (drehfaktor)
	{
		case 0:	
		case 180:
			break;

		case 90:
		case 270: 
			p.x += xoffset;
			break;
		default: // Fehler, Elementbeschriftung als Warnung anzeigen
			p.y += yoffset;	
			p.x += xoffset;
			txt = boost::str( boost::format("ERED %1% %2%'!!!") % elemerk.get_dn() % drehfaktor); 
	}
	add_text(p.x, p.y, txt, 45);
}


void ERed::get_start_coords(Point& p1, Point& p2) 
{
	/**
	Startpunkte X0,Y0 sind am Beginn des Rohrstückes davor. Die Anfangskoordinaten
	müssen erst errechnet werden
	*/
	double delta = (double)(elemerk.get_abstAbs() - laenge)/faktorRohrlaenge;
	switch (drehfaktor)
	{
	case 0:
	
		if (bogen)
		{
			p1.set_x( X0 + delta );
			p1.set_y( Y0 );
			p2.set_x( X0 + delta );
			p2.set_y( Y0 + andn );
		} 
		else 
		{
			p1.set_x( X0 + delta - (double)(lEndeLaenge/faktorRohrlaenge) );
			p1.set_y( Y0 ); 
			p2.set_x( p1.x );
			p2.set_y( Y0 + andn ); 
		}
		break;

	case 90:
		p1.set_x( X0 );
		p1.set_y( Y0 + delta );
		p2.set_x( X0 + andn ); 	
		p2.set_y( Y0 + delta ); 
		break;

	case 180: 
		p1.set_x( X0 - delta );
		p1.set_y( Y0 ); 
		p2.set_x( X0 - delta ); 
		p2.set_y( Y0 + andn ); 
		break;

	case 270: 
		p1.set_x(X0 );
		p1.set_y( Y0 - delta ); 
		p2.set_x( X0 + andn ); 	
		p2.set_y( Y0 - delta ); 
		break;
	}

#ifdef _DEBUG
	//add_circle(p1, 20, GlObj::RED);
	//add_circle(p2, 20, GlObj::GREEN);
#endif
}


void ERed::get_delta_dn(bool swap, int& deltaDN1, int& deltaDN2) const
{
	/**
	Errechnet den Unterschied zwischen dn und andn für beide Endkoordinaten. 
	deltaDN1 wird auf der einen Seite addiert, deltaDN2 auf der anderen. 
	Bei Winkel 90 und 270 sieht man die ERed nicht, die Reduzierung sieht wie eine
	Kred aus. Ob deltaDN horizontal oder vertikal addiert wird hängt von der Laufrichtung
	ab. Mit swap kann die DN Änderung getauscht werden (oben / unten bzw. links / rechts). 
	*/
	switch (elemerk.get_winkel())
	{
		case 0:
			if (!swap)
				deltaDN1 = dn - andn; // Nach oben
			else
				deltaDN2 = - (dn - andn); // Nach oben
			break;
		case 180:
			if (!swap)
				deltaDN2 = - (dn - andn); // Nach unten
			else
				deltaDN1 = dn - andn; // Nach unten
			break;
		case 90:
		case 270:
		default:
			deltaDN1 = (dn - andn) / 2; // Man sieht gleichmässig nur die unterschiedlichen DN
			deltaDN2 = -deltaDN1;
			break;
	}
}


void ERed::get_horizontal_coords(bool left_to_right, Point& p1to, Point& p2to, Point& p3from, Point& p3to) 
{
	/**
	Koordinaten für horizontale Reduzierung errechnen
	Bei left_to_right wird von links nach rechts gezeichnet, sonst von rechts nach links. 
	Die X Koordinaten für alle vier Punkte sind gleich.
	*/
	double f1 = faktorRohrlaenge;
	double xlen = (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge + elemerk.get_laenge())/f1;
	
	if (left_to_right)
		p1to.set_x( X0 + xlen );// nach rechts zeichnen
	else
		p1to.set_x( X0 - xlen ); // nach links zeichnen
	p2to.x = p3from.x = p3to.x = p3from.x = p3to.x = p1to.x;	// X Koordinaten sind immer gleich

	int deltaDN1 = 0;
	int deltaDN2 = 0;

	get_delta_dn(left_to_right ? false : true, deltaDN1, deltaDN2);
		
	// untere Linie
	p1to.set_y( Y0 + deltaDN2);
				   
	// obere Linie
	p2to.set_y( Y0 + andn + deltaDN1 );
					   
	// senkrechte Linie Elementende
	p3from.set_y( Y0 + andn + deltaDN1); 
	p3to.set_y( Y0 + deltaDN2 );
	deltaY0 = deltaDN2;
}


/**
Koordinaten für horizontale Reduzierung errechnen
*/
void ERed::get_vertical_coords(bool up, Point& p1to, Point& p2to, Point& p3from, Point& p3to) 
{
	double f1 = faktorRohrlaenge;
	double ylen = (double)(elemerk.get_abstAbs() - laenge - lEndeLaenge + elemerk.get_laenge())/f1;
	// X Koordinaten sind immer gleich
	if (up)
		p1to.set_y( Y0 + ylen );
	else
		p1to.set_y( Y0 - ylen );
	p2to.y = p3from.y = p3to.y = p3from.y = p3to.y = p1to.y;	

	int deltaDN1 = 0;
	int deltaDN2 = 0;

	get_delta_dn(up ? true : false, deltaDN1, deltaDN2);
		
	// untere Linie
	p1to.set_x( X0 + deltaDN2);
				   
	// obere Linie
	p2to.set_x( X0 + andn + deltaDN1 );
					   
	// senkrechte Linie Elementende
	if (up)
	{
		p3from.set_x( X0 + andn + deltaDN1); 
		p3to.set_x( X0 + deltaDN2 );
	}
	else
	{
		p3from.set_x( X0 + andn + deltaDN1); 
		p3to.set_x( X0 + deltaDN2 );
	}
	deltaX0 = deltaDN2;
}

void ERed::createElement()
{
	/**
	Die E Reduzierung wird mit drei Strichen gezeichnet. Eine davon ist gerade, die andere verjüngt oder
	verbreitert das Rohr. Bei Winkel 90 und 270 sieht man nur eine Verjüngung auf beiden Seiten.
	Die dritte Linie zeichnet den Elementabschluss.
	*/
	Point p1from, p1to, p2from, p2to, p3from, p3to;

	// Startkoordinaten für die beiden Verbindungslinien 
	get_start_coords(p1from, p2from);

	switch (drehfaktor)
	{
	case 0:
		// horizontal von links nach rechts
		get_horizontal_coords(true, p1to, p2to, p3from, p3to);
		break;

	case 90:
		// vertikal von unten nach oben
		get_vertical_coords(true, p1to, p2to, p3from, p3to);
		break;

	case 180: 
		// horizontal von rechts nach links
		get_horizontal_coords(false, p1to, p2to, p3from, p3to);
		break;

	case 270: 
		// vertikal von oben nach unten
		get_vertical_coords(false, p1to, p2to, p3from, p3to);
		break;
	}

	add_line(p1from.x, p1from.y, p1to.x, p1to.y);
	add_line(p2from.x, p2from.y, p2to.x, p2to.y);
	// Abschlusslinie
	add_line(p3from.x, p3from.y, p3to.x, p3to.y);
}

