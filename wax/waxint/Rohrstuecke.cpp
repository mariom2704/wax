#include "stdafx.h"


#include "Rohrstuecke.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Rohrstuecke::Rohrstuecke()
{
	artikelnr = 0;
	anz = 0;
	laenge = 0;
	restlaenge = 0;
	posnr = "";
	lfdnr = 0;
	dn = 0;
	anfang = 0;
	ende = 0;
	beschriftung = "";
	eleanz = 0;
	uposnr = 1;
}

Rohrstuecke::~Rohrstuecke()
{

}

void Rohrstuecke::initDaten(CString posnr, int lfdnr, int anz, int anfang, int ende, int laenge, int dn, CString beschriftung)
{
	this->posnr = posnr;
	this->lfdnr = lfdnr;
	this->anz = anz;
	this->anfang = anfang;
	this->ende = ende;
	this->laenge = laenge;
	this->dn = dn;
	this->beschriftung = beschriftung;
}

void Rohrstuecke::set_artikelnr(int value)
{
	artikelnr = value;
}

const int Rohrstuecke::get_artikelnr() const
{
	return artikelnr;
}

void Rohrstuecke::set_laenge(int value)
{
	laenge = value;
}

const int Rohrstuecke::get_laenge() const
{
	return laenge;
}

void Rohrstuecke::set_anz(int value)
{
	anz = value;
}

const int Rohrstuecke::get_anz() const
{
	return anz;
}

void Rohrstuecke::set_restlaenge(int value)
{
	restlaenge = value;
}

const int Rohrstuecke::get_restlaenge() const
{
	return restlaenge;
}

const int Rohrstuecke::get_eledn(int index) const
{
	return eledn[index];
}

void Rohrstuecke::set_eledn(int index, int value)
{
	eledn[index] = value;
}

const int Rohrstuecke::get_eleid(int index) const
{
	return eleid[index];
}

void Rohrstuecke::set_eleid(int index, int value)
{
	eleid[index] = value;
}

const int Rohrstuecke::get_eleabstand(int index) const
{
	return eleabstand[index];
}

void Rohrstuecke::set_eleabstand(int index, int value)
{
	eleabstand[index] = value;
}

const int  Rohrstuecke::get_elelaenge(int index) const
{
	return elelaenge[index];
}

void Rohrstuecke::set_elelaenge(int index, int value)
{
	elelaenge[index] = value;
}

const int Rohrstuecke::get_dn() const
{
	return dn;
}

void Rohrstuecke::set_dn(int value)
{
	dn = value;
}

const int Rohrstuecke::get_lfdnr() const
{
	return lfdnr;
}

void Rohrstuecke::set_lfdnr(int value)
{
	lfdnr = value;
}

const CString& Rohrstuecke::get_posnr() const
{
	return posnr;
}

void Rohrstuecke::set_posnr(const CString& value)
{
	posnr = value;
}

const int Rohrstuecke::get_uposnr() const
{
	return uposnr;
}

void Rohrstuecke::set_uposnr(const int value)
{
	uposnr = value;
}

const int Rohrstuecke::get_anfang() const
{
	return anfang;
}

void Rohrstuecke::set_anfang(int value)
{
	anfang = value;
}

const int Rohrstuecke::get_ende() const
{
	return ende;
}

void Rohrstuecke::set_ende(int value)
{
	ende = value;
}

const CString& Rohrstuecke::get_beschriftung() const
{
	return beschriftung;
}

void Rohrstuecke::set_beschriftung(const CString& value)
{
	beschriftung = value;
}

const int Rohrstuecke::get_eleanz() const
{
	return eleanz;
}

void Rohrstuecke::set_eleanz(int value)
{
	eleanz = value;
}
