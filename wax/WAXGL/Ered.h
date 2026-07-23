#pragma once

#include "ConstructRohrEle.h"

namespace WaxGrafik
{
	class ERed : public ConstructRohrEle  
	{
	public:
		ERed() {};

		virtual void createBeschriftung();

		virtual void createElement();
	private:
		/// Startpunkte errechnen
		void get_start_coords(Point& p1, Point& p2);
		
		/// Horizontale Reduzierung
		void get_horizontal_coords(bool left_to_right, Point& p1to, Point& p2to, Point& p3from, Point& p3to);

		/// Vertikale Reduzierung
		void get_vertical_coords(bool up, Point& p1to, Point& p2to, Point& p3from, Point& p3to);

		/// Errechnet aus dn und andn und dem Winkel des Elements die Differenz der DN
		void get_delta_dn(bool swap, int& deltaDN1, int& deltaDN2) const;
	};
}
