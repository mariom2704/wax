#pragma once

#include "ConstructFitting.h"

class ConstructKStueck : public ConstructFitting  
  
{
public:
	ConstructKStueck();

	virtual ~ConstructKStueck();

	virtual void createBeschriftung();

	virtual void createElement();

};
