#pragma once

#include "cadmodell_vector.h"
#define _USE_MATH_DEFINES  // für M_PI
#include <math.h>

namespace CadModell
{

	/**
	Einheitsvektor
	*/
	class UnitVector : public CadModell::Vector
	{
		double tolV; //Double Vergleiche
		double tolWinkelInGrad; //Winkelvergleiche 

	public:
		UnitVector(CadModell::Point rb = CadModell::Point()) : Vector(CadModell::Point(), rb)
		{
			tolV = 0.001;

			//Wichtig: Die Toleranz in WaxPlausi GpCheckBauteil.checkAnschluesse() muß diesen Wert entsprechen
			tolWinkelInGrad = 3.5; //3.5° Grad  
		}

		UnitVector reverse() const
		{
			return UnitVector(CadModell::Point(B.x * -1, B.y*-1, B.z*-1));
		}


		bool compareDirection(const UnitVector& other) const
		{
			double winkel = angleTo(other);

			//Winkel in Grad
			winkel *= 180.0 / M_PI;

			if (winkel > tolWinkelInGrad)
				return false;


			return true;
		}


		bool compare(const UnitVector& other) const
		{
			if (compareDouble(B.x, other.B.x) &&
				compareDouble(B.y, other.B.y) &&
				compareDouble(B.z, other.B.z))
				return true;

			//return B.x == other.B.x && B.y == other.B.y && B.z == other.B.z;
			return false;
		}


		

		UnitVector operator - (const UnitVector& other) const
		{
			return UnitVector(Point(B.x-other.B.x, B.y-other.B.y, B.z-other.B.z));
		}



		/**
		 * Winkel von this nach Vector b im Bereich 0 - Pi (0° - 180°)
		 * wird über das Skalarprodukt berechnet
		 */
		double angleTo(const UnitVector& b) const
		{
			double acosi = 0;

			double laenge = len() * b.len();
			if (compareDouble(laenge, 0))
				return 0;


			double aTb = B.x * b.B.x + B.y * b.B.y + B.z * b.B.z;
			acosi = aTb / laenge;

			//runden
			if (acosi > 1 && acosi < 1 + tolV)
				acosi = 1;

			if (acosi < -1 && acosi > (-1 - tolV))
				acosi = -1;

			double winkel = acos(acosi);
				
			

			return winkel;
		}


#if 0
		int get_angle_yz() const
		{
			/**
			Liefert den Winkel in Y/X 2D Koordinaten zum Normalwinkel
			*/
			return calc_angle(B.z,B.y);
		}

		int get_z_angle(const UnitVector& ref) const
		{
			int ret = 0;
			if (ref.B.x > 0 )
			{
				ret = calc_angle(B.z,B.y);
			}
			if (ref.B.x < 0 )
			{
				UnitVector left = reverse();
				ret = calc_angle(left.B.z, left.B.y);
			}
			else if (ref.B.y > 0)
			{
				UnitVector up = reverse();
				ret = calc_angle(up.B.z, up.B.x);
			}
			else if (ref.B.y < 0)
			{
				ret = calc_angle(B.z, B.x);
			}
			return ret; // Fehler: Winkelberechnung nicht möglich
		}


		void rotate_yz(int winkel)
		{
			TRACE("Winkel vorher: %i ", get_angle_yz());
			double piw = (double)winkel / 360.0 * 2 * M_PI;
			B.z = int((double)B.z * cos(piw) - (double)B.y * sin(piw));
			B.y = int((double)B.z * sin(piw) + (double)B.y * cos(piw));
			TRACE("Winkel drehen: %i Winkel gedreht:%i \n", winkel, get_angle_yz());
		}
#endif
		double cosphi(const UnitVector& other) const
		{
			double lenprodukt = len() *  other.len();

			if (lenprodukt < 0.01)
				return 0; // Nicht berechenbar

			double skalar = B.x*other.B.x + B.y*other.B.y + B.z* other.B.z;
			double cosphival = (double)skalar / lenprodukt;
			return cosphival;
		}
	private:
		int calc_angle(int a, int b) const
		{
			/**
			Winkelberechnung mit Delta für Abgänge. 90° geht nach hinten, nach vorne 
			sind es 270°
			*/
			double phi = atan2( (double)a , (double)b );
			phi *= 180.0 / M_PI;
			return int(180.0-phi) % 360;
		}

		UnitVector kreuzprodukt(const UnitVector& other) const
		{
			/// siehe http://de.wikipedia.org/wiki/Kreuzprodukt

			return UnitVector(Point(B.y*other.B.z - B.z*other.B.y, B.z*other.B.x - B.x*other.B.z, B.x*other.B.y - B.y*other.B.x));
		}

		bool compareDouble(const double &d1, const double &d2) const
		{
			if (fabs(d1-d2) < tolV)
				return true;

			return false;
		}
	
	};

}