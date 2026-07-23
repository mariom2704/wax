#pragma once

#include "MergeBase.h"
#include "../PreislistenArtikel.h"

namespace Merge
{
	class MergeMaterialstamm : public MergeBase
	{
	public:
		MergeMaterialstamm(SpanxDb::Database& spanxdb_alt, SpanxDb::Database& spanxdb_neu);

		virtual ~MergeMaterialstamm();

		std::string run();

	private:
		void cache_artnr(int artnr, const std::string& code);
		
		void create_sap_artnr(int anr);

		int extract_artnr(const std::string& artnr) const;

		void insert_artikel(AltTabellen::PreislistenArtikel& art);
		
		void process_artikel();
	
		SpanxDb::Database& get_correct_db(int artnr) const;

		int create_artnr(const std::string& code);
		
		void create_stk_entry(int artnr_komp, int artnr);

		bool exists(int artnr);

		void update_val(int artnr, AltTabellen::PreislistenArtikel& art);

		SpanxDb::Database* stammdb;
	};
}
