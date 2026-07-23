#pragma once


class FetchNl
{
public:
	FetchNl(odbc::otl_connect& xdb) : db(xdb)
	{
	}

	int query(const std::string& user = "user")
	{
		odbc::otl_stream i(50, "SELECT wert from Systemparameter WHERE Name = 'IB_NL_KENNUNG' AND Benutzer = :f1<char[255]>", db );
		
		i << user.c_str();

		char ret[255];
		i >> ret;
		return atoi(ret);
	}

	
private:
	odbc::otl_connect& db;
};