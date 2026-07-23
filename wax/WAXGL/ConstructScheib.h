#pragma once

#include "ConstructEndEle.h"

class ConstructScheib : public ConstructEndEle  
{
private:

public:
	virtual ~ConstructScheib();

	ConstructScheib();

	virtual void createBeschriftung();

	virtual void createElement();

protected:

};

