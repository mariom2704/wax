#pragma once

#include "Element.h"

/**
Verwaltungsklasse für Elementlisten
*/
class ROHRKONSTDLL ElementList 
{
public:
	/// Konstruktor setzt die interne Liste mit Elementpointern
	ElementList(CObList& lst);
	
	/// Destruktor
	virtual ~ElementList();
	
	/// Liefert das Element von Position i
	Element *get_at(int i) const;

	Element *get_prev(int i) const;

	Element *get_next(int i) const;

	Element *get_prevprev(int i) const;

	/// Anzahl der Elemente in der Liste
	int get_count() const;

private:
	/// Interne Liste initialisieren
	void init(CObList &lst);
	vector<Element*> elelist;
	
	///Interner Zaehler
	int counter;
};









