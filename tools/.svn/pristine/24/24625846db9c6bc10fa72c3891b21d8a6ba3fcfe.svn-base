#pragma once


class CPointDouble
{
	public:
		
		CPointDouble()
		{};

		CPointDouble(double x, double y)
		{ X = x; Y = y;	};

		
		double X;
		double Y;
};


class CUtiltyHelper
{
public:
	CUtiltyHelper(void);
	~CUtiltyHelper(void);
	static double GetFaktorForConnectionPoint(CPointDouble GeradePoint1x, CPointDouble dOffsetI,
									 CPointDouble GeradePoint2x, CPointDouble dOffsetII);

	static CPointDouble ChangeOppositeDirectionOfPoint(CPointDouble PointDouble);
};

