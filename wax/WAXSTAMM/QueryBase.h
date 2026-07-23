#pragma once

namespace WaxStammdaten
{

	template <class T>
	class QueryBase
	{
	public:
		QueryBase(const T& tblcontent) : tbl(tblcontent)
		{
		}

		template <class Dx>
		Dx* query(int artikelnr, int id = 0)
		{
			for (POSITION pos = tbl.GetHeadPosition(); pos != NULL; )
			{
				Dx* dt = (Dx*)tbl.GetNext(pos);
				if (dt->get_artikelnr() == artikelnr)
				{
					if (!id || (dt->get_elemid() == id))
						return dt;
				}
			}
			return NULL;
		}

	protected:
		const T& tbl;
	};
}