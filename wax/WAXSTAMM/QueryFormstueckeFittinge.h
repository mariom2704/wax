#pragma once

#include "QueryBase.h"
#include "FormstueckeUndFittinge.h"


namespace WaxStammdaten
{
	class WAXSTAMMDLL QueryFormstueckFittinge : public QueryBase<CObList> 
	{
	public:

		QueryFormstueckFittinge(const CObList& lst) : QueryBase(lst){}
	};
}

