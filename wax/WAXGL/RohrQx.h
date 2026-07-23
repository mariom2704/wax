#pragma once

#include "ConstructRohrEle.h"

namespace WaxGrafik
{
	class RohrQx : public ConstructRohrEle  
	{
	public:		
		enum Einziehung
		{
			// gibt an, ob das Rohr mit Einziehung gezeichnet werden soll:
			OHNE_EINZIEHUNG = 0, // ohne Einziehung
			ANFANG = 1, // 1 = Einziehung am Anfang
			ENDE = 1, // 2 = Einziehung am Ende
			ANFANG_ENDE, // 3 = Einziehung am Anfang und Ende
		};

		RohrQx();

		void init_rohrqx(enum Einziehung einziehung, int count = 0);

		virtual void createBemassung();

		virtual void createBeschriftung();

		virtual void createElement();

		void set_show_dn(int value);

		void set_show_text(bool val = true)
		{
			show_text = val;
		}
	
	private:

		/// Zähler für die Rohrteilstücke, die jeweils neu gezeichnet werden müssen
		int rohrcounter;
	
		enum Einziehung einz;

		bool show_text;
			
		int show_dn;

		bool show_laenge;
	};
}