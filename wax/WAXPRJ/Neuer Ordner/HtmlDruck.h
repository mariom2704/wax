#pragma once


class WaxHtmlFile;

class CObList;



//namespace WaxDruck
//{
	
	

	/// Sammlung von Spezialfunktionen für die Druckausgabe.
	class WAXPRJDLL HtmlDruck 
	{
	public:
		/// /Fuegt die Einzelpreise der Positionen ein 
		void insertEinzelpreise(CStringArray& preise);
		void insertListeUnterschriften();
		/// Fuegt die Rohrqualitaet in eine Druckliste ein
		void insertQuali(int quali, bool twoquali, int dn, int stutzenAnlage2440 = 0, int anlagenttyp = 1 /*Wasser*/);

		void insertVK3GesPreise(double elementepreis, double sonderkosten, double gesamtpreisVK3);

		void insertVK3Preise(EnCObListVk3& elelist, EnCObListVk3& passlist, EnCObListVk3& pulverlist, double elementepreis);

		HtmlDruck();

		/// Zugriff auf die Basisfunktionen der Html-Ausgabe
		 WaxHtmlFile& get_file()  ;

		virtual ~HtmlDruck();

		HtmlDruck(CString fn);
		
		/// Fügt eine Zeile mit der Beschichtung, dem Sichtbereichsflag, dem 
		/// Zeugnisflag und der Zulassungsstelle ein
		void insertOberflaeche(Werkauftrag* walist, const Beschichtung &beschichtung, BOOL zeugnis, BOOL sichtbereich, int zulassungsstelle);

		//Fügt eine Zeile mit 2 Spalten ein
		void insertZeile2Spalten(const CString &spalte1,int breiteSpalte1,bool center1, const CString &spalte2,int breiteSpalte2,bool center2);

		void insertZeile4Spalten(const CString &spalte1,int breiteSpalte1,bool center1, 
								 const CString &spalte2,int breiteSpalte2,bool center2,
								 const CString &spalte3,int breiteSpalte3,bool center3,
								 const CString &spalte4,int breiteSpalte4,bool center4);



		void insertZeiten(double schweisserei, double pulveranlage);


		/// Fuegt die Leitkarte in eine Druckliste ein
		void insertLeitkarte(Leitkarte& leitkarte, Status& status, const CString& name, const CString & ziel, bool umbruch = false);

		void insertKopf(const CString &ueberschrift1, const CString &ueberschrift2,
						const CString &wanr, const CString &sachbearbeiterOd,const CString &besteller,
						const CString & bearbeitungsDat, const CString & freigabeDat, const CString & lieferDat,
						const CString &nl, const CString &auftragsBez, const CString &bauabschnitt, const CString &plz, const CString &ort,
						const CString &sdAuftrag, const CString &sdPos, const CString &sdMaterial, const CString &psp,
						bool umbruch = false,const CString &nichtGesetzt = "-");


		/// Fuegt die Bemerkung aus der Leitkarte in eine Druckliste
		void insert_bemerkung(const std::string& bem, const std::string& ueberschrift, const std::string& stdbemerkung = "");

		/// Fuegt die Kappenfarbe in eine Druckliste
		void insertKFarbe(int kFarbe);


		/// Fuegt die Max. Laenge und das Gesamtgewicht in eine Druckliste ein
		void insertLaengeGewichtBarcode(int maxlaenge, double gewicht, CString kc, int kcPosnr,  string ids );

		/// Fuegt eine Trennlinie in eine Druckliste ein
		void insertTrennlinie();

		/// Fuegt die Sokos in eine Druckliste ein
		void insertSokoPreise(CObList &sokolist, double gesamtpreis);


		/// Fuegt den Sondermuffentext ein
		void insertSondermuffentext(int laenge);



	private:
		CString htmlfile;

		WaxHtmlFile file;

	};
//}