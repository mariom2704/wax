#include ".\enarbeitsfolge.h"

EnArbeitsfolge::EnArbeitsfolge(void)
{
}

EnArbeitsfolge::~EnArbeitsfolge(void)
{
}

//SetMethoden
void EnArbeitsfolge::setNr(int nr)
{
	m_nr = nr;
}
void EnArbeitsfolge::setAnzahl(int anzahl)
{
	m_anzahl = anzahl;
}

void EnArbeitsfolge::setDn(int dn)
{
	m_dn = dn;
}

void EnArbeitsfolge::setText(const QString &text)
{
	m_text = text;
}

//getMethoden
const int EnArbeitsfolge::getNr()const
{
	return m_nr;
}

const int EnArbeitsfolge::getAnzahl()const
{
	return m_anzahl;
}

const int EnArbeitsfolge::getDn()const
{
	return m_dn;
}

const QString EnArbeitsfolge::getText()const
{
	return m_text;
}
