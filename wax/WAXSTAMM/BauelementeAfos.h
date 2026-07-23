#pragma once

#include "StammDat.h"

class WAXSTAMMDLL BauelementeAfos : public StammDat  
{
public:
	
	BauelementeAfos();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		const int get_winkel() const;

		const int get_code4() const;

		const int get_dnvl_bis() const;

		const int get_dnvl_von() const;

		data();

		const int get_code1() const;

		const int get_code2() const;

		const int get_code3() const;

		const int get_dn() const;

		const int get_elemid() const;

		const int get_produktTyp() const;

		const int get_laenge_bis() const;

		const int get_laenge_von() const;

		const bool get_isAnlage() const;

		

	private:

		int produkttyp;

		int winkel;

		int code4;

		int dnvl_von;

		int dnvl_bis;

		int elemid;

		int code1;

		int code2;

		int code3;

		int dn;

		int laenge_von;

		int laenge_bis;

		bool isAnlage;

	friend class BauelementeAfos;
	};

};

