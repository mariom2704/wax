// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_PREISE_3D352E6D0207_INCLUDED
#define _INC_PREISE_3D352E6D0207_INCLUDED

#include "Ausgaben.h"

class WAXPRJDLL Preise : public Ausgaben  
{
public:
	Preise(Werkauftrag *wa);

	// Destruktor
	virtual ~Preise();
	
	// Die verschiedenen Preisausgaben
	enum PREISAUSGABEN 
	{
		PREIS = 1,

		VK3_PREISINFO = 2,

		VK3_KOMPLETTLISTE = 3,
	
		VK3_ELEMENTLISTE = 4,
	
		ZINKKOSTEN = 5,
	};

	// get werkauftrag
	virtual Werkauftrag* get_werkauftrag(); 
	
	// Gewuenschte Ausgabe ausfuehren
	const CStringArray& makeAusgabe(int id  );

	// Liste mit Ausgaben-IDs (ausgaben) und Ausgabennamen initialisieren
	virtual void initAusgaben();

	// Liefert den Namen (fuer Menueeintrag) fuer eine Ausgabe.
	 CString getName(int ausgabe_id    ) const;

	//Kopiert die Liste mit (Unter)Aufträgen
	void set_walist(CObList& value);

	// Setzt Unterobjekte in Liste
	void set_wasublist(CObList& value);

	// Setzt Auftraege in Liste
	void set_walist();

	// Liefert Liste mit Gliederungen
	const CObList& get_gliederunglist() const;

private:
	
	double sonderkosten;				
		
	double geldwert;					
	
	double preisVK3;

	double gesamtpreisVK3;

	// Liste mit Gliederungen
	CObList gliederunglist;

	// Liste mit Unterobjekten
	CObList wasublist;

	bool isEleNull();


	void createKomplettliste();

	//currentAusgabe, ID der zu erzeugenden Ausgabe
	int currentAusgabe;

	void createVK3Abrechnungdata();

protected:
	//Durchläuft die walist und errechnet die Gesamtgewichte und die maximale 
	//Länge
	virtual void setMaxLaengeUndGewicht(bool mat = true, bool stueck = true);

	// resetAttributes, setze die Klassenattribute auf 0
	virtual void resetAttributes();

};

#endif /* _INC_PREISE_3D352E6D0207_INCLUDED */
