// Copyright (C) 2001 Minimax GmbH

#include "stdafx.h"


#include "IconManager.h"

IconManager::~IconManager()
{
}

IconManager::IconManager()
{
}

void IconManager::initObject(HICON& object, UINT id)
{
	object = AfxGetApp()->LoadIcon(id);
}

int IconManager::get_index(UINT icon) const
{
	Iconmap::const_iterator indx;
	indx = indexmap.find(icon);
	return indx == indexmap.end() ? -1 : indx->second;
}

void IconManager::set_index(UINT icon, UINT index)
{
	it_indexmap = indexmap.find(icon);
	if(it_indexmap == indexmap.end())
	{
		indexmap.insert(make_pair(icon, index));
	}
	else
		it_indexmap->second = index;
}

void IconManager::decrUsage(UINT id)
{
	GdiObjectManager<HICON>::decrUsage(id);
	it_usage = usagemap.find(id);
	
	if (it_usage == usagemap.end())
		indexmap.erase(id);
}