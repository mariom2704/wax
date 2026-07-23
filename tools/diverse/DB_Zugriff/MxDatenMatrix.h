#pragma once

#include "MxDatenZeile.h"
#include "MxListe.h"
#include "afxtempl.h"


class MxDatenMatrix : public CObject
	
{
public:
	MxDatenMatrix(void);
	~MxDatenMatrix(void);

	bool addZeile(MxDatenZeile *zeile);		//Zeile dem Resultset zuordnen	

	MxDatenZeile* getStartZeile();			//Erste Zeile aus dem Resultset
	MxDatenZeile* getNextZeile();				//Zeile aus einem Resultset zurückgeben

	int getAnzahlZeilen();
	int getAnzahlSpalten();

	void removeAll();					//Alle Werte in dem Resultset löschen

private:
	MxListe  m_zeilen;					///< Liste mit den Zeilen
	MxIterator m_iter;					///< Iterator für die Liste

	int m_anzSpalten;					///< Anzahl der Spalten
};
