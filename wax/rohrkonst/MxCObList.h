#pragma once
#include "afxcoll.h"
#include "MxListe.h"


class MxCObListIterator;

class ROHRKONSTDLL MxCObList :	public CObList
{
public:
	MxCObList(void);

	virtual ~MxCObList(void);

//Elemente anhängen und löschen 
	
	void append(CObject *newElement);				///< neues Element an die Liste anhängen

	void appendListe(CObList& liste);				///< Liste anhängen

	void insertAt(int pos,CObject *newElement);		///< neues Element an der Stelle pos in die Liste einfügen

	void deleteAllElemente();						///< alle Elemente der Liste löschen

	void deleteAt(int pos,bool deleteElement);		///< Element an der Stelle pos aus der Liste löschen

	//Elementzugriffe

	CObject* getAt(const MxCObListIterator &iterator);///< Zeiger auf Element  


	CObject* getFirst();							///< Zeiger des 1. Elementes in der Liste zurückgeben

	CObject* getLast();								///< Zeiger des letztes Elementes in der Liste zurückgeben

	CObject* getAtIndex(int index);					///< Zeiger auf Element in Lista an Stelle index


	CObject* getNextObject(const MxCObListIterator &iterator);


	bool setAt(const MxCObListIterator &iterator,CObject *newElement);///< Element an der Position pos durch das Element newElement ersetzen.

	//kopieren
	bool copy(MxCObList &other/*,bool deleteThisElemente*/);	///< kopiert übergebene Liste

	
	//sonstige Funktionen
	int getSize()const;						///< Länge der Liste

	bool isEmpty()const;				///< BOOL wenn Liste keine Elemente hat

	/// Muss explizit aufgerufen werden
	void set_auto_delete(bool val = true) { auto_delete = val; }

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const; ///< Dumpausgabe der Liste
	#endif

protected:
	/// Default true, alle Objekte in der Liste werden vom Destruktor gelöscht
	bool auto_delete;
};




/**
Iterator für CObList
*/
class ROHRKONSTDLL MxCObListIterator :	public MxIterator
{
public:
	MxCObListIterator();
	MxCObListIterator(const CObList& liste);
	MxCObListIterator(const MxCObListIterator& iter);

	virtual ~MxCObListIterator();

	void setListe(const CObList &liste);


	MxCObListIterator& operator=(const MxCObListIterator &iter);

	MxCObListIterator operator+(int count); ///< Iterator zurückgeben, der count Positionen hochgezählt wird
	MxCObListIterator operator-(int count); ///< Iterrator zurückgeben, der um count Positionen zurückgezählt ist

};