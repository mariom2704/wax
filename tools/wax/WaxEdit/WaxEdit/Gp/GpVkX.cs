using System;
using System.Collections.Generic;
using System.Text;
using MxLib.Entities;
using RohrKonfBase.Entities;
using MxLib.Dialog;
using RohrKonfBase.Gp;
using MxLib.Math;
using System.Drawing;
using MxLib.Db;
using MxLib.Basis;

namespace WaxEdit.Gp
{
    public struct IntPair
    {
        public int Val1;
        public int Val2;

        public override string ToString()
        {
            return Val1.ToString() + "-" + Val2.ToString();
        }
    }   

	public class GpVkX : GpBase
	{
		string server = "",
			   datenbank = "";

        DbSqlBase mAbfrage;

		string tabVk3 = "Vk3Pos",
			   tabFestpreisArtikel = "Artikelstamm";

		string internerError = "Interner Fehler";

		//Bereich der Ids für Gewinderohr (vk2)
		public static int vk2IdStart = 4000;
		public static int vk2IdEnde = 4999;

		//Ab Spalte 4 fangen die DN Werte an 
		int firstPreisSpalteVk3 = 4;

        //Ab Spalte 11 fangen die Preise bei der Muffenansicht an
        int firstPreisSpalteVk3Muffe = 11;

		int tabSpaltePreis;
		int tabSpalteArtikelNr;
		
		public GpVkX(string server, string db)
		{
			Datenbank = db;
			Server = server;
		}

		public bool readVk3(int werk,
                            bool readVerzinktePreise, 
						    int minDn, int maxDn, 
							int minId, int maxId,
                            int element,
							EnMatrix matrix)
		{
			matrix.clearAll();

            if (maxDn == 0)
                maxDn = 500;

            string eleFilter = "";
            if (element > 0)
                eleFilter = String.Format("and v.Id={0} ", element);

			string sqlStatement = string.Format("SELECT v.Id, b.BezeichnungDeutsch, v.Anlage, v.Dn, v.Dnvl, v.{0}, v.Counter " +
										 "FROM {1} v LEFT JOIN Vk3Bezeichnung b ON v.Id = b.Id " +
                                         "where v.Fertigungsstaette={6} " +
                                         "{7}" +
										 "and v.Dnvl >= {2} and v.Dnvl <= {3} " +
										 "and v.Id >= {4} and v.Id <= {5} " +
										 "and v.Id > 0 " +
                                         
                                         "and v.RohrLaengeBis=0 " + 
                                         "and v.ElementAnzahlVon=0 " +  
                                         "and v.ElementAnzahlBis=0 " +  
                                         "and v.ElementAnzahlId=0 " +  
                                         "and v.ElementAnzahlDnVon=0 " +  
                                         "and v.ElementAnzahlDnBis=0 " + 
                                         "and v.ElementLaengeVon=0 " +  
                                         "and v.ElementLaengeBis=0 " +

										 "order by v.Id, v.Dn, v.Dnvl",
										 getPreisfield(readVerzinktePreise), tabVk3,
										 minDn, maxDn,
										 minId, maxId,
                                         werk, eleFilter);
			
			EnMatrix mat = new EnMatrix();
			if (!start(sqlStatement, mat))
				return false;

			if (!convertInVkMatrix(mat, matrix))
				return false;

			return true;
		}

	


		string getPreisfield(bool isVerzinkt)
		{
			if (isVerzinkt)
				return "PreisVerzinkt";
			else
				return "Preis";
		}

		/**
		 * In eine Matrix mit DNVL als Überschrift konvertieren
		 */
		private bool convertInVkMatrix(EnMatrix matListe, EnMatrix matMatrix)
		{
			string dnPraefix = "DN ";

            string bezSpalteElemId = "ID";
            string bezSpalteBez = "Bezeichnung";
            string bezSpalteAnlage = "Anlage";
            string bezSpalteDn = "DN";

            int spalteElemId = 0;			
			int spalteBez = 1;	
            int spalteAnlage = 2;		
			int spalteDn = 3;                        			
			int spalteDnVl = 4;
			int spaltePreis = 5;
            int spalteCounter = 6;

			matMatrix.ColumnNames.Add(bezSpalteElemId);
			matMatrix.ColumnNames.Add(bezSpalteBez);
            matMatrix.ColumnNames.Add(bezSpalteAnlage);
			matMatrix.ColumnNames.Add(bezSpalteDn);

			//DN Spalten erzeugen
			int zeile;
			for (zeile = 0; zeile < matListe.MaxRow; zeile++)
			{
				int dn = 0;
				matListe.getIntAt(zeile, spalteDnVl, out dn);

				EnRohrDn dnVl = new EnRohrDn();
				dnVl.DnId = dn;
				dnVl.DnBezeichnung = dnPraefix + dn.ToString();

				//prüfen an welcher Stelle dn wert eingetragen werden muss
				int insertSpalte = matMatrix.ColumnNames.Count;
				for (int i = firstPreisSpalteVk3; i < matMatrix.ColumnNames.Count; i++)
				{
					EnRohrDn tmpDn = matMatrix.ColumnNames[i] as EnRohrDn;
					if (tmpDn == null)
						continue;
					if (tmpDn.DnId == dnVl.DnId)
					{
						insertSpalte = -1;
						break;
					}
					else if (dnVl.DnId > tmpDn.DnId)
					{
						insertSpalte = i+1;
					}
					else if (dnVl.DnId < tmpDn.DnId)
					{
						insertSpalte = i;
						break;
					}
				}
				if (insertSpalte > 0)
					matMatrix.ColumnNames.Insert(insertSpalte, dnVl);
			}

			
			//Felder mit Preisen füllen
			for (zeile = 0; zeile < matListe.MaxRow; zeile++)
			{
				int tmpId = (int)matListe.getAt(zeile, spalteElemId);
				int tmpDn = (int)matListe.getAt(zeile, spalteDn);
                bool tmpAnlage = (bool)matListe.getAt(zeile, spalteAnlage);

				//Zeile suchen in der der Wert eingetragen werden soll
				int matZeile = -1;
				for (int i = 0; i < matMatrix.MaxRow; i++)
				{
                    ListViewField anlage = (ListViewField)matMatrix.getAt(i, spalteAnlage);

					if ((int)matMatrix.getAt(i, spalteElemId) == tmpId &&
                        (int)matMatrix.getAt(i, spalteDn) == tmpDn &&
						(bool)anlage.Tag == tmpAnlage)
					{
						matZeile = i;
						break;
					}
				}

				//Wenn noch kein Eintrag vorhanden ist, diesen anlegen
				if (matZeile < 0)
				{
                    ListViewField anlage = new ListViewField();
                    if (tmpAnlage)
                        anlage.setWert("ja", true);
                    else
                        anlage.setWert("nein", true);
                    anlage.Tag = tmpAnlage;

					matZeile = matMatrix.MaxRow;
					matMatrix.setAt(matZeile, spalteElemId, tmpId);
					matMatrix.setAt(matZeile, spalteBez, matListe.getAt(zeile, spalteBez));
                    matMatrix.setAt(matZeile, spalteAnlage, anlage);
					matMatrix.setAt(matZeile, spalteDn, tmpDn);
				}

				//Dn Verteiler holen
				int dnvl = 0;
				matListe.getIntAt(zeile, spalteDnVl, out dnvl);

				//Spalte suchen an der der Preis eingetragen werden muss
				int spCount = -1;
				for (spCount = firstPreisSpalteVk3; spCount < matMatrix.ColumnNames.Count; spCount++)
				{
					EnRohrDn tmpRoDn = matMatrix.ColumnNames[spCount] as EnRohrDn;
					if (tmpRoDn == null)
						continue;
					if (tmpRoDn.DnId == dnvl)
						break;
				}

				if (spCount >= 0)
				{
					ListViewField field = new ListViewField();
					field.setWert((double)matListe.getAt(zeile, spaltePreis), 2, false);
                    field.Tag = (int)matListe.getAt(zeile, spalteCounter);
					matMatrix.setAt(matZeile, spCount, field);
				}
			}

			return true;
		}


		/**
		 * Änderungen zwischen den Matrizen speichern
		 */
		public bool saveVk3(bool isVerzinktPreis, EnMatrix matrix)
		{
            if (!saveVk3(firstPreisSpalteVk3, isVerzinktPreis, matrix))
                return false;
           
			return true;
		}





		public bool hasChangedValues(EnMatrix matrix)
		{
			for (int zeile = 0; zeile < matrix.MaxRow; zeile++)
			{
				for (int spalte = 0; spalte < matrix.MaxColumn && spalte < matrix.ColumnNames.Count; spalte++)
				{
					//Preis lesen
					ListViewField field = matrix.getAt(zeile, spalte) as ListViewField;
					if (field == null)
						continue;

					if (field.HasChangedValue)
						return true;
				}
			}

			return false;
		} 

		private bool initDb()
		{
            if (mAbfrage == null)
            {
                EnConnectionData conData = new EnConnectionData();

                //Datenbank setzen
                conData.DatabaseTyp = EnConnectionData.DbTyp.Access;
                conData.Server = server;
                conData.Datenbank = datenbank;

                DbConnection dbConn = new DbConnection(conData);
                mAbfrage = new DbSqlBase(dbConn);
            }
            else
                mAbfrage.Errorliste.Clear();


			return true;
		}

       
		public string Server
		{
			set
			{
				server = value;
			}
		}

		public string Datenbank
		{
			set
			{
				datenbank = value;
			}
		}

		

		/**
		 * Abfrage starten
		 */
        private bool start(string sql, EnMatrix matrix)
        {
            //Db Objekt zurücksetzen, da sonst nicht immer die aktuellen Werte aus der DB gelesen werden! (warum auch immer!?!)
            mAbfrage = null;

			if (!initDb())
				return false;

            if (matrix == null)
                return false;

			if (string.IsNullOrEmpty(sql))
                return false;

            //Abfrage starten
			if (!mAbfrage.executeSql(sql, matrix))
			{
				appendErrorListeAndClear(mAbfrage);
				return false;
			}

            //Db wieder schließen
            mAbfrage.Connection.closeDb();

            //WaxDatum in String umwandeln
           // waxDateToString(matrix);
					
            return true;
        }



		/**
		 * Wert in Vk3 Tabelle aktualisieren
		 */
		public bool updateVk3(int id, int elementDn, int dnvl, double preis, bool isVerzinktPreis)
		{
			if (!initDb())
				return false;

			//Edit Spalten
			List<string> spalten = new List<string>();
			spalten.Add(getPreisfield(isVerzinktPreis));
			
			//Values
			List<object> values = new List<object>();
			values.Add(preis);

			//Filter
			EnSqlFilter filter = new EnSqlFilter();
			filter.addFilter("Id", id);
			filter.addFilter("Dn", elementDn);
			filter.addFilter("Dnvl", dnvl);

            //Abfrage starten
			int changedRows = 0;
			if (!mAbfrage.updateTable(tabVk3, spalten, values, filter, ref changedRows))
			{
				appendErrorListeAndClear(mAbfrage);
				return false;
			}

			return true;
		}


        public bool updateVk3(int counter, double preis, bool isVerzinktPreis)
        {
            if (!initDb())
                return false;

            //Edit Spalten
            List<string> spalten = new List<string>();
            spalten.Add(getPreisfield(isVerzinktPreis));

            //Values
            List<object> values = new List<object>();
            values.Add(preis);

            //Filter
            EnSqlFilter filter = new EnSqlFilter();
            filter.addFilter("Counter", counter);
            
            //Abfrage starten
            int changedRows = 0;
            if (!mAbfrage.updateTable(tabVk3, spalten, values, filter, ref changedRows))
            {
                appendErrorListeAndClear(mAbfrage);
                return false;
            }

            return true;
        }


		public bool readFestpreis(bool filterPreisNull, EnMatrix matrix)
		{
			string sql = "SELECT a.Artikelgruppe,a.Artikelnummer, a.FestPreis, a.SapPreis, f.dn1, f.dn2, f.dn3, f.dn4, f.GruppeDeutsch as Bez_Fitting, a.BezeichnungDeutsch as Sap_Bezeichnng, b.Deutsch as Wax_Bezeichnung " +
						 "FROM (Artikelstamm a LEFT JOIN formstuecke_und_fittinge f ON a.Artikelnummer = f.artikelnr) LEFT JOIN bauelementgruppen b ON a.Artikelgruppe = b.elemid ";

			if (filterPreisNull)
				sql += "WHERE a.FestPreis>0 ";
			else
				sql += "WHERE a.FestPreis>=0 ";

			//Order by anfügen
			sql += "ORDER BY a.Artikelgruppe, f.GruppeDeutsch, f.dn1, f.dn2, f.dn3, f.dn4, a.BezeichnungDeutsch"; 

			if (!start(sql, matrix))
				return false;

			tabSpalteArtikelNr = 1;
			tabSpaltePreis = 2;
			if (!setPreisToEdit(tabSpaltePreis, matrix))
				return false;

			return true;
		}


		/**
		 * Preisspalte editierbar machen
		 */
		private bool setPreisToEdit(int nrPreisSpalte, EnMatrix matrix)
		{
			if (nrPreisSpalte >= matrix.MaxColumn)
				throw new Exception(internerError);

			for (int zeile = 0; zeile < matrix.MaxRow; zeile++)
			{
				object obj = matrix.getAt(zeile, nrPreisSpalte);
				if (obj == null || obj.GetType() != typeof(double))
					continue;

				ListViewField field = new ListViewField();
				field.setWert((double)obj, 2, false);

				//Wert als Edit Feld speichern
				matrix.setAt(zeile, nrPreisSpalte, field);
			}

			return true;
		}

		/**
		 * Preise in Fitting Tabelle schreiben
		 */
		public bool saveFestpreis(EnMatrix matrix)
		{
			for (int zeile = 0; zeile < matrix.MaxRow; zeile++)
			{
				ListViewField field = matrix.getAt(zeile, tabSpaltePreis) as ListViewField;
				if (field == null)
					throw new Exception(internerError);

				//prüfen ob Wert verändert wurde
				if (!field.HasChangedValue)
					continue;

				double preis = (double)field.Value;

				//Artikelnr
				int artikelnr = 0;
				if (!matrix.getIntAt(zeile, tabSpalteArtikelNr, out artikelnr))
					throw new Exception(internerError);

				if (!updatePreis(tabFestpreisArtikel, "Artikelnummer", artikelnr, "FestPreis", preis))
					return false;

				//Flag zurücksetzen
				field.HasChangedValue = false;
			}

			return true;
		}

		private bool updatePreis(string tabFittinge, string tabKey, int artikelnr, string tabWert, double preis)
		{
			if (!initDb())
				return false;

			//Edit Spalten
			List<string> spalten = new List<string>();
			spalten.Add(tabWert);

			//Values
			List<object> values = new List<object>();
			values.Add(preis);

			//Filter
			EnSqlFilter filter = new EnSqlFilter();
			filter.addFilter(tabKey, artikelnr);
			
			//Abfrage starten
			int changedRows = 0;
			if (!mAbfrage.updateTable(tabFittinge, spalten, values, filter, ref changedRows))
			{
				appendErrorListeAndClear(mAbfrage);
				return false;
			}

			return true;
		}



        public bool readVk3Muffe(int werk, bool readVerzinktePreise, int dnvl, int dnMuffe, int minId, int maxId, int muffenVon, int muffenBis, int rlaengeVon, int rlaengeBis, int element, EnMatrix matrix)
        {
            matrix.clearAll();

            string andDnVl = "";
            if (dnvl > 0)
                andDnVl = String.Format("and {0}.Dnvl = {1}",tabVk3, dnvl.ToString());

            string andMuffeDn = "";
            if (dnMuffe > 0)
                andMuffeDn = String.Format("and {0}.Dn = {1}", tabVk3, dnMuffe.ToString());

            string eleFilter = "";
            if (element > 0)
                eleFilter = String.Format("and {0}.Id={1} ", tabVk3, element);

            string filterEleAnz = "";
            if (muffenVon > 0 || muffenBis > 0)
            {
                filterEleAnz = String.Format("and {0}.ElementAnzahlVon>={1} " +
                                             "and {0}.ElementAnzahlBis={2} ",
                                             tabVk3, muffenVon, muffenBis);
            }

            string filterRohrlanege = "";
            if (rlaengeVon > 0 || rlaengeBis > 0)
            {
                filterRohrlanege = String.Format("and {0}.RohrLaengeVon>={1} " +
                                                 "and {0}.RohrLaengeBis<={2} ",
                                                 tabVk3, rlaengeVon, rlaengeBis);
            }

            string sqlStatement = string.Format("SELECT {1}.Id, b.BezeichnungDeutsch, {1}.Anlage, {1}.Dnvl, {1}.Dn, {1}.ElementAnzahlVon, {1}.ElementAnzahlBis, {1}.ElementAnzahlDnVon, {1}.ElementAnzahlDnBis, {1}.ElementAnzahlId, bauelementgruppen.Deutsch, {1}.RohrLaengeVon, {1}.RohrLaengeBis, {1}.{0}, {1}.Counter " +                                         
                                         
                                         "FROM ({1} INNER JOIN vk3bezeichnung b ON {1}.Id = b.Id) " +
                                         "LEFT JOIN bauelementgruppen ON {1}.ElementAnzahlId = bauelementgruppen.elemid " +
                                         
                                         "where {1}.Fertigungsstaette={6} " +
                                         "{9} " +
                                         "{2} {3} " +
                                         "and {1}.Id >= {4} and {1}.Id <= {5} " +

                                         "and ({1}.ElementAnzahlVon>0 " +
                                           "or {1}.ElementAnzahlBis>0 " +
                                           "or {1}.ElementAnzahlDnVon>0 " +
                                           "or {1}.ElementAnzahlDnBis>0 " +
                                           "or {1}.RohrLaengeVon>0 " +
                                           "or {1}.RohrLaengeBis>0 " +
                                           "or {1}.ElementAnzahlId>0) " +
                                         
                                         "{7} " +
                                         "{8} " +

                                         "order by {1}.Id, {1}.Dnvl, {1}.Dn, {1}.ElementAnzahlVon, {1}.ElementAnzahlBis, {1}.ElementAnzahlDnVon, {1}.ElementAnzahlDnBis, {1}.RohrLaengeVon, {1}.RohrLaengeBis",
                                         getPreisfield(readVerzinktePreise), tabVk3,
                                         andDnVl, andMuffeDn,
                                         minId, maxId,
                                         werk,
                                         filterEleAnz,
                                         filterRohrlanege,
                                         eleFilter);

            EnMatrix mat = new EnMatrix();
            if (!start(sqlStatement, mat))
                return false;

            if (!convertInVkMuffenMatrix(mat, matrix))
                return false;


            return true;
        }

        /// <summary>
        /// In eine Matrix für die Muffenpreispflege konvertieren
        /// </summary>
        /// <param name="matListe"></param>
        /// <param name="matMatrix"></param>
        /// <returns></returns>
        private bool convertInVkMuffenMatrix(EnMatrix matListe, EnMatrix matMatrix)
        {           
            int spalteElemId = 0;
            int spalteBez = 1;
            int spalteAnlage = 2;
            int spalteRohrDn = 3;
            int spalteMuffeDn = 4;            
            int spalteElAnzVon = 5;
            int spalteElAnzBis = 6;
            int spalteElAnzDnVon = 7;
            int spalteElAnzDnBis = 8;            
            int spalteElId = 9;
            int spalteElIdBezeichnung = 10;
            int spalteRohrLaengeVon = 11;
            int spalteRohrLaengeBis = 12;
            
            int spaltePreis = 13;
            int spalteCounter = 14;
            


            
            matMatrix.ColumnNames.Add("ID");
            matMatrix.ColumnNames.Add("Bezeichnung");
            matMatrix.ColumnNames.Add("Anlage");
            matMatrix.ColumnNames.Add("Rohr DN");
            matMatrix.ColumnNames.Add("El DN");
            matMatrix.ColumnNames.Add("Anz El >");
            matMatrix.ColumnNames.Add("Anz El <=");
            matMatrix.ColumnNames.Add("Anz El DN >");
            matMatrix.ColumnNames.Add("Anz El DN <=");
            matMatrix.ColumnNames.Add("El ID");
            matMatrix.ColumnNames.Add("El Bez");

            //Längen Spalten erzeugen
            int zeile;
            for (zeile = 0; zeile < matListe.MaxRow; zeile++)
            {
                int rlaengeVon = 0;
                matListe.getIntAt(zeile, spalteRohrLaengeVon, out rlaengeVon);
                int rlaengeBis = 0;
                matListe.getIntAt(zeile, spalteRohrLaengeBis, out rlaengeBis);
                               
                //prüfen an welcher Stelle der Wert eingetragen werden muss
                int insertSpalte = matMatrix.ColumnNames.Count;
                for (int i = firstPreisSpalteVk3Muffe; i < matMatrix.ColumnNames.Count; i++)
                {
                    IntPair laengen = (IntPair)matMatrix.ColumnNames[i];

                    if (rlaengeVon == laengen.Val1 && rlaengeBis == laengen.Val2)
                    {
                        insertSpalte = -1;
                        break;
                    }
                    else if (rlaengeVon > laengen.Val1 || rlaengeBis > laengen.Val2)
                    {
                        insertSpalte = i + 1;
                    }
                    else if (rlaengeVon < laengen.Val1 || rlaengeBis < laengen.Val2)
                    {
                        insertSpalte = i;
                        break;
                    }
                }
                if (insertSpalte > 0)
                {
                    IntPair pair = new IntPair();
                    pair.Val1 = rlaengeVon;
                    pair.Val2 = rlaengeBis;                    
                    matMatrix.ColumnNames.Insert(insertSpalte, pair);
                }
            }


            //Felder mit Preisen füllen
            for (zeile = 0; zeile < matListe.MaxRow; zeile++)
            {
                int tmpId = (int)matListe.getAt(zeile, spalteElemId);
                int tmpRohrDn = (int)matListe.getAt(zeile, spalteRohrDn);
                int tmpMuffeDn = (int)matListe.getAt(zeile, spalteMuffeDn);
                int tmpAnzElVon = (int)matListe.getAt(zeile, spalteElAnzVon);
                int tmpAnzElBis = (int)matListe.getAt(zeile, spalteElAnzBis);
                int tmpAnzElDnVon = (int)matListe.getAt(zeile, spalteElAnzDnVon);
                int tmpAnzElDnBis = (int)matListe.getAt(zeile, spalteElAnzDnBis);                                
                int tmpElId = (int)matListe.getAt(zeile, spalteElId);
                string tmpElBez = (string)matListe.getAt(zeile, spalteElIdBezeichnung);
                bool tmpAnlage = (bool)matListe.getAt(zeile, spalteAnlage);

                int tmpLaengeVon = (int)matListe.getAt(zeile, spalteRohrLaengeVon);
                int tmpLaengeBis = (int)matListe.getAt(zeile, spalteRohrLaengeBis);


                //Zeile suchen in der der Wert eingetragen werden soll
                int matZeile = -1;
                for (int i = 0; i < matMatrix.MaxRow; i++)
                {
                    ListViewField anlage = (ListViewField)matMatrix.getAt(i, spalteAnlage);

                    if ((int)matMatrix.getAt(i, spalteElemId) == tmpId &&
                        (int)matMatrix.getAt(i, spalteRohrDn) == tmpRohrDn &&
                        (int)matMatrix.getAt(i, spalteMuffeDn) == tmpMuffeDn &&
                        (int)matMatrix.getAt(i, spalteElAnzVon) == tmpAnzElVon &&
                        (int)matMatrix.getAt(i, spalteElAnzBis) == tmpAnzElBis &&
                        (int)matMatrix.getAt(i, spalteElAnzDnVon) == tmpAnzElDnVon &&
                        (int)matMatrix.getAt(i, spalteElAnzDnBis) == tmpAnzElDnBis &&                                                
                        (int)matMatrix.getAt(i, spalteElId) == tmpElId &&
                        (bool)anlage.Tag == tmpAnlage)
                    {
                        matZeile = i;
                        break;
                    }
                }

                //Wenn noch kein Eintrag vorhanden ist, diesen anlegen
                if (matZeile < 0)
                {
                    ListViewField anlage = new ListViewField();
                    if (tmpAnlage)
                        anlage.setWert("ja", true);
                    else
                        anlage.setWert("nein", true);
                    anlage.Tag = tmpAnlage;

                    matZeile = matMatrix.MaxRow;
                    matMatrix.setAt(matZeile, spalteElemId, tmpId);
                    matMatrix.setAt(matZeile, spalteBez, matListe.getAt(zeile, spalteBez));
                    matMatrix.setAt(matZeile, spalteAnlage, anlage);
                    matMatrix.setAt(matZeile, spalteRohrDn, tmpRohrDn);
                    matMatrix.setAt(matZeile, spalteMuffeDn, tmpMuffeDn);
                    matMatrix.setAt(matZeile, spalteElAnzVon, tmpAnzElVon);
                    matMatrix.setAt(matZeile, spalteElAnzBis, tmpAnzElBis);
                    matMatrix.setAt(matZeile, spalteElAnzDnVon, tmpAnzElDnVon);
                    matMatrix.setAt(matZeile, spalteElAnzDnBis, tmpAnzElDnBis);                    
                    matMatrix.setAt(matZeile, spalteElId, tmpElId);
                    matMatrix.setAt(matZeile, spalteElIdBezeichnung, tmpElBez);
                }

                //Rohrlänge holen
                int rlaengeVon = 0;
                matListe.getIntAt(zeile, spalteRohrLaengeVon, out rlaengeVon);
                int rlaengeBis = 0;
                matListe.getIntAt(zeile, spalteRohrLaengeBis, out rlaengeBis);

                //Spalte suchen an der der Preis eingetragen werden muss
                int spCount = -1;
                for (spCount = firstPreisSpalteVk3Muffe; spCount < matMatrix.ColumnNames.Count; spCount++)
                {
                    IntPair laengen = (IntPair)matMatrix.ColumnNames[spCount];

                    if (rlaengeVon == laengen.Val1 && rlaengeBis == laengen.Val2)
                        break;
                }

                if (spCount >= 0)
                {                    
                    ListViewField field = new ListViewField();
                    field.setWert((double)matListe.getAt(zeile, spaltePreis), 2, false);
                    field.Tag = (int)matListe.getAt(zeile, spalteCounter);

                    matMatrix.setAt(matZeile, spCount, field);
                }
            }

            return true;
        }

        public bool saveVk3Muffe(bool isVerzinktPreis, EnMatrix matrix)
        {
            if (!saveVk3(firstPreisSpalteVk3Muffe, isVerzinktPreis, matrix))
                return false;

            return true;
        }

        private bool saveVk3(int fristSpaltePreise, bool isVerzinktPreis, EnMatrix matrix)
        {
            for (int zeile = 0; zeile < matrix.MaxRow; zeile++)
            {

                //Ab Spalte fristSpaltePreise fangen Preise an
                for (int spalte = fristSpaltePreise; spalte < matrix.MaxColumn && spalte < matrix.ColumnNames.Count; spalte++)
                {
                    //Preis lesen
                    ListViewField field = matrix.getAt(zeile, spalte) as ListViewField;
                    if (field == null)
                        continue;

                    if (!field.HasChangedValue)
                        continue;

                    if (field.Value == null || field.Value.GetType() != typeof(double))
                        continue;

                    double preis = (double)field.Value;
                    int counter = (int)field.Tag;


                    //Werte in DB ändern
                    if (!updateVk3(counter, preis, isVerzinktPreis))
                        return false;

                    //Flag zurücksetzen
                    field.HasChangedValue = false;
                }
            }

            return true;
        }


        //Artikelgruppen aus der Datenbank laden. Nur Artikelgruppen laden, wenn dazu ein Preis existiert
        public bool loadElemente(List<EnComboBoxWerte> elemente)
        {
            bool orderId = true;
            string sqlStatement = "SELECT Vk3Pos.Id, vk3bezeichnung.BezeichnungDeutsch " +
                                  "FROM Vk3Pos INNER JOIN vk3bezeichnung ON Vk3Pos.Id = vk3bezeichnung.id " +
                                  "GROUP BY Vk3Pos.Id, vk3bezeichnung.BezeichnungDeutsch ";
            if (orderId)
                sqlStatement += "ORDER BY Vk3Pos.Id";
            else
                sqlStatement += "ORDER BY vk3bezeichnung.BezeichnungDeutsch";


            EnMatrix mat = new EnMatrix();
            if (!start(sqlStatement, mat))
                return false;

            for (int zeile = 0; zeile < mat.MaxRow; zeile++)
            {
                int id = 0;
                if (!mat.getIntAt(zeile, 0, out id))
                    continue;

                if (id == 0)
                    continue;

                string name = "";
                if (!mat.getStringAt(zeile, 1, out name))
                    name = "Keine Bezeichnung vorhanden";

                string bez;                
                if (orderId)
                    bez = String.Format("{0} = {1}",id, name);                   
                else
                     bez = String.Format("{0} (ID={1})", name, id);

                EnComboBoxWerte artgr = new EnComboBoxWerte(bez, id);
                elemente.Add(artgr);
            }
            return true;
        }


        public struct Vk3Preis
        {
            public int Counter;
            public int Werk;
            public int Id;
            public int Dn;
            public int Dnvl;
            public double Preis;
            public double PreisVerzinkt;
            public int RohrLaengeVon;
            public int RohrLaengeBis;
            public int AnzahlElVon;
            public int AnzahlElBis;
            public int AnzahlElementeId;
            public int AnzahlElementeDnVon;
            public int AnzahlElementeDnBis;
            public int ElementLaengeVon;
            public int ElementLaengeBis;
            public bool IsAnlage;
            public string Beschreibung;
        }

        public bool insertPreis(Vk3Preis preis)
        {
            initDb();
                                  
            //Prüfen ob es schon ein Preis zu diesen Bedingungen gibt            
            EnMatrix mat = new EnMatrix();
            if (!getPreisWerte(preis, mat))
                return false;


            if (mat.MaxRow > 0)
            {
                Errorliste.addError(String.Format("Zu diesen Bedingungen gibt es schon einen Eintrag! Counter = {0}", mat.getAt(0, 0)));
                return false;
            }

            //Spalten und Werte für INSERT
            List<string>spalten = new List<string>();
            List<object>values = new List<object>();

            getEditVk3TabValues(preis, spalten, values);            

            //Preis in DB eintragen
            if (!mAbfrage.insertInto(tabVk3, spalten, values))
            {
                appendErrorListe(mAbfrage);
                return false;
            }           

            return true;
        }

        private void getEditVk3TabValues(Vk3Preis preis, List<string> spalten, List<object> values)
        {            
            getSpaltenVk3Tab(spalten);

            //Werte
            values.Add(preis.Werk);
            values.Add(preis.Id);
            values.Add(preis.Dn);
            values.Add(preis.Dnvl);
            values.Add(preis.RohrLaengeVon);
            values.Add(preis.RohrLaengeBis);
            values.Add(preis.AnzahlElVon);
            values.Add(preis.AnzahlElBis);
            values.Add(preis.AnzahlElementeId);
            values.Add(preis.AnzahlElementeDnVon);
            values.Add(preis.AnzahlElementeDnBis);
            values.Add(preis.ElementLaengeVon);
            values.Add(preis.ElementLaengeBis);
            values.Add(preis.IsAnlage ? 1 : 0);
            values.Add(preis.Preis);
            values.Add(preis.PreisVerzinkt);
            values.Add(preis.Beschreibung);
        }

        private void getSpaltenVk3Tab(List<string> spalten)
        {
            spalten.Add("Fertigungsstaette"); 
            spalten.Add("Id");
            spalten.Add("Dn");
            spalten.Add("Dnvl");
            spalten.Add("RohrLaengeVon");
            spalten.Add("RohrLaengeBis");
            spalten.Add("ElementAnzahlVon");
            spalten.Add("ElementAnzahlBis");
            spalten.Add("ElementAnzahlId");
            spalten.Add("ElementAnzahlDnVon");
            spalten.Add("ElementAnzahlDnBis");
            spalten.Add("ElementLaengeVon");
            spalten.Add("ElementLaengeBis");
            spalten.Add("Anlage");
            spalten.Add("Preis");
            spalten.Add("PreisVerzinkt");
            spalten.Add("Beschreibung");
        }

        private bool getPreisWerte(Vk3Preis preis, EnMatrix mat)
        {
            string[] spaltenAbfrage = { "Counter" };

            EnSqlFilter filter = new EnSqlFilter();
            filter.addFilter("Fertigungsstaette", preis.Werk);
            filter.addFilter("Id", preis.Id);
            filter.addFilter("Dn", preis.Dn);
            filter.addFilter("Dnvl", preis.Dnvl);
            filter.addFilter("RohrLaengeVon", preis.RohrLaengeVon);
            filter.addFilter("RohrLaengeBis", preis.RohrLaengeBis);
            filter.addFilter("ElementAnzahlVon", preis.AnzahlElVon);
            filter.addFilter("ElementAnzahlBis", preis.AnzahlElBis);
            filter.addFilter("ElementAnzahlId", preis.AnzahlElementeId);
            filter.addFilter("ElementAnzahlDnVon", preis.AnzahlElementeDnVon);
            filter.addFilter("ElementAnzahlDnBis", preis.AnzahlElementeDnBis);
            filter.addFilter("ElementLaengeVon", preis.ElementLaengeVon);
            filter.addFilter("ElementLaengeBis", preis.ElementLaengeBis);
            filter.addFilter("Anlage", preis.IsAnlage ? 1 : 0);
            
            if (!mAbfrage.read(tabVk3, spaltenAbfrage, filter, mat))
            {
                appendErrorListe(mAbfrage);
                return false;
            }

            return true;
        }

        /// <summary>
        /// Preis aus der DB löschen
        /// </summary>
        /// <param name="counter"></param>
        /// <returns></returns>
        public bool deletePreis(int counter)
        {
            initDb();

            EnSqlFilter filter = new EnSqlFilter();
            filter.addFilter("Counter", counter);
            if (!mAbfrage.delete(tabVk3, filter))
            {
                appendErrorListe(mAbfrage);
                return false;
            }

            return true;
        }

        public bool editPreis(Vk3Preis preis)
        {
            initDb();

            EnMatrix mat = new EnMatrix();
            getPreisWerte(preis, mat);

            //Prüfen ob die Bedinungen schon unter einer anderen Counternummer existieren
            if (mat.MaxRow > 0)
            {
                int tmpCounter = 0;
                mat.getIntAt(0, 0, out tmpCounter);

                if (tmpCounter != preis.Counter)
                {
                    Errorliste.addError(String.Format("Zu diesen Bedingungen gibt es schon einen Eintrag! Counter = {0}", tmpCounter));
                    return false;
                }
            }

            //Spalten und Values
            List<string>spalten = new List<string>();
            List<object>values = new List<object>();
            getEditVk3TabValues(preis, spalten, values);    

            EnSqlFilter filter = new EnSqlFilter();
            filter.addFilter("Counter", preis.Counter);

            int anzEdit = 0;
            if (!mAbfrage.updateTable(tabVk3, spalten, values, filter, ref anzEdit))
            {
                appendErrorListe(mAbfrage);
                return false;
            }

            return true;
        }

        public bool readPreis(ref Vk3Preis preis)
        {
            List<string>spalten = new List<string>();            
            getSpaltenVk3Tab(spalten); 

            EnSqlFilter filter = new EnSqlFilter();
            filter.addFilter("Counter", preis.Counter);

            List<object>values = new List<object>();
            if (!mAbfrage.readOneRow(tabVk3, spalten.ToArray(), filter, values))
            {
                appendErrorListe(mAbfrage);
                return false;
            }

            int counter = 0;

            preis.Werk = (int)values[counter++];
            preis.Id = (int)values[counter++];
            preis.Dn = (int)values[counter++];
            preis.Dnvl = (int)values[counter++];
            preis.RohrLaengeVon = (int)values[counter++];
            preis.RohrLaengeBis = (int)values[counter++];
            preis.AnzahlElVon = (int)values[counter++];
            preis.AnzahlElBis = (int)values[counter++];
            preis.AnzahlElementeId = (int)values[counter++];
            preis.AnzahlElementeDnVon = (int)values[counter++];
            preis.AnzahlElementeDnBis = (int)values[counter++];
            preis.ElementLaengeVon = (int)values[counter++];
            preis.ElementLaengeBis = (int)values[counter++];

            preis.IsAnlage = (bool)values[counter++];

            preis.Preis = (double)values[counter++];
            preis.PreisVerzinkt = (double)values[counter++];


            if (values[counter].GetType() == typeof(string))
                preis.Beschreibung = (string)values[counter];

            return true;
        }
    }
}
