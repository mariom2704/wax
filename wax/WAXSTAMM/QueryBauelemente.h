#pragma once

#include "QueryBase.h"
#include "Bauelemente.h"


namespace WaxStammdaten
{
	class WAXSTAMMDLL QueryBauelemente : public QueryBase<CObList> 
	{
	public:

		QueryBauelemente(const CObList& lst) : QueryBase(lst){}
	};
}

