#pragma once

#include "cadmodell_unitvector.h"

namespace CadModell
{
	class Anschluss
	{
	public:
		Anschluss(__int64 rid = 0, int rdn = 0, int rvbart = 0, int rverlaeng = 0, CadModell::Point rp = CadModell::Point(), CadModell::UnitVector rr = CadModell::UnitVector()) 
		{ 
			cadid = rid; 
			dn = rdn; 
			vbart = rvbart;
			p = rp;
			r = rr;
			rohrverlaengerung = rverlaeng;
		}

		/// Eindeutige CAD ID
		__int64 cadid;

		/// Durchmesser
		int dn;

		/// Verbindungsart
		int vbart;

		/// Rohrverlaengerung in mm. Negativ bei Schweissnaht, Positiv bei z.B. Flachflansch
		/// Positiv bei Fittingen (Einschraubtiefe)
		int rohrverlaengerung;

		/// Anschlusspunkt 
		CadModell::Point p;

		/// Anschlussrichtung
		CadModell::UnitVector r;
	};
}