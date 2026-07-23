#pragma once

#include "ConstructElement.h"

class WAXGLDLL ConstructAbgEle : public ConstructElement  
  
{
private:
	bool avele;

	int abstKnickbem;

public:
	virtual void initEleDaten(const Element& ele);

	const bool get_avele() const;

	//Ist Element Nachbearbeit eingegeben, so ist dies eine Ebene höher als die 
	//Standardelemente zu bemassen.
	void set_avele(bool value);

	void createCOIIMx1230u200Bemassung(COIIVerteiler* pVerteilerCOII);


	void set_abstKnickbem(int val);
	

	virtual ~ConstructAbgEle();

	ConstructAbgEle();

	virtual void createBeschriftung();

	virtual void createElement();

	virtual void createBemassung();

};
