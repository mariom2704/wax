#pragma once

#include <map>

class FetchPreise
{
public:
	FetchPreise(odbc::otl_connect& xdb) : db(xdb)
	{
	}

	int query(std::map<std::string, double>& codelst, std::map<int, double>& artikelnrlst)
	{
		odbc::otl_stream i(50, "Select Code, ArtikelNummer, Preis from PreislistenArtikel", db );
		
		int cnt = 0;
		char code[255] ;
		char artnr[255] ;
		char preis[255] ;

		while (!i.eof())
		{
			i >> code >> artnr >> preis;

			if (atoi(artnr))
				artikelnrlst.insert(std::make_pair(atoi(artnr), atof(preis)));
			else
				codelst.insert(std::make_pair(code, atof(preis)));

			cnt++;
		}	
		return cnt;
	}

private:
	odbc::otl_connect& db;
};