#ifndef __VERTEILER_DLL__
#define __VERTEILER_DLL__

//#ifndef __AFXEXT_H__
//#include <afxext.h>
//#endif



#ifdef _WAXUIDLL 
#define WAXUIDLL __declspec( dllexport )
#else
#define WAXUIDLL __declspec( dllimport )
#endif

#include "..\waxprj\waxprj_dll.h"


#include "GewinderohrData.h"
#include "GewinderohrEingabe.h"
#include "GRohranlageEingabe.h"

#include "VerteilerDaten.h"
#include "VerteilerEingabe.h"
#include "ElementeEingabe.h"
#include "AVElementEingabe.h"

#include "HandelslaengeEingabe.h"
#include "HandelslaengeData.h"

#include "StrangrohrData.h"
#include "StrangrohrEingabe.h"
#include "MuffenEingabe.h"

#include "CO II Verteilereingabe.h"
#include "CO II VerteilerData.h"
#include "Mx200 1230 VerteilerData.h"

#include "StationsverteilerData.h"
#include "PumpenverteilerData.h"
#include "PumpenprobierleitungData.h"
#include "EntnahmeleitungData.h"
#include "FormstueckeUndFittingeDaten.h"

#include "Apax.h"
#include "ApaxPosition.h"

/*
#include "FormstueckeUndFittingeDaten.h"
#include "FormstueckeUndFittingeEingabe.h"

#include "StationsverteilerData.h"
#include "StationsverteilerEingabe.h"
#include "StationsverteilerBemassung.h"

#include "EntnahmeleitungData.h"
#include "EntnahmeleitungEingabe.h"

#include "PumpenprobierleitungData.h"
#include "PumpenprobierleitungEingabe.h"

#include "PumpenverteilerData.h"
#include "PumpenverteilerEingabe.h"

#include "ApaxEingabeForm.h"

#include "Apax.h"
*/
#endif