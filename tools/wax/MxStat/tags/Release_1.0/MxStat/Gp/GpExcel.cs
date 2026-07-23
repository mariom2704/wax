using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using MxStat.Entities;
using MxStat.Db;

namespace MxStat.Gp
{
    public class GpExcel
    {
        string mDatei = "";

        DbSqlBase mAbfrage;

        public GpExcel()
        {
            mAbfrage = new DbSqlBase();
        }

        public string Datei
        {
            set
            {
                mDatei = value;
            }
        }


        public bool writeMatrixToDatei(EnMatrix matrix)
        {
            if (String.IsNullOrEmpty(mDatei))
                return false;

            //evt. vorhandene Datei löschen
            try
            {
                File.Delete(mDatei);
            }
            catch (IOException )
            {
                return false; //Wenn Datei geöffnet ist, kann sie nicht gelöscht werden
            }


            if (!initDb())
                return false;

            string tabName = "Tab1";

            //Exceldatei anlegen
            if (!mAbfrage.createTableAllColumnsAsString(tabName, matrix.ColumnNames))
                return false;


            //Matrix in Exceltabelle schreiben
            if (!mAbfrage.insertInto(tabName, matrix))
                return false;

            mAbfrage.Connection.closeDb();

            return true;
        }

        private bool initDb()
        {
            if (String.IsNullOrEmpty(mDatei))
                return false;

            mAbfrage.Connection.DatabaseTyp = DbConnection.DbTyp.Excel;
            mAbfrage.Connection.Datenbank = mDatei;

            return true;
        }

       

    }
}
