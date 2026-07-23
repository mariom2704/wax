#pragma once

#include "ConstructAbgEle.h"

class ConstructCOBockWinkel : public ConstructAbgEle  
{
private:

public:
	virtual ~ConstructCOBockWinkel();

	ConstructCOBockWinkel();

	virtual void createBeschriftung();
	virtual void createBemassung();
	virtual void createElement();
	void DrawRohrLaenge();

protected:

};
