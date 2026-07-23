#pragma once

#include "Werkauftrag.h"


class ROHRKONSTDLL ApaxWerkauftrag : public Werkauftrag 
{
public:
	ApaxWerkauftrag();

	virtual ~ApaxWerkauftrag();

	void set_vk3Preis(double val);
	double get_vk3Preis();

	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	void copy(ApaxWerkauftrag& other);

	void copy(const Werkauftrag& other);

	///vk3 Liste erstellen
	virtual bool createVk3Liste();

	virtual void createModell();

	virtual CString checkPlausis();

	virtual bool isEmpty() const;

	virtual double getGesGewicht(int teile = 0) const;

	virtual int getMaxLaenge()const;

	virtual bool isApax() const{ return true; }

private:
	double vk3Preis;
};

