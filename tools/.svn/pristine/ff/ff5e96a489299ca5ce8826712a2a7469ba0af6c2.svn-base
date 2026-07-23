#include ".\enmaterial.h"

EnMaterial::EnMaterial(void)
{
}

EnMaterial::~EnMaterial(void)
{
}


void EnMaterial::setArtikelnr(long artikelNr)
{
	m_artikelNr = artikelNr;
}
void EnMaterial::setBezeichnung(const QString &bez)
{
	m_bezeichnung = bez;
}

///Anzahl des Materials in Stück
void EnMaterial::setAnzahl(int anz)
{
	m_anzahl = anz;
}

///Rohrlänge in mm
void EnMaterial::setLaenge(long laenge)
{
	m_laenge = laenge;
}

const long EnMaterial::getArtikelNr()const
{
	return m_artikelNr;
}
const QString EnMaterial::getBezeichnung()const
{
	return m_bezeichnung;
}

///Anzahl des Materials in Stück
const int EnMaterial::getAnzahl()const
{
	return m_anzahl;
}

///Rohrlänge in mm
const long EnMaterial::getLaenge()const
{
	return m_laenge;
}

///Rückgabe der Rohrlänge in Meter
const double EnMaterial::getLaengeinMeter()const
{
	return m_laenge / 1000.0;
}
