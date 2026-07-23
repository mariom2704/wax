#include "stdafx.h"
#include "ConstructCOVGegenflanHautrohr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



ConstructCOGegenflanHauptrohrVflan::ConstructCOGegenflanHauptrohrVflan()
{

}

ConstructCOGegenflanHauptrohrVflan::~ConstructCOGegenflanHauptrohrVflan()
{

}


/*
void ConstructCOGegenflanHauptrohrVflan::createBeschriftung()
{
	text = "FL+BFL";
	
	double f1 = faktorRohrlaenge;
	double x = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1 + 150;
	double y = Y0;
	GlObj::colour color = GlObj::BLACK;
	
	add_text(x, y, text, color, false, 360);
}
´*/


void ConstructCOGegenflanHauptrohrVflan::createElement()
{
	ConstructEndEle::createElement();


	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;
	double x1 = 0, y1 = 0;
	double x2 = 0, y2 = 0;
	double x = 0, y = 0;
	double dx = 0, dy = 0;
	
		// Flansch links
		// V-Flansch links
		if (erstesElement) 
		{
		

			x1 = X0;
			y1 = Y0;
			x2 = X0 - elemerk.get_laenge()/f1*8/10;
			y2 = Y0 - dn/10;
			add_line(x1, y1, x2,	y2, 1, color);

			x1 = X0;
			y1 = Y0 + dn;
			x2 = X0 - elemerk.get_laenge()/f1*8/10;
			y2 = Y0 + dn + dn/10;
			add_line(x1, y1, x2,	y2, 1, color);
			
			x     = X0 - elemerk.get_laenge()/f1;
			y     = Y0 - dn/5;
			dx    = elemerk.get_laenge()/f1*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);

			// 2 Flansch
			x     = X0 - elemerk.get_laenge()/f1 - elemerk.get_laenge()/f1*2/10 -  dn*2/5;
			y     = Y0 - dn/5;
			dx    = elemerk.get_laenge()/f1*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);

			add_line( x - dx - dn*2/5,
					  Y0, 
					  x ,
					  Y0 - dn/10,
					  1, color);

			add_line( x - dx - dn*2/5,
					  Y0 + dn, 
					  x,
					  Y0 + dn + dn/10 ,
					  1, color);

			add_line( x - dx -   dn*2/5,
					  Y0 - dn/10 + 11,
					  x - dx -   dn*2/5,
					  Y0 + dn + dn/10 - 11,
					  1, color);
					 
		} 
		// V-Flansch rechts
		else if (winkel == 0) 
		{
			add_line(X0 + (double)elemerk.get_abstAbs()/f1,								Y0, 
						   X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1*8/10, Y0 - dn/10,
						   1, color);
			add_line(X0 + (double)elemerk.get_abstAbs()/f1, 								Y0 + dn, 
						   X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1*8/10, Y0 + dn + dn/10,
						   1, color);
			x     = X0 + (double)elemerk.get_abstAbs()/f1 + elemerk.get_laenge()/f1*8/10;
			y     = Y0 - dn/5;
			dx    = elemerk.get_laenge()/f1*2/10;
			dy    = dn + dn/5*2;
			add_rectangle(x, y, dx, dy, color);
			
			x = x + dx + dn*2/5;
			add_rectangle(x, y, dx, dy, color);

			add_line(	x + dx,								
						Y0 - dn/10, 
						x + dx +  dn*4/5, 
						Y0,
						1, color);
			
			add_line(	x + dx, 
						Y0 + dn + dn/10, 
						x + dx +  dn*4/5,
						Y0 + dn,
						1, color);
			add_line( x + dx +  dn*4/5,
					  Y0 - dn/10 + 11,
					  x + dx +  dn*4/5,
					  Y0 + dn + dn/10 - 11,
					  1, color);
					   
		} 
		CString strText = "FL+FL";
		dy    = dn + dn/5*2;
		add_text(x, dy, strText, color, false, 360);
}


