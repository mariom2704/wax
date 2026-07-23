#pragma once

#include "StammDat.h"

//Rohrtexte für die Spanx/Wax Schnittstelle
class WAXSTAMMDLL RohrTexte : public StammDat
{
public:
	class WAXSTAMMDLL data 
	: public CObject
	{
	public:
		enum ROHRTYPEN 
		{
			STRANGROHR = 1,

			VERTEILER = 2,

			GEWINDEROHR = 3
		};

	
		const bool get_vortext() const;

		const bool get_verzinkt() const;

		//Liefert einen enum ROHRTYPEN
		const int get_rohrtyp() const;

		const CString& get_langtext() const;

		void set_langtext(const CString& value);

		void set_rohrtyp(int value);

		void set_verzinkt(bool value);

		void set_vortext(bool value);

	private:
		bool verzinkt;

		bool vortext;

		int rohrtyp;

		CString langtext;
	};


	virtual ~RohrTexte();

	RohrTexte();

	virtual void load(const std::string& db);

};


