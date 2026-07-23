#pragma once


namespace MkStammDb
{
	namespace DB
	{
		class PreisCopier
		{
		public:
			PreisCopier(MXDatabase* spanxdatabase, MXDatabase* stammdatabase)
			{
				spanxdb = spanxdatabase;
				stammdb = stammdatabase;
				create_table();
			}

			~PreisCopier()
			{
				create_indices();
			}

			void copy(const std::string& code) const
			{
				std::string artnr_str = spanxdb->exec_SQL("SELECT ArtikelNummer FROM PreislistenArtikel where code = '" + code + "'");

				int artnr = artnr_str.length() > 3  ? atoi(artnr_str.c_str()) : 0;

				std::string sql_where;
				if (artnr)
					sql_where = " artikelnr = " + artnr_str;
				else 
					sql_where = " code = '" + code + "'";
				
				std::string sql = "SELECT code, artikelnr, region, preis FROM preise WHERE " + sql_where;

				spanxdb->set_exec_limiter(";");
				std::string res = spanxdb->exec_SQL(sql);

				if (!res.length())
					return;
				
				std::vector<std::string> vallist;
				boost::algorithm::split(vallist, res, boost::algorithm::is_any_of(";"));

				if (vallist.size() < 4)
					return;
				
				for (unsigned int i = 0; vallist.size() >= (i+4); i+=4)
				{
					std::string sql_insert = 
						boost::str(boost::format("INSERT INTO Preise (code, artikelnr, region, preis) VALUES('%s', %s, %s, %s)") 
						% vallist.at(i) % vallist.at(i+1) % vallist.at(i+2) % vallist.at(i+3));
					stammdb->exec_SQL(sql_insert);
				}
			}

		private:

			void create_table() const
			{
				const std::string sql[] = 
				{
					"DROP TABLE Preise",
					"CREATE TABLE preise (code VARCHAR(255), artikelnr INTEGER, region INTEGER, preis DOUBLE, PRIMARY KEY(code, artikelnr, region))",
					"",
				};
				for (int i = 0; sql[i].length(); i++)
					stammdb->exec_SQL(sql[i]);

			}

			void create_indices() const
			{
				const std::string sql[] = 
				{
					"CREATE INDEX index_code ON preise(code)",
					"CREATE INDEX index_artikelnr ON preise(artikelnr)",
					"CREATE INDEX index_region ON preise(region)",
					"CREATE INDEX index_artregion ON preise(artikelnr, region)",
					"CREATE INDEX index_coderegion ON preise(code, region)",
					"",
				};
				for (int i = 0; sql[i].length(); i++)
					stammdb->exec_SQL(sql[i]);

			}

			MXDatabase* spanxdb;
			MXDatabase* stammdb;

		};

	}
}