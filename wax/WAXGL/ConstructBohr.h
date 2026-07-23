#pragma once

#include "ConstructAbgEle.h"

class WAXGLDLL ConstructBohr : public ConstructAbgEle  
{
public:
	ConstructBohr();
	
	virtual ~ConstructBohr();

	virtual void createBeschriftung();

	virtual void createElement();
};

