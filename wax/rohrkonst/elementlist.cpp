#include "stdafx.h"


#include "elementlist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ElementList::ElementList(CObList& lst)
{
	init(lst);
	counter = 0;
}

ElementList::~ElementList()
{
}
	
void ElementList::init(CObList &lst)
{
	for (POSITION pos = lst.GetHeadPosition(); pos;)
	{
		CObject *obj = lst.GetNext(pos);
		elelist.push_back(dynamic_cast<Element*>(obj));
	}
}

int ElementList::get_count() const
{
	return elelist.size();
}

Element *ElementList::get_at(int i) const
{
	return elelist.at(i);
}

Element *ElementList::get_prev(int i) const
{
	if (i <= 0)
		return NULL;
	return elelist.at(i-1);
}

Element *ElementList::get_prevprev(int i) const
{
	if (i <= 1)
		return NULL;
	return elelist.at(i-2);
}

Element *ElementList::get_next(int i) const
{
	if (i >= (int)elelist.size()-1)
		return NULL;
	return elelist.at(i+1);
}
