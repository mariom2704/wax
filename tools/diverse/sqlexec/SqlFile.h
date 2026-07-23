#pragma once

#include <mxbase/error.h>

namespace SqlExec
{
	class SqlFile
	{
	public:
		SqlFile(const std::string& fn) : fin(fn.c_str(), std::fstream::in)
		{
		}

		boost::signal<void (const std::string&)> signal_comment;
		boost::signal<void (const std::string&)> signal_sql;

		bool is_open() const
		{
			return fin.is_open();
		}

		void process()
		{
			char sql[2048];
			std::string sqlexec;

			while (!fin.eof())
			{
				fin.getline(sql, 2048);
				
				sqlexec += sql;
				boost::algorithm::trim(sqlexec);
							
				if (!sqlexec.length())
					continue;
				
				if (sqlexec.at(0) == '#')
				{			
					signal_comment(sqlexec);
					sqlexec.clear();
					continue;
				}
				
				try
				{
					if (sqlexec.at(sqlexec.length()-1) == ';')
					{	
						signal_sql(sqlexec);
						signal_comment(sqlexec + " -> OK");
						sqlexec.clear();
					}
					else sqlexec += " "; // Statt CR ein Leerzeichen
				}
				catch (MxBase::Error e)
				{
					signal_comment(sqlexec + (e.is_critical() ? " -> Kritischer Fehler" : " -> Unkritischer Fehler"));
					if (e.is_critical())
						throw e;
					sqlexec.clear();
				}
			}
		}
	

	private:
		std::fstream fin;

	};
}