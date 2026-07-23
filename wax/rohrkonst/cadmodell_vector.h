#pragma once

#include <math.h>
#include "cadmodell_point.h"


namespace CadModell
{
	class Vector
	{
	public:
		Vector(CadModell::Point a, CadModell::Point b)
		{
			A = a;
			B = b;
		}
		CadModell::Point A;
		CadModell::Point B;

		double len() const
		{
			return sqrt(pow(double(B.x-A.x),2.0) + pow(double(B.y-A.y),2.0) + pow(double(B.z-A.z),2.0));
		}
		
		/*
		double len1() const
		{
			return sqrt(pow(double(B.x-A.x),2.0) + pow(double(B.y-A.y),2.0) + pow(double(B.z-A.z),2.0));
		}
		*/
		
		bool operator > (const Vector& other) const
		{
			return len() > other.len();
		}
		
		Vector& operator - (const Vector& other)
		{
			A.x -= other.A.x;
			A.y -= other.A.y;
			A.z -= other.A.z;
			return *this;
		}
		
		Vector& operator + (const Vector& other)
		{
			A.x += other.A.x;
			A.y += other.A.y;
			A.z += other.A.z;
			return *this;
		}

		double skalarprodukt(const Vector other) const
		{
			return (B.x-A.x) * (other.B.x-other.A.x) + (B.y-A.y) * (other.B.y-other.A.y) + (B.z-A.z) * (other.B.z-other.A.z);
		}


	
	};
}