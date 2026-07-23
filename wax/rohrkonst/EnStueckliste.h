#pragma once

#include "mxcoblist.h"
#include "Element.h"

class ROHRKONSTDLL EnStueckliste : public MxCObList
{
public:
	
	void add(Element* e);

	void add_copy(const Element& e);

	/// Löschen der Liste
	void clear();

	/// Kopieren der Liste
	void copy(const EnStueckliste& other);

	//Werte vom aktuellen Element in der Liste
	int  getIdFromElement(const MxCObListIterator &iterator);
	double  getMengeLaengeFromElement(const MxCObListIterator &iterator);
	int  getDnFromElement(const MxCObListIterator &iterator);
	int  getDnRohrFromElement(const MxCObListIterator &iterator);
	long getArtikelNrFromElement(const MxCObListIterator &iterator);
	double getPreisFromElement(const MxCObListIterator &iterator);
	CString getBezeichnungFromElement(const MxCObListIterator &iterator);


private:
	Element* castElement(const MxCObListIterator &iterator);		///< dynamic cast auf aktuelles Listenelement
};
