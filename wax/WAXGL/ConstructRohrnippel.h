#pragma once

#include "ConstructAbgEle.h"

class ConstructRohrNippel : public ConstructRohr  
{
	private:
		CString strArtikelnummer; 

	public:
		virtual ~ConstructRohrNippel();
		ConstructRohrNippel();
		virtual void createBeschriftung();
		virtual void createElement();


	protected:
};
