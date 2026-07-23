#pragma once

#include <Database.h>
#include <mxodbc2/Table.h>
#ifdef _DEBUG
#include <mxutil/tracer.h>
#endif

namespace AltTabellen
{
	class PreislistenArtikelLangtext : public MxOdbc::Table
	{
	public:
		PreislistenArtikelLangtext(SpanxDb::Database& db, MxBase::Locale l, const std::string& tblname = "PreislistenArtikel") : MxOdbc::Table(tblname, db)
		{
			add_column<std::string>("code");
			add_column<std::string>("KurzText" + get_sprache());
			add_column<std::string>("LangText" + get_sprache());
			add_column<std::string>("LVText" + get_sprache());
		}

		bool load(const std::string& code)
		{
			if (!start_fetch("code = '" + code + "'") || !fetch_next())
				return false;
		}

		GETSET_STRING(code);

		std::string get_langtext()
		{
			return get_col("LangText" + get_sprache())->to_string();
		}

		std::string get_lvtext()
		{
			return get_col("LangText" + get_sprache())->to_string();
		}
	};
	
	class PreislistenArtikel : public MxOdbc::Table
	{
	public:
		PreislistenArtikel(SpanxDb::Database& db, 
			bool withjoin_codeartikelzuordung, 
			const std::string& tblname = "PreislistenArtikel") : MxOdbc::Table(tblname, db)
		{
			init_base();

			if (withjoin_codeartikelzuordung)
				init_join();
		}

		GETSET_STRING(code);
		
		GETSET_STRING(KurzText);
		GETSET_STRING(KurzTextEnglish);
		GETSET_STRING(KurzTextFrancais);
		GETSET_STRING(KurzTextNederlands);

		GETSET_STRING(LangText);
		GETSET_STRING(LVText);
		GETSET_STRING(LangTextEnglish);
		GETSET_STRING(LangTextFrancais);
		GETSET_STRING(LangTextNederlands);
		GETSET_STRING(LVTextEnglish);
		GETSET_STRING(LVTextFrancais);
		GETSET_STRING(LVTextNederlands);
		GETSET_STRING(ArtikelNummer);
		GETSET_STRING(Hinweis);

		GETSET_STRING(GeaendertVon);
		GETSET_STRING(GeaendertAm);
		GETSET_STRING(WM);
		GETSET_STRING(WL);
		GETSET_STRING(Montagezeit);
		GETSET_STRING(posnr);
		GETSET_INT(IstFremdleistung);
		GETSET_INT(IstSprinkler);

		GETSET_DOUBLE(gewicht);


		GETSET_STRING(Gruppe);

#ifdef _DEBUG
		void dump()
		{
			MxUtil::Tracer::trace2file("PreislistenArtikelDump.txt", "-------------");
			get_col("Liste")->dump2file("PreislistenArtikelDump.txt");
			get_col("Gruppe")->dump2file("PreislistenArtikelDump.txt");
			get_col("code")->dump2file("PreislistenArtikelDump.txt");
			get_col("ArtikelNummer")->dump2file("PreislistenArtikelDump.txt");
			get_col("Montagezeit")->dump2file("PreislistenArtikelDump.txt");
			get_col("WM")->dump2file("PreislistenArtikelDump.txt");
			get_col("WL")->dump2file("PreislistenArtikelDump.txt");
			get_col("KurzText")->dump2file("PreislistenArtikelDump.txt");
			MxUtil::Tracer::trace2file("PreislistenArtikelDump.txt", "-------------");

		}
#endif
	private:

		void init_join()
		{
			add_join<std::string>("CodeArtikelnummer_Zuordnung", "Code", true);
			add_join<int>("CodeArtikelnummer_Zuordnung", "ArtNr");
			add_join<std::string>("CodeArtikelnummer_Zuordnung", "Gruppe");

			add_joined_keys("INNER JOIN CodeArtikelnummer_Zuordnung ON CodeArtikelnummer_Zuordnung.Code = old_PreislistenArtikel.code") ;
		}
		void init_base()
		{
			add_column<std::string>("Liste");
			add_column<std::string>("Gruppe");
			add_column<std::string>("code", true);
			add_column<std::string>("ArtikelNummer");
			add_column<std::string>("Montagezeit");
			add_column<std::string>("WM");
			add_column<std::string>("WL");
			add_column<std::string>("Hinweis");
			add_column<std::string>("posnr");
			add_column<int>("IstFremdleistung");
			add_column<int>("IstSprinkler");
			add_column<int>("IstGeloescht");
			add_column<int>("IstGeaendert");
			add_column<int>("IstNeu");
			add_column<std::string>("GeaendertVon");
			add_column<std::string>("GeaendertAm");
			add_column<double>("gewicht");
			add_column<int>("festpreis");
			add_column<std::string>("KurzText");
			add_column<std::string>("LangText");
			add_column<std::string>("LVText");
			add_column<std::string>("KurzTextEnglish");
			add_column<std::string>("KurzTextFrancais");
			add_column<std::string>("KurzTextNederlands");
			add_column<std::string>("LangTextEnglish");
			add_column<std::string>("LangTextFrancais");
			add_column<std::string>("LangTextNederlands");
			add_column<std::string>("LVTextEnglish");
			add_column<std::string>("LVTextFrancais");
			add_column<std::string>("LVTextNederlands");
		}

	
		
	};


}