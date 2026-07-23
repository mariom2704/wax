#include "stdafx.h"


#include "ConstructCOVflanWiththread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ConstructCOVflanWiththread::~ConstructCOVflanWiththread()
{}



ConstructCOVflanWiththread::ConstructCOVflanWiththread()
{}



void ConstructCOVflanWiththread::createElement()
{
	

	calculateDimisions();
	int YHeight = (int)createFlansch();
	createStutzen(YHeight);
}

