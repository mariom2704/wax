#pragma once

namespace CadModell
{
	class Point
	{

	public:
		double x;
		double y;
		double z;


		Point(double px = 0, double py = 0, double pz = 0)
		{
			x = px;
			y = py;
			z = pz;

			tol = 1;
		}

		bool is_null() const
		{
			return x == 0 && y == 0 && z == 0;
		}

		bool operator == (const Point& other) const
		{
			if (compareDouble(x, other.x) &&
				compareDouble(y, other.y) &&
				compareDouble(z, other.z))
				return true;

			//return x == other.x && y == other.y && z == other.z;
			return false;
		}
		
		bool operator != (const Point& other) const
		{
			if (*this == other)
				return false;

			return true;

			//return x != other.x || y != other.y || z != other.z;
		}

	private:
		double tol;

		bool compareDouble(const double &d1, const double &d2) const
		{
			if (fabs(d1-d2) < tol)
				return true;

			return false;
		}

	};
}