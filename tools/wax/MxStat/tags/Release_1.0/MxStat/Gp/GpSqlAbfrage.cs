using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using MxStat.Db;
using MxStat.Entities;
using System.Windows.Forms;

namespace MxStat.Gp
{
    public class GpSqlAbfrage
    {
	
		string sqlStatement;

      
		string server = "",
			   datenbank = "";

        DbSqlBase mAbfrage;

       
        public GpSqlAbfrage()
        {
            mAbfrage = new DbSqlBase();
        }

		public string SqlStatement
        {
            set
            {
				sqlStatement = value;
            }
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

        /// <summary>
        /// Abfrage starten
        /// </summary>
        /// <param name="matrix"></param>
        /// <returns></returns>
        public bool start(EnMatrix matrix)
        {
            if (matrix == null)
                return false;

			if (string.IsNullOrEmpty(sqlStatement))
                return false;

            //Abfrage starten
			if (!mAbfrage.executeSql(sqlStatement, matrix))
                return false;

            //Db wieder schlieﬂen
            mAbfrage.Connection.closeDb();

            //WaxDatum in String umwandeln
            waxDateToString(matrix);

            return true;
        }

        /// <summary>
        /// wax Datum in String umwandeln
        /// </summary>
        /// <param name="matrix"></param>
        private void waxDateToString(EnMatrix matrix)
        {
            string datum = "datum",
                   tmpCol;

            int nrSpalte = 0,
                find;

            //Pr¸fen ob eine Spalte ein Datum Enth‰lt
            foreach (string col in matrix.ColumnNames)
            {
                tmpCol = col.ToLower();

                find = tmpCol.IndexOf(datum);
                if (find >= 0)
                {
                    setDatumSpalteToString(nrSpalte, matrix);       
                }

                nrSpalte++;
            }
           
        }


        private bool setDatumSpalteToString(int spalte, EnMatrix matrix)
        {
            EnWaxDate enWaxDate = new EnWaxDate();
            DateTime date = new DateTime();

            double dTemp;

            try
            {
                //Datum in String umwandeln
                for (int i = 0; i < matrix.MaxRow; i++)
                {
                    dTemp = (double)matrix.getAt(i, spalte);

                    enWaxDate.Value = (long)dTemp;
                    date = enWaxDate.getDate();

                    matrix.setAt(i, spalte, date.ToShortDateString());

                }
            }
            catch
            {
                return false;
            }

            return true;
        }

        public bool initDb()
        {
			

            //Datenbank setzen
            mAbfrage.Connection.DatabaseTyp = DbConnection.DbTyp.MySql;
            mAbfrage.Connection.Server      = server;
            mAbfrage.Connection.Datenbank   = datenbank;
            mAbfrage.Connection.User        = "wax+";
            mAbfrage.Connection.Passwort    = "rohr1";


            //mAbfrage.Connection.DatabaseTyp = DbConnection.DbTyp.Access;
            //mAbfrage.Connection.Datenbank = "d:\\tmp\\WaxStamm.mdb";

            return true;
        }


       



    }
}
