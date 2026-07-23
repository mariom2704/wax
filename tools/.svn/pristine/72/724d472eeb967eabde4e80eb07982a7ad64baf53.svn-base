#pragma once

#include "Data.h"
#include <fstream>
#include <strstream>
#include <boost/boost.h>
#include <boost/boost_signal.h>


namespace MaterialStamm
{
	class SapTextFile
	{
	public:
		SapTextFile(const std::string& filename)
		{
			fn = filename;

		}

		void load()
		{
			split();
		}

		const std::vector<MaterialStamm::Data>& get_data() const
		{
			return poslist;
		}
		
		boost::signal<void (int)> signal_percent_loaded;


	private:

		/// Liste mit Daten aus der Textdatei
		std::vector<MaterialStamm::Data> poslist;

		/// Dateiname der SAP Textdatei
		std::string fn;

		void split()
		{
			std::fstream file(fn.c_str());

			int length;
			// get length of file:
			file.seekg (0, std::ios::end);
			length = file.tellg();
			file.seekg (0, std::ios::beg);
		

			while (!file.eof())
			{
				char line[4096];
				file.getline(line, 4096);

				using namespace std;
				vector<string> fields;
				string res = line;
				string sep("\t");
				boost::algorithm::split(fields, res, boost::algorithm::is_any_of(sep) );

				if (fields.size() < 15)
					continue;
				int region = atoi(fields.at(0).c_str());

				if (region == 2001 || region == 2005 || region == 2006 || region == 2009 || region == 2010)
				{
					int anr = atoi(fields.at(1).c_str());

					// Preis
					std::string pr = fields.at(15);
					boost::algorithm::replace_all(pr, ".", "");
					boost::algorithm::replace_all(pr, ",", ".");
					double preis = atof(pr.c_str());

					// Mengeneinheit
					std::string me = fields.at(16);
					boost::algorithm::replace_all(me, ".", "");
					boost::algorithm::replace_all(me, ",", ".");
					double mengeneinheit = atof(me.c_str());

					// Preis pro Mengeneinheit
					if (mengeneinheit > 1.0)
						preis /= mengeneinheit;


					std::string kt = boost::algorithm::trim_copy(fields.at(2));
					std::string kt_en = boost::algorithm::trim_copy(fields.at(18));
					
					/// @todo: enum oder aus Stammdb laden!
					poslist.push_back(Data(anr, region, preis, kt, kt_en));
				}
				else
				{
					int anr = atoi(fields.at(0).c_str());
					anr += 2000000; // Service Artikelnummer

					// Preis
					std::string pr = fields.at(6);
					//boost::algorithm::replace_all(pr, ".", "");
					//boost::algorithm::replace_all(pr, ",", ".");
					double bkpreis = atof(pr.c_str());

					// Mengeneinheit
					std::string me = fields.at(16);
					//boost::algorithm::replace_all(me, ".", "");
					//boost::algorithm::replace_all(me, ",", ".");
					double mengeneinheit = atof(me.c_str());

					// Preis pro Mengeneinheit
					//if (mengeneinheit > 1.0)
					//	bkpreis /= mengeneinheit;

					// VK Preis
					pr = fields.at(14);
					//boost::algorithm::replace_all(pr, ".", "");
					//boost::algorithm::replace_all(pr, ",", ".");
					double vkpreis = atof(pr.c_str());
					//if (mengeneinheit > 1.0)
					//	vkpreis /= mengeneinheit;

					std::string kt = boost::algorithm::trim_copy(fields.at(4));
					std::string kt_en = kt;

					if (bkpreis > 0.0 && vkpreis > 0.0)
					{
						Data dat(anr, 2001, bkpreis, kt, kt_en);
						dat.set_serviceaufschlag( vkpreis/bkpreis*100 - 100.0);
						poslist.push_back(dat);
					}
				}
				
				int percent = (int)((double)file.tellg() / (double)length * 100.0);
				signal_percent_loaded(percent);
			}
			
		}
	};
}