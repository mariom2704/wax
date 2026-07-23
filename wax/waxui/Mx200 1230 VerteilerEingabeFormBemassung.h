
#pragma once
#include "GridCtrl\WaxGridCtrl.h"
#include "StretchBitmapControl\StretchBitmapControl.h"
#include "CO II VerteilerEingabeFormBemassung.h"




class WAXUIDLL CMx2001230VerteilerEingabeBemassung : 
public COIIVerteilerBemassung  
{



	DECLARE_DYNCREATE(CMx2001230VerteilerEingabeBemassung)

public:
	//Konstruktor
	CMx2001230VerteilerEingabeBemassung();

	//Destruktor
	virtual ~CMx2001230VerteilerEingabeBemassung();
	virtual COIIVerteilerData* getRohrData();
	virtual void InitAnfangEndeCheckBoxen(int iDn);
	BOOL OnInitDialog();
	void SelChangeRohrDN();
	DECLARE_MESSAGE_MAP()

private:
	//CMx2001230VerteilerData *rohr;
};
