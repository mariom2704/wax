#include "stdafx.h"

#include "TestGruppen.h"


#include <Database.h>
#include <Gruppen.h>
#include <Listen.h>
#include <Materialstamm.h>
#include <Katalog.h>
#include <map>

using namespace SpanxDb;
using namespace std;

TestGruppen::TestGruppen(const std::string& db) : dbname(db)
{
}

bool TestGruppen::do_test(bool fixit)
{
	Database db(dbname);

	if (!db.open())
	{
		error = db.get_first_error().get_description();
		return false;
	}

	Gruppen grp(db);

	if (!grp.start_fetch())
	{
		error = grp.error.get_description();
		return false;
	}

	std::string verwaist = "Verwaiste Gruppe";
	std::string verwaist3 = "Listenzuordnung nicht korrekt";
	while (grp.fetch_next())
	{
		MaterialstammKurz ma(db, MxBase::Locale());

		std::string sql = "Gruppe = '" + grp.get_code() + "'";
		if (!ma.start_fetch(sql))
		{
			error = ma.error.get_description();
			return false;
		}
		if (!ma.fetch_next())
		{
			result.insert( make_pair(verwaist, grp.get_code()) );
		}

		Listen lst(db, MxBase::Locale());

		if ( !lst.load(grp.get_liste()) )
			result.insert( make_pair(verwaist3, grp.get_code()) );


	}

	Katalog cat(db);
	
	std::string katalog;
	for (cat.start_fetch(); cat.fetch_next(); )
	{
		katalog += cat.get_spldatei()+ "\r\n";
	}

	if (katalog.empty())
	{
		error = "Kein Katalog erstellt!";
		return false;
	}
	std::string verwaist2 = "Auch kein Eintrag in Katalog";

	for (multimap<string, string>::iterator it = result.begin(); it != result.end(); it++)
	{
		if (katalog.find(it->second) == string::npos)
			result.insert( make_pair(verwaist2, it->second) );
	}

	if (fixit)
	{
		for (multimap<string, string>::iterator it = result.begin(); it != result.end(); it++)
		{
			if (it->first != verwaist2)
				continue;

			Gruppen grp(db, MxBase::Locale());
			grp.load(it->second);

			if (!grp.remove())
			{
				error = grp.error.get_description();
				return false;
			}
		}

		for (multimap<string, string>::iterator it = result.begin(); it != result.end(); it++)
		{
			if (it->first != verwaist3)
				continue;

			Gruppen grp(db, MxBase::Locale());
			grp.load(it->second);

			string::size_type posgrp = katalog.find("^ " + it->second);
			if ( posgrp == string::npos )
				continue;

			string::size_type lstpos = katalog.rfind("^^", posgrp);

			if ( lstpos == string::npos )
				continue;

			string::size_type lstpos2 = katalog.find("\r\n", lstpos);
			if ( lstpos2 == string::npos )
				continue;

			if (lstpos2-lstpos <= 0)
				continue;
			string list = katalog.substr(lstpos+2, lstpos2-lstpos);

			boost::algorithm::trim(list);
			if (list.empty())
				continue;

			grp.set_liste(list);
			if (!grp.update())
			{
				error = grp.error.get_description();
				return false;
			}
		}
	}
	return true;
}





std::multimap<std::string, std::string> TestGruppen::get_result() const
{
	return result;
}

std::string TestGruppen::get_error() const
{
	return error;
}