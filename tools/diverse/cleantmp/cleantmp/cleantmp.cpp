// cleantmp.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <tools/boost.h>

#include <string>



#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <iostream>
#include <vector>

namespace fs = boost::filesystem;
using namespace std;
using namespace boost;

vector<string> simple_ls( string lspath );

int _tmain(int argc, _TCHAR* argv[])
{
	vector<string> temps;
	temps.push_back("c:\\tmp");
	temps.push_back("c:\\temp");
	temps.push_back("d:\\tmp");
	temps.push_back("d:\\temp");
	
	for (vector<string>::iterator tempdir = temps.begin(); tempdir != temps.end(); tempdir++)
	{
		vector<string> files_and_dirs = simple_ls(*tempdir);

		for (vector<string>::iterator it = files_and_dirs.begin(); it != files_and_dirs.end(); it++)
		{
			string fullpath = *tempdir + "\\" + *it;
			try
			{
				fs::remove_all(fullpath);
			}
			catch (const exception& error)
			{
				cout << "Fehler [" << error.what() << "]:";
			}
			cout << fullpath << endl;
		}
	}
	return 0;
}



vector<string> simple_ls(string lspath)
{
vector<string> ret;
  boost::progress_timer t( std::clog );

  fs::path full_path( fs::initial_path<fs::path>() );

  full_path = fs::system_complete( fs::path(lspath, fs::native ) );
  
  unsigned long file_count = 0;
  unsigned long dir_count = 0;
  unsigned long other_count = 0;
  unsigned long err_count = 0;

  if ( !fs::exists( full_path ) )
  {
    std::cout << "\nNot found: " << full_path.native_file_string() << std::endl;
    return ret;
  }

  if ( fs::is_directory( full_path ) )
  {
    std::cout << "\nIn directory: "
              << full_path.native_directory_string() << "\n\n";
    fs::directory_iterator end_iter;
    for ( fs::directory_iterator dir_itr( full_path );
          dir_itr != end_iter;
          ++dir_itr )
    {
      try
      {
        if ( fs::is_directory( dir_itr->status() ) )
        {
          ++dir_count;
         // std::cout << dir_itr->path().leaf() << " [directory]\n";
		  ret.push_back(dir_itr->path().leaf());
        }
        else if ( fs::is_regular( dir_itr->status() ) )
        {
          ++file_count;
         // std::cout << dir_itr->path().leaf() << "\n";
		  ret.push_back(dir_itr->path().leaf());
        }
        else
        {
          ++other_count;
          std::cout << dir_itr->path().leaf() << " [other]\n";
        }

      }
      catch ( const std::exception & ex )
      {
        ++err_count;
        std::cout << dir_itr->path().leaf() << " " << ex.what() << std::endl;
      }
    }
    std::cout << "\n" << file_count << " files\n"
              << dir_count << " directories\n"
              << other_count << " others\n"
              << err_count << " errors\n";
  }
  else // must be a file
  {
    std::cout << "\nFound: " << full_path.native_file_string() << "\n";    
  }
  return ret;
}

