// Stationsverteiler_dll.h: Schnittstelle für die Klasse StationsverteilerData.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SONDERMZ_DLL_H__DCB5131D_6171_11D4_972C_0050DA35C3DB__INCLUDED_)
#define AFX_SONDERMZ_DLL_H__DCB5131D_6171_11D4_972C_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef DLLINTERFACE
#undef DLLINTERFACE
#endif


#ifdef _WAXUIDLL 
#define DLLINTERFACE __declspec( dllexport )
#else
#define DLLINTERFACE __declspec( dllimport )
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

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


#endif // !defined(AFX_SONDERMZ_DLL_H__DCB5131D_6171_11D4_972C_0050DA35C3DB__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////
// Formularansicht StatVert_Obfl 
