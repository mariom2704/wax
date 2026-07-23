#include "StdAfx.h"

#include ".\mxcoblist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

MxCObList::MxCObList() 
{
	auto_delete = false;
}

MxCObList::~MxCObList(void)
{
	if (auto_delete)
		deleteAllElemente();
}



/**
Alle Elemente der Liste löschen. Es werden auch die Elemente mit delete gelöscht
*/
void MxCObList::deleteAllElemente() 
{
	CObject *pObject = NULL;

	//Liste durchlaufen und Elemente löschen
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		pObject = (CObject*)GetNext(pos);
		
		if (pObject)
		{
			delete pObject;
			pObject = NULL;
		}
	}

	//Liste löschen
	RemoveAll();

}



/**
Ein neues Element an die Liste anhängen
*/
void MxCObList::append(CObject *newElement)
{
	AddTail(newElement);
}

/**
Eine Liste anhängen
*/
void MxCObList::appendListe(CObList& liste)
{
	AddTail(&liste);
}



/**
Länge der Liste zurückgeben
*/
int MxCObList::getSize()const	  
{
	return (int)GetSize();
}

/**
Prüfen ob Liste Elemente hat
*/
bool MxCObList::isEmpty()const
{
	if (IsEmpty())
		return true;

	return false;
}


/**
Listenelement zurückgeben, an dem der Iterator steht.
Der Iterator muß mit dieser Liste initialisiert sein.
*/
CObject* MxCObList::getAt(const MxCObListIterator &iterator)
{
	POSITION pos = iterator.getPosition();

	if (!pos)
		return NULL;

	return GetAt(pos);
}




/**
Zeiger des 1. Elementes in der Liste zurückgeben
*/
CObject* MxCObList::getFirst()
{
	return GetHead();
}


/**
Zeiger des letztes Elementes in der Liste zurückgeben
*/
CObject* MxCObList::getLast()
{
	return GetTail();
}




/**
Übergebene Liste kopieren. Von den Elementen wird nur die Referenz kopiert

\param deleteThisElemente    = true: die Elemente der this-Liste werden vor dem kopieren mit delete gelöscht
*/
bool MxCObList::copy(MxCObList &other/*,bool deleteThisElemente*/)
{
	//Attribute kopieren
	//...

	//Listeelemente löschen
//	if (deleteThisElemente)
		deleteAllElemente();
//	else
//		resetListe();
	
	CObject *pBase = NULL;
		    

	//Alle Listenelemente kopiern
	MxCObListIterator i(other);
	while (i.next())
	{
		pBase = other.getAt(i);

		if (!pBase)
			continue;

		//Neues Objekt in die Liste schreiben
		append(pBase);
	}

	return true;
}




/**
Ein neues Element an der Stelle pos in die Liste einfügen.
Ist pos = 0, wird das Element als 1. Element in die Liste geschrieben.
Ist pos >= getSize() wird es an die Liste angehängt
*/
void MxCObList::insertAt(int pos,CObject *newElement)
{
	if (!newElement)
		return;

	//Prüfen ob gültige Position
	if (pos < 0)
		return;

	//Wenn pos = 0 Element am Anfang einfügen
	if (pos == 0){
		AddHead(newElement);
		return;
	}

	//Wenn pos >= getSize() am Ende anhängen
	if (pos >= getSize()){
		AddTail(newElement);
		return;
	}

	//Liste bis zur Position pos durchlaufen
	MxCObListIterator i(*this);
	for (int iCount=0; iCount<pos; iCount++)
	{
		if (!i.next()){
			//Schwerer Fehler dürfte nicht vorkommen
			return;
		}	
	}

	//Listenelement einfügen
	InsertAfter(i.getPosition(),newElement);

	return;
}

/**
Element an der Position pos durch das Element newElement ersetzen.
ACHTUNG: Daran denken, das der Speicher für das alte Element freigegeben wird
*/
bool MxCObList::setAt(const MxCObListIterator &iterator,CObject *newElement)
{
	//if (!newElement)
	//	return false;

	if (isEmpty())
		return false;


	//Zeiger auf Position
	POSITION position = iterator.getPosition();

	if (!position)
		return false;

	SetAt(position,newElement);

	return true;
}



/**
Ein Element an der Stelle pos aus der Liste löschen
*/
void MxCObList::deleteAt(int pos,bool deleteElement)
{
	CObject *pElement = NULL;

	//Wenn Liste keine Elemente hat Funktion wieder verlassen
	if (getSize() == 0)
		return;

	//Prüfen ob gültige Position
	if (pos < 0 || pos >= getSize())
		return;


	//Element aus Liste suchen und Listenelement löschen
	if (pos == 0)
	{
		//Wenn pos = 0 1. Element löschen
		pElement = (CObject*)GetHead();
		RemoveHead();
	}
	else if (pos == getSize()-1)
	{
		//Wenn pos == getSize()-1 letztes Element löschen
		pElement = (CObject*)GetTail();
		RemoveTail();
	}
	else
	{
		//Liste bis zur Position pos durchlaufen
		MxCObListIterator i(*this);
		
		for (int iCount=0; iCount <= pos; iCount++)
		{
			if (!i.next()){
				//Schwerer Fehler dürfte nie vorkommen
				return;
			}
		}

		pElement = (CObject*)getAt(i);
		//Listenelement löschen
		RemoveAt(i.getPosition());
	}

	//Element löschen
	if (pElement && deleteElement)
	{
		delete pElement; 
		pElement = NULL;
	}

	return;
}

/**
Zeiger auf Element in List an Stelle index zurückgeben. 1. Element hat index=0
*/
CObject* MxCObList::getAtIndex(int index)
{
	if (index < 0)
		return NULL;

	//Liste bis zur Position pos durchlaufen
	MxCObListIterator i(*this);
	if (!i.setIndex(index))
		return NULL;

	return getAt(i);
}


CObject* MxCObList::getNextObject(const MxCObListIterator &iterator)
{
	MxCObListIterator nextIter = iterator;

	if (!nextIter.next())
		return NULL;

	return getAt(nextIter);
}



#ifdef _DEBUG
/**
Ausgabe der Liste
*/
void MxCObList::Dump(CDumpContext &dc)const
{
#ifdef DEBUGLIST
	const CObject *pBase = NULL;

	int iCount = 0;
	POSITION pos;
	for( pos = GetHeadPosition(); pos != NULL; )
	{
		pBase = (CObject*)GetNext(pos);

		dc << "\nElement " << iCount << ":";
		if (pBase)
			dc << *pBase;
		else
			dc << "NULL";

		iCount++;
	}
#endif
}

#endif






////////////////////////////////////////////////////////////////////////


MxCObListIterator::MxCObListIterator()
{
	m_pList = NULL;
	m_position = NULL;
	m_index = -1;
}


MxCObListIterator::MxCObListIterator(const MxCObListIterator& iter)
{
	*this = iter;
}


MxCObListIterator::MxCObListIterator(const CObList& liste)
{
	m_pList = &liste;
	m_position = NULL;
	m_index = -1;
}


MxCObListIterator::~MxCObListIterator(void)
{
	m_pList = NULL;
	m_position = NULL;
	m_index = -1;
}


void MxCObListIterator::setListe(const CObList &liste)
{
	m_pList = &liste;
	m_position = NULL;
	m_index = -1;
}


/**
Iterator zurückgeben, der count Positionen hochgezählt wird
*/
MxCObListIterator MxCObListIterator::operator+(int count)
{
	MxCObListIterator iter(*this);

	for (int i=0; i<count; i++)
	{
		//Abbrechen falls Listenende errreicht ist
		if (!iter.next())
			break;
	}

	return iter;
}

/**
 Iterrator zurückgeben, der um count Positionen zurückgezählt ist
*/
MxCObListIterator MxCObListIterator::operator-(int count)
{
	MxCObListIterator iter(*this);

	for (int i=0; i<count; i++)
	{
		//Abbrechen falls Listenende errreicht ist
		if (!iter.back())
			break;
	}

	return iter;
}


/**
Iterator kopieren
*/
MxCObListIterator& MxCObListIterator::operator=(const MxCObListIterator &iter)
{
	m_pList = iter.m_pList;
	m_position = iter.m_position;
	m_index = iter.m_index;

	return *this;
}

