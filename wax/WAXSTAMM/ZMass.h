#pragma once

#include "StammDat.h"
#include "FormstueckeUndFittinge.h"

class WAXSTAMMDLL ZMass : 
public StammDat 
  
{
public:
	class WAXSTAMMDLL data : 
public CObject 
	  
	{
	public:
		 int get_dn() const;

		 int get_dnvl() const;

		 int get_zmass_verteiler() const;

		 int get_zmass_fitting() const;

		 int get_zmass_muffe() const;

		data();

	private:
		int dn;

		int dnvl;

		int zmass_verteiler;

		int zmass_muffe;

		int zmass_fitting;

	friend class ZMass;
	};


	virtual ~ZMass();

	ZMass();

	virtual void load(const std::string& db);

public:
	//Gibt das ZMass je nach Gruppenid und DN und DNVL zurück
	int get_val(FormstueckeUndFittinge::GRUPPENID  id, int dn, int dnvl) const;

};

