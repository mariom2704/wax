#pragma once
#include "enbase.h"
#include <QtCore>

/**
Entity Klasse für Arbeitsfolgen
*/
class EnArbeitsfolge :
	public EnBase
{
public:
	EnArbeitsfolge(void);
	virtual ~EnArbeitsfolge(void);

	void setNr(int nr);
	void setAnzahl(int anzahl);
	void setDn(int dn);
	void setText(const QString &text);

	const int getNr()const;
	const int getAnzahl()const;
	const int getDn()const;
	const QString getText()const;



private:
	int		m_nr,
			m_anzahl,
			m_dn;

	QString m_text;

};
