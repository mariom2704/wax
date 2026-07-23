#pragma once

#include <Database.h>
#include <mxodbc2/Table.h>

namespace TmpTabellen
{
	class CodeArtikelnummer_Zuordnung : public MxOdbc::Table
	{
	public:
		CodeArtikelnummer_Zuordnung(SpanxDb::Database& db) : MxOdbc::Table("CodeArtikelnummer_Zuordnung", db)
		{
			add_column<std::string>("Code", true);
			add_column<int>("ArtNr");
			add_column<std::string>("Gruppe");
		}

		GETSET_STRING(Code);
		GETSET_INT(ArtNr);
		GETSET_STRING(Gruppe);

		std::string query_code(int artnr)
		{
			std::string sql = boost::str(boost::format("ArtNr = %i") % artnr);
			if (!start_fetch(sql))
				throw error;

			fetch_next();
			return get_Code();
		}

		int query_artnr(const std::string& code)
		{
			std::string sql = boost::str(boost::format("Code = '%s'") % code);
			if (!start_fetch(sql))
				throw error;

			fetch_next();
			return get_ArtNr();
		}
	};




}