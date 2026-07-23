#include "stdafx.h"

#include "CreateSpl.h"
#include <SystemParameter.h>

#include <boost/boost_filesystem.h>

using namespace std;
using namespace boost::filesystem;

using namespace Merge;

CreateSpl::CreateSpl(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu) 
	: MergeBase(spanxdb_alt, spanxdb_neu)
{
	splpath_old = SpanxDb::SystemParameter(db_alt).val("DIR_SPL", "");
	splpath_new = SpanxDb::SystemParameter(db_neu).val("DIR_SPL", "");

	load_cache();
}

std::string CreateSpl::run()
{
	if (!check_paths())
		return info;

	create_filelist();

	convert_files();

	return info;
}

void CreateSpl::convert_files()
{
	for (std::vector<string>::iterator it = filelist.begin(); it != filelist.end(); it++)
		process_file(*it);
}

void CreateSpl::process_file(string file)
{
	fstream in((splpath_old + "\\" + file).c_str(), std::fstream::in);
	fstream out((splpath_new + "\\" + file).c_str(), std::fstream::out);
		
	char buf[1024];
	try
	{
		while (1)
		{
			in.getline(buf, 1024);
			if (in.eof())
				break;
			string str = buf;
			
			bool link = false;
			if (str.substr(0, 2) == "=>")
				link = true;

			
			int artnr = search_cache(link ? str.substr(2, str.length()-2) : str);
			if (artnr)
			{
				str = boost::str(boost::format("%i\r\n") % artnr);
				if (link)
					str = "=>" + str;
			}
			else
				str += "\r\n";
			
			out.write(str.c_str(), (std::streamsize)str.length() ); 
		}
	}
	catch(std::exception& e)
	{
		throw MxBase::Error(e.what(), 0, MxBase::Error::Critical);
	}
	catch(...)
	{
		throw MxBase::Error("Unbekannter Fehler bei SPL Erstellung!",0,MxBase::Error::Critical);
	}
}

bool CreateSpl::check_paths()
{
	string paths[] = { splpath_old, splpath_new, "" };

	string notfound;

	for (int i = 0; paths[i].length(); i++)
	{
		path dir_path(paths[i]);
		
		if ( !exists( dir_path ) ) 
			notfound = "Nicht gefunden: " + paths[i] + "\n";
	}
	info += notfound;

	return !notfound.length();
}


void CreateSpl::create_filelist()
{
	filelist.clear();

	path dir_path(splpath_old);
	directory_iterator end_itr; // default construction yields past-the-end

	for (directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		try
		{
			if ( !is_directory(itr->status()) )
			{
				
				string f = boost::algorithm::to_lower_copy(itr->path().leaf());

				if (f.substr(f.length()-3, 3) == "spl")
					filelist.push_back(f);
			}
		}
		catch ( const std::exception & ex )
		{
			throw MxBase::Error(itr->path().filename() + " " + ex.what());
		}
	}
}


