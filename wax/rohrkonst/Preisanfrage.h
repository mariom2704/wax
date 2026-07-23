#pragma once

#include "mxdate.h"

class ROHRKONSTDLL Preisanfrage : public CObject
{
public:
	Preisanfrage(void);
	virtual ~Preisanfrage(void);

	inline void set_nummer(int val);
	inline void set_position(int val);
	
	inline int get_nummer() const;
	inline int get_position() const;
	
	inline double get_preis() const;
	inline void set_preis(double val);

	inline void set_gueltigBis(const MXDate &date);
	inline MXDate get_gueltigBis()const;

	inline void set_info(const string& val);
	inline const string get_info()const;

	/// Serialisierung
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	void copy(const Preisanfrage& pa);

	void reset();

private:
	int preisanfrageNr;

	int preisanfragePos;

	double preis;

	string info;

	MXDate gueltigBis;
};



inline void Preisanfrage::set_nummer(int val)
{
	preisanfrageNr = val;
}

inline void Preisanfrage::set_position(int val)
{
	preisanfragePos = val;
}

	
inline int Preisanfrage::get_nummer() const
{
	return preisanfrageNr;
}

inline int Preisanfrage::get_position() const 
{
	return preisanfragePos;
}


inline double Preisanfrage::get_preis() const
{
	return preis;
}
	
inline void Preisanfrage::set_preis(double val)
{
	preis = val;
}


inline void Preisanfrage::set_gueltigBis(const MXDate &date)
{
	gueltigBis = date;
}
	
inline MXDate Preisanfrage::get_gueltigBis()const
{
	return gueltigBis;
}


void Preisanfrage::set_info(const string& val)
{
	info = val;
}
	

const string Preisanfrage::get_info()const
{
	return info;
}
