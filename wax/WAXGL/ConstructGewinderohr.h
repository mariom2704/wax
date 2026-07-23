#pragma once

#include "ConstructGraphik.h"

class ConstructGewinderohr : public ConstructGraphik
{
public:
	///  Konstruktor
	ConstructGewinderohr(Rohrleitung* rohr, WaxGrafik::GRAFIKTYP typ, const std::string& fn = ""); 

	/// Einstiegsfunktion zur Graphik
	virtual void createGraphik();

	/// Graphikfaktoren setzen
	virtual void setGraphikfaktoren();

	/// Die Elementliste wird abgearbeitet
	virtual void runElementList();

	/// Die Konstruktionselemente der Rohrstuecke werden erzeugt
	virtual void createRohrList(int rohrLaenge, int rohrDn, CString rohrPos, int rohrAnzahl, int einz, const bool erstesElement);

	virtual void setNewPoint();

private:
	/// Abstand fuer neuen Rohranfangspunkt
	double abstand;

	void setPointBehindWinkel();

	void setPointBehindFitting();
};

