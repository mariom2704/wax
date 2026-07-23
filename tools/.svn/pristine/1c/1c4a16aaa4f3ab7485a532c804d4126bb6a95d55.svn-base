#pragma once

#include "mxodbc/mxdatabase.h"
#include "../helpers/TextFile.h"

namespace MkStammDb
{
	namespace DB
	{
		class KatalogCreator
		{
		public:
			KatalogCreator(MXDatabase* database, const std::string& path_to_spl)
			{
				db = database;
				dir = path_to_spl;
				db->errhandler.clear();
				create_table();
			}


			~KatalogCreator()
			{
				::SQLTransact(AfxGetHENV(), db->m_hdbc, SQL_COMMIT);
			}
			

			MxBase::Error run()
			{
				if (db->errhandler.get_count())
					return *db->errhandler.get_errors().begin();
	

				std::vector<std::string> spanx_spl = Helpers::TextFile(dir + "\\spanx.spl").load();

				std::string insert = "INSERT INTO Katalog (lfdnr, splname, spldatei) VALUES (%i, '%s', '%s')";

				int lfdnr = 1;
				for (std::vector<std::string>::iterator it = spanx_spl.begin(); it != spanx_spl.end(); it++)
				{
					std::string filename = boost::algorithm::to_lower_copy(*it);
					boost::algorithm::trim(filename);

					const std::string tag("#include");
					if (filename.substr(0, tag.length()) != tag)
						continue;
					
					filename = filename.substr(tag.length(), filename.length()-tag.length());
					boost::algorithm::trim(filename);

					std::string file = dir + "\\" + filename;

					std::string content = Helpers::TextFile(file).load_str();

					std::string sql = boost::str(boost::format(insert) % lfdnr % filename % content);
					db->exec_SQL(sql, sql.length() + 1);
					lfdnr++;
				}
				return MxBase::Error();
			}

		private:

			void create_table()
			{
				db->exec_SQL("DROP TABLE Katalog");
				db->errhandler.clear(); // Fehler "Katalog existiert nicht" ignorieren
				db->exec_SQL("CREATE TABLE Katalog (lfdnr INT, splname varchar(254),spldatei MEMO,  PRIMARY KEY (lfdnr))");
			}

			MXDatabase* db;
			std::string dir;
		};
	}
}