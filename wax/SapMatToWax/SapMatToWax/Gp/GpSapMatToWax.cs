using System;
using System.Collections.Generic;
using System.Text;
using SapMatToWax.En;
using SapMatToWax.Db;
using MxLib.Basis;
using System.IO;
using System.Windows.Forms;

namespace SapMatToWax.Gp
{
    public class GpSapMatToWax
    {       

        MxErrorListe errorListe = new MxErrorListe();

        int changedRows = 0;
        int insertRows = 0;

        string server = "";
        string sapdatei = "";
        string stammDb = "";

        bool writePreise = true;

        public GpSapMatToWax(string sapdatei, string stammDb, bool writePreise)
        {
            this.sapdatei = sapdatei;
            this.stammDb = stammDb;
            this.writePreise = writePreise;
        }
        
        public bool start()
        {           

            //Prüfen ob die Datei vorhanden ist
            if (!File.Exists(sapdatei))
            {
                ErrorListe.addError(String.Format("Der Materialstamm wurde noch nicht aus SAP exportiert!\nDie Datei {0} wurde nicht gefunden!", sapdatei));
                return false;
            }

            //Datum prüfen
            DateTime time = File.GetLastWriteTime(sapdatei);
            TimeSpan span = DateTime.Today - time;

            if (span.Days > 0)
            {
                string meldung = String.Format("Der exportierte Materialstamm aus SAP ist älter als 1 Tag!\nDie Datei {0} wurde am {1} erstellt.\n\nWollen Sie trotzdem den Materialstamm einlesen?", sapdatei, time.ToString());
                if (MessageBox.Show(meldung, "Warnung", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) != DialogResult.Yes)
                    return true;
            }


            List<EnArtikel> artikel = new List<EnArtikel>();
            GpSapMaterialstamm sapMat = new GpSapMaterialstamm(sapdatei);
            if (!sapMat.einlesen(artikel))
            {
                errorListe.AddRange(sapMat.ErrorListe);
                return false;
            }
            
            DbArtikelstamm dbArtikelstamm = new DbArtikelstamm(server, stammDb, writePreise);
            if (!dbArtikelstamm.writeArtikel(artikel))
            {
                errorListe.AddRange(dbArtikelstamm.ErrorListe);
                return false;
            }

            changedRows = dbArtikelstamm.ChangedRows;
            insertRows = dbArtikelstamm.InsertRows;

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
