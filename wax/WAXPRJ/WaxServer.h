#if !defined(AFX_WAXSERVER_DLL_H__BF95811F_5E33_11D4_972B_0050DA35C3DB__INCLUDED_)
#define AFX_WAXSERVER_DLL_H__BF95811F_5E33_11D4_972B_0050DA35C3DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#undef DLLINTERFACE

#ifndef DLLINTERFACE
#ifdef _WAXPRJDLL 
#define DLLINTERFACE __declspec( dllexport )
#include "resource.h"
#else
#define DLLINTERFACE __declspec( dllimport )
#define IDD_LEITKARTE 0
#define IDD_GLIEDERUNG 0
#define IDD_PROJEKTLEITKARTE 0
#define IDD_WAXPARTFORM 0
#define IDD_STATUS 0
#endif
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxmt.h>

#include "Server.h"
#include "ServerDBOpen.h"
#include "ServerDBNewWa.h"
#include "ServerDBMyWa.h"
#include "ServerDBPSPElement.h"
#include "Freigabe.h"

#endif // AFX_WAXSERVER_DLL_H__BF95811F_5E33_11D4_972B_0050DA35C3DB__INCLUDED_