#include "stdafx.h"
#include "UtiltyHelper.h"


CUtiltyHelper::CUtiltyHelper(void)
{
}

double  CUtiltyHelper::GetFaktorForConnectionPoint(CPointDouble GeradePointI,  CPointDouble dOffsetI,
									  CPointDouble GeradePointII, CPointDouble dOffsetII)
{
	
	
	if(dOffsetII.Y == 0)
	{
		dOffsetII.Y = 0.00001;
	}
	
	if(dOffsetII.X == 0)
	{
		dOffsetII.X = 0.00001;
	}
	  
	
	double dFaktorI  =  (- GeradePointII.X/dOffsetII.X + GeradePointI.X/dOffsetII.X + GeradePointII.Y/ dOffsetII.Y - GeradePointI.Y / dOffsetII.Y)
					/
					(dOffsetI.Y / dOffsetII.Y - dOffsetI.X / dOffsetII.X);

	return dFaktorI;
}






CUtiltyHelper::~CUtiltyHelper(void)
{
}
