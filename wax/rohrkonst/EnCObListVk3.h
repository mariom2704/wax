#pragma once

#include "Element.h"
#include "encoblistmodell.h"

class ROHRKONSTDLL EnCObListVk3 : public EnCObListModell
{

	public:
		
		enum ERohrType
		{
			eRohrTypeUknownRohr,
			eRohrTypeStrangRohr,
		};
	
	private:
		ERohrType Rortype;

	
	public:
		EnCObListVk3(void);
		virtual ~EnCObListVk3(void);
		
		bool copy( EnCObListVk3 &other);
		void SetRohrType(ERohrType Rohrtyping){Rortype = Rohrtyping;};
		ERohrType GetRohrType(){return Rortype;};
		//Vk3 Preise aus den Stammdaten lesen
		int setVk3Preise(bool verzinkt, int Rohlaenge, int AnzahlElemente, int ElementLaenge);

		//Liste kumulieren, gleiche Elemente zusammenfassen
		bool kumulieren();

		//Rohre auf ganze Meter aufrunden
		//bool rundenVollMeter();

		//Rohre auf Halbe Meter aufrunden
		//bool rundenHalbeMeter();

		//Menge/Anzahl setzen
		bool setMenge(int menge);

		double getGesamtpreis();
};
