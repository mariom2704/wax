using System;
using System.Collections.Generic;
using System.Text;
using MxLib.Entities;



namespace MxStat.Gp
{
    /**
     * Klasse die den Ablauf der Statistik steuert
     */
    public class GpAuswertung : GpBase
    {
        Auswertung mIdAuswertung = Auswertung.NichtGesetzt;

        string sqlStatement;

        EnConnectionData condata;

        //Ergebnis der Auswertung
        EnMatrix mResultMatrix;


        public GpAuswertung(EnConnectionData data)
        {
            this.condata = data;
            mResultMatrix = new EnMatrix();
        }



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
                GpSqlAbfrage abfrage = new GpSqlAbfrage(condata);                
                abfrage.SqlStatement = sqlStatement;

                abfrage.Errorliste.Clear();
                if (!abfrage.start(ResultMatrix))
                {
                    appendErrorListe(abfrage);
                    return false;
                }
            }


            return true;
        }





        public enum Auswertung
        {
            NichtGesetzt = 0,
            SQL_Abfrage = 1
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


      
        /// <summary>
        /// Werte zurücksetzen
        /// </summary>
        public void clear()
        {
            mIdAuswertung = Auswertung.NichtGesetzt;
            sqlStatement = "";
            mResultMatrix.clearAll();
        }

    }
}
