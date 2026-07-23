#pragma once

#include <mxodbc2/Database.h>
#include <mxodbc2/Connection.h>

namespace SqlExec
{

	class SqlExecutor
	{
	private:
		static MxOdbc::Database* xdb;

		
		bool is_critical(std::string& sql) const
		{
			/** Prüft eine SQL Anweisung auf ein ! am Anfang. 
			Damit wird angezeigt, dass ein Fehlschlag der Anweisung zum Abbruch des Programms
			führen und ein Rollback veranlassen soll. 

			Beim SQL String wird das ! entfernt
			*/

			if (sql[0] == '!')
			{
				sql = sql.substr(1, sql.length()-1);
				return true;
			}
			return false;
		}


	public:

		static void set_db(MxOdbc::Database* db)
		{
			xdb = db;
		}
	
		void exec(const std::string& sql)
		{
			std::string sqlstmt = sql;
			if (!xdb)
				throw MxBase::Error("SqlExecutor nicht initialisiert", 0, MxBase::Error::Critical);
			
			xdb->get_errorhandler().clear();

			bool critical = is_critical(sqlstmt);

			xdb->exec_sql(sqlstmt);
			
			if (xdb->get_errorhandler().get_count())
			{
				MxBase::Error e = xdb->get_errorhandler().get_errors().back();
				if (critical)
					e.set_typ(MxBase::Error::Critical);
				throw e;
			}
		}
	};

	MxOdbc::Database* SqlExecutor::xdb = NULL;
}