#include "stdafx.h"

#include "TestStklist.h"

#include <Database.h>
#include <Stueckliste.h>

#include <map>

using namespace SpanxDb;

TestStklist::TestStklist(const std::string& db) : dbname(db)
{
}

bool TestStklist::do_test(bool fixit)
{
	Database db(dbname);

	if (!db.open())
	{
		error = db.get_first_error().get_description();
		return false;
	}

	Stueckliste stklist(db);

	if ( !stklist.start_fetch("", "Materialstamm_ArtNr, lfdNr") )
	{
		error = stklist.error.get_description();
		return false;
	}

	int lfdnr = -1;
	int artnr = -1;
	std::map<int, int> resmap;

	bool donummer = false;

	while (stklist.fetch_next())
	{
		if (stklist.get_materialstamm_artnr() != artnr)
		{
			artnr = stklist.get_materialstamm_artnr();
			lfdnr = stklist.get_nr();
			donummer = false;
		}
		else
		{
			if ( (lfdnr == stklist.get_nr()) || donummer) // Nummer doppelt
			{
				donummer = true; //Setzen bis nächste Artikelnummer
				resmap.insert(std::make_pair(artnr, artnr));
				if (fixit)
				{
					stklist.set_nr(lfdnr+1);
					if (!stklist.update())
					{
						error = stklist.error.get_description();
						return false;
					}
				}
			}
			lfdnr = stklist.get_nr();
		}
	}

	for (std::map<int,int>::iterator it = resmap.begin(); it!=resmap.end(); it++)
		result.push_back(it->first);

	return true;
}



std::vector<int> TestStklist::get_result() const
{
	return result;
}

std::string TestStklist::get_error() const
{
	return error;
}