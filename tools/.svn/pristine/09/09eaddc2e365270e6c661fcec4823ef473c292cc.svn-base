#pragma once

#include "MergeBase.h"

#include <string>
#include <vector>
#include <map>

namespace Merge
{
	/**
	Alle Vortexte der Tabelle PreislistenArtikel (alte DB) werden in die Tabelle Vortexte (neue DB)
	kopiert, bereits vorhandene werden überschrieben, neue werden erstellt. Es werden keine Daten gelöscht.
	*/
	class CreateSpl : public MergeBase
	{
	public:
		CreateSpl(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu);

		std::string run();

		void process_file(std::string file);

	private:
		bool check_paths();

		void create_filelist();

		void convert_files();

		void fill_cache();

		std::vector<std::string> filelist;

		std::string splpath_new;
		std::string splpath_old;

		std::map<std::string, int> code2artnr;
	};
}
