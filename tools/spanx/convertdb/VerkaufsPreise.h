#pragma once

#include <Database.h>
#include <mxodbc2/Table.h>

namespace AltTabellen
{	
	class VerkaufsPreise : public MxOdbc::Table
	{
	public:
		VerkaufsPreise(SpanxDb::Database& db) : MxOdbc::Table("Verkaufspreise", db)
		{
			add_column<std::string>("code");
			add_column<double>("preis");
			add_column<std::string>("doc");
		}

		bool load(const std::string c)
		{
			std::string sql = "code = '" + c + "'";
			if (!start_fetch(sql))
				return false;
			if (!fetch_next())
				return false;
			return true;
		}

		GETSET_STRING(code);
		GETSET_DOUBLE(preis);
		GETSET_STRING(doc);
	};


}