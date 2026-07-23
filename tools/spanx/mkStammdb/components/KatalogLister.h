#pragma once

#include "mxodbc/mxdatabase.h"
#include <set>

namespace MkStammDb
{
	namespace DB
	{
		class KatalogLister
		{
		public:
			KatalogLister(MXDatabase* database)
			{
				db = database;
				db->errhandler.clear();
				create_lists();
			}
			
			std::set<std::string> get_lists() const
			{
				return lsts;
			}
			
			std::set<std::string> get_groups() const
			{
				return grps;
			}
			
			std::set<std::string> get_positions() const
			{
				return pos;
			}
			
		private:

			MXDatabase* db;

			std::set<std::string> lsts;
			std::set<std::string> grps;
			std::set<std::string> pos;

			
			/// Liefert die Listen im Katalog
			void create_lists()
			{
				std::vector<std::string> ret;

				std::string sql = "SELECT spldatei FROM Katalog ORDER BY lfdnr";

				db->set_exec_limiter("\r\n");
				db->exec_SQL(sql, ret, 64000);

				for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); it++)
				{
					std::string val = *it;
					std::vector<std::string> lines;
					boost::algorithm::split(lines, val, boost::algorithm::is_any_of("\r\n") ); 

					for (std::vector<std::string>::iterator lit = lines.begin(); lit != lines.end(); lit++)
					{
						std::string val = *lit;
						if (!val.length())
							continue;
						if (val.find("^^") != std::string::npos)
						{
							boost::algorithm::replace_all(val, "^^", "  ");
							boost::algorithm::trim(val);
							lsts.insert(val);
						}
						else if (val.find("^") != std::string::npos)
						{
							boost::algorithm::replace_all(val, "^", " ");
							boost::algorithm::trim(val);
							grps.insert(val);
						}
						else if (val.find("=") != string::npos || val.find(">") != string::npos || val.find("#") != string::npos)
						{
							continue;
						}
						else
						{
							boost::algorithm::trim(val);
							pos.insert(val);
						}
					}
				}
			}

		};
	}
}