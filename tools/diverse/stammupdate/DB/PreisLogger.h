#pragma once

#include <string>
#include <boost/boost.h>
#include <fstream>
#include <mxutil/numericformatter.h>
#include <mxutil/tracer.h>
#include <mxbase/date.h>
#include "../../../spanx/SpanxDb/Database.h"
#include "../../../spanx/SpanxDb/Preise.h"

#include "../MaterialStamm/Data.h"

namespace DB
{
	/**
	Logfile über Preisänderungen erstellen.
	*/
#define LOGFILE "preislog.txt"

	class PreisLogger
	{
	public:
		PreisLogger()
		{
			std::string dt =  MxBase::Date().get_current().to_string(MxBase::Locale());
			MxUtil::Tracer::trace2file(LOGFILE, "---------Preisupdate vom " + dt + " ----------");
		}

		~PreisLogger()
		{
			std::string dt =  MxBase::Date().get_current().to_string(MxBase::Locale());
			MxUtil::Tracer::trace2file(LOGFILE, "---------Ende Preisupdate vom " + dt + " ----------");
		}

		void append(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			SpanxDb::PreiseTab pr(db);
			MxUtil::NumericFormatter nf;

			std::string val1 = "Neu";

			if (pr.load(dat.get_artikelnr(), dat.get_region()))
				val1 = nf.to_string_locale(pr.get_preis());

			std::string val2 = nf.to_string_locale(dat.get_preis());

			if (val1 != val2)
			{
				std::string log = boost::str(boost::format("Artikelnr %i : Preis %s => %s, region = %i") % dat.get_artikelnr() % val1 % val2 % dat.get_region());
				MxUtil::Tracer::trace2file(LOGFILE, log);
			}
		}

		void appendwax(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			SpanxDb::PreiseTab pr(db,"SpanxPreise");
			MxUtil::NumericFormatter nf;

			std::string val1 = "Neu";

			if (pr.load(dat.get_artikelnr(), dat.get_region()))
				val1 = nf.to_string_locale(pr.get_preis());

			std::string val2 = nf.to_string_locale(dat.get_preis());

			if (val1 != val2)
			{
				std::string log = boost::str(boost::format("Artikelnr %i : Preis %s => %s, region = %i") % dat.get_artikelnr() % val1 % val2 % dat.get_region());
				MxUtil::Tracer::trace2file(LOGFILE, log);
			}
		}

		void append_service(SpanxDb::Database& db, const MaterialStamm::Data& dat)
		{
			SpanxDb::PreiseTab pr(db);
			MxUtil::NumericFormatter nf;

			std::string log = boost::str(boost::format("Artikelnr %i : Serviceaufschlag %.2f") % dat.get_artikelnr() % dat.get_serviceaufschlag() );
			MxUtil::Tracer::trace2file(LOGFILE, log);
		}
	};
}