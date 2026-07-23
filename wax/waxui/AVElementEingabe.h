#pragma once

#include "../waxprj/ElementeEingabeForm.h"

/**
Generischer Eingabereiter für AV Elemente. Wird benutzt in Stationsverteiler und Strangrohr
*/
class WAXUIDLL AVElementEingabe : public ElementeEingabeForm  
{
	DECLARE_DYNCREATE(AVElementEingabe)
	DECLARE_MESSAGE_MAP()
public:
	AVElementEingabe();

	virtual ~AVElementEingabe();

	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	virtual BOOL OnInitDialog();

private:
		/// Pointer auf (AV)ElementArray
	virtual EnContainerRohrElemente* eleArray()
	{
		static EnContainerRohrElemente fallback;
		WaxPart* pt = dynamic_cast<WaxPart*>(get_data());
		XASSERT(pt);
		if (!pt) 
			return &fallback;
		
		Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
		return rohr ? &rohr->get_avelementArray() : &fallback;
	}

	virtual void DoDataExchange(CDataExchange* pDX);

	virtual void elem_setFields();

	virtual void show_elements(int select);

	/// Nacharbeit per Zeichnung/Fax
	void on_clicked_nacharbeit();

	/// Dialogelemente (ausser Nacharbeit) deaktivieren/aktivieren
	void enable_or_disable_dlgitems(bool enable);

	void OnSize(UINT nType, int cx, int cy);

	/// AV Nachbearbeitung per Zeichnung
	CButton zeichnung;
};
