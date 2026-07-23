#pragma once

#include "RadioButtonData.h"

/**
Eingabemaske für Pumpenprobierleitungen
*/
class WAXUIDLL PumpenprobierleitungEingabe : public WaxPartForm  
{
	DECLARE_DYNCREATE(PumpenprobierleitungEingabe)
	DECLARE_MESSAGE_MAP()
public:
	PumpenprobierleitungEingabe();

	virtual ~PumpenprobierleitungEingabe();

	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const ;

	void onKillfocusDn();

	virtual BOOL OnInitDialog();

private:
	//Auswertung der Radio Buttons mit den Elementids der Abgänge
	RadioButtonData radiobuttons;
};
