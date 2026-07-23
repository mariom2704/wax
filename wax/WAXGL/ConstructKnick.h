#pragma once
#include "ConstructAbgEle.h"

class ConstructKnick : public ConstructAbgEle  
{
public:
	ConstructKnick(void);
	virtual ~ConstructKnick(void);

	virtual void createBeschriftung();

	virtual void createElement();
};
