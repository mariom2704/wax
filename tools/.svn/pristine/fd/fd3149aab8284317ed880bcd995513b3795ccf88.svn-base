#pragma once

#include "../../../spanx/SpanxDb/Database.h"
#include "../../../spanx/SpanxDb/Materialstamm.h"
#include <boost/boost.h>

#include "../MaterialStamm/Data.h"

#include "PreisLogger.h"


namespace DB
{
	class WaxStammdb
	{
	public:
		WaxStammdb(const std::string& fn)
		{
			dbfile = fn;
		}

		boost::signal<void (int,int)> signal_percent_updated;

		bool update(const std::vector<MaterialStamm::Data>& data)
		{
			SpanxDb::Database db(dbfile);

			if (!db.open())
				return false;

			DB::PreisLogger log;

			int cnt = 1;
			for (std::vector<MaterialStamm::Data>::const_iterator it = data.begin(); it != data.end(); it++, cnt++)
			{
				log.appendwax(db, *it);

				remove_preis(db, *it);
										
				if (!insert_preis(db, *it))
					break;
		
				int percent = (int)((double)cnt / (double)data.size()* 100.0);
				signal_percent_updated(percent, it->get_artikelnr());
			}
			if (db.get_errorhandler().get_count())
			{
				error = *db.get_errorhandler().get_errors().begin();
				return false;
			}
			return true;
		}

		MxBase::Error error;

	private:

		bool remove_preis(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql =
					boost::str(boost::format("DELETE FROM SpanxPreise WHERE Materialstamm_ArtNr = %i AND Region = %i") % dat.get_artikelnr() % dat.get_region());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		bool insert_preis(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql = 
					boost::str(boost::format("INSERT INTO SpanxPreise (Materialstamm_ArtNr, Region, Preis) VALUES (%i , %i, %.6f)") % dat.get_artikelnr() % dat.get_region() % dat.get_preis());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		
		std::string dbfile;
	};
}