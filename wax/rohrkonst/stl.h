#ifndef __STDSTL__
#define __STDSTL__

#ifdef _MSC_VER
// Fix broken STL implementation in MSVC 4.2
// Include files (for MSVC)
#pragma warning(disable: 4786)  // Kludge to get around bug in MSVC 4.2
#pragma warning(disable: 4788)  // See above
#endif

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;
#endif