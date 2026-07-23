#pragma once



#ifdef _WAXGLDLL 
#define WAXGLDLL __declspec( dllexport )
#else
#define WAXGLDLL __declspec( dllimport )
#endif



#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef WHIP_STATIC_LIB
#define WHIP_STATIC_LIB
#endif

#include "..\Rohrkonst\rohrkonst_dll.h"

#include <afxtempl.h> // für CArray

#include "GlObj.h"
#include "Polygon.h"
#include "TextPlot.h"
#include "Kreis.h"
#include "Linie.h"
#include "Bemassung.h"
#include "ConstructGraphik.h"
#include "ConstructGewinderohr.h"




