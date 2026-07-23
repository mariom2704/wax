#include "StdAfx.h"
#include "UtiltyHelper.h"


#include "..\_Tubes Logic Grafic Adapter\RohrKomponente.h"
#include "..\_Tubes Logic Grafic Adapter\RohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "..\_Tubes Logic Grafic Adapter\RohrBemassungsRohrKomponentenVector.h"
#include "..\_Tubes Logic Grafic Adapter\RohrBemassungsRohrKomponentenVectorVector.h"
#include "..\_Tubes Logic Grafic Adapter\RohrOneAssembeldKomponenteVector.h"

#include "..\SVG File Operations\GraficFileController.h"
#include "GraficTubeController.h"
#include "..\Controller\GraficController.h"
#include "GraficRohrKomponente.h"
#include "GraficRohrBemassungsAnbauChildKomponentenVectorVector.h"
#include "GraficRohrBemassungsRohrKomponentenVector.h"
#include "GraficRohrBemassungsRohrKomponentenVectorVector.h"
#include "GraficRohrOneAssembeldKomponenteVector.h"



CGraficRohrBemassungsRohrKomponentenVector::CGraficRohrBemassungsRohrKomponentenVector(CGraficController* pGraficGraficController): m_pGraficGraficController(pGraficGraficController)
{}



CGraficRohrBemassungsRohrKomponentenVector::~CGraficRohrBemassungsRohrKomponentenVector(void)
{}



bool CGraficRohrBemassungsRohrKomponentenVector::HasChildTubes()
{
	bool bHasChilds = false;
	if(m_GraficRohrBemassungsAnbauChildKomponentenVectorVector.HasChildTubes())
	{
		bHasChilds = true;
	}
	return bHasChilds;
}



CGraficRohrBemassungsAnbauChildKomponentenVectorVector& CGraficRohrBemassungsRohrKomponentenVector::GetGraficRohrBemassungsAnbauChildKomponentenVectorVector()
{
	return m_GraficRohrBemassungsAnbauChildKomponentenVectorVector;
}



CPointDouble CGraficRohrBemassungsRohrKomponentenVector::GetConnectionPointForAbgaenge(double RelativerAbstandzumNaechsten)
{
	
	CPointDouble PointOfConnectionForAbgaenge;

	int iIndexMasterDirectionKomponente = m_pRohrLogicBemassungsKomponentenVector->GetIndexMasterDirectionKomponent();
	
	CPointDouble RightPointOfConnectionForAbgaenge;
	if(at(0)->GetLogicalRohrKomponente()->GetFirstKomponent())
	{
		RightPointOfConnectionForAbgaenge.X = at(0)->GetFirstBorderpoint().X;
		RightPointOfConnectionForAbgaenge.Y = at(0)->GetFirstBorderpoint().Y;
	}
	else
	{
		RightPointOfConnectionForAbgaenge.X = at(0)->GetSecondBorderpoint().X;
		RightPointOfConnectionForAbgaenge.Y = at(0)->GetSecondBorderpoint().Y;
	}


	PointOfConnectionForAbgaenge.X =  RightPointOfConnectionForAbgaenge.X + cos(  at(iIndexMasterDirectionKomponente)->GetWinkelRohrZuXundYAchse() *  3.14 /180) * 
		RelativerAbstandzumNaechsten  * m_pGraficGraficController->GetFactorPhysicalTubeDimisionsToPixelDimison();

	PointOfConnectionForAbgaenge.Y =   RightPointOfConnectionForAbgaenge.Y + sin(  at(iIndexMasterDirectionKomponente)->GetWinkelRohrZuXundYAchse() *  3.14 /180) * 
		RelativerAbstandzumNaechsten* m_pGraficGraficController->GetFactorPhysicalTubeDimisionsToPixelDimison(); 
	
	return PointOfConnectionForAbgaenge;
};



void CGraficRohrBemassungsRohrKomponentenVector::CalculateAadDrawChildDimension(int iRelativerAbstand, int i)
{
	
	
	int iIndexMasterDirectionKomponente = m_pRohrLogicBemassungsKomponentenVector->GetIndexMasterDirectionKomponent();

	CPointDouble StartPointDoubleRohr = at(iIndexMasterDirectionKomponente)->getStartPointCenterLine();
	CPointDouble EndPointDoubleRohr   = at(iIndexMasterDirectionKomponente)->GetEndPointCenterLine();

	double dWinkelRohrZuXundYAchse = GetWinkelRohrZuXundYAchse();
	double xAbstand = cos((dWinkelRohrZuXundYAchse + -90) * 3.14 / 180) 
		* 60 * (i+1);

	double yAbstand = sin((dWinkelRohrZuXundYAchse + -90) * 3.14 / 180)
		* 60 * (i+1);

	StartPointDoubleRohr.X  = StartPointDoubleRohr.X + xAbstand; 
	StartPointDoubleRohr.Y  = StartPointDoubleRohr.Y + yAbstand; 

	EndPointDoubleRohr.X  = EndPointDoubleRohr.X + xAbstand; 
	EndPointDoubleRohr.Y  = EndPointDoubleRohr.Y + yAbstand; 

	CPointDouble DeltaPitchRohr, DeltaPitchRohrQuare;	
	DeltaPitchRohr.Y = EndPointDoubleRohr.Y - (StartPointDoubleRohr.Y );
	DeltaPitchRohr.X = (EndPointDoubleRohr.X - StartPointDoubleRohr.X );
	
	DeltaPitchRohrQuare.Y = EndPointDoubleRohr.X - (StartPointDoubleRohr.X );
	DeltaPitchRohrQuare.X = (EndPointDoubleRohr.Y - StartPointDoubleRohr.Y ) * -1;

	CPointDouble StartPointGemassungskomponente, EndPointGemassungskomponente;
	if(at(0)->GetLogicalRohrKomponente()->GetFirstKomponent())
	{
		StartPointGemassungskomponente.X = at(0)->GetBemassungEndPoint().X;
		StartPointGemassungskomponente.Y = at(0)->GetBemassungEndPoint().Y;
	}
	else
	{	
		StartPointGemassungskomponente.X = at(0)->GetBemassungStartPoint().X;
		StartPointGemassungskomponente.Y = at(0)->GetBemassungStartPoint().Y;
	}

	double dFaktor = CUtiltyHelper::GetFaktorForConnectionPoint( StartPointDoubleRohr, DeltaPitchRohr,
		CPointDouble(StartPointGemassungskomponente.X, StartPointGemassungskomponente.Y), DeltaPitchRohrQuare);

	StartPoint.X = StartPointDoubleRohr.X + dFaktor * DeltaPitchRohr.X;
	StartPoint.Y = StartPointDoubleRohr.Y + dFaktor * DeltaPitchRohr.Y;

	
	CPointDouble PointOfConnectionForAbgaenge =  GetConnectionPointForAbgaenge( iRelativerAbstand);

	 dFaktor = CUtiltyHelper::GetFaktorForConnectionPoint( StartPointDoubleRohr, DeltaPitchRohr,
		CPointDouble(PointOfConnectionForAbgaenge.X, PointOfConnectionForAbgaenge.Y), DeltaPitchRohrQuare);

	EndPoint.X = StartPointDoubleRohr.X + dFaktor * DeltaPitchRohr.X;
	EndPoint.Y = StartPointDoubleRohr.Y + dFaktor * DeltaPitchRohr.Y;
	
	ZeichneLinie( StartPoint.X, StartPoint.Y, EndPoint.X, EndPoint.Y, _T("rgb(142, 142, 142)") );
}






void CGraficRohrBemassungsRohrKomponentenVector::CalculateAadDrawDimension()
{
	
	int iIndexMasterDirectionKomponente = m_pRohrLogicBemassungsKomponentenVector->GetIndexMasterDirectionKomponent();

	CPointDouble StartPointDoubleRohr = at(iIndexMasterDirectionKomponente)->getStartPointCenterLine();
	CPointDouble EndPointDoubleRohr   = at(iIndexMasterDirectionKomponente)->GetEndPointCenterLine();

	double dWinkelRohrZuXundYAchse = GetWinkelRohrZuXundYAchse();
	double xAbstand = cos((dWinkelRohrZuXundYAchse + 90)* 3.14 / 180) 
		* 60;

	double yAbstand = sin((dWinkelRohrZuXundYAchse + 90)* 3.14 / 180)
		* 60;

	StartPointDoubleRohr.X  = StartPointDoubleRohr.X + xAbstand; 
	StartPointDoubleRohr.Y  = StartPointDoubleRohr.Y + yAbstand; 

	EndPointDoubleRohr.X  = EndPointDoubleRohr.X + xAbstand; 
	EndPointDoubleRohr.Y  = EndPointDoubleRohr.Y + yAbstand; 

	CPointDouble DeltaPitchRohr, DeltaPitchRohrQuare;	
	DeltaPitchRohr.Y = EndPointDoubleRohr.Y - (StartPointDoubleRohr.Y );
	DeltaPitchRohr.X = (EndPointDoubleRohr.X - StartPointDoubleRohr.X );
	
	DeltaPitchRohrQuare.Y = EndPointDoubleRohr.X - (StartPointDoubleRohr.X );
	DeltaPitchRohrQuare.X = (EndPointDoubleRohr.Y - StartPointDoubleRohr.Y ) * -1;

	CPointDouble StartPointGemassungskomponente, EndPointGemassungskomponente;
	if(at(0)->GetLogicalRohrKomponente()->GetFirstKomponent())
	{
		StartPointGemassungskomponente.X = at(0)->GetBemassungEndPoint().X;
		StartPointGemassungskomponente.Y = at(0)->GetBemassungEndPoint().Y;
	}
	else
	{	
		StartPointGemassungskomponente.X = at(0)->GetBemassungStartPoint().X;
		StartPointGemassungskomponente.Y = at(0)->GetBemassungStartPoint().Y;
	}

	double dFaktor = CUtiltyHelper::GetFaktorForConnectionPoint( StartPointDoubleRohr, DeltaPitchRohr,
		CPointDouble(StartPointGemassungskomponente.X, StartPointGemassungskomponente.Y), DeltaPitchRohrQuare);

	StartPoint.X = StartPointDoubleRohr.X + dFaktor * DeltaPitchRohr.X;
	StartPoint.Y = StartPointDoubleRohr.Y + dFaktor * DeltaPitchRohr.Y;

	
	EndPointGemassungskomponente.X = at(size()-1)->GetBemassungEndPoint().X;
	EndPointGemassungskomponente.Y = at(size()-1)->GetBemassungEndPoint().Y;

	 dFaktor = CUtiltyHelper::GetFaktorForConnectionPoint( StartPointDoubleRohr, DeltaPitchRohr,
		CPointDouble(EndPointGemassungskomponente.X, EndPointGemassungskomponente.Y), DeltaPitchRohrQuare);

	EndPoint.X = StartPointDoubleRohr.X + dFaktor * DeltaPitchRohr.X;
	EndPoint.Y = StartPointDoubleRohr.Y + dFaktor * DeltaPitchRohr.Y;
	
	ZeichneLinie( StartPoint.X, StartPoint.Y, EndPoint.X, EndPoint.Y, _T("rgb(142, 142, 142)") );
}






void CGraficRohrBemassungsRohrKomponentenVector::ZeichneLinie( double x1, double y1, double x2, double y2, CString strFarbe )
{ 
			CString strTemp;
			strTemp.Format(_T("<line  x1='%f' y1='%f' x2='%f'  y2='%f'   style='stroke:%s;stroke-width:2'/>"), x1, y1, x2, y2, strFarbe);
		m_pGraficGraficController->m_GraficFileController.strSVGContent += strTemp;	
}



double CGraficRohrBemassungsRohrKomponentenVector::GetWinkelRohrZuXundYAchse()
{	
	double dWinkel;
	if(at(0)->GetLogicalRohrKomponente()->GetFirstKomponent())
	{
		dWinkel = 180;
	}
	else
	{
		dWinkel = 0;
	}
	return at(0)->GetWinkelRohrZuXundYAchse() + dWinkel;
}
