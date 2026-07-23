#pragma once

#include "StammDat.h"

class WAXSTAMMDLL Arbeitsplaetze : 
public StammDat 
  
{
public:
	class WAXSTAMMDLL data : 
public CObject 
	  
	{
	public:
		data();

		const CString& get_bezeichnung() const;

		const long get_kostenstelle() const;

		const CString get_nr() const;

		const double get_ruestzeit() const;

		const int get_typ() const;

	private:
		double ruestzeit;

		CString nr;

		long kostenstelle;

		CString bezeichnung;

		int typ;

	friend class Arbeitsplaetze;
	};


	virtual void load(const std::string& db);

	Arbeitsplaetze();

public:
	double getRuestzeit(CString nr);

	long getKostenstelle(CString nr);

	int getTyp(CString nr);

	CString getBezeichnung(CString nr);

};
