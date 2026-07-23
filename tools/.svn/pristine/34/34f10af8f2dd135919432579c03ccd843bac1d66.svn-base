#pragma once


#include <fstream>
#include <boost/boost.h>
#include <boost/boost_filesystem.h>

#include "TmpTabellen.h"

namespace Action
{

	class ReplaceCodesInSpl
	{
	public:
		ReplaceCodesInSpl(SpanxDb::Database& sdb, const std::string directory, const std::string outputdir) : path(directory), outpath(outputdir), db(sdb)
		{
		}

		void run()
		{
			find_files();
			process_files();
		}

	private:

		void find_files()
		{
			files.clear();
			if (!boost::filesystem::is_directory(path))
				return;
			boost::filesystem::directory_iterator end_iter;
			for ( boost::filesystem::directory_iterator dir_itr(path); dir_itr != end_iter; dir_itr++ )
			{
				try
				{
					if (boost::filesystem::is_directory(dir_itr->status()))
						continue;

					if (boost::filesystem::is_regular_file(dir_itr->status()))
					{
						std::string fn = dir_itr->path().filename();
						for (std::string::iterator it = fn.begin(); it != fn.end(); it++)
							*it = tolower(*it);
						if (fn.find(".spl") == std::string::npos || fn.find(".lock") != std::string::npos)
							continue;

						boost::algorithm::replace_all(fn, ".spl", ".txt");
						files.insert(std::make_pair<std::string, std::string>(dir_itr->path().filename(), fn));
					}
				}
				catch ( const std::exception & ex )
				{
					throw MxBase::Error(dir_itr->path().filename() + " " + ex.what());
				}
			}
		}

		void process_files() const
		{
			int cnt = 0;
			for (FileMap::const_iterator it = files.begin(); it != files.end(); it++)
			{
				std::fstream in(std::string(path.string() + "\\" + it->first).c_str(), std::fstream::in);
				std::fstream out((outpath.string() + "\\" + it->second).c_str( ), std::fstream::out);
					
				char buf[1024];
				try
				{
					while (1)
					{
						in.getline(buf, 1024);
						if (in.eof())
							break;
						std::string str = exchange_code(buf) + (std::string)"\n";
						out.write(str.c_str(), (std::streamsize)str.length() ); 
					}
				}
				catch(std::exception& e)
				{
					throw MxBase::Error(e.what(), 0, MxBase::Error::Critical);
				}
				catch(MxBase::Error e)
				{
					throw e;
				}
				catch(...)

				{
					throw MxBase::Error("Unbekannter Fehler bei SPL Erstellung!",0,MxBase::Error::Critical);
				}
				cnt ++;
			}
		}

		std::string exchange_code(const std::string& cod) const
		{
			/**
			Liefert für den Code die entsprechende Artikelnummer.
			Ausnahmen sind Listen, Gruppenbezeichnungen, Kommentare (#) oder Vortexte (Code = "Vortext_xxx")
			*/
			if (cod.find("^") != std::string::npos || cod.find("#") != std::string::npos || cod.find("vortext") != std::string::npos )
				return cod;
			if (cod.find("=>") != std::string::npos)
				// Artikelnummer für den Code erfragen und dann wieder Kennung für Verweis hinzufügen
				return "=>" + get_artikelnr(boost::replace_all_copy(cod, "=>", ""));
			return get_artikelnr(cod);
		}

		std::string get_artikelnr(std::string code) const
		{
			boost::algorithm::trim(code);

			if (!code.length())
				return "";

			TmpTabellen::CodeArtikelnummer_Zuordnung tab(db);
			if (!tab.start_fetch("code = '" + code + "'"))
				throw tab.error;

			if (!tab.fetch_next())
			{
				return code;
				//throw MxBase::Error("Code nicht gefunden: " + code, 0, MxBase::Error::Critical);
			}

			return tab.get_col("ArtNr")->to_string();
		}

		boost::filesystem::path path;
		boost::filesystem::path outpath;

		/// Erster Eintrag ist Original Dateiname, zweiter der generierte Dateiname
		typedef std::map<std::string, std::string> FileMap;
		FileMap files;

		SpanxDb::Database& db;
	};
}