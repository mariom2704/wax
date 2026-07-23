#ifndef __PROPPAGEGEANSICHT_H__
#define __PROPPAGEGEANSICHT_H__

#include "waxproppage.h"

/// PropPage Dialogfeld
class PropPageAnsicht : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstructor
	PropPageAnsicht(MxParameter *para);

	/// Destructor
	~PropPageAnsicht();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung

	///  Generierte Nachrichtenzuordnungsfunktionen
	virtual BOOL OnInitDialog();

	 void onParaBaumfarbe();
	
	/// Default-Werte setzen
	virtual void set_default();

	 BOOL OnEraseBkgnd(CDC* pDC);

private:

	/// Vorschau der Baumfarbe updaten
	void update_baumfarbe();

	/// Icongroesse
	enum GROESSE 
	{  
		/// klein
		KLEIN = 0,	

		/// mittel
		MITTEL = 1,

		/// gross
		GROSS = 2,	
	};	

	/// Symbolleite
	bool paraSymbolleiste;
	
	/// Statusleiste
	bool paraStatusleiste;
	
	/// Graphikfenster unter der Eingabe
	bool paraGraphikfenster;
	
	/// Freigegebene Positionen im Baum zeigen
	bool paraShowfreigabepos;

	/// Icongroesse im Baum
	int paraIcongroesse;

	/// Hintergrundfarbe des Baumes
	COLORREF paraBaumfarbe;

	
};

#endif ///  __PROPPAGEGEANSICHT_H__

