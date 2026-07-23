using System;
using System.Collections.Generic;
using System.Text;
using SapMatToWax.En;
using MxLib.Entities;
using MxLib.Db;
using MxLib.Basis;

namespace SapMatToWax.Db
{
    public class DbArtikelstamm
    {
        string db,
               server;

        string tabArtikelstamm = "artikelstamm";

        string keySpalte = "Artikelnummer";

        MxErrorListe errorListe = new MxErrorListe();
        
        int changedRows = 0;
        int insertRows = 0;

        bool writePreise = true;

        public DbArtikelstamm(string server, string db, bool writePreise)
        {
            this.server = server;
            this.db = db;
            this.writePreise = writePreise;
        }


        /**
         * Werte der Artikel im Artikelstamm aktualisieren
         */
        public bool writeArtikel(List<EnArtikel> artikel)
        {
            //Datenbank initialisieren
            EnConnectionData connData = new EnConnectionData();
            connData.Datenbank = db;            

            DbConnection conn = new DbConnection(connData);

            conn.setDatabaseTyp();

            DbSqlBase abfrage = new DbSqlBase(conn);
            abfrage.CloseDbByAccess = true;

            EnMatrix mat = new EnMatrix();
            listToMatrix(artikel, mat);

            //Key Felder
            List<string> keys = new List<string>();
            keys.Add(keySpalte);

            //Werte aktualisieren   
            if (!abfrage.updateTable(tabArtikelstamm, mat, keys.ToArray(), true, ref changedRows, ref insertRows))
            {
                ErrorListe.addError(String.Format("Anzahl Zugriffe {0}", abfrage.ExecuteReaderCounter));
                ErrorListe.AddRange(abfrage.Errorliste);
                return false;
            }
            
            return true;
        }


        private bool listToMatrix(List<EnArtikel> artikel, EnMatrix mat)
        {
            //Spalten initialisieren
            List<object> spalten = new List<object>();

            spalten.Add(keySpalte);
            spalten.Add("BezeichnungDeutsch");
            spalten.Add("BezeichnungEnglisch");
            spalten.Add("Einheit");
            spalten.Add("Gewicht");
            spalten.Add("Status");

            if (writePreise)
                spalten.Add("SapPreis");

            mat.ColumnNames.Clear();
            mat.ColumnNames.AddRange(spalten);

            int count = 0;
            foreach (EnArtikel art in artikel)
            {
                int spCount = 0;
                mat.setAt(count, spCount++, art.Artikelnummer);
                mat.setAt(count, spCount++, art.Bezeichnung);
                mat.setAt(count, spCount++, art.BezeichnungEng);
                mat.setAt(count, spCount++, art.BezeichnungEinheit);
                mat.setAt(count, spCount++, art.GewichtProEinheit);
                mat.setAt(count, spCount++, art.Status);
                if (writePreise)
                    mat.setAt(count, spCount++, art.SapPreis);
                
                count++;
            }

            return true;
        }

        public MxErrorListe ErrorListe
        {
            get { return errorListe; }
            set { errorListe = value; }
        }

        public int ChangedRows
        {
            get { return changedRows; }
            set { changedRows = value; }
        }

        public int InsertRows
        {
            get { return insertRows; }
            set { insertRows = value; }
        }
    }
}
