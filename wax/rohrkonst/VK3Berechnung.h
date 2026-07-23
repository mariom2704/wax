#pragma once

class Werkauftrag;

#include <mxcontrols/binaryarchive.h>

class ROHRKONSTDLL VK3Berechnung 
{
public:
	const double get_preisPasslaenge() const;

	void set_preisPasslaenge(double value);

	void calc_preisPasslaenge();

	const double get_preisElelist() const;

	void set_preisElelist(double value);

	void calc_preisElelist();

	void serialize(MxFilesystem::BinaryArchive & ar);

	void copy(const VK3Berechnung& other);

	void set_werkauftrag(Werkauftrag* value);

	virtual ~VK3Berechnung();

	VK3Berechnung();

	const double get_preisPulvern() const;

	void set_preisPulvern(double value);

	const double get_preis() const;

	void set_preis(double value);

	void calc_preis();

	void calc_preisPulvern();

private:
	double preisPasslaenge;

	double preisElelist;

	//Pointer auf Werkauftrag
	Werkauftrag *werkauftrag;

	double preis;

	double preisPulvern;

};

