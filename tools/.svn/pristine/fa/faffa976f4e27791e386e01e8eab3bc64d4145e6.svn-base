#pragma once

#include <mxodbc2/Database.h>
//#include <mxodbc/dbconnection.h>

namespace DB
{
	/**
	Prüft eine Datenbank auf Zugehörigkeit zum Spanx oder Wax
	*/
	class Detect
	{
	public:
		Detect(const std::string& fn)
		{
			dbpath = fn;
		}

		enum Type
		{
			Spanx,
			Wax,
			MXDM,
			none
		};

		enum Type get_type() const
		{
			MxOdbc::Database dbmxdm(dbpath, "", "8fdz93b6ye");

			if ( dbmxdm.open() )
			{
				dbmxdm.exec_sql((std::string)"SELECT COUNT(*) FROM ArtikelTexte");

				if (!dbmxdm.get_errorhandler().get_count())
					return MXDM;
			}

			MxOdbc::Database db(dbpath);

			if (!db.open())
				return none;

			db.exec_sql((std::string)"SELECT COUNT(*) FROM Materialstamm");

			if (!db.get_errorhandler().get_count())
				return Spanx;

			db.get_errorhandler().clear();
			
			db.exec_sql((std::string)"SELECT COUNT(*) FROM SpanxPreise");

			if (!db.get_errorhandler().get_count())
				return Wax;
			return none;
		}

	private:
		std::string dbpath;
	};

}