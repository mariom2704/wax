using System;
using System.Collections.Generic;
using System.Text;
using MxLib.Math;
using MxLib.Basis;


namespace SapMatToWax.En
{
    public class EnArtikel
    {
		//*******  bei neuen Eigenschaften  copy-Methode beachten ********

        int artikelnummer,
            artikelgruppe;

        int classId,
            status;

        string bezeichnung = "",
               bezeichnungEng = "",
			   bezeichnungEinheit = "";

		double gewichtProEinheit,
			   menge,
			   sapPreis,
			   festPreis;

        bool isFertigung;					//true wenn es kein Montagematerial ist und über die Vorfertigung läuft. 

		bool isAnlagenFertigung = false;	//true wenn das Bauteil automatisch über die Anlage gefertigt werden kann (z,b. Strangrohr- oder Verteilerrohrstrasse)
		int substituteArtikelnummer;		//Wenn Artikelnummer substituiert wurde. (z.B. Muffe Handschweißung in Muffe Maschinenschweißung, Bogen schwarz in Bogen verzinkt, ...)
		int substituteArtikelgruppe;

        Dictionary<EnArtikel, double> stueckliste = new Dictionary<EnArtikel, double>();

        public enum Einheit
        {
            Stueck,
            Meter,
            Milimeter,
            Kg,
            Quadratmeter,
            Kubikmeter
        }

        /*
		string tagNameArtikel = "Artikel";
		string tagNameArtNr = "Artikelnummer";
		string tagNameClassId = "ClassId";
		string tagNameArtGruppe = "Artikelgruppe";
		string tagNameIsAnlage = "IsAnlage";
		string tagGewicht = "Gewicht";
		string tagEinheit = "Einheit";
		string tagFestPreis = "FestPreis";
        */

		bool appendSchweissnahtToBauteil = true;


		///Muß mit den Werten in der DB übereinstimmen!!!!
		public enum CLASSID
		{
			TypSonstiges = 1,
			TypBogen = 10,
			TypAnbohrschelle = 11,
			TypDlwb = 12,
			TypFlachBandstahl = 13,
			TypFlansch = 14,
			TypKupplung = 15,
			TypMuffeStutzen = 16,
			TypProfil_L = 17,
			TypProfil_U = 18,
			TypReduzierung = 19,
			TypRohr = 20,
			TypRueckschlagklappe = 21,
			TypSchieber = 22,
			TypSprinkler = 23,
			TypTStueck = 24,
			TypRedWinkel = 25,
			TypHalter = 26,
            TypKnick = 27,

			TypZentralenBauteil = 100,
			TypBohrung = 110,
            TypBaugruppe = 120,
		};



        public EnArtikel()
        {
			//CadTyp = CADTYP.typArtikel;
        }

		/**
		 * Werte nur kopieren, wenn es gültige Werte sind
		 */
        public void copy(EnArtikel artikel)
        {
			copy(artikel, true);
        }


		public void copy(EnArtikel artikel, bool mitStueckliste)
		{
			if (artikel.Artikelgruppe > 0)
                artikelgruppe = artikel.Artikelgruppe;

            if (artikel.ClassId > 0)
                classId = artikel.ClassId;

            if (artikel.Artikelnummer > 0)
            {
                artikelnummer = artikel.Artikelnummer;
                bezeichnung = artikel.Bezeichnung;
                bezeichnungEng = artikel.BezeichnungEng;
                bezeichnungEinheit = artikel.BezeichnungEinheit;

                gewichtProEinheit = artikel.GewichtProEinheit;
                menge = artikel.Menge;
                sapPreis = artikel.SapPreis;
                festPreis = artikel.FestPreis;

                isFertigung = artikel.IsFertigung;
                isAnlagenFertigung = artikel.IsAnlagenFertigung;
                substituteArtikelnummer = artikel.substituteArtikelnummer;
                substituteArtikelgruppe = artikel.substituteArtikelgruppe;
            }

			if (mitStueckliste)
				copyStueckliste(artikel.Stueckliste);

            status = artikel.status;
		}

		public void copyStueckliste(Dictionary<EnArtikel, double> stl)
		{
			Stueckliste.Clear();
			if (stl.Count > 0)
			{
				foreach (EnArtikel artikelx in stl.Keys)
				{
					Stueckliste.Add(artikelx, stl[artikelx]);
				}
			}
		}

        public Dictionary<EnArtikel, double> Stueckliste
        {
            get { return stueckliste; }
            set { stueckliste = value; }
        }

        /*
		override public string getTagName()
		{
			return tagNameArtikel;
		}
        */
		public bool AppendSchweissnahtToBauteil
		{
			get { return appendSchweissnahtToBauteil; }
			set { appendSchweissnahtToBauteil = value; }
		}

		public int Artikelnummer
		{
			get { return artikelnummer; }
			set { artikelnummer = value; }
		}


        public int Status
        {
            get { return status; }
            set { status = value; }
        }

		public int SubstituteArtikelnummer
		{
			get { return substituteArtikelnummer; }
			set { substituteArtikelnummer = value; }
		}

		public int SubstituteArtikelgruppe
		{
			get { return substituteArtikelgruppe; }
			set { substituteArtikelgruppe = value; }
		}

		public int Artikelgruppe
		{
			get { return artikelgruppe; }
			set { artikelgruppe = value; }
		}

		public int ClassId
		{
			get { return classId; }
			set { classId = value; }
		}

		//im SAP Artikelstamm ist das Gewicht nicht richtig gepflegt, darum bezieht es sich immer auf Meter
		public double getGewichtProStandardEinheit()
		{
			if (getEinheit() == Einheit.Milimeter)
				return gewichtProEinheit / 1000.0;
			else
				return gewichtProEinheit;
		}

		public double GewichtProEinheit
		{
			get { return gewichtProEinheit; }
			set { gewichtProEinheit = value; }
		}


		
		public double getGewicht(double menge)
		{
			double gewicht = 0;
			if (getEinheit() == Einheit.Meter)
			{
				gewicht = getGewichtProStandardEinheit() * menge / 1000.0;
			}
			else
				gewicht = getGewichtProStandardEinheit() * menge;

			return gewicht;
		}


		public double getPreis(double menge)
		{
			double preis = 0;
			if (FestPreis > 0)
				preis = FestPreis;
			else
				preis = SapPreis;

			double tmpMenge = menge;
			if (getEinheit() == Einheit.Meter)
				tmpMenge /= 1000.0;

			return preis * tmpMenge;
		}



		public double SapPreis
		{
			get { return sapPreis; }
			set { sapPreis = value; }
		}

		public double FestPreis
		{
			get { return festPreis; }
			set { festPreis = value; }
		}

		
		public string Bezeichnung
		{
			get { return bezeichnung; }
			set { bezeichnung = value; }
		}

        public string BezeichnungEng
        {
            get { return bezeichnungEng; }
            set { bezeichnungEng = value; }
        }

		public string BezeichnungEinheit
		{
			get { return bezeichnungEinheit; }
			set { bezeichnungEinheit = value; }
		}

        
		public Einheit getEinheit()
		{
			string einheit = BezeichnungEinheit.ToLower();

			if (einheit == "m")
				return Einheit.Meter;
			else if (einheit == "mm")
				return Einheit.Milimeter;
			else if (einheit == "kg")
				return Einheit.Kg;
			else if (einheit == "m²")
				return Einheit.Quadratmeter;
			else if (einheit == "m³")
				return Einheit.Kubikmeter;

			return Einheit.Stueck;
		}
        

		public double Menge
		{
			get { return menge; }
			set { menge = value; }
		}


		public string mengeToString()
		{
			if (getEinheit() == Einheit.Stueck ||
				getEinheit() == Einheit.Milimeter)
				return Menge.ToString("F0");
			else if (getEinheit() == Einheit.Meter)
			{
				double tmp = Menge / 1000.0;
				return tmp.ToString("F3");
			}
			else
				return Menge.ToString("F3");
		}

		public bool IsFertigung
		{
			get { return isFertigung; }
			set { isFertigung = value; }
		}

		public bool IsAnlagenFertigung
		{
			get { return isAnlagenFertigung; }
			set { isAnlagenFertigung = value; }
		}

		/**
		 * prüfen die this.Artikelgruppe in artGruppen vorkommt
		 */
		public bool checkArtikelgruppe(int[] artGruppen)
		{
			foreach (int artGr in artGruppen)
			{
				if (artGr == Artikelgruppe)
				{
					return true;
				}
			}

			return false;
		}

		/**
		 *  Länge des Artikels
		 */
		virtual public double getLaenge()
		{
			return 0;
		}

		public double getMenge()
		{
			if (getEinheit() != Einheit.Stueck)
				return getLaenge();
			else
				return 1.0;
		}

		/**
		 * prüfen ob Artikel gleich sind
		 */
		virtual public bool compare(EnArtikel artikel, double toleranz)
		{
			if (this.Artikelnummer != artikel.Artikelnummer)
				return false;

			if (this.Artikelgruppe != artikel.Artikelgruppe)
				return false;

            if (Math.Abs(getLaenge() - artikel.getLaenge()) >= toleranz)
				return false;

			return true;
		}

		virtual public bool compareStueckliste(Dictionary<EnArtikel, double> other, double toleranz)
		{
			if (Stueckliste.Count != other.Count)
				return false;

			if (Stueckliste.Count == 0)
				return true;

			foreach (EnArtikel first in Stueckliste.Keys)
			{
				//prüfen ob Eintrag in anderer Liste
				if (!other.ContainsKey(first))
					return false;

				//püfen ob Menge gleich ist
                if (!MathTools.compare(Stueckliste[first], other[first], toleranz))
					return false;
			}

			return true;
		}

        public override string ToString()
        {
			if (String.IsNullOrEmpty(Bezeichnung))
				return String.Format("ArtGr:{0}, ArtNr:{1}", Artikelgruppe, Artikelnummer);			
			else
				return Bezeichnung;
        }

        /**
         * prüfen ob es sich um den gleichen Artikel handelt. Im Gegensatz zu Compare wird nicht die Menge geprüft.
         */
        public override bool Equals(object obj)
        {
            EnArtikel art = obj as EnArtikel;
            if (art == null)
                return false;

            if (art.Artikelnummer != this.Artikelnummer)
                return false;

            return true;
        }

        public override int GetHashCode()
        {
            return this.Artikelnummer;
        }

        /*
		override protected bool exportData(DataElement element)
		{
			element.Name = tagNameArtikel;

			//CadTeil Daten schreiben
			DataElement child = new DataElement();
			if (!base.exportData(child))
				return false;
			element.addChild(child);

			element.addAttribut(new IdWert(tagNameArtNr, this.Artikelnummer));
			element.addAttribut(new IdWert(tagNameArtGruppe, this.Artikelgruppe));
			element.addAttribut(new IdWert(tagNameClassId, this.ClassId));
			element.addAttribut(new IdWert(tagNameIsAnlage, this.IsAnlagenFertigung));
			element.addAttribut(new IdWert(tagGewicht, this.GewichtProEinheit));
			element.addAttribut(new IdWert(tagEinheit, this.BezeichnungEinheit));
			element.addAttribut(new IdWert(tagFestPreis, this.FestPreis));

			return true;
		}

		

		override protected bool importData(DataElement element)
		{
			if (element.Name != tagNameArtikel)
				throw new MxException(null, "err_ImportFehler"); //Fehler beim Import der Daten

			if (element.Attribute == null)
				throw new MxException(null, "err_ImportFehler"); //Fehler beim Import der Daten

			//Artikelwerte einlesen
			foreach (IdWert wert in element.Attribute)
			{
				if (wert.Name == tagNameArtNr)
				{
					this.Artikelnummer = int.Parse(wert.Value.ToString());
				}
				else if (wert.Name == tagNameArtGruppe)
				{
					this.Artikelgruppe = int.Parse(wert.Value.ToString());
				}
				else if (wert.Name == tagNameClassId)
				{
					this.ClassId = int.Parse(wert.Value.ToString());
				}
				else if (wert.Name == tagNameIsAnlage)
				{
					this.IsAnlagenFertigung = bool.Parse(wert.Value.ToString());
				}
				else if (wert.Name == tagGewicht)
				{
					this.GewichtProEinheit = double.Parse(wert.Value.ToString());
				}
				else if (wert.Name == tagEinheit)
				{
					this.BezeichnungEinheit = wert.Value.ToString();
				}
				else if (wert.Name == tagFestPreis)
				{
					this.FestPreis = double.Parse(wert.Value.ToString());
				}
			}

			if (element.Childs != null)
			{
				foreach (DataElement child in element.Childs)
				{
					//CadTeildaten einlesen
					if (child.Name == base.getTagName())
					{
						if (!base.importData(child))
							throw new MxException(null, "err_ImportFehler"); //Fehler beim Import der Daten
					}
				}
			}

			return true;
		}
        */

		

    }
}
