#pragma once

#include "cadmodell_vector.h"

namespace CadModell
{
	class TypRohr
	{
	public:
		TypRohr(int rdn = 0, __int64 rid = 0, CadModell::Point rp1 = CadModell::Point(), CadModell::Point rp2 = CadModell::Point()) 
		{
			dn = rdn;
			id = rid;
			p1 = rp1;
			p2 = rp2;
			rohrverlaengerung = 0;
		}
		int dn;
		__int64 id;
		CadModell::Point p1;
		CadModell::Point p2;

		double len() const
		{
			CadModell::Vector v(p1, p2);
			return v.len();
		}

		int lenAsInt() const
		{
			CadModell::Vector v(p1, p2);
			return (int)(v.len() + 0.5);
		}


	private:
		int rohrverlaengerung;
	};
}