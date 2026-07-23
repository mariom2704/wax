#include "stdafx.h"


#include "kreis.h"
#include "ConstructCOBockWinkel.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConstructCOBockWinkel::~ConstructCOBockWinkel()
{}


ConstructCOBockWinkel::ConstructCOBockWinkel()
{}


void ConstructCOBockWinkel::createBeschriftung()
{
	
	text = "Wi";
		
	ConstructAbgEle::createBeschriftung();
}


void ConstructCOBockWinkel::createBemassung()
{
	
	int iId = elemerk.get_id();
	int iMerkWinkelUnterElement = elemerk.get_dn1(); // wird hier als Merker missbraucht,
	//der Winkel genau unter einem Element liegt, dann muss dieser Bemsst werden
	if( iId == WINKEL_LINKS_COII || iMerkWinkelUnterElement == -1)
	{
	  ConstructAbgEle::createBemassung();
	}
	
}

	
void ConstructCOBockWinkel::createElement()
{
	ConstructAbgEle::createElement();

	
	COIIVerteiler* pCOIIVerteilerUsedAsBase =  dynamic_cast<COIIVerteiler*>(rohrleitung);
	

	double f1 = faktorRohrlaenge;
	int iId = elemerk.get_id();
	GlObj::colour color = GlObj::BLACK;
	double x  = 0, y  = 0;
	
	x     = X0 - (double)(lEndeLaenge - elemerk.get_abstAbs() + laenge)/f1;
	y     = Y0 - elemerk.get_laenge();
	
	static double iAbstandBoecke[] = {0,0,0,0};
	static int bStoretrestles = 0; //Merker, welcher Bock gerade gemeint ist
	
	if( iId == WINKEL_LINKS_COII)
	{
		// winkel
		add_line( x-70,  Y0 - 20, x,  Y0 - 20, 1, color);
		
		iAbstandBoecke[0] = x;
		bStoretrestles = 1;
		
		CString strText;
		strText.Format(_T("%d"), elemerk.get_abstAbs());
		add_text(-lEndeLaenge/f1 +(x-(-lEndeLaenge/f1))/2,  Y0 - 380, strText, GlObj::BLACK, true, 360);
		add_line( -lEndeLaenge/f1,  Y0 - 370, -lEndeLaenge/f1,  Y0 - 390, 1, color);
		add_line( x,  Y0 - 370, x,  Y0 - 390, 1, color);
		
/*
		ELEMENTEUNTEN ElementUnten;
		ElementUnten.xStart =  -lEndeLaenge/f1 ;
		ElementUnten.xEnde  =  x;
		ElementUnten.Laenge =  elemerk.get_abstRel();
		pCOIIVerteiler->SetRohrElementeUnten(ElementUnten);
*/
		DrawRohrLaenge();
		
	}
	else
	{	
		//Querwinkel
		add_line( x+70,  Y0 - 20, x,  Y0 - 20, 1, color);

		
		double xPointElementBefore =  X0 + (double)(elemerk.get_abstAbs() - laenge - elemerk.get_abstRel() - lEndeLaenge)/f1;
			
		if( bStoretrestles == 1)
		{
			
			//Der Startpunkt des vorherigen Elements wird berechnet und gespeichert, um diesen dann zur Bemassung des oberen Elementes zu benutzen
			pCOIIVerteilerUsedAsBase->SetRohrPointElementBeforeWinkel(xPointElementBefore);
			iAbstandBoecke[1] = x;
			bStoretrestles = 2;
			CString strText;
			
			double dNumValue = (iAbstandBoecke[1] - iAbstandBoecke[0])*f1;
			int iNumValue = (int)((iAbstandBoecke[1] - iAbstandBoecke[0])*f1);
			double Rest = dNumValue - iNumValue;
			
			if( Rest > 0.5)
			{
				iNumValue++;
			}

			strText.Format(_T("%d"), iNumValue);
			add_text(iAbstandBoecke[0] + (iAbstandBoecke[1] - iAbstandBoecke[0])/2,  Y0 - 380, strText, GlObj::BLACK, true, 360);
			add_line( x,  Y0 - 370, x,  Y0 - 390, 1, color);
		}
		else
		{		
			if(bStoretrestles == 2)
			{
				//Der Startpunkt des vorherigen Elements wird berechnet und gespeichert, um diesen dann zur Bemassung des oberen Elementes zu benutzen
				// Der hier
				if(pCOIIVerteilerUsedAsBase->GetPreElementId() != WINKEL_RECHTS_COII)
				{
					pCOIIVerteilerUsedAsBase->SetRohrPointElementBeforeWinkel(xPointElementBefore);
				}
				bStoretrestles = 3;
				iAbstandBoecke[2] = x;
				CString strText;
				double dNumValue = (x - iAbstandBoecke[1])*f1;
				int iNumValue = (int) ((x - iAbstandBoecke[1])*f1);
				double Rest = dNumValue - iNumValue;
				if( Rest > 0.5)
				{
					iNumValue++;
				}


				strText.Format(_T("%d"), iNumValue);
				add_text(iAbstandBoecke[1] + (x - iAbstandBoecke[1])/2,  Y0 - 380, strText, GlObj::BLACK, true, 360);
				add_line( x,  Y0 - 370, x,  Y0 - 390, 1, color);
			}
			else
			{
				//Der Startpunkt des vorherigen Elements wird berechnet und gespeichert, um diesen dann zur Bemassung des oberen Elementes zu benutzen
				pCOIIVerteilerUsedAsBase->SetRohrPointElementBeforeWinkel(xPointElementBefore);
				CString strText;
				double dNumValue = (x - iAbstandBoecke[2])*f1;
				int iNumValue = (int) ((x - iAbstandBoecke[2])*f1);
				double Rest = dNumValue - iNumValue;
				if( Rest > 0.5)
				{
					iNumValue++;
				}
				strText.Format(_T("%d"), iNumValue);
				add_text(iAbstandBoecke[2] + (x - iAbstandBoecke[2])/2,  Y0 - 380, strText, GlObj::BLACK, true, 360);
				add_line( x,  Y0 - 370, x,  Y0 - 390, 1, color);
				}
		}
		
	}
	
	add_line( x,  Y0 - 20, x,  Y0 + 50, 1, color);
}


void ConstructCOBockWinkel::DrawRohrLaenge()
{
	//rohr Laengen und Rohrwand dicken angabe
	int LaengeX  = (int) (X0 - (double)(lEndeLaenge - rohrleitung->get_laenge()/2 + laenge)/faktorRohrlaenge);
	Element Ele;
	Ele.initStammdaten(rohrleitung->get_dnvl(), 0, rohrleitung->get_rohrQuali()+ROHR_Q1-1);
	CString txt = getStammdaten().getSapArtikelbezeichnung(Ele.get_artikelnr());
	int iWo1 = txt.Find(" ");
	int iWo2 = txt.Find("x");
	if( iWo2 == -1 )
	{
		iWo2 = txt.Find("X");
	}
	iWo2 = txt.Find(",", iWo2+1);
	CString strText = txt.Mid(iWo1+1, iWo2+1-iWo1);
	add_text(LaengeX, Y0 - dn * 2, strText, GlObj::BLACK, true, 360);
}