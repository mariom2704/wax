#include "stdafx.h"

#include "EnContainerRohrElemente.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

EnContainerRohrElemente::EnContainerRohrElemente()
{
}

EnContainerRohrElemente::~EnContainerRohrElemente()
{
}

Element* EnContainerRohrElemente::get_element_at(int index)
{
	XASSERT(index < GetSize() || index >= 0);
	if (index >= GetSize() || index < 0)
		return NULL;

	Element* ele = NULL;
	int nindex = 0;
	for(ContainerClass::iterator it = arr.begin(); it != arr.end(); it++, nindex++)
		if (nindex == index)
			return *it;
	return ele;
}

Element* const EnContainerRohrElemente::get_element_at(int index) const
{
	XASSERT(index < GetSize() || index >= 0);
	if (index >= GetSize() || index < 0)
		return NULL;

	Element* ele = NULL;
	int nindex = 0;
	for(ContainerClass::const_iterator it = arr.begin(); it != arr.end(); it++, nindex++)
		if (nindex == index)
			return *it;
	return ele;
}

int EnContainerRohrElemente::GetSize() const
{
	return (int)arr.size();
}

int EnContainerRohrElemente::GetCount() const
{
	return arr.size();
}

void EnContainerRohrElemente::InsertAt(int index, Element* ele)
{	
	XASSERT((index >= 0) && index < GetSize());
	ContainerClass::iterator it = arr.begin();
	for (; index; it++, index--);
	arr.insert(it, ele);
}

void EnContainerRohrElemente::RemoveAt(int index)
{
	if (index < 0 || index >= GetSize())
		return;

	ContainerClass::iterator it = arr.begin();
	for (; index; it++, index--);
	arr.erase(it);
}

int EnContainerRohrElemente::Add(Element* e)
{
	arr.push_back(e);
	return arr.size()-1;
} 

int EnContainerRohrElemente::add_copy(const Element& e)
{
	arr.push_back(new Element(e));
	return arr.size()-1;
} 

/**
Relativen Abstand aller Elemenete auf Grundlage des Absolutabstandes  neu berechnen
*/
bool EnContainerRohrElemente::calcRelAbst()
{
	/**
	Relative Abstaende der Elemente neu berechnen.
	Funktioniert nur richtig, wenn vorher die Elemente nach abs. Abständen sortiert wurden
	*/
	Element* pele = NULL;
	for (ContainerClass::iterator it = arr.begin(); it != arr.end(); it++)
	{
		Element* ele = *it;
		if (it != arr.begin())
		{
			// Der relative Abstand eines Elementes ist der absulute Abstand - dem absoluten Abstand 
			// des vorigen Elementes
			ele->set_abstRel(ele->get_abstAbs() - pele->get_abstAbs()); 
		}	
		else
		{
			// Beim ersten Element ist der relative gleich dem absoluten Abstand
			ele->set_abstRel(ele->get_abstAbs());
		}
		pele = *it;
	}
	return true;
}



///Alle Elemente des Array löschen
void EnContainerRohrElemente::deleteAll()
{
	for (ContainerClass::iterator it = arr.begin(); it != arr.end(); it++)
		delete *it;
	arr.clear();
}


/**
	Liefert true, wenn ein Bogen oder eine Reduzierung gefunden werden konnte
*/
bool EnContainerRohrElemente::has_reduzierung_bogen() const
{
	for (ContainerClass::const_iterator it = arr.begin(); it != arr.end(); it++)
		if ((*it)->isEleRedBogEinz())
			return true;
	return false;
}

/**
Sortiert die Elemente nach absoluten Abstand.
\param index Wenn das Element an Position index verschoben wird, wird die neue Position in index zurückgegeben
*/
void EnContainerRohrElemente::sortElements(int& index)
{
	ContainerClass::size_type indx = (ContainerClass::size_type)index;
	if (!arr.size() || arr.size() <= indx)
		return;
	Element* prev = NULL;
	Element* ele = NULL;
	Element* eleat = get_element_at(index);
	for (ContainerClass::iterator it = arr.begin(); it != arr.end();)
	{
		ele = *it;
		if (prev && (prev->get_abstAbs() > ele->get_abstAbs()))
		{
			Element tmp = *prev;
			*prev = *ele;
			*ele = tmp;
			if (ele == eleat)
				index--;
			if (prev == eleat)
				index++;
			it = arr.begin();
			prev = NULL;
		}
		else
		{
			prev = ele;
			it++;
		}
	}
}



/**
Aenderung der DN bei den Elementen
Wenn dnvl geaendert wird, muessen anschlussDN aller Elemente angepasst und kontrolliert werden.
*/
CString EnContainerRohrElemente::changeElementDN(int dnvl)
{
	string err;
	int dn = dnvl;
	
	for (ContainerClass::iterator it = arr.begin(); it != arr.end(); it++)
	{
		Element* ele = *it;
		
		if (!ele->isEleAbgEnde())
			ele->set_anschlussDN(dn);
	
		err = (LPCSTR)ele->check_dn();
		if (err.length()) 
			return err.c_str();
		
		if (ele->isEleRedEinz())	
			dn = ele->get_dn();
		
		// Die Laenge muss neu geholt werden, weil diese von der Nennweite abhaengig ist.
		// Bei diesen Elementen ist Laenge bzw. Durchmesser manuell festzulegen 
		// und darf hier nicht überschrieben werden.
		if (ele->get_id() != ROHR && 
			ele->get_id() != STDSTUTZEN && 
			ele->get_id() != BOHR50 && 
			ele->get_id() != BOHR24 &&
			ele->get_id() != SONDERMUFFE &&
			ele->get_id() != SPRINKLERSCHELLE_VIROTEC) 
		{
			int l = 0; double d = 0;
			ele->get_Stammdaten(l, d, ele->get_id(), ele->get_dn(), ele->get_anschlussDN());
			ele->set_laenge(l);
		}

		if (ele->get_id() == SONDERMUFFE) 
			ele->set_mass1(ele->get_laenge()); //Wegen Beschriftung
	}
	return err.c_str();
}



/**
absolute Abstaende auf Grundlage der Relativabstände in den Elementen neu berechnen
*/
bool EnContainerRohrElemente::calcAbsolutAbst()
{
	//Liefert zusaetzlich den letzten absoluten Abstand
	Element* prev = NULL;
	for (ContainerClass::iterator it = arr.begin(); it != arr.end(); it++)
	{
		Element* ele = *it;

		// erstes Element
		if (it == arr.begin()) 
		{ 
			ele->set_abstAbs(ele->get_abstRel());
		} 
		// Verkettung
		else if (ele->get_verkettung()) 
		{
			ele->set_abstAbs(prev->get_abstAbs());
		} 
		else 
		{
			ele->set_abstAbs(prev->get_abstAbs() + ele->get_abstRel());
		}
		prev = ele;
	} 
	return true;
}

void EnContainerRohrElemente::copy(const EnContainerRohrElemente& other)
{
	deleteAll();
	for (int i = 0; i < other.GetSize(); i++)	
	{
		const Element* const ele = other.get_element_at(i);
		if (!ele)
			continue;
		add_copy(*ele);
	}
}

void EnContainerRohrElemente::RemoveAll()
{
	arr.clear();
}

#ifdef _DEBUG
/**
Ausgabe der Liste
*/
void EnContainerRohrElemente::Dump(CDumpContext &dc)const
{

	for (int i=0; i<GetSize();i++)
	{
		const Element* const pBase = get_element_at(i);

		dc << "\nElement " << i << ":";
		if (pBase)
			dc << *pBase;
		else
			dc << "NULL";

	}
}

#endif
