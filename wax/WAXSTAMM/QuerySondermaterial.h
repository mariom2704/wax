#pragma once

#include "QueryBase.h"
#include "Sondermaterial.h"


namespace WaxStammdaten
{
	class WAXSTAMMDLL QuerySondermaterial : public QueryBase<const CObList> 
	{
	public:
		QuerySondermaterial(const CObList& lst) : QueryBase(lst){}
		
	};
}
