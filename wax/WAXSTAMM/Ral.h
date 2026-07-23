#pragma once

#include "StammDat.h"



class WAXSTAMMDLL Ral : public StammDat  
  
{
public:
	Ral();

	virtual void load(const std::string& db);

	CString getBezeichnung(int id);

	class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		data();

		 int get_id() const;

		const CString& get_bezeichnung() const;

	private:
		friend class Ral;

		int id;

		CString bezeichnung;
	};

};
