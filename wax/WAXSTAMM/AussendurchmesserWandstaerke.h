#pragma once

#include "StammDat.h"

class WAXSTAMMDLL AussendurchmesserWandstaerke : 
public StammDat  
  
{
public:
	class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		const double get_innendurchmesser() const;

		data();

		 int get_elemid() const;

		 int get_dn() const;

		 double get_aussendurchmesser() const;

		 double get_wandstaerke() const;

	private:
		double innendurchmesser;

		int elemid;

		int dn;

		double aussendurchmesser;

		double wandstaerke;

	friend class AussendurchmesserWandstaerke;
	};


	virtual void load(const std::string& db);

	AussendurchmesserWandstaerke();
};


