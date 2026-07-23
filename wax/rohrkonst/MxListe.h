#pragma once
#include "WaxBase.h"


class MxIterator;



/**
Liste um Objekte vom Typ WaxBase zu speichern. 
Siehe unten auch das Beispiel.
*/
class ROHRKONSTDLL MxListe :
	public WaxBase
{
public:
	MxListe();
	virtual ~MxListe(void);

	//Elemente anhängen 
	
	void append(WaxBase *newElement);				///< neues Element an die Liste anhängen

	void appendListe(MxListe& liste);				///< Liste anhängen

	void insertAt(int pos,WaxBase *newElement);		///< neues Element an der Stelle pos in die Liste einfügen


	//Elemente löschen

	void deleteAllElemente();						///< alle Elemente der Liste löschen

	void deleteAt(int pos,bool deleteElement);		///< Element an der Stelle pos aus der Liste löschen

	void deleteAt(const MxIterator &iterator,bool deleteElement); ///< Element auf dem der Iterator steht löschen

	void resetListe();								///< Nur die Liste zurücksetzen, die Elemente werden nicht freigegeben


	//Elementzugriffe

	WaxBase* getAt(const MxIterator &iterator)const;	///< Zeiger auf Element  

	WaxBase* getFirst()const;						///< Zeiger des 1. Elementes in der Liste zurückgeben

	WaxBase* getLast()const;							///< Zeiger des letztes Elementes in der Liste zurückgeben

	WaxBase* getAtIndex(int index);					///< Zeiger auf Element in Lista an Stelle index


	bool setAt(const MxIterator &iterator,WaxBase *newElement);	///< Element an der Position pos durch das Element newElement ersetzen.

	//kopieren
	bool copy(MxListe &other,bool deleteThisElemente,bool deepCopyOtherElemente);	///< kopiert die übergebene Liste. 

	virtual WaxBase *deepCopy()const;				///< Gibt eine neues mit new erzeugtes Objekt zurück

	
	//sonstige Funktionen

	int getSize();						///< Länge der Liste

	bool isEmpty()const;				///< bool wenn Liste keine Elemente hat

	const CObList& getCObList()const;	///< Referenz auf CObList Objekt zurückgeben



	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const; ///< Dumpausgabe der Liste
	#endif

private:
	CObList		m_list;				///< Liste mit den Objekten

};


/**
Iterator zum Navigieren in einer MxListe
*/
class ROHRKONSTDLL MxIterator :	public WaxBase
{
public:
	MxIterator();
	MxIterator(const MxIterator &iter);
	MxIterator(const MxListe &liste);
	virtual ~MxIterator(void);

	void setListe(const MxListe &liste);

	//Positionierung

	bool next();				///< Iterator auf nächstes Element setzen
	bool operator ++();
	bool operator ++(int);


	bool back();				///< Iterator auf vorheriges Element setzten
	bool operator --();
	bool operator --(int);

	bool first();				///< Internen Iterator auf das erste Element setzen	
	bool last();				///< Internen Iterator auf das letzte Element setzen
	bool setIndex(int index);	///< Internen Iterator auf das Element an Stelle Index setzen

	void resetIterator();		///< Internen Iterator zurücksetzen. 


	MxIterator& operator=(const MxIterator &iter);

	MxIterator operator+(int count); //< Iterator zurückgeben, der count Positionen hochgezählt wird
	MxIterator operator-(int count); //< Iterrator zurückgeben, der um count Positionen zurückgezählt ist


	bool isValid();				///< Prüft ob der Iterator auf einer gültigen Position steht

	int  getIndex();			///< Gibt die Nummer der Position in der Liste zurück auf der der Iterator steht (1. Element = 0)


	const POSITION getPosition()const;	///Position des Iterators zurückgegeben

protected:
	const CObList	*m_pList;	///< Zeiger auf Liste

	POSITION	m_position;		///< Aktuelle Position in der Liste

	int			m_index;		///< Aktuelle Position in der Liste als Nummer 
								///< (ACHTUNG: Funktioniert nur wenn während des Durchlaufs keine Listenelemente gelöscht bzw. eingefügt werden)
};


/*
Beispiele für die Benutzung der Klasse
	
	EnTest *test;
	MxListe liste;


	test = new EnTest;
	test->wert = 0;
	liste.append(test);

	test = new EnTest;
	test->wert = 1;
	liste.append(test);


	MxIterator i(liste);
	
	while (i.next())
	{
		test = (EnTest*)liste.getAt(i);
	}

	for (i.first();i.isValid();i++)
	{
		test = (EnTest*)liste.getAt(i);
	}


	for (i.last();i.isValid();i--)
	{
		test = (EnTest*)liste.getAt(i);
	}


	i.resetIterator();
	while (i.back())
	{
		test = (EnTest*)liste.getAt(i);
	}

*/
