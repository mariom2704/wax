#include "stdafx.h"
#include ".\MxListe.h"


MxListe::MxListe()
{
}


MxListe::~MxListe(void)
{
}

/**
Ein neues Element an die Liste anhängen
*/
void MxListe::append(MxBase *newElement)
{
	m_list.AddTail(newElement);
}

/**
Liste anhängen
*/
void MxListe::appendListe(MxListe& liste)
{
	m_list.AddTail(&liste.m_list);
}

/**
Alle Elemente der Liste löschen. Es werden auch die Elemente mit delete gelöscht
*/
void MxListe::deleteAllElemente() 
{
	MxBase *element = NULL;

	//Liste durchlaufen und Elemente löschen
	POSITION pos;
	for( pos = m_list.GetHeadPosition(); pos != NULL; )
	{
		element = (MxBase*)m_list.GetNext(pos);
		
		if (element)
		{
			delete element;
			element = NULL;
		}
	}

	//Liste löschen
	m_list.RemoveAll();
}

/**
nur die Liste löschen. Die Elemente müssen extra mit delete gelöscht werden
*/
void MxListe::resetListe()
{
	//nur Liste löschen
	m_list.RemoveAll();
}

/**
Länge der Liste zurückgeben
*/
int MxListe::getSize()	  
{
	return (int)m_list.GetSize();
}

/**
Prüfen ob Liste Elemente hat
*/
bool MxListe::isEmpty()const
{
	if (m_list.IsEmpty())
		return true;

	return false;
}


/**
Listenelement zurückgeben, an dem der Iterator steht.
Der Iterator muß mit dieser Liste initialisiert sein.
*/
MxBase* MxListe::getAt(const MxIterator &iterator)const
{
	POSITION pos = iterator.getPosition();

	if (!pos)
		return NULL;

	return (MxBase*)m_list.GetAt(pos);
}

/**
Zeiger des 1. Elementes in der Liste zurückgeben
*/
MxBase* MxListe::getFirst()const
{
	return (MxBase*)m_list.GetHead();
}


/**
Zeiger des letztes Elementes in der Liste zurückgeben
*/
MxBase* MxListe::getLast()const
{
	return (MxBase*)m_list.GetTail();
}




/**
Übergebene Liste kopieren. Von den Elementen wird nur die Referenz kopiert

\param deleteThisElemente    = true: die Elemente der this-Liste werden vor dem kopieren mit delete gelöscht

\param deepCopyOtherElemente = true: von den Elementen der übergebenen Liste wird eine Kopie mit deepCopy erzeugt 
								     deepCopy muß in der Elementklasse implementiert sein!!!
							 = false: von den Elementen der übergebenen Liste wird nur die Referenz kopiert
*/
bool MxListe::copy(MxListe &other,bool deleteThisElemente,bool deepCopyOtherElemente)
{
	//Attribute kopieren
	//...

	//Listeelemente löschen
	if (deleteThisElemente)
		deleteAllElemente();
	else
		resetListe();
	
	MxBase *pBase = NULL;

	//Alle Listenelemente kopiern
	MxIterator i(other);
	while (i.next())
	{
		if (deepCopyOtherElemente)
			pBase = other.getAt(i)->deepCopy();
		else
			pBase = other.getAt(i);

		if (!pBase)
			continue;

		//Neues Objekt in die Liste schreiben
		append(pBase);
	}

	return true;
}


/**
Gibt eine neues mit new erzeugtes Objekt zurück
Die Listenelemente müssen auch deepCopy implementiert haben
*/
MxBase *MxListe::deepCopy()const
{
	MxListe *pNewListe = new MxListe();

	MxBase *pBase = NULL,
		   *pNew  = NULL;


	MxIterator i(*this);
	while (i.next())
	{
		pBase = getAt(i);

		if (!pBase)
			continue;
		
		pNew = pBase->deepCopy();


		pNewListe->append(pNew);
	}


	return pNewListe;
}


/**
Ein neues Element an der Stelle pos in die Liste einfügen.
Ist pos = 0, wird das Element als 1. Element in die Liste geschrieben.
Ist pos >= getSize() wird es an die Liste angehängt
*/
void MxListe::insertAt(int pos,MxBase *newElement)
{
	if (!newElement)
		return;

	//Prüfen ob gültige Position
	if (pos < 0)
		return;

	//Wenn pos = 0 Element am Anfang einfügen
	if (pos == 0){
		m_list.AddHead(newElement);
		return;
	}

	//Wenn pos >= getSize() am Ende anhängen
	if (pos >= getSize()){
		m_list.AddTail(newElement);
		return;
	}

	//Liste bis zur Position pos durchlaufen
	MxIterator i(*this);
	for (int iCount=0; iCount<pos; iCount++)
	{
		if (!i.next()){
			//Schwerer Fehler dürfte nicht vorkommen
			return;
		}	
	}

	//Listenelement einfügen
	m_list.InsertAfter(i.getPosition(),newElement);

	return;
}

/**
Element an der Position pos durch das Element newElement ersetzen.
ACHTUNG: Daran denken, das der Speicher für das alte Element freigegeben wird
*/
bool MxListe::setAt(const MxIterator &iterator,MxBase *newElement)
{
	//if (!newElement)
	//	return false;

	if (isEmpty())
		return false;


	//Zeiger auf Position
	POSITION position = iterator.getPosition();

	if (!position)
		return false;

	m_list.SetAt(position,newElement);

	return true;
}

/**
Element auf dem der Iterator steht löschen
*/
void MxListe::deleteAt(const MxIterator &iterator,bool deleteElement)
{
	MxBase *pBase = (MxBase*)getAt(iterator);

	if (pBase)
		m_list.RemoveAt(iterator.getPosition());

	if (pBase && deleteElement)
	{
		delete pBase;
		pBase = NULL;
	}
}

/**
Ein Element an der Stelle pos aus der Liste löschen
*/
void MxListe::deleteAt(int pos,bool deleteElement)
{
	MxBase *pElement = NULL;

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
		pElement = (MxBase*)m_list.GetHead();
		m_list.RemoveHead();
	}
	else if (pos == getSize()-1)
	{
		//Wenn pos == getSize()-1 letztes Element löschen
		pElement = (MxBase*)m_list.GetTail();
		m_list.RemoveTail();
	}
	else
	{
		//Liste bis zur Position pos durchlaufen
		MxIterator i(*this);
		
		for (int iCount=0; iCount <= pos; iCount++)
		{
			if (!i.next()){
				//Schwerer Fehler dürfte nie vorkommen
				return;
			}
		}

		pElement = (MxBase*)getAt(i);
		//Listenelement löschen
		m_list.RemoveAt(i.getPosition());
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
MxBase* MxListe::getAtIndex(int index)
{
	if (index < 0)
		return NULL;

	//Liste bis zur Position pos durchlaufen
	MxIterator i(*this);
	if (!i.setIndex(index))
		return NULL;

	return getAt(i);
}


/**
Referenz auf CObList Objekt zurückgeben
*/
const CObList& MxListe::getCObList()const
{
	return m_list;
}





#ifdef _DEBUG
/**
Ausgabe der Liste
*/
void MxListe::Dump(CDumpContext &dc)const
{
	const MxBase *pBase = NULL;

	MxIterator i(*this);
	int iCount=0;
	while (i.next())
	{
		pBase = getAt(i);

		dc << "\nElement " << iCount << ":";
		if (pBase)
			dc << *pBase;
		else
			dc << "NULL";

		iCount++;
	}
}

#endif




/////////////////////////////////////////////////////////////////////////////////////////////
// MxIterator Memberfunktionen
/////////////////////////////////////////////////////////////////////////////////////////////

MxIterator::MxIterator()
{
	m_pList = NULL;
	m_position = NULL;
	m_index = -1;
}

MxIterator::MxIterator(const MxIterator &iter)
{
	*this = iter;
}


MxIterator::MxIterator(const MxListe &liste)
{
	m_pList = &liste.getCObList();
	m_position = NULL;
	m_index = -1;
}


MxIterator::~MxIterator(void)
{
	m_pList = NULL;
	m_position = NULL;
	m_index = -1;
}


void MxIterator::setListe(const MxListe &liste)
{
	m_pList = &liste.getCObList();
	m_position = NULL;
	m_index = -1;
}


/**
Iterator auf nächstes Element setzen. Steht der Iterator auf keinem Element,
wird er auf das erste Element gesetzt.
*/
bool MxIterator::next() 
{
	if (!m_pList)
		return false;

	if (!m_position)
	{
		m_position = m_pList->GetHeadPosition();
		m_index = 0;
	}
	else
	{
		m_pList->GetNext(m_position);
		m_index++;
	}

	if (!m_position)
	{
		m_index = -1;
		return false;
	}
	
	return true;
}

/**
Iterator auf nächstes Element setzen. Steht der Iterator auf keinem Element,
wird er auf das erste Element gesetzt.
*/
bool MxIterator::operator ++()
{
	return next();
}
/**
Iterator auf nächstes Element setzen. Steht der Iterator auf keinem Element,
wird er auf das erste Element gesetzt.
*/
bool MxIterator::operator ++(int)
{
	return next();
}


/**
Iterator auf vorherige Element setzen. Steht der Iterator auf keinem Element,
wird er auf das letzte Element gesetzt.
*/
bool MxIterator::back()
{
	if (!m_pList)
		return false;

	if (!m_position)
	{
		m_position = m_pList->GetTailPosition();
		m_index = (int)m_pList->GetSize() -1;
	}
	else
	{
		m_pList->GetPrev(m_position);
		m_index--;
	}

	if (!m_position)
	{
		m_index = -1;
		return false;
	}
	
	return true;
}
/**
Iterator auf vorherige Element setzen. Steht der Iterator auf keinem Element,
wird er auf das letzte Element gesetzt.
*/
bool MxIterator::operator --()
{
	return back();
}
/**
Iterator auf vorherige Element setzen. Steht der Iterator auf keinem Element,
wird er auf das letzte Element gesetzt.
*/
bool MxIterator::operator --(int)
{
	return back();
}

/**
 Internen Iterator auf das erste Element setzen
*/
bool MxIterator::first()				  
{
	if (!m_pList)
		return false;

	m_position = m_pList->GetHeadPosition();
	m_index = 0;

	if (!m_position)
	{
		m_index = -1;
		return false;
	}
	
	return true;
}

/**
Internen Iterator auf das letzte Element setzen
*/
bool MxIterator::last()				   
{
	if (!m_pList)
		return false;

	m_position = m_pList->GetTailPosition();
	m_index = (int)m_pList->GetSize() -1;

	if (!m_position)
	{
		m_index = -1;
		return false;
	}
	
	return true;
}

/**
Internen Iterator zurücksetzen. 
*/
void MxIterator::resetIterator()					    
{
	m_position = NULL;
	m_index = -1;
}


/**
POSITION Zeiger auf das aktuelle Listenobjekt zurückgeben
*/
const POSITION MxIterator::getPosition()const
{
	return m_position;
}

/**
Prüft ob der Iterator auf einer gültigen Position steht
*/
bool MxIterator::isValid()
{
	if (m_position)
		return true;

	return false;
}

/**
Gibt die Nummer der Position in der Liste zurück auf dem der Iterator steht (1. Element = 0)
Steht der Iterator auf keinem Element wird -1 zurückgegeben

ACHTUNG: Funktioniert nur wenn während des Durchlaufs keine Listenelemente gelöscht bzw. eingefügt werden
*/
int MxIterator::getIndex()
{
	return m_index;
}

/**
Iterator zurückgeben, der count Positionen hochgezählt wird
*/
MxIterator MxIterator::operator+(int count)
{
	MxIterator iter(*this);

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
MxIterator MxIterator::operator-(int count)
{
	MxIterator iter(*this);

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
MxIterator& MxIterator::operator=(const MxIterator &iter)
{
	m_pList = iter.m_pList;
	m_position = iter.m_position;
	m_index = iter.m_index;

	return *this;
}

/**
Internen Iterator auf das Element an Stelle Index setzen
\return Wenn index<0 oder >= getSize() wird false zurückgegeben
*/
bool MxIterator::setIndex(int index)
{
	if (index < 0)
	{
		resetIterator();
		return false;
	}

	//zur Stelle index springen
	resetIterator();
	for (int i=0; i <= index; i++)
	{
		if (!next())
			return false;
	}

	return true;
}



