#include "StdAfx.h"
#include ".\MxDatenMatrix.h"

MxDatenMatrix::MxDatenMatrix(void)
{
	m_anzSpalten = 0;
	m_iter.setListe(m_zeilen);
}

MxDatenMatrix::~MxDatenMatrix(void)
{
	//Speicher freigeben
	removeAll();
}

/**
Neue Zeile an den Resultset hängen. 
Es wird dabei geprüft, ob alle Zeilen die gleiche Spaltenanzahl hat
*/
bool MxDatenMatrix::addZeile(MxDatenZeile *zeile)
{
	if (!zeile)
		return false;

	if (m_zeilen.getSize() == 0)
		m_anzSpalten = zeile->getAnzahlSpalten();
	else
	{
		//Prüfen ob neue Zeile genausoviel Spalten hat wie erste
		if (m_anzSpalten != zeile->getAnzahlSpalten())
			return false;
	}

	m_zeilen.append(zeile);
	return true;
}

///Liste auf die Startposition setzen
MxDatenZeile* MxDatenMatrix::getStartZeile()
{
	m_iter.resetIterator();
	return getNextZeile();
}

/**
Zeile aus einem Resultset zurückgeben
*/
MxDatenZeile *MxDatenMatrix::getNextZeile()
{
	if (m_iter.next())
		return dynamic_cast<MxDatenZeile*>(m_zeilen.getAt(m_iter));
	else
		return NULL;
}



void MxDatenMatrix::removeAll()
{
	m_zeilen.deleteAllElemente();
	m_anzSpalten = 0;
}



int MxDatenMatrix::getAnzahlZeilen()
{
	return m_zeilen.getSize();
}

int MxDatenMatrix::getAnzahlSpalten()
{
	return m_anzSpalten;
}

