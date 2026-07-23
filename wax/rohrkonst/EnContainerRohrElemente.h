#pragma once

#include "Element.h"
#include <list>

/**
Array für Rohrelemente.
Beinhaltet Objekte vom Typ Element
*/
class ROHRKONSTDLL EnContainerRohrElemente 
{
private :
	typedef std::list<Element*> ContainerClass;
	ContainerClass arr;

public:
	EnContainerRohrElemente();
	virtual ~EnContainerRohrElemente();

	///Alle Elemente des Array löschen
	void deleteAll();

	///Relativen Abstand aller Elemenete auf Grundlage des Absolutabstandes  neu berecnen
	bool calcRelAbst();

	///absolute Abstaende auf Grundlage der Relativabstände in den Elementen neu berechnen
	bool calcAbsolutAbst();

	///Liefert true, wenn ein Bogen oder eine Reduzierung gefunden werden konnte
	bool has_reduzierung_bogen() const;

	/// Sortiert die Elemente nach absoluten Abstand.
	void sortElements(int & index);

	/// Aenderung der DN bei den Elementen
	CString changeElementDN(int dnvl);

	Element* get_element_at(int index);

	Element* const get_element_at(int index) const;

	/// @todo Size und Count ????? Unterschiedliche Verwendung prüfen!
	int GetSize() const;

	int GetCount() const;

	int Add(Element* e);

	int add_copy(const Element& e);

	void InsertAt(int index, Element* ele);

	void RemoveAt(int index);

	void copy(const EnContainerRohrElemente& other);

	void RemoveAll();

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const; ///Dumpausgabe der Liste
#endif

};
