using System;
using System.Collections.Generic;
using System.Text;

using MxStat.Entities;

namespace MxStat.Gp
{
    public class GpAuswertung
    {
        Auswertung mIdAuswertung = Auswertung.NichtGesetzt;

        string sqlStatement;

		string server = "",
			   datenbank = "";


        EnMatrix mResultMatrix;


        public GpAuswertung()
        {
            mResultMatrix = new EnMatrix();
        }


        public enum Auswertung
        {
            NichtGesetzt = 0,
            SQL_Abfrage = 1
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

       



        public EnMatrix ResultMatrix
        {
            set
            {
                mResultMatrix = value;
            }
            get
            {
                return mResultMatrix;
            }
        }

        public Auswertung IdAuswertung
        {
            set
            {
                mIdAuswertung = value;
            }
        }

		public string SqlStatement
        {
            set
            {
				sqlStatement = value;
            }
        }


        /**
         * Auswertung starten
         */
        public bool start()
        {
			ResultMatrix.clearAll();

            if (mIdAuswertung == Auswertung.NichtGesetzt)
                return false;


            if (mIdAuswertung == Auswertung.SQL_Abfrage)
            {
                if (string.IsNullOrEmpty(sqlStatement))
                    return false;

                //SQL Abfrage durchführen
                GpSqlAbfrage abfrage = new GpSqlAbfrage();

				abfrage.Datenbank = datenbank;
				abfrage.Server = server;
				abfrage.SqlStatement = sqlStatement;
          
				abfrage.initDb();

                if (!abfrage.start(ResultMatrix))
                    return false;
            }


            return true;
        }

        /**
        * alles Zurücksetzen
        */
        public void clear()
        {
            mIdAuswertung = Auswertung.NichtGesetzt;
            sqlStatement = "";
            mResultMatrix.clearAll();
        }

    }
}
