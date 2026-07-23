#include "stdafx.h"


#include "constructfitting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructFitting::~ConstructFitting()
{

}

ConstructFitting::ConstructFitting()
{
	gfzeichen = "";
	showGfzeichen = false;
	offset = 0;
}

void ConstructFitting::createBeschriftung()
{
	if (text.GetLength() == 0)
		return;

	ConstructRohrEle::createBeschriftung();

	double f1 = faktorRohrlaenge;
	GlObj::colour color = GlObj::BLACK;

	CString zusaetzlicheFittinge = "";

	if (elemerk.get_uelementList().GetCount() > 0)
	{
		for (POSITION pos = elemerk.get_uelementList().GetHeadPosition(); pos != NULL;)	
		{
			Element *ele = dynamic_cast<Element*>(elemerk.get_uelementList().GetNext(pos));
			XASSERT(ele);
			if (ele)
				zusaetzlicheFittinge += ("+" + ele->get_gfzeichen());
		}
	}

	text += zusaetzlicheFittinge;

	switch (typ) 
	{
	case IMROHR:
		double x = 0, y = 0;
		int winkel = 45;
		if (get_showText())
		{
			if (elemerk.get_firstele())
			{
				x = X0;
				y = Y0 + rohrDn + 100;
			}
			else if (drehfaktor == 0 && bogen) 
			{
				x = X0 + elemerk.get_abstAbs() - laenge + offset;
				y = Y0 + rohrDn + 100;
			}
			else if (drehfaktor == 0) 
			{
				x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge - offset)/f1;	
				y = Y0 + rohrDn + 100; 
			}
			else if (drehfaktor == 180) 
			{
				x = X0 - elemerk.get_abstAbs() + laenge,					
				y = Y0 + rohrDn + 100; 
			}
			else if (drehfaktor == 90) 
			{
				if (elemerk.get_winkel() == 180)
				{
					x = X0 + rohrDn + 100;
				}
				else
				{
					winkel = 135;
					x = X0 - 100;		
				}
				y = Y0 + elemerk.get_abstAbs() - laenge; 
			}
			else if (drehfaktor == 270)
			{
				x = X0 + rohrDn + 100;		
				y = Y0 - elemerk.get_abstAbs() + laenge; 
			}
			add_text(x, y, text, color, false, winkel);
		}
		if (showGfzeichen)
		{
			gfzeichen = elemerk.get_gfzeichen();
			
			if (drehfaktor == 0 && bogen) 
			{
				x = X0 + elemerk.get_abstAbs() - laenge;					
				y = Y0 - 300; 
			}
			else if (drehfaktor == 0) 
			{
				x = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;	
				y = Y0 - 300; 
			}
			else if (drehfaktor == 180) 
			{
				x = X0 - elemerk.get_abstAbs() + laenge;					
				y = Y0 - 300; 
			}
			else if (drehfaktor == 90) 
			{
				x = X0 - 300;		
				y = Y0 + elemerk.get_abstAbs() - laenge; 
			}
			else if (drehfaktor == 270)
			{
				x = X0 - 300;		
				y = Y0 - elemerk.get_abstAbs() + laenge; 
			}
			add_text(x, y, gfzeichen, color, false, 90);
		}
		break;
	}
}

const CString& ConstructFitting::get_gfzeichen() const
{
	return gfzeichen;
}

void ConstructFitting::set_gfzeichen(const CString& value)
{
	gfzeichen = value;
}

const bool ConstructFitting::get_showGfzeichen() const
{
	return showGfzeichen;
}

void ConstructFitting::set_showGfzeichen(bool value)
{
	showGfzeichen = value;
}

const int ConstructFitting::get_offset() const
{
	return offset;
}

void ConstructFitting::set_offset(int value)
{
	offset = value;
}
