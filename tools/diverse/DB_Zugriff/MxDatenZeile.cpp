#include "StdAfx.h"
#include ".\MxDatenZeile.h"

MxDatenZeile::MxDatenZeile(void)
{
}

MxDatenZeile::~MxDatenZeile(void)
{
}


void MxDatenZeile::addWert(const CString &val)
{
	m_zeile.Add(val);
}

bool MxDatenZeile::getWert(int nr,CString &val)
{
	if (nr >= m_zeile.GetSize() || nr < 0)
	{
		val.Empty();
		return false;
	}

	val = m_zeile[nr];
	return true;
}


bool MxDatenZeile::getWertAsDouble(int nr, double &val)
{
	CString str;
	if (!getWert(nr,str))
		return false;

	val = _wtof(str);
	return true;
}

bool MxDatenZeile::getWertAsInt(int nr, int &val)
{
	CString str;
	if (!getWert(nr,str))
		return false;

	val = _wtoi(str);
	return true;
}
bool MxDatenZeile::getWertAsBool(int nr, bool &val)
{
	CString str;
	if (!getWert(nr,str))
		return false;

	int iDummy = _wtoi(str);

	(iDummy ? val=true : val=false);

	return true;
}





MxDatenZeile& MxDatenZeile::operator=(const MxDatenZeile& zeile)
{
	m_zeile.Copy(zeile.m_zeile);

	return *this;
}


void MxDatenZeile::removeAll()
{
	m_zeile.RemoveAll();
}

int MxDatenZeile::getAnzahlSpalten()
{
	return (int)m_zeile.GetSize();
}