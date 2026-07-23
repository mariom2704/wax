#pragma once
#include "ccbase.h"

class CcFehlermeldungen :
	public CcBase
{
public:
	CcFehlermeldungen(void);
	virtual ~CcFehlermeldungen(void);

	bool readFromDb(EnError &error);
};
