#pragma once

#include "StammDat.h"

class WAXSTAMMDLL PutzenZeiten : public StammDat  
{
public:
	PutzenZeiten();

	virtual void load(const std::string& db);

	double getZeit(int id, int dn); 

	class WAXSTAMMDLL data : public CObject  
	{
	public:
		data();

		const int get_elemid() const;

		const int get_dn() const;

		const double get_zeit() const;

	private:
		int elemid;

		int dn;

		double zeit;

	friend class PutzenZeiten;
	};
};

