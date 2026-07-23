#pragma once

#include "../../../spanx/SpanxDb/Database.h"
#include "../../../spanx/SpanxDb/Materialstamm.h"
#include <boost/boost.h>

#include "../MaterialStamm/Data.h"

#include "PreisLogger.h"


namespace DB
{

	class MatstammSize : public MxOdbc::Table
	{
	public:
		MatstammSize(MxOdbc::Database& db) : MxOdbc::Table("Materialstamm", db)
		{
			add_column<int>("Count");
		}

		bool load()
		{
			error.set_typ(MxBase::Error::NoError);
			
			if (!db.is_valid())
			{
				std::string msg = "Database invalid!";

				if (db.get_errorhandler().get_count())
					msg += "\n" + db.get_errorhandler().get_errors().begin()->get_description();
				error.set_description(msg);
				error.set_typ(MxBase::Error::Critical);
				return false;
			}

			try
			{
				std::string select = "SELECT Count(*) FROM Materialstamm";
				odbc_stream = new odbc::otl_stream(10, select.c_str(), db.get_db());
			}
			catch(odbc::otl_exception& p)
			{ 
				set_error_from_exception(p);
				reset_odbc_stream();
				return false;
			}

			fetch_next();

			return true;
		}
		
		int get_count() const
		{
			return get_col("Count")->to_int();
		}

	};

	class SpanxStammdb
	{
	public:
		SpanxStammdb(const std::string& fn)
		{
			dbfile = fn;
		}

		const std::vector<MaterialStamm::Data>& get_data() const
		{
			return poslist;
		}
		
		boost::signal<void (int)> signal_percent_loaded;
		boost::signal<void (int,int)> signal_percent_updated;

		void load()
		{
			SpanxDb::Database db(dbfile);

			if (!db.open())
				return;

			MatstammSize mz(db);
			mz.load();
			int count = mz.get_count();

			SpanxDb::MaterialstammEx mx(db);


			mx.start_fetch();

			poslist.clear();

			int cnt = 0;
			while (mx.fetch_next())
			{
				poslist.push_back(MaterialStamm::SpanxData(mx.get_artnr(), 0, 0.0, mx.get_kurztext(), mx.get_KurzTextEnglish()));
				int percent = (int)((double)cnt++ / (double)count*100.0);
				signal_percent_loaded(percent);
			}
		}

		
		bool update(const std::vector<MaterialStamm::Data>& data)
		{
			SpanxDb::Database db(dbfile);

			if (!db.open())
				return false;

			DB::PreisLogger log;

			int cnt = 1;
			for (std::vector<MaterialStamm::Data>::const_iterator it = data.begin(); it != data.end(); it++, cnt++)
			{
				log.append(db, *it);

				if (!exists(db, *it))
				{
					if (!insert(db, *it))
						break;
				}
				else if (!remove_preis(db, *it))
					break;
						
				if (!insert_preis(db, *it))
					break;
		
				update_kurztexte(db, *it);
			
				if (it->get_serviceaufschlag() > 0.01)
				{
					log.append_service(db, *it);
					update_service(db, *it);
				}
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
		/// Liste mit Daten aus der Datenbank
		std::vector<MaterialStamm::Data> poslist;

		bool exists(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			SpanxDb::Materialstamm mat(db);
			if (!mat.load(dat.get_artikelnr()))
				return false;

			if (mat.get_kurztext().length() < 3 && dat.get_kurztext().length() > 3)
			{
				mat.set_kurztext(dat.get_kurztext());
				mat.update();
			}
			return true;
		}

		bool remove_preis(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql =
					boost::str(boost::format("DELETE FROM Preise WHERE Materialstamm_ArtNr = %i AND Region = %i") % dat.get_artikelnr() % dat.get_region());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		bool insert_preis(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql = 
					boost::str(boost::format("INSERT INTO Preise (Materialstamm_ArtNr, Region, Preis) VALUES (%i , %i, %.6f)") % dat.get_artikelnr() % dat.get_region() % dat.get_preis());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		bool update_kurztexte(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql = 
				boost::str(boost::format("UPDATE Materialstamm SET KurztextEnglish = '%s' WHERE KurztextEnglish IS NULL AND ArtNr = %i") % dat.get_kurztext_en() % dat.get_artikelnr());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		bool update_service(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql = 
			boost::str(boost::format("UPDATE Materialstamm SET WM_Service = %.2f, Sparte = 8 WHERE ArtNr = %i") % dat.get_serviceaufschlag() % dat.get_artikelnr());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}
		

		bool insert(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			std::string sql = 
				boost::str(boost::format("INSERT INTO Materialstamm (ArtNr,  "
										 "KurzText, KurzTextEnglish, KurzTextFrancais, KurzTextNederlands, Montagezeit) "
										 "VALUES(%1%, '%2%', '%2%', '%2%', '%2%', 0.0)" ) % dat.get_artikelnr() % dat.get_kurztext());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		std::string dbfile;
	};
}