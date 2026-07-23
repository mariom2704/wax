#pragma once


#include <afxtempl.h>

class Rohrstuecke : public CObject  
{
private:
	CString beschriftung;

	CString posnr;

	int uposnr;

	int lfdnr;

	int eleanz;

	int anfang;

	int ende;

	int dn;

	int artikelnr;

	int laenge;

	int anz;

	int restlaenge;

public:
	CArray<int,int> eleabstand;
	
	CArray<int,int> eledn;

	CArray<int,int>  elelaenge;

	CArray<int,int> eleid;

	Rohrstuecke();
	
	virtual ~Rohrstuecke();

	void initDaten(CString posnr, int lfdnr, int anz, int anfang, int ende, int laenge, int dn, CString beschriftung);

	const int get_elelaenge(int index) const;

	void set_elelaenge(int index, int value);

	const int get_uposnr() const;

	void set_uposnr(int value);

	const int get_eleanz() const;

	void set_eleanz(int value);

	const CString& get_beschriftung() const;

	void set_beschriftung(const CString& value);

	const int get_anfang() const;

	void set_anfang(int value);

	const int get_ende() const;

	void set_ende(int value);

	const int get_eleabstand(int index) const;

	void set_eleabstand(int index, int value);

	const int get_eledn(int index) const;

	void set_eledn(int index, int value);

	const int get_eleid(int index) const;

	void set_eleid(int index, int value);

	const int get_dn() const;

	void set_dn(int value);

	const int get_lfdnr() const;

	void set_lfdnr(int value);

	const CString& get_posnr() const;

	void set_posnr(const CString& value);

	void set_artikelnr(int value);
	
	const int get_artikelnr() const;

	void set_laenge(int value);
	
	const int get_laenge() const;

	void set_anz(int value);
	
	const int get_anz() const;

	void set_restlaenge(int value);

	const int get_restlaenge() const;

};
