#pragma once

#include <MxOdbc2/Database.h>
#include <boost/boost.h>

#include "../MaterialStamm/Data.h"

namespace DB
{
	class ArtikelTexte : public MxOdbc::Table
	{
	public:
		ArtikelTexte(MxOdbc::Database& db, MxBase::Locale l = MxOdbc::Table::baseloc) : MxOdbc::Table("ArtikelTexte", db, l)
		{
			init_columns();
		}
		
		GETSET_INT(Artikelnummer);
		GETSET_STRING(Eingabe);
		GETSET_STRING(TextDeutsch);
		GETSET_STRING(TextEnglish);

		bool load(int artnr)
		{
			std::string sql = boost::str(boost::format("ArtNr = %i") % artnr);
			if (!start_fetch(sql))
				return false;
			return fetch_next();
		}
				
	protected:

		void init_columns()
		{
			add_column<int>("ArtNr", true);
			add_column<std::string>("Eingabe");
			add_column<std::string>("TextDeutsch");
			add_column<std::string>("TextEnglish");
		}
	};


	class MxdmStammTab : public MxOdbc::Table
	{
	public:
		MxdmStammTab(MxOdbc::Database& db, const std::string& tabname) : MxOdbc::Table(tabname, db, MxOdbc::Table::baseloc)
		{
			init_columns();
		}
		
		GETSET_INT(ArtNr);
		
		bool load()
		{
			if (!start_fetch())
				return false;
			return true;
		}
				
	protected:

		void init_columns()
		{
			add_column<int>("ArtNr", true);
		}
	};

	class MXDMStammdb
	{
	public:
		MXDMStammdb(const std::string& fn)
		{
			dbfile = fn;
		}

		boost::signal<void (int,int)> signal_percent_updated;

		bool update(const std::vector<MaterialStamm::Data>& data)
		{
			// MXDM Stammdatenbank öffnen
			MxOdbc::Database db(dbfile, "", "8fdz93b6ye");

			if (!db.open())
				return false;

			std::map<int,std::string> artnrlist;

			try
			{
				// Artikelnummer aller relevanten Tabellen zusammenklauben
				artnrlist = collect_artnr(db);
			}
			catch (MxBase::Error e)
			{
				return false;
			}

			int cnt = 1;
			for (std::vector<MaterialStamm::Data>::const_iterator it = data.begin(); it != data.end(); it++, cnt++)
			{
				std::map<int,std::string>::iterator itx = artnrlist.find(it->get_artikelnr());
				if ( itx != artnrlist.end() )
				{
					if (!exists(db, *it))
					{
						if (!insert(db, *it, itx->second))
							break;
					}
					
					update_kurztexte(db, *it);
					
					int percent = (int)((double)cnt / (double)data.size()* 100.0);
					signal_percent_updated(percent, it->get_artikelnr());
					artnrlist.erase(itx);
				}
				
			}
			for (std::map<int,std::string>::iterator it = artnrlist.begin(); it != artnrlist.end(); it++)
			{
				if (!exists(db, it->first))
				{
					if (!insert(db, it->first, it->second))
						break;
				}
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

		std::map<int,std::string> collect_artnr(MxOdbc::Database& db) const
		{
			/**
			Kumuliert alle Artikelnummern. Als zweiter Parameter in der Map steht ein int, default auf 0.
			Als Bearbeitungsflag
			*/
			std::map<int,std::string> res;

			std::vector<std::string> tablst;
			tablst.push_back("Bereichsventil"); 
			tablst.push_back("Bereichsverteiler"); 
			tablst.push_back("Duesen"); 
			tablst.push_back("Fittinge"); 
			tablst.push_back("Flaschen"); 
			tablst.push_back("Rohre"); 
			tablst.push_back("T_Stuecke"); 
			tablst.push_back("Winkel"); 
			tablst.push_back("Materialstamm");

			for (std::vector<std::string>::iterator it = tablst.begin(); it != tablst.end(); it++)
			{
				MxdmStammTab tab(db, *it);
				if (!tab.load())
					throw tab.error;

				while (tab.fetch_next())
				{
					res.insert( std::make_pair<int,std::string>(tab.get_ArtNr(), *it) );
				}
			}
			return res;
		}

		bool exists(MxOdbc::Database& db, const MaterialStamm::Data& dat)
		{
			return exists( db, dat.get_artikelnr() );
		}
	
		bool exists(MxOdbc::Database& db, int artnr)
		{
			ArtikelTexte texte(db);
			if ( !texte.load(artnr) )
				return false;
						
			return true;
		}

		bool manuell(MxOdbc::Database& db, int artnr)
		{
			ArtikelTexte texte(db);
			if ( !texte.load(artnr) )
				return false;
						
			return texte.get_Eingabe() == "manuell";
		}
	
	

		bool update_kurztexte(MxOdbc::Database& db, const MaterialStamm::Data& dat)
		{
			if ( manuell(db, dat.get_artikelnr()) )
				return true;

			std::string sql = 
				boost::str(boost::format("UPDATE ArtikelTexte SET TextEnglish = '%s' WHERE ArtNr = %i") % dat.get_kurztext_en() % dat.get_artikelnr());
			db.exec_sql(sql);
			
			sql = boost::str(boost::format("UPDATE ArtikelTexte SET TextDeutsch = '%s' WHERE ArtNr = %i") % dat.get_kurztext() % dat.get_artikelnr());
			db.exec_sql(sql);

			sql = boost::str(boost::format("UPDATE ArtikelTexte SET Eingabe = '%s' WHERE ArtNr = %i") % dat.get_source() % dat.get_artikelnr());
			db.exec_sql(sql);

			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}	

		bool insert(MxOdbc::Database& db, const MaterialStamm::Data& dat, const std::string& tabelle)
		{
			std::string sql = 
				boost::str(boost::format("INSERT INTO ArtikelTexte (ArtNr, Tabelle, Eingabe,"
										 "TextDeutsch, TextEnglish) "
										 "VALUES(%1%, '%2%', '%3%', '%4%', '%4%')" ) % dat.get_artikelnr() % tabelle % dat.get_source() % dat.get_kurztext());
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		bool insert(MxOdbc::Database& db, int artnr, const std::string& tabelle)
		{
			std::string sql = 
				boost::str(boost::format("INSERT INTO ArtikelTexte (ArtNr, Tabelle, "
										 "TextDeutsch, TextEnglish) "
										 "VALUES(%1%, '%2%', '%3%', '%4%', '%4%')" ) % artnr % tabelle % "???");
			db.exec_sql(sql);
			if (db.get_errorhandler().get_count())
				return false;
			return true;
		}

		std::string dbfile;
	};
}