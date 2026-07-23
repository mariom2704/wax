#pragma once
#include "afx.h"
#include "mxbase.h"

class MxDatenZeile :	public MxBase
{
public:
	MxDatenZeile(void);
	~MxDatenZeile(void);

	void addWert(const CString &val);
	bool getWert(int nr,CString &val);

	bool getWertAsDouble(int nr, double &val);
	bool getWertAsInt(int nr, int &val);
	bool getWertAsBool(int nr, bool &val);


	int getAnzahlSpalten();

	void removeAll();


	MxDatenZeile& operator=(const MxDatenZeile& zeile);

private:
	CStringArray m_zeile;
};
