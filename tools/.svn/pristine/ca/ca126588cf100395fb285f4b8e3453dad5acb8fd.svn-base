#pragma once



class FetchRegion
{
public:
	FetchRegion(odbc::otl_connect& xdb) : db(xdb)
	{
	}

	int query(int nl, int defaultregion)
	{
		odbc::otl_stream i(50, "select region from niederlassungen where nl = :f1<int> ", db );
		
		i << nl;

		int ret;
		i >> ret;

		if (ret < 2000 || ret > 3000)
			return defaultregion;
		return ret;
	}

private:
	odbc::otl_connect& db;
};