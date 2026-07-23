#pragma once

#include "Rohrleitung.h"


/**
Definition der Handelslängen
Handelslängen sind Einfach-Rohre mit oder ohne Nut
*/
class ROHRKONSTDLL Handelslaenge : public Rohrleitung  
{
public:
	//Konstruktor
	Handelslaenge();

	//Destruktor
	virtual ~Handelslaenge();

	//Speichern und Laden der Rohrleitung. Achtung:
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	//Rohrkonstruktion erzeugen
	virtual void createModell();

	//Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	//Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	/// Gesamtgewicht der Rohrleitung berechnen
	virtual double getGesGewicht(int teile = 0) const;

	/// Wird eigentlich nur für die Handelslängenkennung beim CAD Werkauftrag benötigt. Sollte hier aber nicht false liefern.
	virtual bool isHandelslaenge() const { return true; }

	virtual const CString getBemassungstext() const;

	///vk3 Liste erstellen
	virtual bool createVk3Liste();

protected:
	//Berechnungs des Aufschlags fuer Rohrverschnitt
	virtual void addRohrausschuss(CObList& list);

	virtual void check_elementArray();

	///Rohrquali auf neuen Standard aendern
	virtual void switchRohrquali();
};


