#pragma once

#ifdef _DEBUG
#include <mxutil/tracer.h>
#endif

#include <mxutil/numericformatter.h>

namespace MaterialStamm
{
	class Data
	{
	public:
		Data() : artikelnr(0), preis(0.0), region(0), serviceaufschlag(0)
		{
			source = "SAP";
		}

		Data(int anr, int reg, double pr, const std::string& kt, const std::string& kt_en)
		{
			set_artikelnr(anr);
			set_region(reg);
			set_preis(pr);
			set_kurztext(kt);
			set_kurztext_en(kt_en);
			set_serviceaufschlag(0.0);
			source = "SAP";
		}

		Data(int anr, int reg, double pr, double serviceaufschlag )
		{
			set_artikelnr(anr);
			set_region(reg);
			set_preis(pr);
			set_serviceaufschlag(serviceaufschlag);
			source = "SAP";
		}

		Data(const Data& other)
		{
			copy(other);
		}

		Data& operator = (const Data& other)
		{
			copy(other);
			return *this;
		}

		int get_region() const
		{
			return region;
		}

		void set_region(int val)
		{
			region = val;
		}
		
		int get_artikelnr() const
		{
			return artikelnr;
		}

		void set_artikelnr(int val)
		{
			artikelnr = val;
		}

		std::string get_kurztext() const
		{
			return kurztext;
		}

		void set_kurztext(const std::string& val)
		{
			kurztext = val;
		}

		std::string get_kurztext_en() const
		{
			return kurztext_en;
		}

		void set_kurztext_en(const std::string& val)
		{
			kurztext_en = val;
		}

		double get_preis() const
		{
			return preis;
		}

		std::string get_preis_formatted() const
		{
			return MxUtil::NumericFormatter().to_string_locale(get_preis());
		}

		void set_preis(double val)
		{
			preis = val;
		}
		
		void set_serviceaufschlag(double val)
		{
			serviceaufschlag = val;
		}

		double get_serviceaufschlag() const
		{
			return serviceaufschlag;
		}

		std::string get_source() const
		{
			return source;
		}

#ifdef _DEBUG
		void Dump() const
		{
			std::string out = boost::str(boost::format("Region:%i, Artikelnr: %i, Preis: %s")
				% get_region() % get_artikelnr() % get_preis_formatted() );
			MxUtil::Tracer::trace(out);
		}
#endif
	protected:
			std::string source;
	private:

		int region;
		int artikelnr;
		double preis;
		double serviceaufschlag;
		std::string kurztext;
		std::string kurztext_en;
		

		void copy(const Data& other)
		{
			set_region(other.get_region());
			set_artikelnr(other.get_artikelnr());
			set_preis(other.get_preis());
			set_kurztext(other.get_kurztext());
			set_kurztext_en(other.get_kurztext_en());
			set_serviceaufschlag( other.get_serviceaufschlag() );
			source = other.get_source();
		}

	};

	class SpanxData : public Data
	{
	public:
		SpanxData() : Data()
		{
			source = "Spanx";
		}

		SpanxData(int anr, int reg, double pr, const std::string& kt, const std::string& kt_en)
			: Data(anr, reg, pr, kt, kt_en)
		{
			source = "Spanx";
		}

		SpanxData(int anr, int reg, double pr, double serviceaufschlag )
			: Data(anr, reg, pr, serviceaufschlag)
		{
			source = "Spanx";
		}
	};
}