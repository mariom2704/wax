#pragma once

#include "StammDat.h"

class WAXSTAMMDLL ProfileSaegezeiten : public StammDat
{
public:
	ProfileSaegezeiten();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data 
	: public CObject
	{
	public:
		const int get_code() const;

		const int get_ende() const;

		const int get_laengeBis() const;

		const int get_laengeVon() const;

		const double get_zeit() const;

		const double get_ruestzeit() const;

		const CString& get_typ() const;

	private:
		int code;

		CString typ;

		int ende;

		int laengeVon;

		int laengeBis;

		double ruestzeit;

		double zeit;

	friend class ProfileSaegezeiten;
	};
};
