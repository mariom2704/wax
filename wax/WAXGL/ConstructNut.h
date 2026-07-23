#pragma once

#include "ConstructEndEle.h"

class ConstructNut : public ConstructEndEle  
{
private:

public:
	virtual ~ConstructNut();

	ConstructNut();

	virtual void createBeschriftung();

	virtual void createElement();

protected:

};
