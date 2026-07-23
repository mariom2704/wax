#pragma once

#include "../rohrkonst/rohrleitung.h"

#include <string>

namespace WaxGrafik
{
	enum GRAFIKTYP
	{
		DWF,
		SVG,
	};

	class WAXGLDLL Creator
	{
	public:
		
		static std::string create(Rohrleitung* rohr,  const std::string& grafik_file = "");
		
		static string get_fileextension(enum GRAFIKTYP grafiktyp)
		{
			switch (grafiktyp)
			{
				case WaxGrafik::DWF: return "dwf";
				case WaxGrafik::SVG: return "svg";
			}
			return "tmp";
		}

	};
}
