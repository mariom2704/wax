#pragma once

#include "ConstructElement.h"

class WAXGLDLL ConstructEndEle : public ConstructElement 
  
{
public:
	virtual void initEleDaten(const Element& ele);

	virtual ~ConstructEndEle();

	ConstructEndEle();

	virtual void createBemassung();

	virtual void createBeschriftung();

	virtual void createElement();

};
