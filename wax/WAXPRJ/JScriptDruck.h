#pragma once


class WaxHtmlFile;

typedef vector<CString> CVectorHeadercontent;

class WAXPRJDLL CJScriptPrinting 
{

	private:
		const CString m_Seperator, m_SeperatorJpg, m_SeperatorAnzahl;
		const CString m_JScriptDoubleblank;
		const CString m_HochformatMarker;
		
		WaxHtmlFile file;
		
		CVectorHeadercontent m_strHeader;
		CString m_strShowRows;
		CString m_strGraficLinks;
		CString m_strAnzahl;

	public:

		CJScriptPrinting();
		void write_header(Werkauftrag* pWerkauftrag, string listname);
			void insertDwfOrSvgLink( const string& fname);
			void insertQuali(int quali, bool twoquali, int dn, int stutzenAnlage2440 = 0, int anlagentyp = 1 /* wasser*/);
			void insertOberflaeche(const Beschichtung &beschichtung, BOOL zeugnis, BOOL sichtbereich, int zulassungsstelle);
			void insertZeiten(double schweisserei, double pulveranlage);
			void insertLeitkarte(Leitkarte& leitkarte, Status& status, const CString& name, const CString & ziel, bool umbruch = false);
			void insert_bemerkung(const std::string& bem, const std::string& ueberschrift, const std::string& stdbemerkung = "");
			void insertLaengeGewichtBarcode(int maxlaenge, double gewicht, CString kc, int kcPosnr,  string ids );
			string createBarcode(Werkauftrag* pWerk, int breite, int hoehe, string txt, string align);
			void Initialize();
			void createBlankJPG(Werkauftrag* pWerk);
			static std::string CJScriptPrinting::get_tempdirWaxHTML();
		

			CVectorHeadercontent GetHeadercontent()
			{
				return m_strHeader;
			};
			CString GetShowRows()
			{
				return m_strShowRows;
			};
			CString GetGraficLinks()
			{
				return m_strGraficLinks;
			};
			

			static string GetApplicationPath()
		    {
				string strPurePath;
				
				TCHAR path[_MAX_PATH];
				GetModuleFileName(0, path, _MAX_PATH);
				CString strPath = path;
				int iWhere = strPath.ReverseFind('\\');
				strPurePath = strPath.Mid(0, iWhere);
				
				return strPurePath;
		    }

			WaxHtmlFile& get_file();
		virtual ~CJScriptPrinting();
};
