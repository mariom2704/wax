#pragma once
#include "enbase.h"
#include <QtCore>


/**
Entity Klasse für Material
*/
class EnMaterial :
	public EnBase
{
public:
	EnMaterial(void);
	virtual ~EnMaterial(void);

	//Set Methoden
	void setArtikelnr(long artikelNr);
	void setBezeichnung(const QString &bez);
	void setAnzahl(int anz);				///Anzahl des Materials in Stück
	void setLaenge(long laenge);			///Rohrlänge in mm

	const long		getArtikelNr()const;
	const QString	getBezeichnung()const;
	const int		getAnzahl()const;		///Anzahl des Materials in Stück
	const long		getLaenge()const;		///Rohrlänge in mm

	const double	getLaengeinMeter()const; //Rückgabe der Rohrlänge in Meter

private:

	long	m_artikelNr,
		    m_laenge;

	QString m_bezeichnung;

	int		m_anzahl;
};
