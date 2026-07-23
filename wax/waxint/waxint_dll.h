#pragma once



#ifdef _WAXINT 
#define WAXINT __declspec( dllexport )
#else
#define WAXINT __declspec( dllimport )
#endif

#include "..\waxui\waxui_dll.h"

#include "Sap.h"
#include "SapfileOpen.h"
#include "SapSearch.h"
#include "Excel.h"
#include "DialogExcel.h"
#include "Monitor.h"
#include "MonitorOpen.h"
#include "Rohrstuecke.h"
#include "ServerDBOpen.h"
#include "SpanxServer.h"

