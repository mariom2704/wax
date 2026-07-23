#pragma once

#include "ConstructElement.h"

class ConstructRohrEle : public ConstructElement 
  
{
public:
	ConstructRohrEle();

	virtual ~ConstructRohrEle();

	virtual void createBemassung();

	virtual void createBeschriftung() {};

	virtual void createElement(){};

};
