#pragma once





	class  CObListzuschnittliste_GewindeAuftrag : public CObList
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

	

	class  RohrlisteAuftrag
	{
	public:
		RohrlisteAuftrag(Werkauftrag* w);
		virtual ~RohrlisteAuftrag();

		
		

		void createSaegeMaschinenGewinderohrlaengen(Werkauftrag* werkauftrag);
		/// Löscht die Zuschnittslisten und die Rohrnutenlisten
		void deleteMaschinenlisten();
		/// Erzeugt die Rohrnutenlisten und die Zuschnittslisten
		void createMaschinenlisten(Werkauftrag *werkauftrag, bool loeschen = true);

		
	

	

		CObListzuschnittliste_GewindeAuftrag zuschnittliste_Gewinde_gesamt;
		
		CObList zuschnittliste_Gewinde_ab20Stueck_von50bis500_38030;

		CObList zuschnittliste_Gewinde_ab10Stueck_32070;

		CObList zuschnittliste_Gewinde_bis10Stueck_31010;


	private:
		
	};

