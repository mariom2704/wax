// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CONSTRUCTSATST_3D171F620360_INCLUDED
#define _INC_CONSTRUCTSATST_3D171F620360_INCLUDED

#include "ConstructAbgEle.h"

#include "ConstructElement.h"

class DLLINTERFACE ConstructSatst : 
public ConstructAbgEle  
{
private:

public:
	virtual ~ConstructSatst();

	ConstructSatst();

	virtual void createBeschriftung();

	virtual void createElement();

protected:

};

#endif /* _INC_CONSTRUCTSATST_3D171F620360_INCLUDED */
