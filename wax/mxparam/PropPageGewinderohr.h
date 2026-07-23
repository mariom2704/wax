#pragma once

#include "waxproppage.h"
#include <string>

/// PropPage Dialogfeld
class PropPageGewinderohr : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()

public:
	/// Konstructor
	PropPageGewinderohr(MxParameter *para);

	/// Destructor
	~PropPageGewinderohr();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung

	virtual BOOL OnInitDialog();

	/// Default-Werte setzen
	virtual void set_default();

	/// Beispieltexte setzen für Kurztext, wenn Trennzeichen geändert wurde
	void update_examples();

	/// einen Beispieltext setzen
	void set_example(std::string newchar, UINT idc, string ids);
private:
	/// KURZTEXTFORMAT
	enum KURZTEXT 
	{  
		/// kurz
		KURZ = 0,	

		/// mittel
		MITTEL = 1,

		/// lang
		LANG = 2,	
	};	

	/// Format des Kurztextes der Fittinge 
	int paraTextformat;
	
	/// Schaltet alle Nennweitenangabe fuer Gewinderohr um
	bool paraDN2Zoll;
	
	/// Bestimmt die Ausgabe der Winkelangabe in der Gewinderohrliste 
	bool paraWinkelAusrichtung;

	/// Abzug der ZMasse bei den Laengenangaben der Rohrstuecke in der Gewinderohrliste
	bool paraZMassListe;

	/// Abzug der ZMasse bei den Laengenangaben der Rohrstuecke in der Gewinderohrgraphik
	bool paraZMassGraphik;

	
};

