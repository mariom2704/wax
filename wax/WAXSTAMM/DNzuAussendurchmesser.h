#pragma once

#include "StammDat.h"

class WAXSTAMMDLL DNzuAussendurchmesser : public StammDat  
{
public:
	
	DNzuAussendurchmesser();

	virtual void load(const std::string& db);

	int getAussendurchmesserToInt(int dn);
	int getRadiusToInt(int dn);

	double getAussendurchmesser(int dn);
	
	class WAXSTAMMDLL data : public CObject  
	{
	public:
		data();

		 int get_dn() const;

		 double get_aussendurchmesser() const;

	private:
		int dn;

		double aussendurchmesser;

	friend class DNzuAussendurchmesser;
	};

	

};
