#pragma once

#include "QueryBase.h"
#include "RohrstutzenBrennzugabe.h"


namespace WaxStammdaten
{
	class WAXSTAMMDLL QueryBrennzugabe : public QueryBase<CObList> 
	{
	public:
		QueryBrennzugabe(const CObList& lst) : QueryBase(lst){}

		int get_brennzugabe(int verteilerdn, int stutzendn) const
		{
			for (POSITION pos = tbl.GetHeadPosition(); pos != NULL; )
			{
				RohrstutzenBrennzugabe::data* dt = (RohrstutzenBrennzugabe::data*)tbl.GetNext(pos);
				if (dt->get_verteilerdn() == verteilerdn && dt->get_stutzendn() == stutzendn)
					return dt->get_brennzugabe();
			}
			return 0;
		}
	};
}