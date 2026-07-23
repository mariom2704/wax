#pragma once


#pragma once


class PreisInsert
{
public:
	PreisInsert(odbc::otl_connect& xdb) : db(xdb)
	{
	}

	void insert(std::string code, int artikelnummer, int region, double preis)
	{
		if (artikelnummer)
			code = "-";
		remove(code, artikelnummer);
		odbc::otl_stream i(50, "INSERT INTO preise (code, artikelnr, region, preis) VALUES (:f1<char[255]>, :f2<int>, :f3<int>, :f4<double>)", db );
		
		i << code.c_str() << artikelnummer << region << preis;
	}

	
private:
	void remove(const std::string& code, int artikelnummer)
	{
		odbc::otl_stream i(50, "DELETE FROM preise WHERE code = :f1<char[255]> AND artikelnr = :f2<int>", db );
		
		i << code.c_str() << artikelnummer;
	}

	odbc::otl_connect& db;
};