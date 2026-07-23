#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PUMPENVERTEILEREINGABE_3E365E0002B8_INCLUDED
#define _INC_PUMPENVERTEILEREINGABE_3E365E0002B8_INCLUDED

#include "RadioButtonData.h"

/**
Eingabemaske für Pumpenverteiler
*/
class WAXUIDLL PumpenverteilerEingabe : public WaxPartForm  
{
	DECLARE_DYNCREATE(PumpenverteilerEingabe)
public:
	PumpenverteilerEingabe();

	virtual ~PumpenverteilerEingabe();

	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	RadioButtonData radiobuttons;
};

#endif /* _INC_PUMPENVERTEILEREINGABE_3E365E0002B8_INCLUDED */
