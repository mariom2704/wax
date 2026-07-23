#pragma once

#include "StammDat.h"

class WAXSTAMMDLL AbzugRadius : 
public StammDat  
  
{
public:
	AbzugRadius();

	int getRadius(int dn, int dnvl);

	int getAbzug(int dn, int dnvl);


	class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		data();

		int get_dn() const;

		int get_dnvl() const;

		int get_radius() const;

		int get_abzug() const;

	private:
		int dn;

		int dnvl;

		int radius;

		int abzug;

	friend class AbzugRadius;
	};

	virtual void load(const std::string& db);
};

