using System;
using System.Collections.Generic;
using System.Text;
using System.IO;


using System.Windows.Forms;
using MxLib.Db;
using MxLib.Entities;

namespace MxStat.Gp
{
    public class GpSqlAbfrage : GpBase
    {
	
		string sqlStatement;

     

        DbSqlBase mAbfrage;

       
        public GpSqlAbfrage(EnConnectionData data)
        {         
            DbConnection con = new DbConnection(data);
            mAbfrage = new DbSqlBase(con);
        }

		public string SqlStatement
        {
            set
            {
				sqlStatement = value;
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
            {
                appendErrorListe(mAbfrage);
                return false;
            }

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
            EnWaxDateTime enWaxDate = new EnWaxDateTime();
            DateTime date = new DateTime();

            double dTemp;

            try
            {
                //Datum in String umwandeln
                for (int i = 0; i < matrix.MaxRow; i++)
                {
                    dTemp = (double)matrix.getAt(i, spalte);

                    enWaxDate.Value = (int)dTemp;
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

       


       



    }
}
