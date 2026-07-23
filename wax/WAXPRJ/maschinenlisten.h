#pragma once





namespace WaxDruck
{
	
	class  WAXPRJDLL CObListzuschnittliste_Gewinde : public CObList
	{
		private:
			int m_Gesamtlaenge;
			int m_AnzahlRohrstuecke;
			int m_AnzahlFittinge;

		public:

			void ResetGesamtlaenge()
			{
				m_Gesamtlaenge = 0;
			}

			void SetGesamtlaenge(int iGesamtlaenge)
			{
				m_Gesamtlaenge += iGesamtlaenge;
			}

			int GetGesamtlaenge()
			{
				return m_Gesamtlaenge;
			}

			void ResetAnzahlRohrstuecke()
			{
				m_AnzahlRohrstuecke = 0;
			}

			void SetAnzahlRohrstuecke(int iAnzahlRohrstuecke)
			{
				m_AnzahlRohrstuecke += iAnzahlRohrstuecke;
			}

			int GetAnzahlRohrstuecke()
			{
				return m_AnzahlRohrstuecke;
			}

			void ResetAnzahlFittinge()
			{
				m_AnzahlFittinge = 0;
			}

			void SetAnzahlFittinge(int iAnzahlm_AnzahlFittinge)
			{
				m_AnzahlFittinge += iAnzahlm_AnzahlFittinge;
			}

			int GetAnzahlFittinge()
			{
				return m_AnzahlFittinge;
			}
	};

	//Anzahl Rohr liste

	class WAXPRJDLL Maschinenlisten
	{
		MxCObList Werkauftraege32020;
		MxCObList Werkauftraege32060;
		MxCObList Werkauftraege32070;

		MxCObList Werkauftraege38030;
		

	public:
		Maschinenlisten(WaxPart* w);
		virtual ~Maschinenlisten();

		
		MxCObList* GetWerkauftraege32020()
		{
			return &Werkauftraege32020;
		}


		MxCObList* GetWerkauftraege38030()
		{
			return &Werkauftraege38030;
		}


		MxCObList* GetWerkauftraege32060()
		{
			return &Werkauftraege32060;
		}

		MxCObList* GetWerkauftraege32070()
		{
			return &Werkauftraege32070;
		}
		/// Wird gesetzt, um Zugriff auf die DNVL bzw Rohrqualität zu haben
		const Rohrleitung* rohr;

		void createSaegeMaschinenGewinderohrlaengen(Werkauftrag* werkauftrag);
		/// Löscht die Zuschnittslisten und die Rohrnutenlisten
		void deleteMaschinenlisten();

		void createMaschinenRohrleitungCOIIVerteiler(Rohrleitung* rohr);

		/// Erzeugt die Rohrnutenlisten und die Zuschnittslisten
		void createMaschinenlisten(Werkauftrag *werkauftrag, bool loeschen = true);

		void createMaschinenRohrleitung(Werkauftrag *werkauftrag);

		void createMaschinenApax(Werkauftrag *werkauftrag);

			/// Zuschnittsliste fuer Anlage 32020
		CObList zuschnittliste_32020;

		/// Zuschnittsliste fuer Anlage 38030 Handschweißung
		CObList zuschnittliste_38030_handsw;

		/// Zuschnittsliste fuer Anlage 38030 Anlage
		CObList zuschnittliste_38030_anlage;

		/// Zuschnittsliste fuer Anlage 32060
		CObList zuschnittliste_32060;

		/// Zuschnittsliste fuer Anlage 32070
		CObList zuschnittliste_32070;

		CObList zuschnittliste_EK;

		CObListzuschnittliste_Gewinde zuschnittliste_Gewinde_gesamt;
		
		CObList zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030;

		CObList zuschnittliste_Gewinde_ab10Stueck_32070;

		CObList zuschnittliste_Gewinde_bis10Stueck_31010;

		int stutzenAnlage2440;

	
	};
}
