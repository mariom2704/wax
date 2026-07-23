#pragma once

#include "ConstructEndEle.h"

class WAXGLDLL ConstructGewind : public ConstructEndEle  
{
private:

public:
	virtual ~ConstructGewind();

	ConstructGewind();

	virtual void createBeschriftung();

	virtual void createElement();
};
